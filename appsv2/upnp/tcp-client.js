/*
Copyright 2012 Google Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Boris Smus (smus@chromium.org)
*/

(function(exports) {
  /**
   * Define some local variables here.
   */
  const socket = chrome.experimental.socket;
  const dns = chrome.experimental.dns;

  function T(host, port) {
    this.host = host;
    this.port = port;

    // Callback functions.
    this.callbacks = {
      connect: null,    // Called when socket is connected.
      disconnect: null, // Called when socket is disconnected.
      recv: null,       // Called when client recieves data from server.
      sent: null        // Called when client sends data to server.
    };

    // Socket.
    this.socketId = null;
    this.isConnected = false;

    log('initialized tcp client');
  }

  /**
   * Connects to the TCP socket, and creates an open socket.
   */
  T.prototype.connect = function(callback) {
    // First resolve the hostname to an IP.
    dns.resolve(this.host, function(result) {
      var addr = result.address;
      var options = {onEvent: this._onEvent.bind(this)};
      socket.create('tcp', addr, this.port, options, this._onCreate.bind(this));
      this.isConnected = true;

      // Register connect callback.
      this.callbacks.connect = callback;
    }.bind(this));
  }

  T.prototype.sendMessage = function(msg, callback) {
    socket.write(this.socketId, msg + '\n', this._onWriteComplete.bind(this));

    // Register sent callback.
    this.callbacks.sent = callback;
  }

  T.prototype.addResponseListener = function(callback) {
    // Register received callback.
    this.callbacks.recv = callback;
  }

  T.prototype.disconnect = function() {
    socket.disconnect(this.socketId);
    this.isConnected = false;
  }

  T.prototype._onCreate = function(socketInfo) {
    this.socketId = socketInfo.socketId;
    log('socketId: ' + this.socketId);
    if (this.socketId > 0) {
      socket.connect(this.socketId, this._onConnectComplete.bind(this));
    } else {
      error('Unable to create socket');
    }
  }

  T.prototype._onEvent = function(socketEvent) {
    log('onEvent type: ' + socketEvent.type);
    if (socketEvent.type == 'connectComplete') {
      // Temporarily disabled to prevent multiple sockets from connecting.
      //this._onConnectComplete(socketEvent.resultCode);
    } else if (socketEvent.type == 'dataRead') {
      this._onDataRead({message: socketEvent.data});
    } else if (socketEvent.type == "writeComplete") {
      this._onWriteComplete(socketEvent.resultCode);
    } else {
      console.log('Unhandled socketEvent of type: ' + socketEvent.type);
    }
  }

  T.prototype._onConnectComplete = function(resultCode) {
    // Start polling for reads.
    setInterval(this._periodicallyRead.bind(this), 500)

    if (this.callbacks.connect) {
      console.log('connect complete');
      this.callbacks.connect();
    }
    log('onConnectComplete');
  }

  T.prototype._periodicallyRead = function() {
    socket.read(this.socketId, this._onDataRead.bind(this));
  }

  T.prototype._onDataRead = function(readInfo) {
    // Call received callback if there's data in the response.
    if (readInfo.message && this.callbacks.recv) {
      log('onDataRead');
      this.callbacks.recv(readInfo.message);
    }
  }

  T.prototype._onWriteComplete = function(writeInfo) {
    log('onWriteComplete');
    // Call sent callback.
    if (this.callbacks.sent) {
      this.callbacks.sent(writeInfo);
    }
  }

  function log(msg) {
    //document.getElementById('log').innerHTML += msg + '<br/>';
    console.log(msg);
  }

  function error(msg) {
    //document.getElementById('log').innerHTML += '<strong>Error: </strong>' + msg + '<br/>';
    console.error(msg);
  }

  exports.TcpClient = T;

})(window);

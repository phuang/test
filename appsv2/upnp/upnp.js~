(function(exports) {

  function Server(url) {
    this.init_ = false;
    this.url_ = url;
    this.cdService = null;
    this.cmService = null;
  }

  Server.prototype.init = function(callback) {
    if (this.init_) {
      callback(true);
      return;
    }

    _self = this;
    $.get(url, function(data) {
      _self.descritption_ = xmlToJson(data);

      var services = _self.description_.root.device.serviceList;
      for (var i = 0; i < services.lenght; i++) {
        switch(services[i].serviceId) {
          case "urn:upnp-org:serviceId:ContentDirectory":
            _self.cdService = service;
            breal;
          case "urn:upnp-org:serviceId:ConnectionManager":
            _self.cmService = service;
            break;
        }
      }
      _self.init_ = true;
      callback(true);
    });
  };

  Server.prototype.makeURL = function(path) {
    return this.url_ + path;
  };

  Server.prototype.getIcon = function() {
    return this.makeURL(this.description_.root.device.iconList[0].url);
  };

  Server.prototype.browser = function(path, pattern, callback) {
  }

  function T() {
    this.value = 1;
    this.callbacks = {
      serverFound: null;
    };
  }

  T.prototype.startDiscover = function() {
    this.servers_ = [];
    // TODO(penghuang): support real UPNP discover
    servers = ["http://172.23.176.2:9999/description.xml"];
    for (var i = 0 ; i < servers.length; i++) {
      server = new Server(servers[i]);
      this.servers_.push(server);
      this._onServerFound(server);
    }
  };

  T.prototype.getServers = function() {
    return this.servers_;
  };

  T.prototype._onServerFound = function(server) {
    if (this.callbacks.serverFound)
      this.callbacks.serverFound(server);
  };

  exports.UPNPClient = T;

})(window);

/* 
function m_search() {
  var address = '239.255.255.250';
  var port = 1900;
  socket.create('udp', address, port, {}, function(socketInfo) {
    var socketId = socketInfo.socketId;
    console.log(socketInfo);
    socket.connect(socketId, function(){
    var data = [
      'M-SEARCH * HTTP/1.1',
      'MX: 5',
      'ST: upnp:rootdevice',
      'MAN: "ssdp:discover"',
      'User-Agent: Platinum/0.5.3.0, DLNADOC/1.50',
      'Host: 239.255.255.250:1900'
    ];
    console.log(socketId);
    socket.sendTo(socketId, data.join("\r\n") + "\r\n\r\n", address, port, function(sendInfo) {
      console.log(sendInfo);
      setTimeout(function() {
        socket.recvFrom(socketId, function(recvInfo) {
          console.log(recvInfo);
      })}, 2000);
    });
  });
  });
}
*/



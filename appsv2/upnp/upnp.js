// vim: et sts=2 ts=2 :
(function(exports) {

  function UPNPServer(url, callback) {
    this.init_ = false;
    this.url_ = url;
    this.cdService_ = null;
    this.cmService_ = null;

    this._init(callback);
  }

  UPNPServer.prototype._init = function(callback) {
    _this = this;
    $.get(url, function(xml) {
      _this.descritption_ = xmlToJson(data);

      var services = _this.description_.root.device.serviceList;
      for (var i = 0; i < services.lenght; i++) {
        switch(services[i].serviceId) {
          case "urn:upnp-org:serviceId:ContentDirectory":
            _this.cdService = service;
            breal;
          case "urn:upnp-org:serviceId:ConnectionManager":
            _this.cmService = service;
            break;
        }
      }
      callback(true);
    });
  };

  UPNPServer.prototype.makeURL = function(path) {
    return this.url_ + path;
  };

  UPNPServer.prototype.getIcon = function() {
    return this.makeURL(this.description_.root.device.iconList[0].url);
  };

  UPNPServer.prototype.browser = function(id, start, count, callback) {
    if (this.cdService_ == null)
      return false;
    
    var url = this.makeURL(this.cdService_.cc);
    var params = new SOAPClientParameters();
    params.add("ObjectID", id);
    params.add("BrowseFlag", "BrowseDirectChildren");
    params.add("Filter", "*");
    params.add("StartingIndex", start);
    params.add("RequestedCount", count);
    params.add("SortCriteria", "");

    var namespace = "urn:schemas-upnp-org:service:ContentDirectory:1";
    SOAPClient._sendSoapRequestInternal(url,namespace, "Browse", params, true, function(o, r) {
       
      console.log(r);
    }, null);
    return true;
  };

  UPNPServer.prototype.soapCall = function(obj, callback) {
  };

  function T() {
    this.value = 1;
    this.callbacks = {
      serverFound: null,
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



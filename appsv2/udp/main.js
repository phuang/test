

var socket = chrome.experimental.socket;

function search(callback) {
  var option = {
    onEvent: function(info) {
      console.log(info);
    }
  };

  socket.create("udp", option, function(info) {
    var id = info.socketId;
    socket.bind(id, "0.0.0.0", 0, function(info) {
      const ADDRESS = '239.255.255.250';
      const PORT = 1900;
      const DATA = [
        'M-SEARCH * HTTP/1.1',
        'MX: 5',
        'ST: upnp:rootdevice',
        'MAN: "ssdp:discover"',
        'User-Agent: Platinum/0.5.3.0, DLNADOC/1.50',
        'Host: 239.255.255.250:1900',
        '\r\n'
      ];
      
      var search_message = DATA.join("\r\n").split("").map(function(i){
        return i.charCodeAt(0);
      });

      socket.sendTo(id, search_message, ADDRESS, PORT, function(info) {
        function waitReply() {
          var finished = 0;
          var servers = [];
          for (var i = 0; i < 10; i++) {
            socket.recvFrom(id, function(info) {
              if (info.data.length > 0) {
                data = info.data.map(function(i) {
                  return String.fromCharCode(i);
                }).join("").split("\r\n");
                $(data).each(function() {
                  var values = this.split(" ");
                  if (values[0] == "LOCATION:")
                    servers.push(values[1]);
                });
              }
              finished ++;
              if (finished == 10)
                callback(servers);
            });
          }
        };
        setTimeout(waitReply, 1000);
      });
    });
  });
}

search(function(servers){
  console.log(servers);
});

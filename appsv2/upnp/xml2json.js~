// Changes XML to JSON
function xmlToJson(xml) {
  
  if (xml.nodeType == 3) { // text
    return xml.nodeValue.trim();
  }

  // Create the return object
  var obj = {};

  if (xml.nodeType == 1) { // element
    // do attributes
    if (xml.attributes.length > 0) {
    obj["@attributes"] = {};
      for (var j = 0; j < xml.attributes.length; j++) {
        var attribute = xml.attributes.item(j);
        obj["@attributes"][attribute.nodeName] = attribute.nodeValue;
      }
    }
  }

  // do children
  if (xml.hasChildNodes()) {
    for(var i = 0; i < xml.childNodes.length; i++) {
      var item = xml.childNodes.item(i);
      var nodeName = item.nodeName;
      if (typeof(obj[nodeName]) == "undefined") {
        obj[nodeName] = xmlToJson(item);
      } else {
        if (nodeName == "#text") {
          obj[nodeName] += xmlToJson(item);
        } else {
          if (typeof(obj[nodeName].length) == "undefined" ||
              typeof(obj[nodeName].push) == "undefined") {
            var old = obj[nodeName];
            obj[nodeName] = [];
            obj[nodeName].push(old);
          }
          obj[nodeName].push(xmlToJson(item));
        }
      }
    }
  }
  
  var keys = [];
  for (var key in obj) {
    keys.push(key);
  }
  // console.log(keys);
  if (keys.length == 1 && keys[0] == "#text")
    return obj["#text"]
  if (obj["#text"] == "")
    delete obj["#text"];
  return obj;
};

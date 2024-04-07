var http = {
    get: function(url) {
      return new Promise(function(resolve, reject) {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', url, true);
        xhr.onload = function() {
          if (xhr.status >= 200 && xhr.status < 300) {
            resolve(xhr.responseText);
          } else {
            reject(xhr.statusText);
          }
        };
        xhr.onerror = function() {
          reject(xhr.statusText);
        };
        xhr.send();
      });
    },
    
    post: function(url, data) {
      return new Promise(function(resolve, reject) {
        var xhr = new XMLHttpRequest();
        xhr.open('POST', url, true);
        xhr.setRequestHeader('Content-Type', 'application/json');
        xhr.onload = function() {
          if (xhr.status >= 200 && xhr.status < 300) {
            resolve(xhr.responseText);
          } else {
            reject(xhr.statusText);
          }
        };
        xhr.onerror = function() {
          reject(xhr.statusText);
        };
        xhr.send(JSON.stringify(data));
      });
    },
    
    delete: function(url, data) {
      return new Promise(function(resolve, reject) {
        var xhr = new XMLHttpRequest();
        xhr.open('DELETE', url, true);
        xhr.onload = function() {
          if (xhr.status >= 200 && xhr.status < 300) {
            resolve(xhr.responseText);
          } else {
            reject(xhr.statusText);
          }
        };
        xhr.onerror = function() {
          reject(xhr.statusText);
        };
        xhr.send(JSON.stringify(data));
      });
    },
    
    update: function(url, data) {
      return new Promise(function(resolve, reject) {
        var xhr = new XMLHttpRequest();
        xhr.open('PUT', url, true);
        xhr.setRequestHeader('Content-Type', 'application/json');
        xhr.onload = function() {
          if (xhr.status >= 200 && xhr.status < 300) {
            resolve(xhr.responseText);
          } else {
            reject(xhr.statusText);
          }
        };
        xhr.onerror = function() {
          reject(xhr.statusText);
        };
        xhr.send(JSON.stringify(data));
      });
    }
  };
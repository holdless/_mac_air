var app = require('http').createServer(handler)
var io = require('socket.io')(app);
var fs = require('fs');
var url = require('url');


app.listen(8088);

function handler (req, res) {
    // Parse the request containing file name
    var fileName = url.parse(req.url).pathname;
	// Parse the request whole message
    var msg = req.url;
	console.log(msg);
    console.log(fileName);

  fs.readFile(__dirname + '/io_client.html',
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading index.html');
    }

    res.writeHead(200);
    res.end(data);
  });
}

io.on('connection', function (socket) {
    console.log('a user connected...');
    socket.on('disconnect', function() {
        console.log('user disconnected!')
    });
    socket.emit('news', { hello: 'world' });
    socket.on('my other event', function (data) {
    console.log(data);
  });
});
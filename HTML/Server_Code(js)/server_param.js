var http = require('http')
var url = require('url')

http.createServer(function (req,res){
	res.writeHead(200, {'Content-Type':'text/html'})
	var urlParsed = url.parse(req.url, true)
	var query = urlParsed.query
	res.end(`
		<!DOCTYPE html>
		<html>
			<head>
			<title>Home</title>
				<style>
					div{
						background-color: red;
					}
				</style>
			</head>
			<body>
				<h1>This is a header for server_param...</h1>
				<p>This is a paragraph for server_param...</p>
				<div>This is a division for sever_param...</div>
				<p>req = ${req.url}</p>
				<p>sum = ${parseInt(query.x)+parseInt(query.y)}</p>
			</body>
		</html>
	`)
	}).listen(8080, function(){
		console.log("Server is running...")
	})
var http = require('http')
http.createServer(function (req,res){
	res.writeHead(200, {'Content-Type':'text/html'})
	if (req.url.includes("home")){
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
					<h1>This is a header for home...</h1>
					<p>This is a paragraph for home...</p>
					<div>This is a division for home...</div>
					<p>req = ${req.url}</p>
				</body>
			</html>
		`)
	} else if (req.url.includes("about")){
		res.end(`
		<!DOCTYPE html>
			<html>
				<head>
				<title>About</title>
					<style>
						div{
							background-color: yellow;
						}
					</style>
				</head>
				<body>
					<h1>This is a header for about...</h1>
					<p>This is a paragraph for about...</p>
					<div>This is a division for about...</div>
					<p>req = ${req.url}</p>
				</body>
			</html>
		`)
	} else {
		res.end(`
		<!DOCTYPE html>
		<html>
			<head>
				<title>Invalid Page</title>
			</head>
			<body>
				<h1>This is an invalid page...</h1>
			</body>
		</html>
		`)
	}
	}).listen(8080, function(){
		console.log("Server is running...")
	})
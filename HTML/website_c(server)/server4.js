var http = require("http")
var url = require("url")
var qs = require("querystring")

http.createServer(function (req,res){
	
	if (req.url.includes("action")){
		var body = ""
		req.on("data",function (chunk){
			body += chunk
		})
		req.on("end",function (){
			res.writeHead(200,{"Content-Type":"text/html"})
			var query = qs.parse(body)
			res.end(`
				<!DOCTYPE html>
				<html>
					<head>
						<title>Submission Successful</title>
						<style>
							body{
								background-color: lightgray;
							}
						</style>
					</head>
					<body>
						<h1>Form Submitted Successfully</h1>
						<p>Your username ${query.name} has been received.</p>
						<a href="form">Go back</a>
					</body>
				</html>
			`)
		})
	} else {
		res.writeHead(200,{"Content-Type":"text/html"})
		res.end(`
			<!DOCTYPE html>
			<html>
				<head>
					<title>Server 4</title>
					<style>
						body{
							background-color: lightgray;
						}
						input{
							margin: 5px;
						}
					</style>
				</head>
				<body>
					<form method="POST" action="action">
						Name:<input type="text" name="name"><br>
						Password: <input type="password" name="password"><br>
						<input type="submit">
					</form>
				</body>
			</html>
		`)
	}
}).listen(8080, function(){
	console.log("Server is running...")
})
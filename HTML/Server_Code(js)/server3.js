var http = require('http')
var url = require('url')

http.createServer(function (req,res){
	res.writeHead(200, {'Content-Type':'text/html'})
	var urlParsed = url.parse(req.url, true)
	var query = urlParsed.query
	
	if (req.url.includes("form")){
		
		res.end(`
			<!DOCTYPE html>
			<html>
				<form action="action.html">
					Name: <input type="text"><br>
					Age: <input type="text"><br>
				<input type="button" value="Submit">
				</form>	
			</html>
		`)
	} else if (req.url.includes("action")){
		var urlParsed = url.parse(req.url, true)
		var query = urlParsed.query
		users.push(query.firstname)
		res.end(`
			<!DOCTYPE html>
			<html>
				<body>
					<h1>Welcome ${query.firstname}!</h1>
					<p>Users: ${users.join(" ")}<p>
				</body>
			</html>
	`)
	}
	
}).listen(8080, function(){
	console.log("Server is running...")
})
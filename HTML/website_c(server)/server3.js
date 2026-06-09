var http = require('http')
var url = require('url')
var users = []

function getUsers(){
	var userList = '<ul>'
	for(var i=0;i<users.length;i++){
		userList += '<li>' + users[i].name + '	' + users[i].email + '</li>'
	}
	userList += '</ul>'
	return userList
}
http.createServer(function (req,res){
	res.writeHead(200, {'Content-Type':'text/html'})
	var urlParsed = url.parse(req.url, true)
	var query = urlParsed.query
	
	
	
	if (req.url.includes("action")){
		var urlParsed = url.parse(req.url, true)
		var query = urlParsed.query
		users.push({'name':query.name, 'email':query.email})
		res.end(`
			<!DOCTYPE html>
			<html>
				<head>
					<title>Submission Recieved</title>
					<script>
						var list = document.getElementById("list")
						for (var i = 0; i < ${users.length}; i++){
							list.innerHTML = "<li>${users}[i]</li>"
						}
					</script>
				</head>
				<body style="background-color: lightgray">
					<h1>Submission Received</h1>
					<h2>Name: ${query.name}</h2>
					<h2>Email: ${query.email}</h2>
					<br>
					<h1>All Submissions:</h1>
					${getUsers()}
					<a href="form">Go back</a>
				</body>
			</html>
	`)
	} else {
		res.end(`
		<!DOCTYPE html>
		<html>
			<head>
				<title>Server 3</title>
				<style>
					input{
						margin: 5px;
					}
				</style>
			</head>
			<body style="background-color: lightgray">
				<form action="action">
					Name: <input type="text" name="name"><br>
					Email: <input type="text" name="email"><br>
					<input type="submit" value="Submit">
				</form>
			</body>
		</html>
		`)
	}
	
}).listen(8080, function(){
	console.log("Server is running...")
})
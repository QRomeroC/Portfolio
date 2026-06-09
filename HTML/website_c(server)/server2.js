var http = require('http')
var url = require('url')

http.createServer(function (req,res){
	res.writeHead(200, {'Content-Type':'text/html'})
	var urlParsed = url.parse(req.url, true)
	var query = urlParsed.query
	var num1 = parseInt(query.num1)
	var num2 = parseInt(query.num2)
	var op = query.operation
	var result = null
	if (op == "add"){
		result = num1 + num2
	} else if (op == "subtract"){
		result = num1 - num2
	} else if (op == "multiply"){
		result = num1 * num2
	} else if (op == "divide"){
		result = num1/num2
	} 
	if (!result){
		res.end(`
			<!DOCTYPE html>
			<html>
				<head>
					<title>Invalid submission</title>
				</head>
				<body style="background-color: lightgray">
					<p>Invalid operation or operation unsupported</p>
				</body>
			</html
		`)
	} else {
		res.end(`
			<!DOCTYPE html>
			<html>
				<head>
					<title>Calculation Successful</title>
				</head>
				<body style="background-color: lightgray">
					<p>The result of ${query.num1} and ${query.num2} under ${query.operation} is:</p><br>
					<p>${result}</p>
				</body>
			</html>
		`)
	}
}).listen(8080, function(){
		console.log("Server is running...")
})
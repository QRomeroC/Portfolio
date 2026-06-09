var http = require('http')

function handleRequest(req, res){
	res.writeHead(200,{'Content-Type':'text/html'})
	res.end(`
	<!DOCTYPE html>
	<html>
		<head>
			<title>Server 1</title>
			<style>
				body{
					background-color: lightgray;
				}
				table{
					width: 100%;
					border-collapse: collapse;
					text-align: center;
					margin-bottom: 5px;
				}
				table, th, td{
					border: 1px solid black;
				}
				a{
					font-size: 18px;
				}
			</style>
		</head>
		<body>
			<h1>Welcome to My Web Page</h1>
			<p>This is a simple page to demonstrate HTML tags</p>
			<ul>
				<li>First item</li>
				<li>Second item</li>
				<li>Third item</li>
			</ul>
			<table>
				<th>Header 1</th>
				<th>Header 2</th>
				<th>Header 3</th>
				<tr>
					<td>Row 1, Col 1</td>
					<td>Row 1, Col 2</td>
					<td>Row 1, Col 3</td>
				</tr>
				<tr>
					<td>Row 2, Col 1</td>
					<td>Row 2, Col 2</td>
					<td>Row 2, Col 3</td>
				</tr>
			</table>	
			<a href="">Click here to visit a link</a>	
		</body>
	</html>
	`)
}

var server = http.createServer(handleRequest)
server.listen(8080)
console.log('Server running ...')
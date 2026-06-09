var express = require('express')
var path = require('path')
var app = express()
var port = 8080

var public_html = path.join(__dirname, 'public_html')

app.get('/',(req,res) => res.sendFile(path.join(public_html,'form_express.html')))

app.get('/action',function(req, res){ 
	var op = req.params.operation
	var n1 = parseFloat(req.params.num1)
	var n2 = parseFloat(req.params.num2)
	
	if (op == "add"){
		res.send(n1+n2)
	} else if (op == "subtraction"){
		res.send(n1-n2)
	} else if (op == "multiply"){
		res.send(n1*n2)
	} else {
		res.send(n1/n2)
	}
}) 

app.listen(port,() => 
	console.log(`Server is running...\n listening at: http://localhost:${port}`))
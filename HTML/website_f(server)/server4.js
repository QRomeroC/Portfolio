var express = require('express')
var path = require('path')
var fs = require('fs')

var app = express()

var dir = __dirname

app.get('/',function(req,res){
	res.sendFile(path.join(dir,'form_post.html'))
})

app.get('/home',function(req,res){
	res.sendFile(path.join(dir,'form_post.html'))
})

app.get('/form',function(req,res){
	res.sendFile(path.join(dir,'form_post.html'))
})

app.post('/submit', express.urlencoded(),function(req,res){
	var query = req.body
	var name = query.name
	var age = query.age
	var email = query.email
	var sub = query.subscribe
	
	if (sub != 'on'){
		sub = "No"
	} else {
		sub = "Yes"
	}
	
	res.send(`
	<!DOCTYPE html>
	<html>
		<head>
			<title>Submission Successful</title>
			<style>
				body{
					background-color: lightgray;
				}
				h1{
					font-size: 26px;
				}
				h2{
					font-size: 24px;
				}
				p{
					font-size: 18px;
					margin-bottom: 5px;
				}
			</style>
		</head>
		<body>
			<h1>Form Submission Successful</h1>
			<p>Thank you for your submission!</p>
			<h2>Entered Data:</h2>
			<p>Name: ${name}</p>
			<p>Age: ${age}</p>
			<p>Email: ${email}</p>
			<p>Subscribe: ${sub}</p>
		</body>
	</html>
	`)
})

app.listen(8080,function(){
	console.log('Server is running...')
})
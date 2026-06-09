var express = require('express')
var fs = require('fs')
var path = require('path')
var app = express()


app.get('/',function(req,res){
	res.sendFile(path.join(__dirname, 'sampleHome.html'))
})
app.get('/home',function(req,res){
	res.sendFile(path.join(__dirname, 'sampleHome.html'))
})
	

app.get('/about',function(req,res){
	res.sendFile(path.join(__dirname, 'sampleAbout.html'))
})


app.listen(8080, function(){
	console.log('Server is running...')
})
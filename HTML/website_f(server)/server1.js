var express = require('express')
var path = require('path')
var fs = require('fs')

var app = express()

var dir = __dirname

app.get('/',function(req,res){
	res.sendFile(path.join(dir,'index.html'))
})

app.get('/home',function(req,res){
	res.sendFile(path.join(dir,'index.html'))
})

app.get('/index',function(req,res){
	res.sendFile(path.join(dir,'index.html'))
})

app.get('/about',function(req,res){
	res.sendFile(path.join(dir,'about.html'))
})

app.listen(8080,function(){
	console.log('Server is running...')
})
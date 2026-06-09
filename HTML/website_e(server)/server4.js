var http = require('http')
var qs = require('querystring')
var url = require('url')
var fs = require('fs')

var user_data = []
var form_file = 'user_data.json'

function writeFormData(name,age,email,sub){
	var data = name + "," + age + "," + email + "," + sub + "\n"
	try{
		fs.appendFileSync(form_file,data,{'encoding':'utf8'})
	}catch(err){
		console.log(err)
	}
}

http.createServer(function (req, res){
    res.writeHead(200, {'Content-Type':'text/html'})
    // check whether it is an admin and update
    var parsedUrl = url.parse(req.url, true)
    var query = parsedUrl.query
    if(req.url.includes('home') || req.url.includes('form')|| !req.url.includes('submit')){
        var htmlContent = ''
        try{
            htmlContent = fs.readFileSync('form_post.html', {'encoding':'utf8'})    
        }catch(err){
            console.log(err)
        }
        res.end(htmlContent)
    }else if(req.url.includes('submit')){
		var body = ''
		req.on('data',function(chunk){
			body += chunk
		})
		req.on('end',function (){
			var query = qs.parse(body)
			var name = query.name
			var age = query.age
			var email = query.email
			var sub = query.subscribe
			if (sub != "on"){
				sub = "No"
			} else {
				sub = "Yes"
			}
			user_data.push({'name':name,'age':age,'email':email,'subscribe':sub})
			writeFormData(name,age,email,sub)
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
    }
}).listen(8080, function(){
    console.log("Server is Running...")
})
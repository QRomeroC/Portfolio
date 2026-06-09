var http = require('http')
var qs = require('querystring')
var url = require('url')
var fs = require('fs')

http.createServer(function (req, res){
    res.writeHead(200, {'Content-Type':'text/html'})
    // check whether it is an admin and update
    var parsedUrl = url.parse(req.url, true)
    var query = parsedUrl.query
    if(req.url.includes('home') || req.url.includes('product')|| !req.url.includes('contact')){
        var htmlContent = ''
        try{
            htmlContent = fs.readFileSync('product.html', {'encoding':'utf8'})    
        }catch(err){
            console.log(err)
        }
        res.end(htmlContent)
    }else if(req.url.includes('contact')){
        var htmlContent = ''
        try{
            htmlContent = fs.readFileSync('contact.html', {'encoding':'utf8'})    
        }catch(err){
            console.log(err)
        }
        res.end(htmlContent)
    }
}).listen(8080, function(){
    console.log("Server is Running...")
})
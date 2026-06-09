var http = require('http')
var qs = require('querystring')
var url = require('url')

var users = []
var sessionList = []

function checkLogin(username, usertype){
    for(var i=0;i<users.length;i++){
        var cur = users[i]
        if((cur.username==username)&&(cur.usertype==usertype)){
            return true
        }
    }
    return false
}

function checkProducer(username){
    for(var i=0;i<users.length;i++){
        var cur = users[i]
        if((cur.username==username)&&(cur.usertype=='producer')){
            return true
        }
    }
    return false
	
function checkSession(username){
	for (var i = 0; i < sessionList.length; i++){
		var cur = sessionList[i]
		if(cur.username == username){
			return true
		}
	}
}

}

var head_str = `
    <head>
        <script>
            function storeUsername(){
                var username = document.getElementById('username').value
				var radios = document.getElementById('usertype')
				var role = ''
				for (var i = 0; i < radios.length; i++){
					if(radios[i].checked){
						role = radio[i].value
					}
					break
				}
                window.localStorage.setItem('username', username)
				window.localStorage.setItem('usertype',role)
            }
            function updateUrls(){
                var username = window.localStorage.getItem('username')
                if(username!=null){
                    var links = document.getElementsByTagName('a')
                    for(var i=0;i<links.length;i++){
                        links[i].href += '?username=' + username
                    }
                }
            }
            function sendReq(url){
                var username = window.localStorage.getItem('username')
				var usertype = window.localStorage.getItem('usertype')
                var newUrl = url
                if(username!=null){
                    newUrl += '?username=' + username + "&" + "usertype=' + usertype
                }
                fetch(newUrl)
                .then(function (res){
                    return res.text()
                })
                .then(function (text){
                    document.open()
                    document.write(text)
                    document.close()
                })
                .catch(function (err){
                    console.log(err)
                })
            }
        </script>
    </head>
`

var nav_consumer_str = `
<nav>
    <a onclick="sendReq('home')">Home</a>
    <a onclick="sendReq('login')">Login</a>
</nav>
`

var nav_producer_str = `
<nav>
    <a onclick="sendReq('home')">Home</a>
    <a onclick="sendReq('login')">Login</a>
</nav>
`

http.createServer(function (req, res){
    res.writeHead(200, {'Content-Type':'text/html'})
    var nav_str = nav_consumer_str
    // check whether it is an admin and update
    var parsedUrl = url.parse(req.url, true)
    var query = parsedUrl.query
	var role = ""
	if (query.usertype != "" && query.usertype != null){
		role = query.usertype
		console.log(role)
	}
    if(checkProducer(query.username) && checkSession(query.username)){
        nav_str = nav_producer_str_str
    }
    if(req.url.includes('home') || !req.url.includes('login')){
        res.end(`
<!DOCTYPE html>
<html>
    ${head_str}
    <body>
        ${nav_str}
        <div>
            <p>Welcome ${role}!</p>
        </div>
    </body>
</html>
            `)
    } else if(req.url.includes('login')) {
        res.end(`
<!DOCTYPE html>
<html>
    ${head_str}
    <body>
        ${nav_str}
        <div>
            <form action="lgn_action" method="post">
                Username: <input type="text" name="username" id="username"><br>
                User type: <input type="radio" name="usertype" value="consumer" checked>Consumer
				<input type="radio" name="usertype" value="producer">Producer<br>
                <input type="submit" onclick="storeUsername()">
            </form>
        </div>
    </body>
</html>
            `)
    }
    else if(req.url.includes('lgn_action')){
        var body = ''
        req.on('data', function (chunk){
            body += chunk
        })
        req.on('end', function (){
            var query = qs.parse(body)
            // check login info
            var msg = ''
            if(checkLogin(query.username, query.usertype)){
                // login successful
                msg = 'Successful login!'
				sessionList.push({'username': query.username})
            }
            else{
                // info is not matching
                msg = 'Username or role is incorrect!'
            }
            res.end(`
<!DOCTYPE html>
<html>
    ${head_str}
    <body>
        ${nav_str}
        <div>
            ${msg}
        </div>
    </body>
</html>
                `)
        })

    }
}).listen(8080, function(){
	console.log("Server is Running...")	
})
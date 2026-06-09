var http = require('http')
var qs = require('querystring')
var url = require('url')

var users = []
var session = []

function checkLogin(username, password){
    for(var i=0;i<users.length;i++){
        var cur = users[i]
        if((cur.username==username)&&(cur.password==password)){
            return true
        }
    }
    return false
}

function checkAdmin(username){
    for(var i=0;i<users.length;i++){
        var cur = users[i]
        if((cur.username==username)&&(cur.usertype=='admin')){
            return true
        }
    }
    return false

}

function getUser(username){
	for(var i = 0; i < users.length; i++){
		var cur = users[i]
		if (cur.username == username){
			return cur
		}
	}
	return null
}

function generateToken(){
	var token = Math.random().toString(36).slice(2) + Math.random().toString(36).slice(2)
	return token
}

function addSession(username, token){
	session.push({'username':username,'token':token})
}

function checkSession(username, token){
	for (var i = 0; i < session.length; i++){
		var cur = session[i]
		if (cur.username == username && cur.token == token){
			return true
		}
	}
	return false
}

var head_str = `
    <head>
        <script>
            function storeUsername(){
                var username = document.getElementById('username').value
                window.localStorage.setItem('username', username)
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
				var token = window.localStorage.getItem('token')
                var userObj = {}
                if(username!=null){
                    userObj = {'username':username, 'token':token}
                }
                fetch(url, {
                    method:'POST',
                    headers:{
                        'Content-Type':'application/json'
                    },
                    body: JSON.stringify(userObj)
                })
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

var nav_user_str = `
<nav>
    <a onclick="sendReq('home')">Home</a>
    <a onclick="sendReq('create_user')">Create user</a>
    <a onclick="sendReq('login')">Login</a>
</nav>
`

var nav_admin_str = `
<nav>
    <a onclick="sendReq('home')">Home</a>
    <a onclick="sendReq('create_user')">Create user</a>
    <a onclick="sendReq('login')">Login</a>
    <a onclick="sendReq('manage')">Manage</a>
</nav>
`

http.createServer(function (req, res){
    res.writeHead(200, {'Content-Type':'text/html'})
    var nav_str = nav_user_str
    
    if(req.url.includes('home') || (!req.url.includes('create_user') && 
	   !req.url.includes('create_action') && !req.url.includes('login') &&
	   !req.url.includes('lgn_action'))
	   ){
        var body = ''
        req.on('data', function (chunk){
            body += chunk
        })
        req.on('end', function (){
            if(body==''){
                body = '{}'
            }
            var query = JSON.parse(body)
            if(checkAdmin(query.username)){
                nav_str = nav_admin_str
            }
            res.end(`
<!DOCTYPE html>
<html>
    ${head_str}
    <body>
        ${nav_str}
        <div>
            <p>This is the homepage.</p>
        </div>
    </body>
</html>
            `)

        })
        
    }
    else if(req.url.includes('create_user')){
        res.end(`
<!DOCTYPE html>
<html>
    ${head_str}
    <body>
        ${nav_str}
        <div>
            <form action="create_action" method="post">
                Username: <input type="text" name="username"><br>
                Password: <input type="password" name="password"><br>
                User type: <input type="radio" name="usertype" value="user" checked>User
                <input type="radio" name="usertype" value="admin">Admin<br>
                <input type="submit">
            </form>
        </div>
    </body>
</html>
            `)
    }
    else if(req.url.includes('create_action')){
        var body = ''
        req.on('data', function (chunk){
            body += chunk
        })
        req.on('end', function (){
            var query = qs.parse(body)
            users.push({'username':query.username, 'password':query.password, 'usertype':query.usertype})
            console.log('Number of users', users.length)
            res.end(`
<!DOCTYPE html>
<html>
    ${head_str}
    <body>
        ${nav_str}
        <div>
            Created user successfully!
        </div>
    </body>
</html>
                `)
        })

    }
    else if(req.url.includes('login')){
        res.end(`
<!DOCTYPE html>
<html>
    ${head_str}
    <body>
        ${nav_str}
        <div>
            <form action="lgn_action" method="post">
                Username: <input type="text" name="username" id="username"><br>
                Password: <input type="password" name="password"><br>
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
            if(checkLogin(query.username, query.password)){
				var user = getUser(query.username)
				var token = generateToken()
				addSession(user.username,token)
                if(user.usertype == 'admin'){
					msg = 'Welcome Admin ' + user.username
				} else {
					msg = 'Welcome User ' + user.username
				}
            }
            else{
                // info is not matching
                msg = 'Username or password is incorrect!'
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
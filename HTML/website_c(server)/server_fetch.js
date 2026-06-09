var http = require('http')
var qs = require('querystring')
var url = require('url')

var users = []
var sessionList = []

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

function checkSession(username){
	for (var i = 0; i < sessionList.length; i++){
		var cur = sessionList[i]
		if(cur.username == username){
			return true
		}
	}
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
                var newUrl = url
                if(username!=null){
                    newUrl += '?username=' + username
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

var nav_student_str = `
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
    var nav_str = nav_student_str
    // check whether it is an admin and update
    var parsedUrl = url.parse(req.url, true)
    var query = parsedUrl.query
    if(checkAdmin(query.username) && checkSession(query.username)){
        nav_str = nav_admin_str
    }
    if(req.url.includes('home')){
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
                User type: <input type="radio" name="usertype" value="student" checked>Student
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
                // login successful
                msg = 'Successful login!'
				sessionList.push({'username': query.username})
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
}).listen(8080)
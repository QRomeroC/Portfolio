var http = require('http')
var qs = require('querystring')
var url = require('url')

var users = []
var sessionUsers = []

function checkInstructor(username){
    for(var i=0;i<users.length;i++){
        var cur = users[i]
        if((cur.username==username)&&(cur.usertype=='instructor')){
            return true
        }
    }
    return false
}

function checkSession(username){
	for(var i = 0; i < sessionUsers.length; i++){
		var cur = sessionUsers[i]
		if(cur.username == username){
			return true
		}
	}
	return false
}

var studentHome =`
	<head>
		<title>Welcome</title>
	</head>
	<body>
		<nav>
			<a href="home">Home</a>
			<a href="login">Login</a>
		</nav>
		<p>Welcome Student</p>
	</body>
`

var instructorHome =`
	<head>
		<title>Welcome</title>
	</head>
	<body>
		<nav>
			<a href="home">Home</a>
			<a href="login">Login</a>
		</nav>
		<p>Welcome Instructor</p>
	</body>
`
var defaultHome =`
	<head>
		<title>Welcome</title>
	</head>
	<body>
		<nav>
			<a href="home">Home</a>
			<a href="login">Login</a>
		</nav>
		<p>Welcome</p>
	</body>
`

var defaultNav =`
	<nav>
		<a href="home">Home</a>
		<a href="login">Login</a>
	</nav>
`

http.createServer(function (req,res){
	res.writeHead(200,{'Content-Type':'text/html'})
	var parsedURL = url.parse(req.url,true)
	var query = parsedURL.query
	
	var homePage = defaultHome
	if (checkInstructor(query.username)){
		homePage = instructorHome
	} else if (!checkInstructor(query.username) && checkSession(query.username)){
		homePage = studentHome
	}
	
	if(req.url.includes('home') || !req.url.includes('login')){
		res.end(`
		<!DOCTYPE html>
		<html>
			${homePage}
		</html>
		`)
	} else if (req.url.includes('login')){
		res.end(`
		<!DOCTYPE html>
		<html>
			<head>
				<title>Login</title>
				<script>
					function storeUser(){
						var username = document.getElementById('username')
						var usertype = document.getElementById('usertype')
						window.localStorage.setItem('username',username)
						window.localStorage('usertype',usertype)
						console.log(window.localStorage.getItem('username'))
						console.log(window.localStrorage.getItem('usertype'))
					}
				</script>
			</head>
			<body>
				${defaultNav}
				<div>
					<form action="lgn_action">
						Username: <input type="text" name="username" id="username"><br>
						User type: <input type="radio" name="usertype" value="student" checked>Student
						<input type="radio" name="usertype" value="instructor">Instructor<br>
						<input type="submit" onclick="storeUser()">
					</form>
				</div>
			</body>
		</html>
	`)
	} else if (req.url.includes('lgn_action')){
		var body = ''
		req.on('data',function(chunk){
			body += chunk
		})
		req.on('end',function(){
			var query = qs.parse(body)
			var role = query.usertype
			console.log(role)
			if(role == 'instructor'){
				res.end(`
				<!DOCTYPE html>
				<html>
					<head>
						<Title>Logged In</title>
					</head>
					<body>
						${defaultNav}
						<div>
							<p>Welcome ${role}</p>
						</div>
					</body>
				</html>
				`)
			} else {
				res.end(`
				<!DOCTYPE html>
				<html>
					<head>
						<Title>Logged In</title>
					</head>
					<body>
						${defaultNav}
						<div>
							<p>Welcome ${role}</p>
						</div>
					</body>
				</html>
			`)
			}
		})
	}	
}).listen(8080,function(){
	console.log("Server is Running...")
})
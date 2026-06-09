var fs = require('fs')

var myObj = {
	'k3':'v3'
}

fs.appendFile('my_file.json', JSON.stringify(myObj), {'encoding':'utf8'}, function (err){
	if (!err){
		console.log("append succesfull")
	} else {
		console.log(err)
	}
})
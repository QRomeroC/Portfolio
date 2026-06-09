var fs = require('fs')

fs.readFile('my_file.json',{'encoding':'utf8'}, function(err, data) {
	if(!err){
		console.log(data)
	} else {
		console.log(err)
	}
})
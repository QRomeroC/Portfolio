var fs = require('fs')

try {
	var data = fs.readFileSync('my_file.txt',{'encoding':'utf8'})
	console.log(data)
} catch(err){
	console.log(err)
}
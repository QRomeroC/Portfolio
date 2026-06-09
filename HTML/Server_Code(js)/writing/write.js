var fs = require('fs')

var myObj = {
	'k1':'v1',
	'k2':'v2'
}

fs.writeFile('my_file.json', JSON.stringify(myObj),{'encoding':'utf8'}, function(err){
		if(!err){
			console.log('write successful')
		} else {
			console.log(err)
		}
})
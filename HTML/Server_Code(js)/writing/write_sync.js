var fs = require('fs')

var my_text = `Apple
Banana`

var myList = ['abc','def']
var my_str = ''
for (var i = 0; i < myList.length; i++){
	my_str += myList[i] + "\n"
}

try{
	fs.writeFileSync('my_file.txt',my_str,{'encoding':'utf8'})
	console.log('write successful')
} catch(err){
	console.log(err)
}
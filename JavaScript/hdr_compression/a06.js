/*
  Template for CS 433 544 HW6
  
  Base is written by: Amir Mohammad Esmaieeli Sikaroudi
  Note: You are free to change the template as much as you want and do it in your way. The sliders are required to work and the output needs to be downloaded.
  Date: April 29, 2026
  
  Aaron C. Posey
  Cesar D. Quihuis-Romero

  Date: Apr 30, 2026
 */


//access DOM elements we'll use
var input = document.getElementById("load_image");
var output = document.getElementById("save_image");
var outputP3 = document.getElementById("save_imageP3");
var canvas = document.getElementById('canvas');
var canvasP3 = document.getElementById('canvasP3');
var gammaSlider = document.getElementById('gammaVolume');
var gammaSliderP3 = document.getElementById('gammaVolumeP3');
var ctx = canvas.getContext('2d');
var ctxP3 = canvasP3.getContext('2d');

var hdr_data;//HDR data read from file
var width;
var height;
var LDRData;//Final image in RGB [0,255] ready to download/save
var LDRDataP3;//Final image in RGB [0,255] ready to download/save
var luminance;//"L" that was used in Overleaf


//Connect event listeners
input.addEventListener("change", upload);
output.addEventListener("click", download);
outputP3.addEventListener("click", downloadP3);

gammaSlider.addEventListener("change", function(evt) {
    adjustGamma(gammaSlider.value);
  },false);
gammaSliderP3.addEventListener("change", function(evt) {
    adjustGammaP3(gammaSliderP3.value);
  },false);


//Function to process upload
function upload() {
  if (input.files.length > 0) {
    var file = input.files[0];
    //console.log("You chose", file.name);
    //if (file.type) console.log("It has type", file.type);

    var fReader = new FileReader();
    //for HDR, we'll readAsArrayBuffer
    fReader.readAsArrayBuffer(file)

    fReader.onload = function(e) {
      //if successful, file data has the contents of the uploaded file
      var file_data = fReader.result;

      //calling parseHdr from hdr.js will process the data
      hdr_data = parseHdr(file_data);
	  adjustGamma(gammaSlider.value);
	  adjustGammaP3(gammaSliderP3.value);
	  
	  /*
		below was for paliminary data scouting and testing
	  */
      //hdr_data.shape[0] has the width
      //hdr_data.shape[1] has the height
      //hdr_data.data is an array of size 4xWxH of floats for the pixels
	  /*
	  console.log("hdr_data.shape[0]: ", hdr_data.shape[0]);
	  console.log("hdr_data.shape[1]: ", hdr_data.shape[1]);
	  console.log("len(hdr_data.data: ", hdr_data.data.length);
	  
	  console.log("hdr_data.data[0]: ", hdr_data.data[0]);
	  console.log("hdr_data.data[1]: ", hdr_data.data[1]);
	  console.log("hdr_data.data[2]: ", hdr_data.data[2]);
	  console.log("hdr_data.data[3]: ", hdr_data.data[3]);
	  */
	  /*
	  console.log("hdr_data.data[0][1]: ", hdr_data.data[0][1]);
	  console.log("hdr_data.data[1][1]: ", hdr_data.data[1][1]);
	  console.log("hdr_data.data[2][1]: ", hdr_data.data[2][1]);
	  console.log("hdr_data.data[3][1]: ", hdr_data.data[3][1]);
	  
	  console.log("hdr_data.data[0][1][2]: ", hdr_data.data[0][1][2]);
	  console.log("hdr_data.data[1][1][2]: ", hdr_data.data[1][1][2]);
	  console.log("hdr_data.data[2][1][2]: ", hdr_data.data[2][1][2]);
	  console.log("hdr_data.data[3][1][2]: ", hdr_data.data[3][1][2]);
	  */
	  //console.log("hdr_data: ", hdr_data.data);
	  /*
	  console.log("hdr_data[4]: ", hdr_data.data[4]);
	  console.log("hdr_data[5]: ", hdr_data.data[5]);
	  console.log("hdr_data[6]: ", hdr_data.data[6]);
	  console.log("hdr_data[7]: ", hdr_data.data[7]);
	  
	  console.log("hdr_data[656]: ", hdr_data.data[656]);
	  console.log("hdr_data[657]: ", hdr_data.data[657]);
	  console.log("hdr_data[658]: ", hdr_data.data[658]);
	  console.log("hdr_data[659]: ", hdr_data.data[659]);
	  
	  console.log("hdr_data[660]: ", hdr_data.data[660]);
	  
	  console.log("hdr_data[999]: ", hdr_data.data[999]);
	  
	  console.log("hdr_data[2_623_999]: ", hdr_data.data[2623999]);
	  */
	  
	  //var gamma = Number(adjustGamma);
	  
	  //var cols = 4 * hdr_data.shape[0];
	  //var rows = hdr_data.data.length / cols;
	  //console.log("cols: ", cols);
	  //console.log("rows: ", rows);
	  
	  /*
	  for (var i = 0; i < rows - 4; i+=4){
		  for(var j = 0; j < cols; j++){
			var r = hdr_data.data[i];
			var g = hdr_data.data[i+1];
			var b = hdr_data.data[i+2];
			var a = hdr_data.data[i+3];
			console.log("r: %f, g: %f, b: %f, a: %d", r,g,b,a);
			var L =  1/61(20.0 * r + 40.0 * g + b);
			var LPrime = L ^ adjustGamma;
			var s = LPrime / L; 
			var rPrime = s * r;
			var gPrime = s * g;
			var bPrime = s * b;
			var R' = fClamp255(rPrime);
			var G' = fClamp255(gPrime);
			var B' = fClamp255(bPrime); //Aposey Note: fClamp255 needs to be created. Float to rgba and then clamp to 255
			
		  }
	  }
	  */
	  
	  //Call "adjustGamma" and "adjustGammaP3" to update the canvases after the file is loaded.
	  //Feel free to do it in your way.
    }
  }
}


//Adjust gamma for part 2 (slider's listener function)
function adjustGamma(gammaValue)
{
	//Update the value in the HTML view
	var gammaLabel = document.getElementById("gammaValueText");
	gammaLabel.innerHTML = gammaSlider.value;
	gammaSlider.label = "Gamma: "+gammaSlider.value;//refresh gamma text
	//TODO: follow the formulas for basic tone mapping then draw it on the first canvas
	//L(p_i,j) = 1/61(20.0R(p_i,j) + 40G(p_i,j) + B(p_i,j))
	//L'(p_i,j) = L(p_i,j)^gamma
	//s = L'/L 
	//R' = sR, G' = sG, B' = sB floatToRGBA and clamp
	width = hdr_data.shape[0];
    height = hdr_data.shape[1];
    canvas.width = width;
    canvas.height = height;

    var imageData = ctx.createImageData(width, height);
    var src = hdr_data.data;
    var dst = imageData.data;
	var gamma = Number(gammaSlider.value);
	
	for (var i = 0; i < src.length; i += 4){
		  var r = src[i];
		  var g = src[i + 1];
		  var b = src[i + 2];
		  var a = src[i + 3];
		  
		  var L = (1/61) * (20.0 * r + 40.0 * g + b);
		  
		  var rPrime = 0;
		  var gPrime = 0;
		  var bPrime = 0;
		  
		  if (L > 0){
			  var LPrime = Math.pow(L,gamma);
			  var s = LPrime / L;
			  
			  rPrime = s * r;
			  gPrime = s * g;
			  bPrime = s * b;
		  }
		  
		  dst[i] = fClamp255(rPrime * 255);
		  dst[i+1] = fClamp255(gPrime * 255);
		  dst[i+2] = fClamp255(bPrime * 255);
		  dst[i+3] = 255;
	  }
	  
	  ctx.putImageData(imageData,0,0);
	  LDRData = dst;
	
}

//Adjust gamma for part 3 (slider's listener function)
function adjustGammaP3(gammaValue)
{
	//Update the value in the HTML view
	var gammaLabel = document.getElementById("gammaValueTextP3");
	gammaLabel.innerHTML = gammaSliderP3.value;
	gammaSliderP3.label = "Gamma: "+gammaSliderP3.value;
	//TODO: follow the formulas for part 3 then draw it on the first canvas
	/*
		tent =     [[1,2,3,2,1],
				    [2,4,6,4,2],
				    [3,6,9,6,3],
				    [2,4,6,4,2],
				    [1,2,3,2,1]]
		
		cone = 	   [[0,0,1,0,0],
				    [0,2,2,2,0],
				    [1,2,5,2,1],
				    [0,2,2,2,0],
				    [0,0,1,0,0]]
				
		gaussian = [[1, 4, 7, 4,1],
					[4,16,28,16,4],
					[7,28,49,28,7],
					[4,16,28,16,4],
					[1, 4, 7, 4,1]]
	*/
	
	width = hdr_data.shape[0];
	height = hdr_data.shape[1];
	
	canvasP3.width = width;
	canvasP3.height = height;
	
	var src = hdr_data.data;
	var imageData = ctxP3.createImageData(width, height);
	var dst = imageData.data;
	
	var gamma = Number(gammaSliderP3.value);
	
	//all the arrrays specified in spec sheet
	var L = new Float32Array(width * height);
	var Q = new Float32Array(width * height);
	var B = new Float32Array(width * height);
	var H = new Float32Array(width * height);
	var QPrime = new Float32Array(width * height);
	var LPrime = new Float32Array(width * height);
	
	//guard agains log(0) case
	var epsilon = 0.000001;
	
	//create L and Q
	for (var y = 0; y < height; y++){
		for (var x = 0; x < width; x++){
			var pixIndex = y * width + x;
			var rgbaIndex = pixIndex * 4;
			
			var r = src[rgbaIndex];
			var g = src[rgbaIndex + 1];
			var b = src[rgbaIndex + 2];
			//same L as p2
			var currLum = (1/61)*(20.0*r + 40.0*g + b);
			//guard against lum == 0 and trying to take the log of zero
			if (currLum < epsilon){
				currLum = epsilon;
			}
			
			L[pixIndex] = currLum;
			Q[pixIndex] = Math.log10(currLum);
		}
	}
	
	//low pass B = Q convolved with g
	B = getLowPass(Q, width, height);
	//get high pass
	H = getHighPass(Q,B,width,height);
	
	//steps 4-6: QPrime, LPrime, and rescale
	for (var y = 0; y < height; y++){
		for (var x = 0; x < width; x++){
			var pixIndex = y * width + x;
			var rgbaIndex = pixIndex * 4;
			
			QPrime[pixIndex] = gamma * B[pixIndex] + H[pixIndex];
			
			LPrime[pixIndex] = Math.pow(10,QPrime[pixIndex]);
			
			var scale = LPrime[pixIndex] / L[pixIndex];
			
			var rOut = scale * Math.floor(255 * src[rgbaIndex]);
			var gOut = scale * Math.floor(255 * src[rgbaIndex + 1]);
			var bOut = scale * Math.floor(255 * src[rgbaIndex + 2]);
			
			dst[rgbaIndex] = fClamp255(rOut);
			dst[rgbaIndex + 1] = fClamp255(gOut);
			dst[rgbaIndex + 2] = fClamp255(bOut);
			dst[rgbaIndex + 3] = 255;
		}
	}
	
	ctxP3.putImageData(imageData, 0, 0);
	LDRDataP3 = dst;
	
}

//Low pass (B)
//TODO: finish this function for part3
//uses mean filter (box)
function getLowPass(Q, width, height)
{
	/*
		kernal = 1/25[[1,1,...],
				       .
					   .
					   .
					  [...,1]]
	*/
	var B = new Float32Array(width * height);
	
	var kernalSize = 5;
	var radius = Math.floor(kernalSize/2);
	
	for (var y = 0; y < height; y++){
		for (var x = 0; x < width; x++){
			var sum = 0;
			var count = 0;
			
			for (var kernalY = -radius; kernalY <= radius; kernalY++){
				for (var kernalX = - radius; kernalX <= radius; kernalX++){
					var sampleX = x + kernalX;
					var sampleY = y + kernalY;
					
					if (sampleX >= 0 && sampleX < width && sampleY >= 0 && sampleY < height){
						var sampleIndex = sampleY * width + sampleX;
						sum += Q[sampleIndex];
						count++;
					}
				}
			}
			
			var pixIndex = y * width + x;
			//mean
			B[pixIndex] = sum/count;
		}
	}
	
	return B;
}

//Low pass (H)
//TODO: finish this function for part3
function getHighPass(Q, B, width, height)
{
	var H = new Float32Array(width * height);
	
	for (var i = 0; i < width * height; i++){
		H[i] = Q[i] - B[i];
	}
	return H;
}

function fClamp255(num){
	var corrected = Math.floor(num);
	if (corrected < 0){
		corrected = 0;
	} else if (corrected > 255){
		corrected = 255;
	}
	return corrected;
}

//Uses a library to save a buffer to file "FileSaver.js" (part 2)
function download() {
	if (hdr_data !== undefined && LDRData !== undefined)
	{
		var testBuffer=convertToPPM(hdr_data,LDRData);
		var blob = new Blob([testBuffer]);
		saveAs(blob, "output.ppm");
	}
}

//Uses a library to save a buffer to file "FileSaver.js" (part 3)
function downloadP3() {
	if (hdr_data !== undefined && LDRDataP3 !== undefined)
	{
		var testBuffer=convertToPPM(hdr_data,LDRDataP3);
		var blob = new Blob([testBuffer]);
		saveAs(blob, "outputP3.ppm");
	}
}

//Convert LDR (RGB in [0,255]) to PPM 
function convertToPPM(original,input)
{
	var convertedToPPM="P6";
	convertedToPPM+=(String.fromCharCode('10'));
	convertedToPPM+=(original.shape[0]);
	convertedToPPM+=(" ");
	convertedToPPM+=(original.shape[1]);
	convertedToPPM+=(String.fromCharCode('10'));
	convertedToPPM+=("255");
	convertedToPPM+=(String.fromCharCode('10'));
	var headerBuffer = new Uint8Array(convertedToPPM.length);
	for (var i=0, strLen=convertedToPPM.length; i < strLen; i++) {
		headerBuffer[i] = convertedToPPM.charCodeAt(i);
	}
	var pixelData=new Uint8Array(width*height*3);
	for(var i = 0; i < width*height; i++){
		pixelData[i*3]=input[i*4];
		pixelData[i*3+1]=input[i*4+1];
		pixelData[i*3+2]=input[i*4+2];
	}
	var finalBuffer = new Uint8Array(headerBuffer.length + pixelData.length);
	finalBuffer.set(headerBuffer);
	finalBuffer.set(pixelData, headerBuffer.length);
	convertedToPPM = new TextDecoder("ascii").decode(finalBuffer);
	return finalBuffer;
}
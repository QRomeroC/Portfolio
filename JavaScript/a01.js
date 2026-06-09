/*
  Basic File I/O for displaying
  Skeleton Author: Joshua A. Levine
  Modified by: Amir Mohammad Esmaieeli Sikaroudi
  Email: amesmaieeli@email.arizona.edu
*/


//access DOM elements we'll use
var input = document.getElementById("load_image");
var canvas = document.getElementById('canvas');
var ctx = canvas.getContext('2d');

// The width and height of the image
var width = 0;
var height = 0;
// The image data
var ppm_img_data;

//Function to process upload
var upload = function () {
    if (input.files.length > 0) {
        var file = input.files[0];
        console.log("You chose", file.name);
        if (file.type) console.log("It has type", file.type);
        var fReader = new FileReader();
        fReader.readAsBinaryString(file);

        fReader.onload = function(e) {
            //if successful, file data has the contents of the uploaded file
            var file_data = fReader.result;
            parsePPM(file_data);

            /*
            * TODO: ADD CODE HERE TO DO 2D TRANSFORMATION and ANIMATION
            * Modify any code if needed
            * Hint: Write a rotation method, and call WebGL APIs to reuse the method for animation
            */
	    
            // *** The code below is for the template to show you how to use matrices and update pixels on the canvas.
            // *** Modify/remove the following code and implement animation
	/*-------------------Start of my stuff-----------------------------------*/
			var angle = 0;
			/*
				Name: insideImage
				Purpose: helper: clamp and check if (x,y) are inside image
				Arguments: x - represents the x-coordinate
						   y - represents the y-coordinate
				return: true - when (x,y) are in bounds
					    false - o.w.
			*/
			function insideImage(x,y){
				return x>= 0 && x < width && y >= 0 && y < height;
			}
			/*
				Name: copyPixel
				Purpose: helper: copy a source pixel to target
				Arguments: srcImg - represents the image data from source
				           tgtImg - represents the image data for the target
						   sx,sy - represents the source (x,y) coordinate
						   tx,ty - represents the target (x,y) coordinate
				return: N/A - copies pixel data from source to target
				NOTE: imageData.data is flat array [+0 offset] = R, [+1] = G, [+2] = B, [+3] = Alpha
			*/
			function copyPixel(srcImg, tgtImg, sx, sy, tx, ty){
				var srcIndex = (sy * width + sx) * 4;
				var tgtIndex = (ty * width + tx) * 4;
				
				tgtImg.data[tgtIndex] = srcImg.data[srcIndex];
				tgtImg.data[tgtIndex + 1] = srcImg.data[srcIndex + 1];
				tgtImg.data[tgtIndex + 2] = srcImg.data[srcIndex + 2];
				tgtImg.data[tgtIndex + 3] = 255;
			}
			/*
				Name: drawRotatedFrame
				Purpose: draw one frame at a given angle 
				Arguments: deg - represents the degree of rotation
				Return: N/A - utilizes MathUtilities.js functions and nearest neighbor
				        sampling to rotate the target img and draw.
			*/
			function drawRotatedFrame(deg){
				//target
				var tgtImageData = ctx.createImageData(width,height)
				
				//rotate about center: M = T(c) * R(deg) * T(-c)
				var cx = width/2;
				var cy = height/2;
				//translate center, rotate, translate back
				var translation1 = GetTranslationMatrix(-cx,-cy);
				var rotation = GetRotationMatrix(deg);
				var translation2 = GetTranslationMatrix(cx,cy);
				//M and M^-1
				var M = MultiplyMatrixMatrix(translation2,MultiplyMatrixMatrix(rotation,translation1));
				var invM = InverseMatrix3x3(M);
				
				//nested for-loop - for each pixel in target, get its data from source
				for (var y = 0; y < height; y++){
					for (var x = 0; x < width; x++){
						//p in homogenous coordinates
						var p = [x,y,1]
						//source coordinates
						var src = MultiplyMatrixVector(invM,p);
						
						//nearest neighbor **SIGNIFICANT ALIASING**
						var sx = Math.floor(src[0]);
						var sy = Math.floor(src[1]);
						//check if (x,y) are valid else draw black rgb(0,0,0) opacity = 255
						if(insideImage(sx,sy)){
							copyPixel(ppm_img_data,tgtImageData,sx,sy,x,y);
						}else{
							var ti = (y * width + x) * 4;
							tgtImageData.data[ti] = 0;
							tgtImageData.data[ti + 1] = 0;
							tgtImageData.data[ti + 2] = 0;
							tgtImageData.data[ti + 3] = 255;
						}
					}
				}
				
				//clear and draw using showMatrix() [given]
				ctx.clearRect(0,0,canvas.width,canvas.height);
				//putimage === top-left, correct so top-left isn't center and avoid spliting image
				//*** ~line 171 in commeted out given section ***
				ctx.putImageData(tgtImageData,canvas.width/2 - width/2,canvas.height/2 - height/2);
				showMatrix(M);
			}
			
			/*
				Name: animate
				Purpose: animate roation of image
				Arguments: N/A 
				Return: N/A
			*/
			function animate(){
				angle += 1;
				angle = angle % 360; 
				
				drawRotatedFrame(angle)
				requestAnimationFrame(animate);
			}
			animate();
		};
	}
};
/*----------------- end of my stuff ------------------------------*/
			/*
			// Create a new image data object to hold the new image
			var newImageData = ctx.createImageData(width, height);
			var transMatrix = GetTranslationMatrix(0, height);// Translate image
			var scaleMatrix = GetScalingMatrix(1, -1);// Flip image y axis
			var matrix = MultiplyMatrixMatrix(transMatrix, scaleMatrix);// Mix the translation and scale matrices
            
            // Loop through all the pixels in the image and set its color
            for (var i = 0; i < ppm_img_data.data.length; i += 4) {

                // Get the pixel location in x and y with (0,0) being the top left of the image
                var pixel = [Math.floor(i / 4) % width, 
                             Math.floor(i / 4) / width, 1];
        
                // Get the location of the sample pixel
                var samplePixel = MultiplyMatrixVector(matrix, pixel);

                // Floor pixel to integer
                samplePixel[0] = Math.floor(samplePixel[0]);
                samplePixel[1] = Math.floor(samplePixel[1]);

                setPixelColor(newImageData, samplePixel, i);
            }

            // Draw the new image
            ctx.putImageData(newImageData, canvas.width/2 - width/2, canvas.height/2 - height/2);
	    
	    // Show matrix
            showMatrix(matrix);
        }
    }
}
*/
// Show transformation matrix on HTML
function showMatrix(matrix){
    for(let i=0;i<matrix.length;i++){
        for(let j=0;j<matrix[i].length;j++){
            matrix[i][j]=Math.floor((matrix[i][j]*100))/100;
        }
    }
    document.getElementById("row1").innerHTML = "row 1:[ " + matrix[0].toString().replaceAll(",",",\t") + " ]";
    document.getElementById("row2").innerHTML = "row 2:[ " + matrix[1].toString().replaceAll(",",",\t") + " ]";
    document.getElementById("row3").innerHTML = "row 3:[ " + matrix[2].toString().replaceAll(",",",\t") + " ]";
}

// Sets the color of a pixel in the new image data
function setPixelColor(newImageData, samplePixel, i){
    var offset = ((samplePixel[1] - 1) * width + samplePixel[0] - 1) * 4;

    // Set the new pixel color
    newImageData.data[i    ] = ppm_img_data.data[offset    ];
    newImageData.data[i + 1] = ppm_img_data.data[offset + 1];
    newImageData.data[i + 2] = ppm_img_data.data[offset + 2];
    newImageData.data[i + 3] = 255;
}

// Load PPM Image to Canvas
// Untouched from the original code
function parsePPM(file_data){
    /*
   * Extract header
   */
    var format = "";
    var max_v = 0;
    var lines = file_data.split(/#[^\n]*\s*|\s+/); // split text by whitespace or text following '#' ending with whitespace
    var counter = 0;
    // get attributes
    for(var i = 0; i < lines.length; i ++){
        if(lines[i].length == 0) {continue;} //in case, it gets nothing, just skip it
        if(counter == 0){
            format = lines[i];
        }else if(counter == 1){
            width = lines[i];
        }else if(counter == 2){
            height = lines[i];
        }else if(counter == 3){
            max_v = Number(lines[i]);
        }else if(counter > 3){
            break;
        }
        counter ++;
    }
    console.log("Format: " + format);
    console.log("Width: " + width);
    console.log("Height: " + height);
    console.log("Max Value: " + max_v);
    /*
     * Extract Pixel Data
     */
    var bytes = new Uint8Array(3 * width * height);  // i-th R pixel is at 3 * i; i-th G is at 3 * i + 1; etc.
    // i-th pixel is on Row i / width and on Column i % width
    // Raw data must be last 3 X W X H bytes of the image file
    var raw_data = file_data.substring(file_data.length - width * height * 3);
    for(var i = 0; i < width * height * 3; i ++){
        // convert raw data byte-by-byte
        bytes[i] = raw_data.charCodeAt(i);
    }
    // update width and height of canvas
    document.getElementById("canvas").setAttribute("width", window.innerWidth);
    document.getElementById("canvas").setAttribute("height", window.innerHeight);
    // create ImageData object
    var image_data = ctx.createImageData(width, height);
    // fill ImageData
    for(var i = 0; i < image_data.data.length; i+= 4){
        let pixel_pos = parseInt(i / 4);
        image_data.data[i + 0] = bytes[pixel_pos * 3 + 0]; // Red ~ i + 0
        image_data.data[i + 1] = bytes[pixel_pos * 3 + 1]; // Green ~ i + 1
        image_data.data[i + 2] = bytes[pixel_pos * 3 + 2]; // Blue ~ i + 2
        image_data.data[i + 3] = 255; // A channel is deafult to 255
    }
    ctx.putImageData(image_data, canvas.width/2 - width/2, canvas.height/2 - height/2);
    //ppm_img_data = ctx.getImageData(0, 0, canvas.width, canvas.height);   // This gives more than just the image I want??? I think it grabs white space from top left?
    ppm_img_data = image_data;
}

//Connect event listeners
input.addEventListener("change", upload);
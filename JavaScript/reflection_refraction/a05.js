/*
 This file is a template for a05 CS433/533
 
 Author: Amir Mohammad Esmaieeli Sikaroudi
 Email: amesmaieeli@email.arizona.edu
 Date: March, 2025
 
 Sources uses for this template:
 First Obj parser:
 https://webglfundamentals.org/
 Second Obj parser:
 https://github.com/WesUnwin/obj-file-parser
 The library for decoding PNG files is from:
 https://github.com/arian/pngjs
*/

var input = document.getElementById("load_scene");
input.addEventListener("change", readScene);
var dummy_canvas = document.getElementById('dummy_canvas');
var ctx = dummy_canvas.getContext('2d');

var renderingCanvas = document.querySelector("#canvas");
var gl = renderingCanvas.getContext("webgl2",{preserveDrawingBuffer: true});
var previousFrameTime = 0;
var modelRotationRadians = degToRad(0);

var modelMatrix;

var currentScene;//Current rendering scene

var doneLoading=false;//Checks if the scene is done loading to prevent renderer draw premuturly.
var doneProgramming=false;
var filesToRead=[];//List of files to be read
var poolImage;//The image contents are stored separately here
var posX;
var negX;
var posY;
var negY;
var posZ;
var negZ;
var scene;//The code can save multiple scenes but no HTML element is made to give user option of switching scenes without selecting file agail. By default the firt scene is shown and the other selected scenes are just stored.
var objParsed;

// Mirror camera position
var cameraPositionPrime;

var billboardProgram;
var backgroundProgram;

var camX = document.getElementById('camXID');//Slider for cam position
var camY = document.getElementById('camYID');//Slider for cam position
var camZ = document.getElementById('camZID');//Slider for cam position

var camTX = document.getElementById('camTXID');//Slider for cam position
var camTY = document.getElementById('camTYID');//Slider for cam position
var camTZ = document.getElementById('camTZID');//Slider for cam position

camX.addEventListener("input", function(evt) {
	if(doneLoading==true){
		currentScene.camera.position.x=Number(camX.value);
		var camXLabel = document.getElementById("camXLabelID");
		camXLabel.innerHTML = camX.value;
		camX.label = "Cam X: "+camX.value;//refresh camX text
	}
},false);
camY.addEventListener("input", function(evt) {
	if(doneLoading==true){
		currentScene.camera.position.y=Number(camY.value);
		var camYLabel = document.getElementById("camYLabelID");
		camYLabel.innerHTML = camY.value;
		camY.label = "Cam Y: "+camY.value;//refresh camY text
	}
},false);
camZ.addEventListener("input", function(evt) {
	if(doneLoading==true){
		currentScene.camera.position.z=Number(camZ.value);
		var camZLabel = document.getElementById("camZLabelID");
		camZLabel.innerHTML = camZ.value;
		camZ.label = "Cam Z: "+camZ.value;//refresh camZ text
	}
},false);

camTX.addEventListener("input", function(evt) {
	if(doneLoading==true){
		currentScene.camera.target.x=Number(camTX.value);
		var camTXLabel = document.getElementById("camTXLabelID");
		camTXLabel.innerHTML = camTX.value;
		camTX.label = "Cam target X: "+camTX.value;//refresh camTX text
	}
},false);
camTY.addEventListener("input", function(evt) {
	if(doneLoading==true){
		currentScene.camera.target.y=Number(camTY.value);
		var camTYLabel = document.getElementById("camTYLabelID");
		camTYLabel.innerHTML = camTY.value;
		camTY.label = "Cam target Y: "+camTY.value;//refresh camTY text
	}
},false);
camTZ.addEventListener("input", function(evt) {
	if(doneLoading==true){
		currentScene.camera.target.z=Number(camTZ.value);
		var camTZLabel = document.getElementById("camTZLabelID");
		camTZLabel.innerHTML = camTZ.value;
		camTZ.label = "Cam target Z: "+camTZ.value;//refresh camTZ text
	}
},false);


var waterHeight=0.3;
var wh = document.getElementById('whID');//Slider for water height
wh.value=waterHeight;
wh.addEventListener("input", function(evt) {
	if(doneLoading==true){
		waterHeight=Number(wh.value);
		var whLabel = document.getElementById("whLabelID");
		whLabel.innerHTML = wh.value;
		wh.label = "Water height: "+wh.value;//refresh wh text
	}
},false);

var alphaValue=0.8;
var alpha = document.getElementById('alphaID');//Slider for water height
alpha.value=alphaValue;
alpha.addEventListener("input", function(evt) {
	if(doneLoading==true){
		alphaValue=Number(alpha.value);
		var alphaLabel = document.getElementById("alphaLabelID");
		alphaLabel.innerHTML = alpha.value;
		alpha.label = "Water transparency: "+alpha.value;//refresh wh text
	}
},false);

var waveActive = false;
var dropStartTime = 0;
document.addEventListener("keydown",function(evt){
	if(evt.key == "d" || evt.key == "D"){
		waveActive = true;
		dropStartTime = performance.now() * 0.001;
		console.log("dropStartTime: ", dropStartTime);
	}
});


function readScene()//This is the function that is called after user selects multiple files of images and scenes
{
	if (input.files.length > 0) {
		if(doneLoading==true)//This condition checks if this is the first time user has selected a scene or not. If doneLoading==true, then the user has selected a new scene while rendering
		{
			newSceneRequested=true;
			filesToRead=[];//List of files to be read
			imageData=[];//The image contents are stored separately here
			objsData=[];
			scenes=[];//List of scenes
		}
		doneLoading=false;
		for(var i=0;i<input.files.length;i++)
		{
			var file = input.files[i];
			var reader = new FileReader();
			filesToRead[i]=true;
			reader.onload = (function(f,index) {
				return function(e) {
					//Get the file name
					let fileName = f.name;
					//Get the file Extension 
					let fileExtension = fileName.split('.').pop();
					if(fileExtension=='ppm')
					{
						var file_data = this.result;
						let img=parsePPM(file_data,fileName);//Parse image
						imageData.push(img);
						filesToRead[index]=false;
					}else if(fileExtension=='js')
					{
						var file_data = this.result;
						scene=parseScene(file_data);//Parse scene
						filesToRead[index]=false;
					}else if(fileExtension=='json')
					{
						var file_data = this.result;
						scene=parseScene(file_data);//Parse scene
						filesToRead[index]=false;
					}else if(fileExtension=='obj')
					{
						var file_data = this.result;
						objParsed=parseOBJ(file_data);//Parse obj to almost buffer-ready Float32Array arrays.
						
						filesToRead[index]=false;
					}else if(fileExtension=='obj')
					{
						var file_data = this.result;
						objParsed=parseOBJ(file_data);//Parse obj to almost buffer-ready Float32Array arrays.
						
						filesToRead[index]=false;
					}else if(fileExtension=='png')
					{
						var file_data = this.result;
						
						var pngImage = new PNGReader(file_data);
						
						pngImage.parse(function(err, png){
							if (err) throw err;
							
							let img = parsePNG(png,fileName);
							
							let width=img.width;
							let height=img.height;
							document.getElementById("dummy_canvas").setAttribute("width", img.width);
							document.getElementById("dummy_canvas").setAttribute("height", img.height);
							let showCaseData = ctx.createImageData(width, height);
							for(var i = 0; i < img.data.length; i+=1){
								showCaseData.data[i*4]=img.data[i].r;
								showCaseData.data[i*4+1]=img.data[i].g;
								showCaseData.data[i*4+2]=img.data[i].b;
								showCaseData.data[i*4+3]=img.data[i].a;
							}
							// This part processes fixed image names
							if(fileName=='pool.png'){
								ctx.putImageData(showCaseData, dummy_canvas.width/2 - width/2, dummy_canvas.height/2 - height/2);
							
								let imageRead=ctx.getImageData(0, 0, dummy_canvas.width, dummy_canvas.height);
								poolImage=imageRead;
							}else if(fileName=='posx.png'){
								posX=showCaseData;
							}else if(fileName=='negx.png'){
								negX=showCaseData;
							}else if(fileName=='posy.png'){
								posY=showCaseData;
							}else if(fileName=='negy.png'){
								negY=showCaseData;
							}else if(fileName=='posz.png'){
								posZ=showCaseData;
							}else if(fileName=='negz.png'){
								negZ=showCaseData;
							}
							filesToRead[index]=false;
						});
					}
				};
			})(file,i);
			let fileName = file.name;
			let fileExtension = fileName.split('.').pop();
			if(fileExtension=='ppm' || fileExtension=='js' || fileExtension=='json' || fileExtension=='obj')
			{
				reader.readAsBinaryString(file);
			}else if(fileExtension=='png'){
				reader.readAsArrayBuffer(file);
			}
			
		}
		drawScene();//Enter the drawing loop();
	}
}

// Draw the scene.
function drawScene(now) {
	if(doneLoading==false)
	{
		var isReaminingRead=false;
		for(let j=0;j<filesToRead.length;j++)
		{
			if(filesToRead[j]==true)//Check if each file is read
			{
				isReaminingRead=true;//If one is not read, then make sure drawing scene will wait for files to be read
			}
		}
		if(isReaminingRead==false)//If all files are read
		{
			currentScene=scene;
			currentScene.billboard.img=poolImage;
			
			doneLoading=true;
		}
	}else if(doneLoading==true)//If scene is completely read
	{
		if(doneProgramming==false){
			programAll();
			preprocessBuffers();
			doneProgramming=true;
			
			// Support for Alpha
			gl.enable(gl.BLEND)
			gl.colorMask(true, true, true, true);
			gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
			gl.enable(gl.DEPTH_TEST);
			gl.enable(gl.LEQUAL);
		}else{
			renderingFcn(now);
		}
	}
	
	// Call drawScene again next frame with delay to give user chance of interacting GUI
	requestAnimationFrame(drawScene);
}

function renderingFcn(now){
	gl.disable(gl.CULL_FACE);
	gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
	webglUtils.resizeCanvasToDisplaySize(gl.canvas);
	
	gl.clearColor(currentScene.camera.DefaulColor[0], currentScene.camera.DefaulColor[1], currentScene.camera.DefaulColor[2], 1.0);
	
	// Clear the canvas AND the depth buffer.
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	renderBackground();
	renderBillboard(now);
}

function renderBillboard(now){
	gl.disable(gl.CULL_FACE);
	
	// Tell it to use our program (pair of shaders)
    gl.useProgram(billboardProgram.program);
	
    // Turn on the position attribute
    gl.enableVertexAttribArray(billboardProgram.positionLocationAttrib);

    // Bind the position buffer.
    gl.bindBuffer(gl.ARRAY_BUFFER, currentScene.billboard.positionBuffer);
	
	// Tell the position attribute how to get data out of positionBuffer (ARRAY_BUFFER)
    var size = 3;          // 3 components per iteration
    var type = gl.FLOAT;   // the data is 32bit floats
    var normalize = false; // don't normalize the data
    var stride = 0;        // 0 = move forward size * sizeof(type) each iteration to get the next position
    var offset = 0;        // start at the beginning of the buffer
    gl.vertexAttribPointer(
        billboardProgram.positionLocationAttrib, size, type, normalize, stride, offset);
		
	
	// Turn on the textcoord attribute
    gl.enableVertexAttribArray(billboardProgram.textureLocationAttrib);

    // Bind the textcoord buffer.
    gl.bindBuffer(gl.ARRAY_BUFFER, currentScene.billboard.textureBuffer);
	
	// Tell the textcoord attribute how to get data out of textcoordBuffer (ARRAY_BUFFER)
    var size = 2;          // 2 components per iteration
    var type = gl.FLOAT;   // the data is 32bit floats
    var normalize = false; // don't normalize the data
    var stride = 0;        // 0 = move forward size * sizeof(type) each iteration to get the next normal
    var offset = 0;        // start at the beginning of the buffer
    gl.vertexAttribPointer(
        billboardProgram.textureLocationAttrib, size, type, normalize, stride, offset);
	
    // Compute the projection matrix
    var aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
    var projectionMatrix =
        m4.perspective(degToRad(currentScene.camera.fov), aspect, currentScene.camera.near, currentScene.camera.far);

	var cameraMatrix;
	// Compute the camera's matrix using look at.
	cameraMatrix = m4.lookAt([currentScene.camera.position.x,currentScene.camera.position.y,currentScene.camera.position.z], [currentScene.camera.target.x,currentScene.camera.target.y,currentScene.camera.target.z], [currentScene.camera.up.x,currentScene.camera.up.y,currentScene.camera.up.z]);

    // Make a view matrix from the camera matrix.
    var viewMatrix = m4.inverse(cameraMatrix);

    var viewProjectionMatrix = m4.multiply(projectionMatrix, viewMatrix);
	
    // Set the viewProjectionMatrix.
	gl.uniformMatrix4fv(billboardProgram.worldViewProjectionUniformLocation, false, viewProjectionMatrix);

	var camLocUL = gl.getUniformLocation(billboardProgram.program, "u_camLocation");

	gl.uniform3fv(camLocUL, new Float32Array([currentScene.camera.position.x,currentScene.camera.position.y,currentScene.camera.position.z]));
	
	// Tell the shader to use texture unit 1 for u_texture
    gl.uniform1i(billboardProgram.textureUniformLocation, 1);
	//TODO: You need to send various data to the shader code such as time, skybox texture, etc.
	gl.uniform1i(billboardProgram.skyboxUniformLocation, 0);
	
	var currentTime = now * 0.001;
	var elapsed = 0.0;
	if (waveActive){
		elapsed = currentTime - dropStartTime;
	}
	gl.uniform1f(billboardProgram.timeUniformLocation, elapsed);
	gl.uniform1f(billboardProgram.waterHeightUniformLocation, waterHeight);
	gl.uniform1f(billboardProgram.alphaUniformLocation,alpha.value);
	gl.uniform3fv(
		billboardProgram.cameraLocationUniformLocation,
		new Float32Array([
			currentScene.camera.position.x,
			currentScene.camera.position.y,
			currentScene.camera.position.z
			])
	);
	
	gl.drawArrays(gl.TRIANGLES, 0, 6);
}

function renderBackground(){
	// Tell it to use our program (pair of shaders)
    gl.useProgram(backgroundProgram.program);
	
    // Turn on the position attribute
    gl.enableVertexAttribArray(backgroundProgram.positionLocationAttrib);

    // Bind the position buffer.
    gl.bindBuffer(gl.ARRAY_BUFFER, currentScene.obj.positionBuffer);
	
	// Tell the position attribute how to get data out of positionBuffer (ARRAY_BUFFER)
    var size = 3;          // 3 components per iteration
    var type = gl.FLOAT;   // the data is 32bit floats
    var normalize = false; // don't normalize the data
    var stride = 0;        // 0 = move forward size * sizeof(type) each iteration to get the next position
    var offset = 0;        // start at the beginning of the buffer
    gl.vertexAttribPointer(
        backgroundProgram.positionLocationAttrib, size, type, normalize, stride, offset);
	

    // Compute the projection matrix
    var aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
    var projectionMatrix =
        m4.perspective(degToRad(currentScene.camera.fov), aspect, currentScene.camera.near, currentScene.camera.far);

	var cameraMatrix;
	

	// Compute the camera's matrix using look at.
	cameraMatrix = m4.lookAt([currentScene.camera.position.x,currentScene.camera.position.y,currentScene.camera.position.z], [currentScene.camera.target.x,currentScene.camera.target.y,currentScene.camera.target.z], [currentScene.camera.up.x,currentScene.camera.up.y,currentScene.camera.up.z]);

	
    // Make a view matrix from the camera matrix.
    var viewMatrix = m4.inverse(cameraMatrix);
	
	var viewProjectionMatrix = m4.multiply(projectionMatrix, viewMatrix);
	
	// Translation is not implemented in the given file. You need to implement it yourself.
	modelMatrix = m4.identity();
	m4.scale(modelMatrix,1,1,1,modelMatrix);
	m4.translate(modelMatrix,currentScene.obj.position[0],currentScene.obj.position[1],currentScene.obj.position[2],modelMatrix);
	m4.yRotate(modelMatrix,modelRotationRadians,modelMatrix);
	
	var worldViewProjectionMatrix = m4.multiply(viewProjectionMatrix, modelMatrix);
	
	gl.uniformMatrix4fv(backgroundProgram.worldUniformLocation, false, modelMatrix);

    // Set the viewProjectionMatrix.
	gl.uniformMatrix4fv(backgroundProgram.worldViewProjectionUniformLocation, false, worldViewProjectionMatrix);
	

	gl.drawArrays(gl.TRIANGLES, 0, currentScene.obj.numVertices);
}

function programAll(){
	programBillboard();
	programBackground();
}

function preprocessBuffers(){
	makeBillboardBuffers();
	makeBackgroundBuffers();
}

function makeBackgroundBuffers(){
	let sceneObj=currentScene.obj;
	sceneObj.setParsedObj(objParsed);
	let parsedObj=sceneObj.parsedObj;
	
	// Create a buffer for positions
    let objPositionBuffer = gl.createBuffer();
    // Bind it to ARRAY_BUFFER (think of it as ARRAY_BUFFER = positionBuffer)
    gl.bindBuffer(gl.ARRAY_BUFFER, objPositionBuffer);
    // Put the positions in the buffer
    setGeometryPositionBuffer(gl,parsedObj);
	
    // provide texture coordinates for the rectangle.
    let objTextcoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, objTextcoordBuffer);
    // Set Texcoords.
    setTextureCoordBuffer(gl,parsedObj);
  
    // Create a buffer to put normals in
    let billboardNormalBuffer = gl.createBuffer();
    // Bind it to ARRAY_BUFFER (think of it as ARRAY_BUFFER = normalBuffer)
    gl.bindBuffer(gl.ARRAY_BUFFER, billboardNormalBuffer);
    // Put normals data into buffer
    setNormalBuffer(gl,parsedObj);
  
    sceneObj.setBuffers(objPositionBuffer,objTextcoordBuffer,billboardNormalBuffer);
}

function makeBillboardBuffers(){
	let sceneBillboard=currentScene.billboard;
	
	// Create a buffer for positions
    let billboardPositionBuffer = gl.createBuffer();
    // Bind it to ARRAY_BUFFER (think of it as ARRAY_BUFFER = positionBuffer)
    gl.bindBuffer(gl.ARRAY_BUFFER, billboardPositionBuffer);
    // Put the positions in the buffer
    setBillboardGeometry(gl,sceneBillboard);
	
    // provide texture coordinates for the rectangle.
    let billboardTextcoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, billboardTextcoordBuffer);
    // Set Texcoords.
    setBillboardTexcoords(gl,sceneBillboard);
  
    // Create a buffer to put normals in
    let billboardNormalBuffer = gl.createBuffer();
    // Bind it to ARRAY_BUFFER (think of it as ARRAY_BUFFER = normalBuffer)
    gl.bindBuffer(gl.ARRAY_BUFFER, billboardNormalBuffer);
    // Put normals data into buffer
    setBillboardNormals(gl,sceneBillboard);
	
	// Create a texture.
	var billboardTextureBuffer = gl.createTexture();
	gl.activeTexture(gl.TEXTURE1);
	gl.bindTexture(gl.TEXTURE_2D, billboardTextureBuffer);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA,gl.UNSIGNED_BYTE, poolImage);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.generateMipmap(gl.TEXTURE_2D);

	
	// === Cubemap Texture ===
	var dummyImage = new ImageData(posX.width, posX.height);
	var cubemapTextureBuffer = gl.createTexture();
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTextureBuffer);
    gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_X, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, posX);
    gl.generateMipmap(gl.TEXTURE_CUBE_MAP);

	gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTextureBuffer);
    gl.texImage2D(gl.TEXTURE_CUBE_MAP_NEGATIVE_X, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, negX);
    gl.generateMipmap(gl.TEXTURE_CUBE_MAP);

	gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTextureBuffer);
    gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_Y, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, posY);
    gl.generateMipmap(gl.TEXTURE_CUBE_MAP);

	gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTextureBuffer);
    gl.texImage2D(gl.TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, negY);
    gl.generateMipmap(gl.TEXTURE_CUBE_MAP);

	gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTextureBuffer);
    gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_Z, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, posZ);
    gl.generateMipmap(gl.TEXTURE_CUBE_MAP);

	gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTextureBuffer);
    gl.texImage2D(gl.TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, negZ);
    gl.generateMipmap(gl.TEXTURE_CUBE_MAP);



	gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
	gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
	gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_R, gl.CLAMP_TO_EDGE);	
	
  
    sceneBillboard.setBuffers(billboardPositionBuffer,billboardTextcoordBuffer,billboardNormalBuffer,billboardTextureBuffer,cubemapTextureBuffer);
}

class BillboardProgram{
	constructor(program,positionLocationAttrib,normalLocationAttrib,textureLocationAttrib,textureUniformLocation,skyboxUniformLocation,worldViewProjectionUniformLocation,
				cameraLocationUniformLocation,timeUniformLocation,waterHeightUniformLocation,alphaUniformLocation){
		this.program=program;
		this.positionLocationAttrib=positionLocationAttrib;
		this.normalLocationAttrib=normalLocationAttrib;
		this.textureLocationAttrib=textureLocationAttrib;
		this.textureUniformLocation=textureUniformLocation;
		this.skyboxUniformLocation=skyboxUniformLocation;
		this.worldViewProjectionUniformLocation=worldViewProjectionUniformLocation;
		this.cameraLocationUniformLocation=cameraLocationUniformLocation;
		this.timeUniformLocation=timeUniformLocation;
		this.waterHeightUniformLocation=waterHeightUniformLocation;
		this.alphaUniformLocation=alphaUniformLocation;
	}
}

class BackgroundProgram{
	constructor(program,positionLocationAttrib,uSkyboxUniformLocation,worldViewProjectionUniformLocation){
		this.program=program;
		this.positionLocationAttrib=positionLocationAttrib;
		this.worldViewProjectionUniformLocation=worldViewProjectionUniformLocation;
		this.uSkyboxUniformLocation=uSkyboxUniformLocation;
		this.worldViewProjectionUniformLocation=worldViewProjectionUniformLocation;
	}
}

function programBillboard(){
	//Todo: Change the shader programs to support diffuse and specular (graduates) shading. For gouraud shading you need to calculate new normals when processing the OBJ file.
	var vShaderObj = 
				"attribute vec4 a_position;\n"+
				"attribute vec3 a_normal;\n"+
				"attribute vec2 a_texcoord;\n"+
				"uniform mat4 u_worldViewProjection;\n"+
				"varying vec2 v_texcoord;\n"+
				"varying vec3 v_worldPos;\n"+
				"void main() {\n"+
					"v_texcoord=a_texcoord;\n"+
					"v_worldPos = a_position.xyz;\n"+
					"gl_Position = u_worldViewProjection * a_position;\n"+
				"}";
	//h(rho,theta)=Asin((vt-rho)2pi)
	//h(x,y,z)=Asin((vt-sqrt(x^2+y^2)2pi)
	//d/dx*h(x)=h'(x)=-2Apicos(2pi(vt-rho)) -- plane y=0
	//partial x h(x,y) = -Ax (cos(t*v - sqrt(x^2+y^2)))/sqrt(x^2+y^2)...same for partial y 
	var fShaderObj = 	
					"precision mediump float;\n"+
					"uniform sampler2D u_texture;\n"+
					"uniform samplerCube u_skybox;\n"+
					"uniform vec3 u_cameraLocation;\n"+
					"uniform float u_time;\n"+
					"uniform float u_waterHeight;\n"+
					"uniform float u_alpha;\n"+
					"varying vec2 v_texcoord;\n"+
					"varying vec3 v_worldPos;\n"+
					"void main() {\n"+
						"float PI = 3.14159265;\n"+
						"float waveSpeed = 0.3;\n"+
						"vec2 centered = v_texcoord - vec2(0.5,0.5);\n"+
						"float rho = length(centered);\n"+
						"vec2 dir = vec2(0.0,0.0);\n"+
						"if (rho > 0.0001){\n"+
							"dir = centered/rho;\n"+
						"}\n"+
						"float phase = 2.0 * PI * (u_time * waveSpeed - rho);\n"+
						"float dhdr = 0.0;\n"+
						"if (rho <= u_time * waveSpeed){\n"+
							"dhdr = -u_waterHeight * 2.0 * PI * cos(phase);\n"+
						"}\n"+
						"float dhdx = dhdr * dir.x;\n"+
						"float dhdy = dhdr * dir.y;\n"+
						"vec3 normal = normalize(vec3(-dhdx,1.0,-dhdy));\n"+
						"vec3 incident = normalize(vec3(0.0,-1.0,0.0));\n"+
						"float eta = 1.0/2.0;\n"+
						"vec3 refractDir = refract(incident,normal,eta);\n"+
						"vec2 refractOffset = refractDir.xz * u_waterHeight * 0.15;\n"+
						"vec2 refractedUV = clamp(v_texcoord + refractOffset, 0.0, 1.0);\n"+
						"vec4 refractedColor = texture2D(u_texture,refractedUV);\n"+
						"vec3 viewDir = normalize(v_worldPos - u_cameraLocation);\n"+
						"vec3 reflectDir = reflect(viewDir, normal);\n"+
						"vec4 reflectedColor = textureCube(u_skybox, reflectDir);\n"+
						"gl_FragColor = mix(refractedColor,reflectedColor,u_alpha);\n"+
					"}";
	programBill = webglUtils.createProgramFromSources(gl, [vShaderObj,fShaderObj])
	
	//Todo: Add new varialbes for linking to the shader program.
	//The attribute variables from the shader program can be obtained as below.
	// look up where the vertex data needs to go.
    positionLocationAttrib = gl.getAttribLocation(programBill, "a_position");
	normalLocationAttrib = gl.getAttribLocation(programBill, "a_normal");
	textureLocationAttrib = gl.getAttribLocation(programBill, "a_texcoord");
	
	//Todo: Add new varialbes for linking to the shader program.
	//The uniform variables from the shader program can be obtained as below.
	// lookup uniforms
    textureUniformLocation = gl.getUniformLocation(programBill, "u_texture");
	skyboxUniformLocation = gl.getUniformLocation(programBill, "u_skybox");
	worldViewProjectionUniformLocation = gl.getUniformLocation(programBill, "u_worldViewProjection");
	cameraLocationUniformLocation = gl.getUniformLocation(programBill,"u_cameraLocation");
	timeUniformLocation = gl.getUniformLocation(programBill,"u_time");
	waterHeightUniformLocation = gl.getUniformLocation(programBill,"u_waterHeight");
	alphaUniformLocation = gl.getUniformLocation(programBill,"u_alpha");
	
	/*program,
	positionLocationAttrib,
	normalLocationAttrib,
	textureLocationAttrib,
	textureUniformLocation,
	skyboxUniformLocation,
	worldViewProjectionUniformLocation,
	cameraLocationUniformLocation,
	timeUniformLocation,
	waterHeightUniformLocation,
	alphaUniformLocation
	*/
	//Todo: You can store the variable addresses into a class similar to what is shown below so that in the rendering loop you don't get the variables each time.
	billboardProgram=new BillboardProgram(programBill,
										  positionLocationAttrib,
										  normalLocationAttrib,
										  textureLocationAttrib,
										  textureUniformLocation,
										  skyboxUniformLocation,
										  worldViewProjectionUniformLocation,
										  cameraLocationUniformLocation,
										  timeUniformLocation,
										  waterHeightUniformLocation,
										  alphaUniformLocation);
}

function programBackground(){
	//Todo: Change the shader programs to support diffuse and specular (graduates) shading. For gouraud shading you need to calculate new normals when processing the OBJ file.
	var vShaderObj = "attribute vec4 a_position;\n"+
				//"varying vec2 v_UV;\n"+
				"uniform mat4 u_worldViewProjection;\n"+
				"varying vec3 v_UV;\n"+
				"void main() {\n"+
					//"v_UV = a_position * 0.5 + 0.5;\n"+
					"v_UV=a_position.xyz;\n"+
					"gl_Position = u_worldViewProjection*a_position;\n"+
				"}";
	var fShaderObj = 	"precision mediump float;\n"+
					"uniform samplerCube u_Skybox;\n"+
					"varying vec3 v_UV;\n"+
					"uniform vec3 u_camLocation;\n"+
					"void main() {\n"+
						"vec3 rayDir = normalize(v_UV-u_camLocation);\n"+
						"gl_FragColor = textureCube(u_Skybox, rayDir)+0.2*vec4(0.0,0.5,0.0,1.0);\n"+
					"}";
	var programBack = webglUtils.createProgramFromSources(gl, [vShaderObj,fShaderObj])
	
    var positionLocationAttrib = gl.getAttribLocation(programBack, "a_position");

	var uSkyboxUniformLocation = gl.getUniformLocation(programBack, 'u_Skybox');
	var worldViewProjectionUniformLocation = gl.getUniformLocation(programBack, 'u_worldViewProjection');
	
	//Todo: You can store the variable addresses into a class similar to what is shown below so that in the rendering loop you don't get the variables each time.
	backgroundProgram=new BackgroundProgram(programBack,positionLocationAttrib,uSkyboxUniformLocation,worldViewProjectionUniformLocation);
}

//The function for parsing PNG is done for you. The output is a an array of RGBA instances.
function parsePNG(png,fileName){
	let rawValues = png.getRGBA8Array();
	let width = png.getWidth();
	let height = png.getHeight();
	var readImageValues=[];//Array of RGBA instances
	var counterMain=0;//It is used for array of RGBAValue instances.
	for(var i = 0; i < rawValues.length; i++){
		let r=rawValues[i*4];
		let g=rawValues[i*4+1];
		let b=rawValues[i*4+2];
		let a=rawValues[i*4+3];
		readImageValues[counterMain]=new RGBAValue(r,g,b,a);
		counterMain=counterMain+1;
	}
	return new PNGImage(readImageValues,width,height,fileName);
}

class PNGImage{
	constructor(data,width,height,fileName){
		this.data=data;// The 1D array of RGBA pixel instances
		this.fileName=fileName;// Filename is useful to connect this image to appropriate Billboard after all materials are read.
		this.width=width;// Width of image
		this.height=height;// Height of image
	}
}

class RGBAValue{
	constructor(r,g,b,a)
	{
		this.r=r;
		this.g=g;
		this.b=b;
		this.a=a;
	}
}

function radToDeg(r) {
	return r * 180 / Math.PI;
}

function degToRad(d) {
	return d * Math.PI / 180;
}

// A utility function to convert a javascript Floar32Array to a buffer. This function must be called after the buffer is bound.
function setGeometryPositionBuffer(gl,obj) {
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(obj.geometries[0].data.position), gl.STATIC_DRAW);
}

// A utility function to convert a javascript Floar32Array to a buffer. This function must be called after the buffer is bound.
function setTextureCoordBuffer(gl,obj) {
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(obj.geometries[0].data.texcoord), gl.STATIC_DRAW);
}

// A utility function to convert a javascript Floar32Array to a buffer. This function must be called after the buffer is bound.
function setNormalBuffer(gl,obj) {
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(obj.geometries[0].data.normal), gl.STATIC_DRAW);
}

//This is a utility function to set vertex colors by random numbers
function setColorBuffer(gl,obj) {
	var numVertices=obj.geometries[0].data.position.length;
	var colors = new Float32Array(numVertices*3);
	var myrng = new Math.seedrandom('123');
	for(let i=0;i<numVertices*3;i++){
		colors[i]=0.4+myrng()/2;
	}
	gl.bufferData(gl.ARRAY_BUFFER, colors, gl.STATIC_DRAW);
}

// Complete this function with counter clock-wise vertices of the billboard. The billboard should be made of two triangles.
function setBillboardGeometry(gl,billboard) {
	var positions = new Float32Array([
	//billboard.UpperLeft.x, billboard.UpperLeft.y, billboard.UpperLeft.z,  // first triangle
    //billboard.LowerLeft.x, billboard.LowerLeft.y, billboard.LowerLeft.z,
    //billboard.UpperRight.x, billboard.UpperRight.y, billboard.UpperRight.z,
    //billboard.UpperRight.x,  billboard.UpperRight.y, billboard.UpperRight.z,  // second triangle
    //billboard.LowerLeft.x,  billboard.LowerLeft.y, billboard.LowerLeft.z,
    //billboard.LowerRight.x,  billboard.LowerRight.y, billboard.LowerRight.z
	
	-6, 1, -6,  // first triangle
    -6, 1, 6,
    6, 1, -6,
    6,  1, -6,  // second triangle
    -6,  1, 6,
    6,  1, 6
	]);
	gl.bufferData(gl.ARRAY_BUFFER, positions, gl.STATIC_DRAW);
}

function setBillboardTexcoords(gl,billboard) {
  gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array([	  
	  0,0,
	  0,1,
	  1,0,
	  1,0,
	  0,1,
	  1,1
	  ]),
      gl.STATIC_DRAW);
}

function setBillboardNormals(gl,billboard) {
  let vec1=Vector3.minusTwoVectors(billboard.UpperLeft,billboard.LowerLeft);
  let vec2=Vector3.minusTwoVectors(billboard.LowerRight,billboard.LowerLeft);
  var normalVector=Vector3.crossProduct(vec2,vec1);//billboard normal vector
  gl.bufferData(
      gl.ARRAY_BUFFER,
      new Float32Array([	  
	  normalVector.x,normalVector.y,normalVector.z,
	  normalVector.x,normalVector.y,normalVector.z,
	  normalVector.x,normalVector.y,normalVector.z,
	  normalVector.x,normalVector.y,normalVector.z,
	  normalVector.x,normalVector.y,normalVector.z,
	  normalVector.x,normalVector.y,normalVector.z
	  ]),
      gl.STATIC_DRAW);
}

//This function is given to you for parsing the OBJ file.
function parseOBJ(text) {
  // because indices are base 1 let's just fill in the 0th data
  const objPositions = [[0, 0, 0]];
  const objTexcoords = [[0, 0]];
  const objNormals = [[0, 0, 0]];

  // same order as `f` indices
  const objVertexData = [
    objPositions,
    objTexcoords,
    objNormals,
  ];

  // same order as `f` indices
  let webglVertexData = [
    [],   // positions
    [],   // texcoords
    [],   // normals
  ];

  const materialLibs = [];
  const geometries = [];
  let geometry;
  let groups = ['default'];
  let material = 'default';
  let object = 'default';

  const noop = () => {};

  function newGeometry() {
    if (geometry && geometry.data.position.length) {
      geometry = undefined;
    }
  }

  function setGeometry() {
    if (!geometry) {
      const position = [];
      const texcoord = [];
      const normal = [];
      webglVertexData = [
        position,
        texcoord,
        normal,
      ];
      geometry = {
        object,
        groups,
        material,
        data: {
          position,
          texcoord,
          normal,
        },
      };
      geometries.push(geometry);
    }
  }

  function addVertex(vert) {
    const ptn = vert.split('/');
    ptn.forEach((objIndexStr, i) => {
      if (!objIndexStr) {
        return;
      }
      const objIndex = parseInt(objIndexStr);
      const index = objIndex + (objIndex >= 0 ? 0 : objVertexData[i].length);
      webglVertexData[i].push(...objVertexData[i][index]);
    });
  }

  const keywords = {
    v(parts) {
      objPositions.push(parts.map(parseFloat));
    },
    vn(parts) {
      objNormals.push(parts.map(parseFloat));
    },
    vt(parts) {
      objTexcoords.push(parts.map(parseFloat));
    },
    f(parts) {
      setGeometry();
      const numTriangles = parts.length - 2;
      for (let tri = 0; tri < numTriangles; ++tri) {
        addVertex(parts[0]);
        addVertex(parts[tri + 1]);
        addVertex(parts[tri + 2]);
      }
    },
    s: noop,    // smoothing group
    mtllib(parts, unparsedArgs) {
      materialLibs.push(unparsedArgs);
    },
    usemtl(parts, unparsedArgs) {
      material = unparsedArgs;
      newGeometry();
    },
    g(parts) {
      groups = parts;
      newGeometry();
    },
    o(parts, unparsedArgs) {
      object = unparsedArgs;
      newGeometry();
    },
  };

  const keywordRE = /(\w*)(?: )*(.*)/;
  const lines = text.split('\n');
  for (let lineNo = 0; lineNo < lines.length; ++lineNo) {
    const line = lines[lineNo].trim();
    if (line === '' || line.startsWith('#')) {
      continue;
    }
    const m = keywordRE.exec(line);
    if (!m) {
      continue;
    }
    const [, keyword, unparsedArgs] = m;
    const parts = line.split(/\s+/).slice(1);
    const handler = keywords[keyword];
    if (!handler) {
      console.warn('unhandled keyword:', keyword);  // eslint-disable-line no-console
      continue;
    }
    handler(parts, unparsedArgs);
  }

  for (const geometry of geometries) {
    geometry.data = Object.fromEntries(
        Object.entries(geometry.data).filter(([, array]) => array.length > 0));
  }

  return {
    geometries,
    materialLibs,
  };
}

//Extra math functions. This can not be used in shader program. GLSL has its own math functions.
class Vector3{
	constructor(x,y,z){
		this.x=x;
		this.y=y;
		this.z=z;
	}
	static multiplyVectorScalar(vec,scalar){
		return new Vector3(vec.x*scalar,vec.y*scalar,vec.z*scalar);
	}
	static sumTwoVectors(vec1,vec2){
		return new Vector3(vec1.x+vec2.x,vec1.y+vec2.y,vec1.z+vec2.z);
	}
	static minusTwoVectors(vec1,vec2){
		return new Vector3(vec1.x-vec2.x,vec1.y-vec2.y,vec1.z-vec2.z);
	}
	static normalizeVector(vec){
		let sizeVec=Math.sqrt(Math.pow(vec.x,2)+Math.pow(vec.y,2)+Math.pow(vec.z,2));
		return new Vector3(vec.x/sizeVec,vec.y/sizeVec,vec.z/sizeVec);
	}
	static crossProduct(vec1,vec2){
		return new Vector3(vec1.y * vec2.z - vec1.z * vec2.y,vec1.z * vec2.x - vec1.x * vec2.z,vec1.x * vec2.y - vec1.y * vec2.x);
	}
	static negate(vec){
		return new Vector3(-vec.x,-vec.y,-vec.z);
	}
	static dotProduct(vec1,vec2){
		var result = 0;
		result += vec1.x * vec2.x;
		result += vec1.y * vec2.y;
		result += vec1.z * vec2.z;
		return result;
	}
	static distance(p1,p2){
		return Math.sqrt(Math.pow(p1.x-p2.x,2)+Math.pow(p1.y-p2.y,2)+Math.pow(p1.z-p2.z,2));
	}
	static getMagnitude(vec){
		return Math.sqrt(Math.pow(vec.x,2)+Math.pow(vec.y,2)+Math.pow(vec.z,2));
	}
}


class Billboard{
	constructor(UpperLeft,LowerLeft,UpperRight,LowerRight,imgFile,img,ambient){
		this.UpperLeft=UpperLeft;
		this.LowerLeft=LowerLeft;
		this.UpperRight=UpperRight;
		this.LowerRight=LowerRight;
		this.imgFile=imgFile;
		this.img=img;
		this.ambient=ambient;
	}
	
	setBuffers(positionBuffer,textureBuffer,normalBuffer,billboardTextureBuffer,cubemapTextureBuffer){
		this.positionBuffer=positionBuffer;
		this.textureBuffer=textureBuffer;
		this.normalBuffer=normalBuffer;
		this.billboardTextureBuffer=billboardTextureBuffer;
		this.cubemapTextureBuffer=cubemapTextureBuffer;
	}
}

class Object3D{
	constructor(position,fileName){
		this.position=position;
		this.fileName=fileName;
	}
	
	setParsedObj(parsedObj){
		this.parsedObj=parsedObj;
		this.numVertices=parsedObj.geometries[0].data.position.length/3;
	}
	
	setBuffers(positionBuffer,textureBuffer,normalBuffer){
		this.positionBuffer=positionBuffer;
		this.textureBuffer=textureBuffer;
		this.normalBuffer=normalBuffer;
	}
}

class SunLight{//Light source
	constructor(locationPoint){
		this.locationPoint=locationPoint;
	}
}

class Camera{
	constructor(position,target,up,fov,far,near,DefaulColor){
		this.position=position;
		this.target=target;
		this.up=up;
		this.fov=fov;//IMPORTANT: It is assumed that FOV is the angle between the center vector and edge of the frustum (half pyramid) but not the entire frustum (full pyramid).
		this.far=far;
		this.near=near;
		this.DefaulColor=DefaulColor;
	}
	setVectors(w,nw,u,v){
		this.w=w;
		this.nw=nw;
		this.u=u;
		this.v=v;
	}
}

class Scene{//This object technically stores everything required for a scene
	constructor(light,billboard,obj,camera){
		this.light=light;
		this.billboard=billboard;
		this.camera=camera;
		this.obj=obj;
	}
}

class Ray{
	constructor(origin,direction){
		this.origin=origin;
		this.direction=direction;
	}
}

function parseScene(file_data)//A simple function to read JSON and put the data inside a scene class and return the read scene
{
	var sceneFile = JSON.parse(file_data);
	let pos=new Vector3(sceneFile.eye[0],sceneFile.eye[1],sceneFile.eye[2]);
	let lookat=new Vector3(sceneFile.lookat[0],sceneFile.lookat[1],sceneFile.lookat[2]);
	let up=new Vector3(sceneFile.up[0],sceneFile.up[1],sceneFile.up[2]);
	let fov=sceneFile.fov_angle;
	let near=sceneFile.near;
	let far=sceneFile.far;
	let DefaulColor=sceneFile.DefaulColor;
	var camera=new Camera(pos,lookat,up,fov,far,near,DefaulColor);
	let light=new SunLight(new Vector3(sceneFile.SunLocation[0],sceneFile.SunLocation[1],sceneFile.SunLocation[2]));
	var billboard;
	if ('billboard' in sceneFile) {//If billboard exists in scene
		let upperLeft=new Vector3(sceneFile.billboard.UpperLeft[0],sceneFile.billboard.UpperLeft[1],sceneFile.billboard.UpperLeft[2]);
		let lowerLeft=new Vector3(sceneFile.billboard.LowerLeft[0],sceneFile.billboard.LowerLeft[1],sceneFile.billboard.LowerLeft[2]);
		let upperRight=new Vector3(sceneFile.billboard.UpperRight[0],sceneFile.billboard.UpperRight[1],sceneFile.billboard.UpperRight[2]);
		let billboardHeight=upperLeft.y-lowerLeft.y;
		let lowerRight=new Vector3(upperRight.x,upperRight.y-billboardHeight,upperRight.z);
		
		billboard=new Billboard(upperLeft,lowerLeft,upperRight,lowerRight,sceneFile.billboard.filename,null,null);//Image is assigned to billboard later
	}
	var obj=null;
	if ('obj' in sceneFile) {//If billboard exists in scene
		let position=sceneFile.obj.position;
		let fileName=sceneFile.obj.filename;
		obj=new Object3D(position,fileName);
	}
	return new Scene(light,billboard,obj,camera);
}
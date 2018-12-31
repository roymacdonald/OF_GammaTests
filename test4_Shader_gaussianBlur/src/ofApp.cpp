#include "ofApp.h"
#include "gamma_tests.h"

//#define LOAD_SHADER
//--------------------------------------------------------------
void ofApp::setup(){
	
	
	ofxGuiSetDefaultWidth(500);
	gui.setup();
	gui.add(kernelSize.set("kernelSize", 4,0,100));
	gui.add(bNormalizeKernel.set("Normalize Kernel", true));
	gui.add(bUseXYShaders.set("UseXYShaders", false));
	kernelListeners.push(kernelSize.newListener([&](size_t&){
		makeShader();
	}));
	
	kernelListeners.push(bNormalizeKernel.newListener([&](bool&){
		makeShader();
	}));
	
	
	makeShader();
	
	
	renderTestPatternToImg(testPattern, 1, 1);
	
	
	
	fboX.allocate(testPattern.getWidth(), testPattern.getHeight(),GL_RGBA32F_ARB);
	fboY.allocate(testPattern.getWidth(), testPattern.getHeight(),GL_RGBA32F_ARB);
}
//--------------------------------------------------------------
void ofApp::makeShader(){
#ifdef LOAD_SHADER
	shader.load("shadersGL2/shader");
#else
	stringstream ssv;
	ssv << "#version 120\n";
	ssv << "varying vec2 texCoordVarying;\n";
	ssv << "void main(void)\n";
	ssv << "{\n";
	ssv << "	texCoordVarying = gl_MultiTexCoord0.xy;\n";
	ssv << "	gl_Position = ftransform();\n";
	ssv << "}\n";
	
	// type: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER_EXT etc.
	shader.setupShaderFromSource(GL_VERTEX_SHADER, ssv.str());
	// shader.setupShaderFromFile(GL_VERTEX_SHADER, ofToDataPath("shadersGL2/shader.vert"));
	
	stringstream ss;
	auto kSize = kernelSize.get() *2 +1;
	int ks  = kernelSize.get();
	
	ss << "#version 120\n";
	ss << "uniform sampler2DRect tex0;\n";
	ss << "varying vec2 texCoordVarying;\n";
	
	std::vector<std::vector<float> > gk;
	std::vector<glm::vec3> gd;
	if(bUseXYShaders){
		ss << "uniform vec2 direction;\n";
		gd = getGaussianDistribution(0, 1, 3.1, kSize, bNormalizeKernel);
	}else{
		gk = getGaussianKernel(kSize, bNormalizeKernel);
	}
	
	
	
	ss << "void main()\n";
	ss << "{\n";
	ss << "	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n";
	
	for (int y = 0; y < kSize; y++) {
		if(bUseXYShaders){
			ss << "color += " <<  fixed << setprecision(10) << gd[y].y << "* texture2DRect(tex0, texCoordVarying + direction*" << (y - ks) << ");\n";
		}else{
			for (int x = 0; x < kSize; x++) {
				ss << "color += " << fixed << setprecision(10) << gk[x][y] << "* texture2DRect(tex0, texCoordVarying + vec2(" << x - ks << ", " << y - ks << "));\n";
			}	
		}
	}		
	ss << "gl_FragColor = color;\n}";
	//	ss << "gl_FragColor = texture2DRect(tex0, mousePos);}";
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, ss.str());
	shader.linkProgram();
	//	cout << "//--------------------------------------------------------------" << endl; 
	//	cout << ss.str() << endl;
	
	
	
	
	
	
#endif
}
//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if(bUseXYShaders){
		fboX.begin();
		
		shader.begin();
		shader.setUniform2f("direction", 1, 0);
		
		testPattern.draw(0, 0);
		
		shader.end();
		
		fboX.end();
		
		//----------------------------------------------------------
		fboY.begin();
		
		shader.begin();
		shader.setUniform2f("direction", 0, 1);
		
		fboX.draw(0, 0);
		
		shader.end();
		
		fboY.end();
		
		//----------------------------------------------------------
		ofSetColor(ofColor::white);
		fboY.draw(0, 0);
	}else{
		
		
		shader.begin();
		//	shader.setUniform2f("mousePos", ofGetMouseX(), ofGetMouseY());
		ofSetColor(ofColor::white);
		
		testPattern.draw(0, 0);
		
		shader.end();
	}
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key == ' '){
		makeShader();
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}

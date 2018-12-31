#include "ofApp.h"
#include "gamma_tests.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	
	ofxGuiSetDefaultWidth(500);
	gui.setup();
	gui.add(kernelSize.set("kernelSize", 4,0,100));
	gui.add(bNormalizeKernel.set("Normalize Kernel", true));
	
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
	stringstream ssv;
	ssv << "#version 120\n";
	ssv << "varying vec2 texCoordVarying;\n";
	ssv << "void main(void)\n";
	ssv << "{\n";
	ssv << "	texCoordVarying = gl_MultiTexCoord0.xy;\n";
	ssv << "	gl_Position = ftransform();\n";
	ssv << "}\n";
	
	
	shader.setupShaderFromSource(GL_VERTEX_SHADER, ssv.str());
	
	
	auto kSize = kernelSize.get() *2 +1;
	int ks  = kernelSize.get();
	auto gd = getGaussianDistribution(0, 1, 3.1, kSize, bNormalizeKernel);
	
	stringstream ss;
	ss << "#version 120\n";
	ss << "uniform sampler2DRect tex0;\n";
	ss << "varying vec2 texCoordVarying;\n";
	ss << "uniform vec2 direction;\n";
	ss << "void main()\n";
	ss << "{\n";
	ss << "	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n";
	
	for (int y = 0; y < kSize; y++) {
		ss << "color += " <<  fixed << setprecision(10) << gd[y].y << "* texture2DRect(tex0, texCoordVarying + direction*" << (y - ks) << ");\n";
	}		
	ss << "gl_FragColor = color;\n}";
	
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, ss.str());
	shader.linkProgram();
	
}
//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
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
	
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
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

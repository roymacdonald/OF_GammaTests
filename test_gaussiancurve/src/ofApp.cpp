#include "ofApp.h"
#include "gamma_tests.h"
//--------------------------------------------------------------
void ofApp::setup(){

	
	ofxGuiSetDefaultWidth(500);
	gui.setup();
	gui.add(mean.set("mean", 0,-10,10));
	gui.add(sigma.set("sigma", 1,0,2));
	gui.add(width.set("width", 3,0,10));
	gui.add(scale.set("scale", 1, 0, 10));
	gui.add(numPoints.set("numPoints", 100,0,200));
	gui.add(kernelSize.set("kernelSize", 4,0,100));
	gui.add(bGrayscaleKernel.set("Grayscale Kernel", false));
	gui.add(bNormalizeKernel.set("Normalize Kernel", false));
	
	kernelListeners.push(kernelSize.newListener([&](size_t&){
		makeKernel();
	}));

	kernelListeners.push(bGrayscaleKernel.newListener([&](bool&){
		makeKernel();
	}));
	
	kernelListeners.push(bNormalizeKernel.newListener([&](bool&){
		makeKernel();
	}));
	
	
	guiListener = gui.getParameter().castGroup().parameterChangedE().newListener([&](ofAbstractParameter&){
		makeMesh();
	});
	
	mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
	
//	glEnable(GL_FRAMEBUFFER_SRGB);
	makeMesh();
	makeKernel();
	
	cam.removeAllInteractions();
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
	cam.enableOrtho();
	cam.setNearClip(-1000000);
	cam.setFarClip(1000000);
	cam.setVFlip(true);
}
//--------------------------------------------------------------
void ofApp::makeKernel(){
	auto kSize = kernelSize*2 + 1;
	kernel.clear();
	kernel.allocate(kSize, kSize, (bGrayscaleKernel.get()?OF_IMAGE_GRAYSCALE:OF_IMAGE_COLOR));
	auto& px = kernel.getPixels();
	auto  k = getGaussianKernel(kSize);
	
	if(bNormalizeKernel){
		float mx = 0;
		for(int y = 0; y < kSize; y++){
			for(int x = 0; x < kSize; x++){
				if(k[x][y] > mx) mx = k[x][y]; 
			}
		}
		
		float factor = 1/mx;
		for(int y = 0; y < kSize; y++){
			for(int x = 0; x < kSize; x++){
				k[x][y] *=  factor;
			}
		}
	}
	
	for(int y = 0; y < kSize; y++){
		for(int x = 0; x < kSize; x++){
			if(bGrayscaleKernel.get()){
				int i = y * kSize + x;
				px.getData()[i] = k[x][y]*255;
			}else{
				px.setColor(x, y, ofColor::fromHsb(k[x][y]*255, 255, 255));
			}
			 
		}
	}
	kernel.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	kernel.update();
}
//--------------------------------------------------------------
void ofApp::makeMesh(){
	auto verts = getGaussianDistribution(mean, sigma, width, numPoints);
//	
	mn = {1000000,100000};
	mx = {-1000000,-100000};
	for(auto& v: verts){
		if(v.x < mn.x) mn.x = v.x;
		if(v.y < mn.y) mn.y = v.y;
		if(v.x > mx.x) mx.x = v.x;
		if(v.y > mx.y) mx.y = v.y;
	}

	mesh.clear();
	mesh.addVertices(verts);

	float m = 1/255.0f;
	for(auto& v: verts){
		if(int(v.y *255) < 1 ){
			mesh.addColor(ofColor::white);
		}else{
			mesh.addColor(ofColor::red);
		}
	}
	

}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	ofRectangle s(0,0,ofGetWidth(), ofGetHeight());
	ofRectangle r(0,0, kernel.getWidth(), kernel.getHeight());
	r.scaleTo(s);
	ofSetColor(255);
	kernel.draw(r);
	
	cam.begin();
	ofPushMatrix();
	ofScale(scale.get());
	
	
	ofSetLineWidth(1);
	ofDrawGrid(0.1, 50, true, false, false, true);
	ofSetLineWidth(3);
	
	mesh.drawWireframe();

	ofSetLineWidth(1);
	float y = 1/255.0;
	
	ofSetColor(ofColor::yellow);
	ofDrawLine(-5, y, 5, y);
	
	float pi = glm::pi<float>();
	
	ofDrawLine(pi, -5, pi, 5);
	
	ofPopMatrix();
	cam.end();
	
	stringstream ss;
	ss << "min: " << mn << "\nmax: " << mx << "\n";
	
	ofDrawBitmapStringHighlight(ss.str(), 20, ofGetHeight() - 50);
	
	gui.draw();
	

	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

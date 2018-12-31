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
		makeMesh2();
	});
	
//	float mean = 0.0;
//	float sigma = 1.0;
//	
//	int  kSize = ofGetWidth();
	//	int kStart = -(kSize-1)/2;
	//	int kEnd = kSize - kStart;
	
	
	
//	numWeights = 100;
//	
//	GaussianKernelCalculator kc;
//	weights = kc.updateKernel(1, numWeights);
//	
//	meshes.resize(3);
	
//	for(auto& m : meshes){
		mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
//	}
	
//	float y_step = ofGetHeight();///kSize;
//	
//	float s2 = 2.0 * sigma * sigma;
//	float p_sq = (1.0 / (sigma * sqrt(glm::two_pi<float>())));
//	
//	for(int i = 0; i <kSize; i++){
//		float x = ofMap(i, 0, kSize -1, -3, 3);
//		auto z = exp(-(pow(x - mean, 2.0) /(2.0 * pow(sigma.get(), 2.0)))) * (1.0 / (sigma * sqrt(2.0 * 3.1415)));
//		meshes[0].addVertex({ofMap(x,-3, 3, 0, ofGetWidth()), ofMap(z, 0, 0.4, 0, ofGetHeight()),0});
//	
//		 x = ofMap(i, 0, kSize-1, -1, 1);
//				auto g = exp(-(pow(x*glm::pi<float>(), 2)) ) ;//* p_sq;
//		//		cout << i << "  " << x << "  " << gaussian[i] << endl;
//				meshes[1].addVertex({ofMap(x,  -1, 1, 0, ofGetWidth()), g * y_step,0});
//		
//	}
//	
//	auto verts = getGaussianDistribution(0, 1, 3, 100);
//	
//	glm::vec2 mn, mx;
//	for(auto& v: verts){
//		if(v.x < mn.x) mn.x = v.x;
//		if(v.y < mn.y) mn.y = v.y;
//		if(v.x > mx.x) mx.x = v.x;
//		if(v.y > mx.y) mx.y = v.y;
//	}
//	
//	for(auto& v: verts){
//		v.x = ofMap(v.x, mn.x, mx.x, 0, ofGetWidth());
////		v.y = ofGetHeight() * (v.y);
//		v.y = ofMap(v.y, mn.y, mx.y, 0, ofGetHeight());		
//	}
//	
//	mesh.addVertices(verts);
	
	
//	cout << endl;
//	vector<float> gaussian;
//	gaussian.resize(kSize);
//		
//	for(int i = 0; i < kSize; i++ ){
//		auto x = ofMap(i, 0, kSize-1, -1, 1);
//		gaussian[i] = exp(-(pow(x*glm::pi<float>(), 2)) ) ;//* p_sq;
////		cout << i << "  " << x << "  " << gaussian[i] << endl;
//		m.addVertex({ofMap(x,  -1, 1, 0, ofGetWidth()), gaussian[i]* y_step,0});
//	}
	
	
//	kernel.allocate(kSize, kSize, OF_IMAGE_GRAYSCALE);

	
	
//	glm::vec3 center(ofGetWidth()/2, 0,0);
//	for(auto& v: m.getVertices()){
//		v += center;
//	}
	
	glEnable(GL_FRAMEBUFFER_SRGB);
	makeMesh2();
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
			//			px.setColor(x, y, ofColor::fromHsb(gaussian[x]*gaussian[y]*255, 255, 255));
			//			px[i]= 
		}
	}
	kernel.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	kernel.update();
}
//--------------------------------------------------------------
void ofApp::makeMesh2(){
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
//	
//	float xMult =  ofGetWidth()/(mx.x - mn.x);
//	float yMult =  ofGetHeight()/(mx.y - mn.y);
	
	mesh.clear();
	mesh.addVertices(verts);

	float m = 1/255.0f;
	for(auto& v: verts){
		if(int(v.y *255) < 1 ){
			mesh.addColor(ofColor::white);
		}else{
			mesh.addColor(ofColor::red);
		}
//		v.x *= xMult;//  = ofMap(v.x, mn.x, mx.x, 0, ofGetWidth());
//		//		v.y = ofGetHeight() * (v.y);
//		v.y *= yMult;// ofMap(v.y, mn.y, mx.y, 0, ofGetHeight());		
	}
//	
	

}
//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
//	kernel.draw(0,0);
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
	
//	ofSeedRandom(123);
//	for(auto& m : meshes){
//		ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
		mesh.drawWireframe();
//	
//	ofSetColor(ofColor::red);
//	meshes[1].drawWireframe();
//	}

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
	
//	ofSetColor(ofColor::yellow, 100);
//	float w = ofGetWidth()/(float)weights.size();
//	for(int i = 0; i < weights.size(); i++){
//		ofDrawRectangle(i*w, 0, w, ofGetHeight()*weights[i]);
//	}
	
	
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

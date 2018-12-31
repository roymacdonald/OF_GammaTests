#include "ofApp.h"
#include "ofxCv.h"
#include "gamma_tests.h"
//--------------------------------------------------------------
void ofApp::setup(){
	
	
	renderTestPatternToImg(img);
}


//--------------------------------------------------------------
void ofApp::update(){
	
}


//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackground(0);
	
	ofImage bImg = img;
	
	if(bProcessGamma) {
		decodeGamma(bImg, 2.2);
	}
	
	ofxCv::GaussianBlur(bImg, ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 50, true)*2 +1);
	
	if(bProcessGamma){
		encodeGamma(bImg, 2.2);
	}else{
		bImg.update();
	}

	
	bImg.draw(0,0);
	
	stringstream ss;
	
	ss << "Decode Gama before blur: " << (bProcessGamma?"YES":"NO") << "\n";
	ss << "\nPress the space bar to switch gamma decoding/encoding.";
	ss << "\nMove the mouse horizontally to change the blur amount.";
	
	
	ofDrawBitmapStringHighlight(ss.str(), 20, 20);
	
	
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' '){
		bProcessGamma ^= true;
	}
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

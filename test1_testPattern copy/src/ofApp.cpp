#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	
	colors.push_back(ofColor::red);
	colors.push_back(ofColor::green);
	colors.push_back(ofColor::blue);
	colors.push_back(ofColor::yellow);
	colors.push_back(ofColor::cyan);
	colors.push_back(ofColor::magenta);
	
//	rect.setup({100, 100, 400,400},ofColor::cyan,20);
	
//	
//		float w = ofGetWidth()/(float)colors.size();
//		float h = ofGetHeight();
//		
//		float b = 255*1;
//		
//	int blur = 10;
//	
//		for(int i = 0; i < colors.size(); i++){
//			ofColor c = colors[i];
//			c.setBrightness(b);
//			c.setSaturation(b);
////			ofSetColor(c);
//			ofRectangle r(w*i, 0, w, h);
//			rects.push_back(bluredRect());
//			rects.back().setup(r, c, blur);
//		}
//		
//		w = ofGetWidth();
//		 h = ofGetHeight()/(float)((colors.size()+1)*3);
//		
//		for(int i = 0; i < colors.size(); i++){
//			ofColor c = colors[i];
//			c.setBrightness(b);
//			c.setSaturation(b);
////			ofSetColor(c);
//			ofDrawRectangle r(0,(i+1)*3*h, w, h);
//			
//			rects.push_back(bluredRect());
//			rects.back().setup(r, c, blur);
//			
//		}
//	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0);
	

//	rect.mesh.draw();
	
	float w = ofGetWidth()/(float)colors.size();
	float h = ofGetHeight();
	
	float b = 255*0.5;
	
	for(int i = 0; i < colors.size(); i++){
		ofColor c = colors[i];
		c.setBrightness(b);
		c.setSaturation(b);
		ofSetColor(c);
		ofDrawRectangle(w*i, 0, w, h);	
	}
	
	w = ofGetWidth();
	 h = ofGetHeight()/(float)((colors.size()+1)*3);
	
	for(int i = 0; i < colors.size(); i++){
		ofColor c = colors[i];
		c.setBrightness(b);
		c.setSaturation(b);
		ofSetColor(c);
		ofDrawRectangle(0,(i+1)*3*h, w, h);	
	}
	
	
	
	
	
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

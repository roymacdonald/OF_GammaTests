#pragma once

#include "ofMain.h"
#include "ofxGui.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	void makeMesh();
	
	void makeKernel();
	
	ofMesh mesh;
	ofImage kernel;
	
	vector<float> weights;
	int numWeights;
	
	ofxPanel gui;
	
	ofParameter<float>mean, sigma, width, scale;
	ofParameter<size_t>numPoints, kernelSize;
	ofParameter<bool> bGrayscaleKernel, bNormalizeKernel;
	
	
	ofEventListener guiListener;
	ofEventListeners kernelListeners;
	
ofEasyCam cam;
		glm::vec2 mn, mx;
};

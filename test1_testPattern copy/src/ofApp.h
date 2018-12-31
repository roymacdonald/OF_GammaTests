#pragma once

#include "ofMain.h"

using namespace glm;

class bluredRect{
public:
	void setup(ofRectangle rect,ofColor color, float blurSize =5){
		this->rect = rect;
		this->color = color;
		this-> blurSize = blurSize;
		buildMesh();
	
	}
	ofRectangle rect;
	ofColor color;
	float blurSize =5;
	
	void buildMesh(){
		auto b = blurSize;
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		mesh.addVertex(rect.getTopLeft()+ vec3(-b, -b, 0));//0
		mesh.addVertex(rect.getTopRight()+ vec3(b, -b, 0));//1
		mesh.addVertex(rect.getBottomRight()+ vec3(b, b, 0));//2
		mesh.addVertex(rect.getBottomLeft()+ vec3(-b, b, 0));//3
		
		for(int i = 0; i < 4; i++){
			mesh.addColor(ofColor(color, 0));
		}
		
		mesh.addVertex(rect.getTopLeft());//4
		mesh.addVertex(rect.getTopRight());//5
		mesh.addVertex(rect.getBottomRight());//6
		mesh.addVertex(rect.getBottomLeft());//7

		for(int i = 0; i < 4; i++){
			mesh.addColor(color);
		}
		
		
		mesh.addIndex(0);
		mesh.addIndex(1);
		mesh.addIndex(4);
		
		mesh.addIndex(4);
		mesh.addIndex(1);
		mesh.addIndex(5);
		
		mesh.addIndex(5);
		mesh.addIndex(1);
		mesh.addIndex(2);
		
		mesh.addIndex(5);
		mesh.addIndex(2);
		mesh.addIndex(6);
		
		mesh.addIndex(6);
		mesh.addIndex(2);
		mesh.addIndex(3);
		
		mesh.addIndex(6);
		mesh.addIndex(3);
		mesh.addIndex(7);
		
		mesh.addIndex(7);
		mesh.addIndex(3);
		mesh.addIndex(0);
		
		mesh.addIndex(7);
		mesh.addIndex(0);
		mesh.addIndex(4);
		
		mesh.addIndex(4);
		mesh.addIndex(5);
		mesh.addIndex(6);
		
		mesh.addIndex(4);
		mesh.addIndex(6);
		mesh.addIndex(7);
		
		
	
	}
	
	ofMesh mesh;

};

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

	
	vector<ofColor> colors;
	
//	vector<bluredRect> rects;
	
	
	ofFbo fbo;
	
	
	
};

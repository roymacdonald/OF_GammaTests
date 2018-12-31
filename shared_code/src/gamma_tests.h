#pragma once
#include "ofMain.h"

//---------------------------------------------------------------------------------------------
inline std::vector<glm::vec3> getGaussianDistribution(float mu, float sigma, float width, size_t numSamples, bool bNormalize = false){
	
	float s2 = 2.0 * sigma * sigma;
	float p_sq = (1.0 / (sigma * sqrt(glm::two_pi<float>())));
	
	std::vector<glm::vec3> gaussian;
	gaussian.resize(numSamples);
	float sum = 0;
	for(int i = 0; i < numSamples; i++ ){
		auto x = ofMap(i, 0, numSamples-1, -width, width);
		x -= mu;
//		gaussian[i] = {x, exp(-(x * x))};
		gaussian[i] = {x, exp(-x * x / s2) * p_sq, 0};
		sum+= gaussian[i].y;
	}
	
	if(bNormalize){
		for(auto & g: gaussian){
			g.y /= sum;
		}
	}
	
	return gaussian;
}
//---------------------------------------------------------------------------------------------
inline std::vector<std::vector<float> > getGaussianKernel(size_t kernelSize, bool bNormalize = true){
	//when bNormalize is set to true the sum of all the kernel elements will be 1
	
	auto gaussian = getGaussianDistribution(0, 1, 3.1, kernelSize);
	std::vector<std::vector<float> > kernel;
	kernel.resize(kernelSize);
	float sum = 0;
	for(int x = 0; x < kernelSize; x++){
		kernel[x].resize(kernelSize);
		for(int y = 0; y < kernelSize; y++){
			kernel[x][y] = gaussian[x].y * gaussian[y].y;
			sum+= kernel[x][y];
		}
	}
	if(bNormalize){
		for(int x = 0; x < kernelSize; x++){
			for(int y = 0; y < kernelSize; y++){
				kernel[x][y] /= sum;
			}
		}
	}
	
	return kernel;
}
//---------------------------------------------------------------------------------------------
inline void decodeGamma(ofImage& img, float gamma){
	auto px = img.getPixels().getData();
	for(size_t i = 0; i < img.getPixels().size(); i++){
		px[i] = pow(px[i]/255.0, gamma) *255;
	} 
	img.update();
}
//---------------------------------------------------------------------------------------------
inline void encodeGamma(ofImage& img, float gamma){
	decodeGamma(img, 1/gamma);
}
//---------------------------------------------------------------------------------------------
inline void drawGammaTestPattern(float brightness = 1.0f, float saturation = 1.0f){

	std::vector<ofColor> colors = {
						ofColor::red,
						ofColor::green,
						ofColor::blue,
						ofColor::yellow,
						ofColor::cyan,
						ofColor::magenta
					};

	float w = ofGetWidth()/(float)colors.size();
	float h = ofGetHeight();
	
	float b = 255*brightness;
	float s = 255*saturation;
	
	for(int i = 0; i < colors.size(); i++){
		ofColor c = colors[i];
		c.setBrightness(b);
		c.setSaturation(s);
		ofSetColor(c);
		ofDrawRectangle(w*i, 0, w, h);	
	}
	
	w = ofGetWidth();
	 h = ofGetHeight()/(float)((colors.size()+1)*3);
	
	for(int i = 0; i < colors.size(); i++){
		ofColor c = colors[i];
		c.setBrightness(b);
		c.setSaturation(s);
		ofSetColor(c);
		ofDrawRectangle(0,(i+1)*3*h, w, h);	
	}
	
}
//---------------------------------------------------------------------------------------------
void renderTestPatternToImg(ofImage & img, float brightness = 1.0f, float saturation = 1.0f){
	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight(),GL_RGBA32F_ARB);
	fbo.begin();
	ofClear(255,255,255,0);
	drawGammaTestPattern(brightness , saturation);
	fbo.end();
	
	
	img.allocate(fbo.getWidth(), fbo.getHeight(), OF_IMAGE_COLOR_ALPHA);
	fbo.readToPixels(img.getPixels());
	img.update();

}	


	




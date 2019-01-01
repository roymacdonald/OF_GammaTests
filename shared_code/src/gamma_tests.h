#pragma once
#include "ofMain.h"

//---------------------------------------------------------------------------------------------

/**
computes and returns a vector with a discretized gausian distribution.

 @param mu this is the mean of the distribution, it sets where the center of the distribution will be. Normally just use zero. 
 @param sigma This is like the width of the gaussian bell. If you use sigma = 1 then the bell will "reach" zero around 3.1 and -3.1. I say "reach" because it will not really reach zero but when this distribution is represented in 8 bits (as used for images) it is at this point where it becomes zero.
 @param width How far away the distribution gets. use 3.1 in this parameter if you want it to reach zero when you convert it into 8 bits.
 @param numSamples The number of samples used you'll get back.
 @param bNormalize If set to true the summ of all the values will be one.
 @return This distribution is returned as a vector of vec3s. The x value is the position at which it is being sampled this distribution. y has the value of the gaussian curve. If you are just interested on the gaussian distribution values, just use the y component of each of the returned elements. z is always zero.
 */
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
/**
 Returns a 2d kernel with a gaussian distribution.
 Parameters for gaussian distribution have been adjusted so that the edges of the kernel are always zero.

 @param kernelSize size of the kernel, the kernel is square so this applies for width and height
 @param bNormalize when set to true it will normalize the kernel so the sum of all of its values are 1.
 @return a 2 dimension vector of floats with the kernel. 
 */
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
/**
 Decodes the gamma from some pixels with gamma encoded pixels, which is usually always the case, into pixels in a linear color space.
 It decodes inplace, so it modiefies the pixels passed.

 @param pix The pixels to decode.
 @param gamma the gamma to decode. The default is 2.2 which is the most common value.
 */
inline void decodeGamma(ofPixels& pix, float gamma = 2.2){
	auto px = pix.getData();
	for(size_t i = 0; i < pix.size(); i++){
		px[i] = pow(px[i]/255.0, gamma) *255;
	} 
}
/**
 Decodes the gamma from an image with gamma encoded pixels, which is usually always the case, into pixels in a linear color space.
 It decodes inplace, so it modiefies the image passed. after modifying the pixels it will update the texture of the image.

 @param pix The image to decode.
 @param gamma the gamma to decode. The default is 2.2 which is the most common value.
 */
inline void decodeGamma(ofImage& img, float gamma = 2.2){
	decodeGamma(img.getPixels(), gamma); 
	img.update();
}
//---------------------------------------------------------------------------------------------
/**
 Encode  some pixels with pixels in linear space, into pixels in a logarithmic color space.
 It encodes inplace, so it modiefies the pixels passed.

 @param pix The pixels to encode.
 @param gamma the gamma to encode. The default is 2.2 which is the most common value.
 
 */
inline void encodeGamma(ofPixels& pix, float gamma = 2.2){
	decodeGamma(pix, 1/gamma);
}
/**
 Encode  some pixels with pixels in linear space, into pixels in a logarithmic color space.
 It encodes inplace, so it modiefies the image passed. After modifying the pixels it will update the texture of the image.

 @param pix The image to encode.
 @param gamma the gamma to encode. The default is 2.2 which is the most common value.
 */
inline void encodeGamma(ofImage& img, float gamma = 2.2){
	encodeGamma(img.getPixels(), gamma);
	img.update();
}
//---------------------------------------------------------------------------------------------
/**
 Draws the test pattern

 @param brightness sets the max brightness of the pattern.
 @param saturation sets the max saturation of the pattern.
 */
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
/**
 renders the testpattern into an ofImage.

 @param img the target image; where the pattern will be stored
 @param brightness sets the max brightness of the pattern.
 @param saturation sets the max saturation of the pattern.
  */
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


	




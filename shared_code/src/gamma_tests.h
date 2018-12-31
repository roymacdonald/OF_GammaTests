#pragma once
#include "ofMain.h"

inline float gaussianDistribution (float x, float mu, float sigma){
	float d = x - mu;
	float n = 1.0 / (sqrt(2 * glm::pi<float>()) * sigma);
	return exp(-d*d/(2 * sigma * sigma)) * n;
}
inline std::vector<glm::vec3> getGaussianDistribution(float mu, float sigma, float width, size_t numSamples){
	
	float s2 = 2.0 * sigma * sigma;
	float p_sq = (1.0 / (sigma * sqrt(glm::two_pi<float>())));
	
	//	z = (tf.exp(tf.negative(tf.pow(x - mean, 2.0) / s2) * p_sq)
	
	std::vector<glm::vec3> gaussian;
	gaussian.resize(numSamples);
	
	for(int i = 0; i < numSamples; i++ ){
		auto x = ofMap(i, 0, numSamples-1, -width, width);
		x -= mu;
//		gaussian[i] = {x, exp(-(x * x))};
		gaussian[i] = {x, exp(-x * x / s2) * p_sq, 0};
	}
	return gaussian;
}

inline std::vector<std::vector<float> > getGaussianKernel(size_t kernelSize){

	
	auto gaussian = getGaussianDistribution(0, 1, 3.1, kernelSize);
	std::vector<std::vector<float> > kernel;
	kernel.resize(kernelSize);
	for(int x = 0; x < kernelSize; x++){
		kernel[x].resize(kernelSize);
		for(int y = 0; y < kernelSize; y++){
			kernel[x][y] = gaussian[x].y * gaussian[y].y;
		}
	}
	return kernel;
}


inline void decodeGamma(ofImage& img, float gamma){
	auto px = img.getPixels().getData();
	for(size_t i = 0; i < img.getPixels().size(); i++){
		px[i] = pow(px[i]/255.0, gamma) *255;
	} 
	img.update();
}
inline void encodeGamma(ofImage& img, float gamma){
	decodeGamma(img, 1/gamma);
}

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

class GaussianKernelCalculator{
public:

	
	
	
	std::vector<glm::vec2> sampleInterval( float sigma, float minInclusive, float maxInclusive, int sampleCount){
		std::vector<glm::vec2> result;
		float stepSize = (maxInclusive - minInclusive) / (sampleCount-1);
		
		for(int s=0; s< sampleCount; ++s){
			float x = minInclusive + s * stepSize;
			float y = gaussianDistribution(x, 0, sigma);
			
			result.push_back({x, y});
		}
		
		return result;
	}
	
	float integrateSimphson( std::vector<glm::vec2> samples){
		float result = samples[0].y + samples[samples.size()-1].y;
		
		for(int s = 1; s < samples.size()-1; ++s){
			float sampleWeight = (s % 2 == 0) ? 2.0 : 4.0;
			result += sampleWeight * samples[s].y;
		}
		
		float h = (samples[samples.size()-1].x - samples[0].x) / (samples.size()-1);
		return result * h / 3.0;
	}
	
	float roundTo( float num, float decimals){
		auto shift = pow(10, decimals);
		return round(num * shift) / shift;
	}
	
	std::vector<float>  updateKernel(float sigma, size_t kernelSize){
		size_t sampleCount = 1000;
		int samplesPerBin = ceil(sampleCount / (float) kernelSize);
		if(samplesPerBin % 2 == 0){ // need an even number of intervals for simpson integration => odd number of samples
			++samplesPerBin;
		}
		float weightSum = 0;
		float kernelLeft = -floor(kernelSize/2.0);
		
		
		// get samples left and right of kernel support first
		auto outsideSamplesLeft  = sampleInterval(sigma, -5 * sigma, kernelLeft - 0.5, samplesPerBin);
		auto outsideSamplesRight = sampleInterval(sigma, -kernelLeft+0.5, 5 * sigma, samplesPerBin);
		
//		var allSamples = [[outsideSamplesLeft, 0]];
		
		// now sample kernel taps and calculate tap weights
		std::vector<float> weights;
		weights.push_back(0);
		
		for(int tap=0; tap<kernelSize; ++tap)
		{
			float left = kernelLeft - 0.5 + tap;
			
			auto tapSamples = sampleInterval(sigma, left, left+1, samplesPerBin);
			auto tapWeight = integrateSimphson(tapSamples);
			
//			allSamples.push([tapSamples, tapWeight]);
			weights.push_back(tapWeight);
			weightSum += tapWeight;
		}
		weights.push_back(0);
		
//		allSamples.push([outsideSamplesRight, 0]);
		
		// renormalize kernel and round to 6 decimals
		
		for(int i=0; i<weights.size(); ++i){
			weights[i] /= weightSum;
			//			weights[i] = roundTo(weights[i] / weightSum, 6);
//			std::cout << weights[i] << std::endl; 
		}
		return weights;
		
	}

};

	




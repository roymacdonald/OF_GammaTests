# OF_GammaTests

Testing different approaches to process gamma properly on openFrameworks 


## intro

In short, on electronic devices that either capture or display images, [gamma](https://en.wikipedia.org/wiki/Gamma_correction) is the curve that relates the stored data and the light intensity that it represents. Human beings see in a sort of logarithmic way while electronic devices do it on a more linear fashion, so gamma correction needs to be done.

After reading [this article](https://blog.johnnovak.net//2016/09/21/what-every-coder-should-know-about-gamma/) I realized that openFrameworks was not handling gamma properly and as an extent neither was I. Considering that image processing is an important feature of openFrameworks, processing gamma properly then is an important thing.
This repository contains various test for propper gamma handling, via cpu, gpu (using openGl calls and via custom shaders). also it contains some tests for openCV (for which gamma is handled incorrectly :O )


## Testing
A very simple test in order to see if the gamma is processed correcly is to apply a gaussian blur.

The following test pattern will be used:
![TestPattern](testPattern.png)

If we apply a gaussian blur to it we would expect the following, which is what is achieved when gamma is handled properly
![testPattern blured GammaCorrected](testPattern_blured_GammaCorrected.png)

Yet, when gamma is not handled properly you get the following image, with this noticeable darker banding. This is what you currently get in OF when simply applying a gaussian blur, regardless of doing it via cpu, via gpu or via opencv.

![testPattern blured NO GammaCorrection](testPattern_blured_noGammaCorrection.png)

## Compatibility
This tests wede done using the latest gitbub master, although it should work with openFrameworks version 0.10 onwards.

## Instalation
Just place this folder inside the `[your openFramwoks folder]/apps` folder. Create or update projects if needed using the Project Generator.

## Dependencies
Some of the test need you to have installed the following addons

* [ofxCv](https://github.com/kylemcdonald/ofxCv)

## test apps

There are a few test apps here. So far
### test1_testPattern
This is just an app for creating the test pattern. It does nothing more than that and draws it to the screen.
### test2_cvGaussianBlur
Applies opencv's gaussian blur via ofxCv and allows for gamma correction before and after applying the blur. Gamma correction is cpu based done via helper functions in `shared_code/src/gamma_tests.h`.
### test3_cvGaussianBlur_GL_sRGB
Same as previous; applies opencv's gaussian blur via ofxCv and allows for gamma correction before and after applying the blur, but in this case gamma correction is done using openGl's `glEnable(GL_FRAMEBUFFER_SRGB);`
### test4_Shader_gaussianBlur
Gaussian blur applied via shader and gamma correction is done via cpu, using helper functions in `shared_code/src/gamma_tests.h`.

### test5_Shader_gaussianBlur_inShaderCorrection
Gaussian blur and gamma correction applied via shader.

### test_gaussiancurve
Just testing the right algorithm and parameters for creating a gaussian distribution curve and kernel. Used for the shader based gaussian blur. 

### shared_code
Some helper functions used along all this tests.
These are:

#### std::vector< glm::vec3 > getGaussianDistribution(float mu, float sigma, float width, size_t numSamples, bool bNormalize 
computes and returns a vector with a discretized gausian distribution.

Parameters:

* **mu** this is the mean of the distribution, it sets where the center of the distribution will be. Normally just use zero. 
* **sigma** This is like the width of the gaussian bell. If you use sigma = 1 then the bell will "reach" zero around 3.1 and -3.1. I say "reach" because it will not really reach zero but when this distribution is represented in 8 bits (as used for images) it is at this point where it becomes zero.
* **width** How far away the distribution gets. use 3.1 in this parameter if you want it to reach zero when you convert it into 8 bits.
* **numSamples** The number of samples used you'll get back.
* **bNormalize** If set to true the summ of all the values will be one.
@return This distribution is returned as a vector of vec3s. The x value is the position at which it is being sampled this distribution. y has the value of the gaussian curve. If you are just interested on the gaussian distribution values, just use the y component of each of the returned elements. z is always zero.

#### std::vector< std::vector< float > > getGaussianKernel(size_t kernelSize, bool bNormalize = true)
Returns a 2d kernel with a gaussian distribution.
The parameters for gaussian distribution have been adjusted so that the edges of the kernel are always zero.

Parameters:

* **kernelSize** size of the kernel, the kernel is square so this applies for width and height
* **bNormalize** when set to true it will normalize the kernel so the sum of all of its values are 1.
@return a 2 dimension vector of floats with the kernel. 

#### void decodeGamma(ofPixels& pix, float gamma = 2.2)
Decodes the gamma from some pixels with gamma encoded pixels, which is usually always the case, into pixels in a linear color space.
It decodes inplace, so it modiefies the pixels passed.

Parameters:

* **pix** The pixels to decode.
* **gamma** the gamma to decode. The default is 2.2 which is the most common value.

#### void decodeGamma(ofImage& img, float gamma = 2.2)
Decodes the gamma from an image with gamma encoded pixels, which is usually always the case, into pixels in a linear color space.
It decodes inplace, so it modiefies the image passed. after modifying the pixels it will update the texture of the image.

Parameters:

* **pix** The image to decode.
* **gamma** the gamma to decode. The default is 2.2 which is the most common value.

#### void encodeGamma(ofPixels& pix, float gamma = 2.2)
Encode  some pixels with pixels in linear space, into pixels in a logarithmic color space.
It encodes inplace, so it modiefies the pixels passed.

Parameters:

* **pix** The pixels to encode.
* **gamma** the gamma to encode. The default is 2.2 which is the most common value.

#### void encodeGamma(ofImage& img, float gamma = 2.2){
Encode  some pixels with pixels in linear space, into pixels in a logarithmic color space.
It encodes inplace, so it modiefies the image passed. After modifying the pixels it will update the texture of the image.

Parameters:

* **pix** The image to encode.
* **gamma** the gamma to encode. The default is 2.2 which is the most common value.

#### void drawGammaTestPattern(float brightness = 1.0f, float saturation = 1.0f)
Draws the test pattern

Parameters:

* **brightness** sets the max brightness of the pattern.
* **saturation** sets the max saturation of the pattern.

#### void renderTestPatternToImg(ofImage & img, float brightness = 1.0f, float saturation = 1.0f){
renders the testpattern into an ofImage.

Parameters:

* **img** the target image; where the pattern will be stored
* **brightness** sets the max brightness of the pattern.
* **saturation** sets the max saturation of the pattern.

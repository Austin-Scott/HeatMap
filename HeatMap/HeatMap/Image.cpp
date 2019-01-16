#include "Image.h"

using namespace std;

Image::Image(unsigned int width, unsigned int height, unsigned char * data)
{
	this->data = data;
	this->width = width;
	this->height = height;
}

Image::Image(unsigned int width, unsigned int height, Color fillColor)
{
	this->width = width;
	this->height = height;
	data = new unsigned char[width*height * 4];
	for (int i = 0; i < width*height * 4; i += 4) {
		data[i] = fillColor.getR();
		data[i+1] = fillColor.getG();
		data[i+2] = fillColor.getB();
		data[i+3] = fillColor.getA();
	}
}

Image::Image(std::string filename)
{
	vector<unsigned char> dataBuffer;
	unsigned int error = lodepng::decode(dataBuffer, width, height, filename);
	if (error) {
		cout << "lodepng error " << error << ": " << lodepng_error_text(error) << endl;
	}
	else {
		data = new unsigned char[dataBuffer.size()];
		for (int i = 0; i < dataBuffer.size(); i++) {
			data[i] = dataBuffer[i];
		}
	}
}

Image::~Image()
{
	if (data != nullptr) {
		delete[] data;
		data = nullptr;
	}
}

unsigned int Image::getWidth()
{
	return width;
}

unsigned int Image::getHeight()
{
	return height;
}

Color Image::getPixel(int x, int y)
{
	if (x < width && y < height) {
		int offset = (y*width * 4) + (x * 4);
		return Color(data[offset], data[offset + 1], data[offset + 2], data[offset + 3]);
	}
	else {
		return Color(0,0,0,0);
	}
}

string Image::saveImage(string filename)
{
	vector<unsigned char> png;
	unsigned int error = lodepng::encode(png, data, width, height);
	if (!error) 
		error = lodepng::save_file(png, filename.c_str());
	if (error) {
		return "lodepng error " + to_string(error) + ": " + string(lodepng_error_text(error));
	}
	return "";
}

Image * Image::overlayImage(Image * other, unsigned char alpha)
{
	if (width != other->getWidth() || height != other->getHeight())
		return nullptr;

	unsigned char* result = new unsigned char[width*height * 4];
	int index = 0;

	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Color blendedColor = getPixel(x, y).blend(other->getPixel(x, y), alpha);
			result[index] = blendedColor.getR();
			result[index+1] = blendedColor.getG();
			result[index+2] = blendedColor.getB();
			result[index+3] = blendedColor.getA();

			index += 4;
		}
	}
	return new Image(width, height, result);
}

Image * Image::addImage(Image * other)
{
	if (width != other->getWidth() || height != other->getHeight())
		return nullptr;

	unsigned char* result = new unsigned char[width*height * 4];
	int index = 0;


	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Color myColor = getPixel(x, y);
			Color theirColor = other->getPixel(x, y);
			result[index] = min(myColor.getR() + theirColor.getR(), 255);
			result[index + 1] = min(myColor.getG() + theirColor.getG(), 255);
			result[index + 2] = min(myColor.getB() + theirColor.getB(), 255);
			result[index + 3] = min(myColor.getA() + theirColor.getA(), 255);

			index += 4;
		}
	}
	return new Image(width, height, result);
}

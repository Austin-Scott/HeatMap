#include "Image.h"

using namespace std;

Image::Image(unsigned int width, unsigned int height, unsigned char * data)
{
	this->data = data;
	this->width = width;
	this->height = height;
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

Color Image::getPixel(unsigned int x, unsigned int y)
{
	if (x < width && y < height) {
		int offset = (y*width * 4) + (x * 4);
		return Color(data[offset], data[offset + 1], data[offset + 2], data[offset + 3]);
	}
	else {
		return Color();
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

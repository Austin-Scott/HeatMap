#include "Image.h"

using namespace std;

Image::Image(int width, int height, unsigned char * data)
{
	this->data = data;
	this->width = width;
	this->height = height;
}

Image::~Image()
{
	if (data != nullptr) {
		delete[] data;
		data = nullptr;
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

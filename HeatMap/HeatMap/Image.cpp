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

void Image::saveImage(string filename)
{
	vector<unsigned char> png;
	unsigned int error = lodepng::encode(png, data, width, height);
	if (!error) lodepng::save_file(png, filename.c_str());
	if (error) {
		cerr << "lodepng error " << error << ": " << lodepng_error_text(error) << endl;
	}
}

#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "../../lodepng/lodepng.h"
#include "Color.h"

class Image {
private:
	unsigned int width;
	unsigned int height;
	unsigned char* data;
public:
	Image(unsigned int width, unsigned int height, unsigned char* data);
	Image(std::string filename);
	~Image();
	Color getPixel(unsigned int x, unsigned int y);
	std::string saveImage(std::string filename);
};
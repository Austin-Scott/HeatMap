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
	Image(unsigned int width, unsigned int height, Color fillColor);
	Image(std::string filename);
	~Image();
	unsigned int getWidth();
	unsigned int getHeight();
	Color getPixel(int x, int y);
	std::string saveImage(std::string filename);
	Image* overlayImage(Image* other, unsigned char alpha=255);
	Image* addImage(Image* other);
};
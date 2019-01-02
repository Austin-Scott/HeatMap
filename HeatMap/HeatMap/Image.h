#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "../../lodepng/lodepng.h"

class Image {
private:
	int width;
	int height;
	unsigned char* data;
public:
	Image(int width, int height, unsigned char* data);
	~Image();
	std::string saveImage(std::string filename);
};
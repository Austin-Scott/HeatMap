#pragma once
#include <string>

class Color {
private:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	unsigned char twoDigitHexToChar(std::string hex);
	unsigned char lerp(unsigned char a, unsigned char b, double alpha);
public:
	Color();
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	Color(std::string hexCode);
	Color blend(Color other);
	Color lerp(Color other, double alpha);
	unsigned char getR();
	unsigned char getG();
	unsigned char getB();
	unsigned char getA();
};
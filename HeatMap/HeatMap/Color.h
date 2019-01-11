#pragma once
#include <string>
#include <regex>

using namespace std;

class Color {
private:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	string charToHexCode(unsigned char c);
	unsigned char twoDigitHexToChar(std::string hex);
	unsigned char lerp(unsigned char a, unsigned char b, double alpha);
	unsigned char blend(unsigned char top, unsigned char bottom, double topAlpha, double bottomAlpha);
public:
	Color();
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	Color(std::string hexCode);
	Color blend(Color other);
	Color lerp(Color other, double alpha);
	string toHex();
	unsigned char getR();
	unsigned char getG();
	unsigned char getB();
	unsigned char getA();
	void setR(unsigned char v);
	void setG(unsigned char v);
	void setB(unsigned char v);
	void setA(unsigned char v);
};
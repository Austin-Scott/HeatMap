#include "Color.h"

string Color::charToHexCode(unsigned char c)
{
	int leastSig = c % 16;
	int mostSig = c / 16;
	string firstChar;
	string secondChar;
	if (mostSig < 10) {
		firstChar.push_back('0' + mostSig);
	}
	else {
		firstChar.push_back('A' + (mostSig - 10));
	}
	if (leastSig < 10) {
		secondChar.push_back('0' + leastSig);
	}
	else {
		secondChar.push_back('A' + (leastSig - 10));
	}
	return firstChar + secondChar;
}

unsigned char Color::twoDigitHexToChar(std::string hex)
{
	unsigned char result = 0;
	for (int i = 0; i < 2; i++) {
		unsigned char value = 0;
		if (hex[i] >= '0'&&hex[i] <= '9') {
			value = hex[i] - '0';
		}
		else if (hex[i] >= 'a'&&hex[i] <= 'f') {
			value = (hex[i] - 'a') + 10;
		}
		else if (hex[i] >= 'A'&&hex[i] <= 'F') {
			value = (hex[i] - 'A') + 10;
		}

		result += i == 0 ? value * 16 : value;
	}
	return result;
}

unsigned char Color::lerp(unsigned char a, unsigned char b, double alpha)
{
	if (alpha <= 0.0) return a;
	if (alpha >= 1.0) return b;
	return (unsigned char)(((1.0 - alpha)*(double)a) + (alpha*(double)b));
}

double toDouble(unsigned char value) {
	return (double)value / 255.0;
}

unsigned char Color::blend(unsigned char top, unsigned char bottom, double topAlpha, double bottomAlpha)
{
	return (toDouble(top)*topAlpha + toDouble(bottom)*bottomAlpha*(1.0 - topAlpha)) * 255;
}

Color::Color()
{
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(std::string hexCode)
{
	if (hexCode.length() == 9 && regex_match(hexCode, regex("#[0-9a-fA-F]{8}"))) {
		unsigned char R = 0, G = 0, B = 0, A = 0;
		R = twoDigitHexToChar(hexCode.substr(1, 2));
		G = twoDigitHexToChar(hexCode.substr(3, 2));
		B = twoDigitHexToChar(hexCode.substr(5, 2));
		A = twoDigitHexToChar(hexCode.substr(7, 2));
		this->r = R;
		this->g = G;
		this->b = B;
		this->a = A;
	}
	else {
		Color();
	}
}

Color Color::getInverse(bool includeAlpha)
{
	if(includeAlpha)
		return Color(255 - r, 255 - g, 255 - b, 255-a);

	return Color(255-r, 255-g, 255-b, 255);
}

string Color::toHex() {
	return "#" + charToHexCode(r) + charToHexCode(g) + charToHexCode(b) + charToHexCode(a);
}

Color Color::blend(Color other)
{
	
	double alpha = 1.0-((double)a / 255.0);
	if (other.getA()!=255) {
		return lerp(other, alpha);
	}
	else {
		Color color = lerp(other, alpha);
		return Color(color.getR(), color.getG(), color.getB(), 255);
	}
	
	/*
	double topAlpha = (double)a / 255.0;
	double bottomAlpha = (double)other.getA() / 255.0;
	return Color(blend(r, other.getR(), topAlpha, bottomAlpha), blend(g, other.getG(), topAlpha, bottomAlpha), blend(b, other.getB(), topAlpha, bottomAlpha), bottomAlpha!=1.0?min(topAlpha + bottomAlpha * (1.0 - topAlpha), 1.0) * 255:255);
	*/
}

Color Color::lerp(Color other, double alpha)
{
	return Color(lerp(r,other.getR(), alpha),lerp(g,other.getG(),alpha),lerp(b,other.getB(), alpha), lerp(a, other.getA(), alpha));
}

unsigned char Color::getR()
{
	return r;
}

unsigned char Color::getG()
{
	return g;
}

unsigned char Color::getB()
{
	return b;
}

unsigned char Color::getA()
{
	return a;
}

void Color::setR(unsigned char v)
{
	r = v;
}

void Color::setG(unsigned char v)
{
	g = v;
}

void Color::setB(unsigned char v)
{
	b = v;
}

void Color::setA(unsigned char v)
{
	a = v;
}

#include "HeatMap.h"

#include <math.h>

using namespace std;

double HeatMap::latToDoubleY(double lat)
{
	return (1.0-((lat - lowerLeft.getLat()) / (upperRight.getLat() - lowerLeft.getLat())))*(double)height;
}

double HeatMap::lonToDoubleX(double lon)
{
	return ((lon - lowerLeft.getLon()) / (upperRight.getLon() - lowerLeft.getLon()))*(double)width;
}

int HeatMap::latToIntY(double lat)
{
	return toCoordInt(latToDoubleY(lat));
}

int HeatMap::lonToIntX(double lon)
{
	return toCoordInt(lonToDoubleX(lon));
}

void HeatMap::drawPoint(int x, int y, double c)
{
	if (x > 0 && x <= width && y > 0 && y <= height) {
		cells[x - 1][y - 1].addToValue(c);
	}
}

int HeatMap::toCoordInt(double value)
{
	return (int)floor(value) + 1;
}

void HeatMap::swap(double & a, double & b)
{
	double temp = a;
	a = b;
	b = temp;
}

double HeatMap::fpart(double value)
{
	return value - floor(value);
}

double HeatMap::rfpart(double value)
{
	return 1 - fpart(value);
}

double HeatMap::sign(double value)
{
	if (value < 0.0) return -1.0;
	if (value > 0.0) return 1.0;
	return 0.0;
}

void HeatMap::xiaolinWu(GeographicCoordinate from, GeographicCoordinate to)
{
	double x0 = lonToDoubleX(from.getLon());
	double y0 = latToDoubleY(from.getLat());
	double x1 = lonToDoubleX(to.getLon());
	double y1 = latToDoubleY(to.getLat());
	bool steep = abs(to.getLat() - from.getLat()) > abs(to.getLon() - from.getLon());
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	double dx = x1 - x0;
	double dy = y1 - y0;

	double gradient = 1.0;
	if (dx != 0.0) {
		gradient = dy / dx;
	}

	double xend = round(x0);
	double yend = y0 + gradient * (xend - x0);
	double xgap = rfpart(x0 + 0.5);
	int xpxl1 = (int)xend;
	int ypxl1 = (int)floor(yend);
	if (steep) {
		drawPoint(ypxl1, xpxl1, rfpart(yend)*xgap);
		drawPoint(ypxl1 + 1, xpxl1, fpart(yend)*xgap);
	}
	else {
		drawPoint(xpxl1, ypxl1, rfpart(yend)*xgap);
		drawPoint(xpxl1, ypxl1 + 1, fpart(yend)*xgap);
	}
	double intery = yend + gradient;

	xend = round(x1);
	yend = y1 + gradient * (xend - x1);
	xgap = fpart(x1 + 0.5);
	int xpxl2 = (int)xend;
	int ypxl2 = (int)floor(yend);
	if (steep) {
		drawPoint(ypxl2, xpxl2, rfpart(yend)*xgap);
		drawPoint(ypxl2 + 1, xpxl2, fpart(yend)*xgap);
	}
	else {
		drawPoint(xpxl2, ypxl2, rfpart(yend)*xgap);
		drawPoint(xpxl2, ypxl2 + 1, fpart(yend)*xgap);
	}

	if (steep) {
		for (int x = xpxl1 + 1; x < xpxl2; x++) {
			drawPoint((int)floor(intery), x, rfpart(intery));
			drawPoint((int)floor(intery) + 1, x, fpart(intery));
			intery += gradient;
		}
	}
	else {
		for (int x = xpxl1 + 1; x < xpxl2; x++) {
			drawPoint(x, (int)floor(intery), rfpart(intery));
			drawPoint(x, (int)floor(intery) + 1, fpart(intery));
			intery += gradient;
		}
	}
}

void HeatMap::bresenham(GeographicCoordinate from, GeographicCoordinate to)
{
	int x0 = (int)lonToDoubleX(from.getLon());
	int y0 = (int)latToDoubleY(from.getLat());
	int x1 = (int)lonToDoubleX(to.getLon());
	int y1 = (int)latToDoubleY(to.getLat());

	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) {
			plotLineLow(x1, y1, x0, y0);
		}
		else {
			plotLineLow(x0, y0, x1, y1);
		}
	}
	else {
		if (y0 > y1) {
			plotLineHigh(x1, y1, x0, y0);
		}
		else {
			plotLineHigh(x0, y0, x1, y1);
		}
	}
}

void HeatMap::plotLineLow(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = y0;
	for (int x = x0; x < x1; x++) {
		drawPoint(x, y, 1.0);
		if (D > 0) {
			y = y + yi;
			D = D - (2 * dx);
		}
		D = D + (2 * dy);
	}
}

void HeatMap::plotLineHigh(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}
	int D = 2 * dx - dy;
	int x = x0;
	for (int y = y0; y < y1; y++) {
		drawPoint(x, y, 1.0);
		if (D > 0) {
			x = x + xi;
			D = D - 2 * dy;
		}
		D = D + 2 * dx;
	}
}

HeatMap::HeatMap(int width, int height, GeographicCoordinate lowerLeft, GeographicCoordinate upperRight)
{
	this->width = width;
	this->height = height;
	this->lowerLeft = lowerLeft;
	this->upperRight = upperRight;

	cells = new HeatMapCell*[width];
	for (int i = 0; i < width; i++) {
		cells[i] = new HeatMapCell[height];
	}
}

HeatMap::~HeatMap()
{
	for (int i = 0; i < width; i++) {
		delete[] cells[i];
	}
	delete[] cells;
}

void HeatMap::addActivity(Activity & activity)
{
	if (activity.getTrack().size() > 1) {
		cout << "\t*Recording activity on Heat Map..." << endl;
		for (int i = 0; i < activity.getTrack().size() - 1; i++) {
			drawLine(activity.getTrack()[i], activity.getTrack()[i + 1], true);
		}
	}
}

void HeatMap::normalizeMap()
{
	vector<HeatMapCell*> nonZeroCells;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			HeatMapCell* currentCell = &cells[x][y];
			if (currentCell->getValue() > 0.0) {
				nonZeroCells.push_back(currentCell);
			}
			else {
				currentCell->setNormalizedValue(0.0);
			}
		}
	}
	sort(nonZeroCells.begin(), nonZeroCells.end(), [](HeatMapCell* a, HeatMapCell* b) {return a->getValue() < b->getValue(); });
	int topOfTier = -1;
	for (int i = 0; i < nonZeroCells.size(); i++) {
		if (topOfTier < i) {
			topOfTier = i;
			while (topOfTier < nonZeroCells.size() - 1 && nonZeroCells[topOfTier] == nonZeroCells[topOfTier + 1]) {
				topOfTier++;
			}
		}

		nonZeroCells[i]->setNormalizedValue((double)(topOfTier+1) / (double)nonZeroCells.size());
	}
}

Image* HeatMap::renderImage(Color backgroundColor, Color minimumActivityColor, Color maximumActivityColor)
{
	unsigned char* data = new unsigned char[width * height * 4];
	int index = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			double normalizedValue = cells[x][y].getNormalizedValue();
			if (normalizedValue <= 0.0) {
				data[index] = backgroundColor.getR();
				data[index+1] = backgroundColor.getG();
				data[index+2] = backgroundColor.getB();
				data[index+3] = backgroundColor.getA();
			}
			else {
				Color rawColor = minimumActivityColor.lerp(maximumActivityColor, normalizedValue);
				if (rawColor.getA() != 255) {
					rawColor = rawColor.blend(backgroundColor);
				}
				data[index] = rawColor.getR();
				data[index + 1] = rawColor.getG();
				data[index + 2] = rawColor.getB();
				data[index + 3] = rawColor.getA();
			}

			index += 4;
		}
	}
	return new Image(width, height, data);
}

void HeatMap::debugDrawCells()
{
	double min = cells[0][0].getValue();
	double max = min;
	double sum = 0.0;
	int n = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			double value = cells[x][y].getValue();
			if (value == 0.0) {
				cout << " ";
			} else if (value < 0.5) {
				cout << "`";
			} else if (value < 1.0) {
				cout << "*";
			}
			else {
				cout << "+";
			}
			if (value != 0.0) {
				if (value > max) max = value;
				if (value < min) min = value;
				sum += value;
				n++;
			}
		}
		cout << endl;
	}
	cout << "Max cell value: " << max << endl;
	cout << "Min cell value: " << min << endl;
	cout << "Average nonzero cell value: " << sum / (double)n << endl;
}

void HeatMap::drawLine(GeographicCoordinate from, GeographicCoordinate to, bool smooth)
{
	if (smooth) {
		xiaolinWu(from, to);
	}
	else {
		bresenham(from, to);
	}
}

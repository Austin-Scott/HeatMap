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

void HeatMap::addActivity(TrainingCenterXML & activity)
{
	for (int i = 0; i < activity.getTrack().size() - 1; i++) {
		drawLine(activity.getTrack()[i], activity.getTrack()[i + 1]);
	}
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

void HeatMap::drawLine(GeographicCoordinate from, GeographicCoordinate to)
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

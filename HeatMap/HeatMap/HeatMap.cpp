#include "HeatMap.h"

#include <math.h>

using namespace std;

double HeatMap::DEG2RAD(double a)
{
	return (a / (180.0 / M_PI));
}

double HeatMap::RAD2DEG(double a)
{
	return (a*(180.0 / M_PI));
}

double HeatMap::latToWorldY(double lat)
{
	return RAD2DEG(log(tan(DEG2RAD(lat) / 2.0 + M_PI / 4.0)));
}

double HeatMap::lonToWorldX(double lon)
{
	return lon;
}

double HeatMap::worldYToLat(double y)
{
	return RAD2DEG(atan(exp(DEG2RAD(y))) * 2.0 - M_PI / 2.0);
}

double HeatMap::worldXToLon(double x)
{
	return x;
}

double HeatMap::latToDoubleY(double lat)
{
	return (1.0-((latToWorldY(lat) - latToWorldY(configuration.lowerLeft.getLat())) / (latToWorldY(configuration.upperRight.getLat()) - latToWorldY(configuration.lowerLeft.getLat()))))*(double)configuration.height;
}

double HeatMap::lonToDoubleX(double lon)
{
	return ((lonToWorldX(lon) - lonToWorldX(configuration.lowerLeft.getLon())) / (lonToWorldX(configuration.upperRight.getLon()) - lonToWorldX(configuration.lowerLeft.getLon())))*(double)configuration.width;
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
	if (x > 0 && x <= configuration.width && y > 0 && y <= configuration.height) {
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

HeatMap::HeatMap(HeatMapConfiguration configuration)
{
	this->configuration = configuration;

	cells = new HeatMapCell*[this->configuration.width];
	for (int i = 0; i < this->configuration.width; i++) {
		cells[i] = new HeatMapCell[this->configuration.height];
	}
}

HeatMap::~HeatMap()
{
	for (int i = 0; i < configuration.width; i++) {
		delete[] cells[i];
	}
	delete[] cells;
}

void HeatMap::addActivity(Activity & activity)
{
	if (includeActivity(activity, configuration) && activity.getTrack().size() > 1) {
		for (int i = 0; i < activity.getTrack().size() - 1; i++) {
			drawLine(activity.getTrack()[i], activity.getTrack()[i + 1], configuration.useAntiAliasing);
		}
	}
	for (int x = 0; x < configuration.width; x++) {
		for (int y = 0; y < configuration.height; y++) {
			cells[x][y].incrementCounter();
		}
	}
}

void HeatMap::normalizeMap()
{
	vector<HeatMapCell*> nonZeroCells;
	for (int x = 0; x < configuration.width; x++) {
		for (int y = 0; y < configuration.height; y++) {
			HeatMapCell* currentCell = &cells[x][y];
			if (currentCell->getValue() > 0.0 && currentCell->getActivities()>1) {
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

Image* HeatMap::renderImage(Image* backgroundImage)
{
	unsigned char* data = new unsigned char[configuration.width * configuration.height * 4];
	int index = 0;
	for (int y = 0; y < configuration.height; y++) {
		for (int x = 0; x < configuration.width; x++) {
			double normalizedValue = cells[x][y].getNormalizedValue();
			int activities = cells[x][y].getActivities();
			double value = cells[x][y].getValue();
			if (activities==0) {
				if (backgroundImage == nullptr) {
					data[index] = configuration.backgroundColor.getR();
					data[index + 1] = configuration.backgroundColor.getG();
					data[index + 2] = configuration.backgroundColor.getB();
					data[index + 3] = configuration.backgroundColor.getA();
				}
				else {
					Color background = backgroundImage->getPixel(x, y);
					data[index] = background.getR();
					data[index + 1] = background.getG();
					data[index + 2] = background.getB();
					data[index + 3] = background.getA();
				}
			}
			else if(activities>1) {
				Color rawColor = configuration.minimumActivityColor.lerp(configuration.maximumActivityColor, normalizedValue);
				if (rawColor.getA() != 255) {
					if (backgroundImage != nullptr) {
						rawColor = rawColor.blend(backgroundImage->getPixel(x, y));
					}
					else {
						rawColor = rawColor.blend(configuration.backgroundColor);
					}
				}
				data[index] = rawColor.getR();
				data[index + 1] = rawColor.getG();
				data[index + 2] = rawColor.getB();
				data[index + 3] = rawColor.getA();
			}
			else {
				double clampedValue = min(max(value, 0.0), 1.0);
				Color backgroundPixel = backgroundImage != nullptr ? backgroundImage->getPixel(x, y) : configuration.backgroundColor;
				Color rawColor = configuration.minimumActivityColor;
				rawColor.setA(rawColor.getA()*clampedValue);
				Color resultColor = rawColor.blend(backgroundPixel);
				data[index] = resultColor.getR();
				data[index + 1] = resultColor.getG();
				data[index + 2] = resultColor.getB();
				data[index + 3] = resultColor.getA();
			}

			index += 4;
		}
	}
	return new Image(configuration.width, configuration.height, data);
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

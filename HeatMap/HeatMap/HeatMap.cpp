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

long HeatMap::pascalValue(int row, int col)
{
	if (row < 0) return 0;
	if (col<0 || col>row) return 0;
	if (col == 0 || col == row) return 1;
	return pascalValue(row - 1, col - 1) + pascalValue(row - 1, col);
}

vector<long> HeatMap::getPascalTriangleRow(int length)
{
	vector<long> result;
	for (int i = 0; i < length; i++) {
		result.push_back(pascalValue(length - 1, i));
	}
	return result;
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
			drawLine(activity.getTrack()[i], activity.getTrack()[i + 1]);
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
	sort(nonZeroCells.begin(), nonZeroCells.end(), [](HeatMapCell* a, HeatMapCell* b) {return a->getActivities() < b->getActivities(); });
	int topOfTier = -1;
	for (int i = 0; i < nonZeroCells.size(); i++) {
		if (topOfTier < i) {
			topOfTier = i;
			while (topOfTier < nonZeroCells.size() - 1 && nonZeroCells[topOfTier]->getActivities() == nonZeroCells[topOfTier + 1]->getActivities()) {
				topOfTier++;
			}
		}

		nonZeroCells[i]->setNormalizedValue((double)(topOfTier+1) / (double)nonZeroCells.size());
	}
}

Image* HeatMap::renderImage()
{
	unsigned char* data = new unsigned char[configuration.width * configuration.height * 4];
	int index = 0;
	for (int y = 0; y < configuration.height; y++) {
		for (int x = 0; x < configuration.width; x++) {
			double normalizedValue = cells[x][y].getNormalizedValue();
			int activities = cells[x][y].getActivities();
			double value = cells[x][y].getValue();
			if (activities==0) {
				data[index] = 0;
				data[index + 1] = 0;
				data[index + 2] = 0;
				data[index + 3] = 0;
			}
			else if(activities>1) {
				Color rawColor;
				if (normalizedValue < 0.33) {
					double alpha = normalizedValue / 0.33;
					rawColor = configuration.minimumActivityColor.lerp(configuration.activity33Color, alpha);
				}
				else if (normalizedValue < 0.66) {
					double alpha = (normalizedValue- 0.33) / 0.33;
					rawColor = configuration.activity33Color.lerp(configuration.activity66Color, alpha);
				}
				else {
					double alpha = (normalizedValue- 0.66) / 0.33;
					rawColor = configuration.activity66Color.lerp(configuration.maximumActivityColor, alpha);
				}

				rawColor.setA(rawColor.getA()*(value / (double)activities));

				data[index] = rawColor.getR();
				data[index + 1] = rawColor.getG();
				data[index + 2] = rawColor.getB();
				data[index + 3] = rawColor.getA();
			}
			else {
				double clampedValue = min(max(value, 0.0), 1.0);
				Color rawColor = configuration.minimumActivityColor;
				rawColor.setA(rawColor.getA()*clampedValue);
				data[index] = rawColor.getR();
				data[index + 1] = rawColor.getG();
				data[index + 2] = rawColor.getB();
				data[index + 3] = rawColor.getA();
			}

			index += 4;
		}
	}
	return new Image(configuration.width, configuration.height, data);
}

Image * HeatMap::createGlowImage(Image * renderedImage, int diameter)
{
	if (configuration.width != renderedImage->getWidth() || configuration.height != renderedImage->getHeight())
		return nullptr;

	unsigned char* result = new unsigned char[renderedImage->getWidth()*renderedImage->getHeight() * 4];
	int width = diameter %2==1? diameter : diameter +1;
	vector<long> pascalRow = getPascalTriangleRow(width);
	vector<vector<long>> gaussianKernel(width, vector<long>(width, 0));
	long long kernalSum = 0;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < width; y++) {
			long value = pascalRow[x] * pascalRow[y];
			kernalSum += value;
			gaussianKernel[x][y] = value;
		}
	}
	int index = 0;
	int radius = diameter / 2;
	for (int y = 0; y < configuration.height; y++) {
		for (int x = 0; x < configuration.width; x++) {
			long sumR = 0;
			long sumG = 0;
			long sumB = 0;
			long sumA = 0;
			for (int gx = 0; gx < diameter; gx++) {
				for (int gy = 0; gy < diameter; gy++) {
					int sampleX = (x - radius) + gx;
					int sampleY = (y - radius) + gy;
					Color sample = renderedImage->getPixel(sampleX, sampleY);
					double weight = 0.0;
					if (sampleX >= 0 && sampleX < configuration.width&&sampleY >= 0 && sampleY < configuration.height) {
						weight = cells[sampleX][sampleY].getNormalizedValue();
					}
					/*
					sumR += (int)((double)sample.getR()*(double)gaussianKernel[gx][gy] * weight);
					sumG += (int)((double)sample.getG()*(double)gaussianKernel[gx][gy] * weight);
					sumB += (int)((double)sample.getB()*(double)gaussianKernel[gx][gy] * weight);
					sumA += (int)((double)sample.getA()*(double)gaussianKernel[gx][gy] * weight);
					*/

					/*
					sumR += (int)((double)(sample.getR()*gaussianKernel[gx][gy]) * weight);
					sumG += (int)((double)(sample.getG()*gaussianKernel[gx][gy]) * weight);
					sumB += (int)((double)(sample.getB()*gaussianKernel[gx][gy]) * weight);
					sumA += (int)((double)(sample.getA()*gaussianKernel[gx][gy]) * weight);
					*/

					sumR += ((sample.getR()*gaussianKernel[gx][gy]));
					sumG += ((sample.getG()*gaussianKernel[gx][gy]));
					sumB += ((sample.getB()*gaussianKernel[gx][gy]));
					sumA += ((sample.getA()*gaussianKernel[gx][gy]));
				}
			}
			if (sumR > 0) {
				cout << "Break point ;)" << endl;
			}
			result[index] = (unsigned char)((double)sumR / (double)kernalSum);
			result[index+1] = (unsigned char)((double)sumG / (double)kernalSum);
			result[index+2] = (unsigned char)((double)sumB / (double)kernalSum);
			result[index+3] = (unsigned char)((double)sumA / (double)kernalSum);

			index += 4;
		}
	}
	return new Image(configuration.width, configuration.height, result);
}

void HeatMap::drawLine(GeographicCoordinate from, GeographicCoordinate to)
{
	xiaolinWu(from, to);
}

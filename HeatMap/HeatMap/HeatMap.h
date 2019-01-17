#pragma once
#include "HeatMapCell.h"
#include "GeographicCoordinate.h"
#include "Image.h"
#include "Color.h"

#include "Activity.h"
#include "HeatMapConfiguration.h"
#include "PrivacyZones.h"

#include <iostream>
#include <algorithm>

class HeatMap {
private:
	HeatMapConfiguration configuration;

	HeatMapCell** cells;

	double static DEG2RAD(double a);
	double static RAD2DEG(double a);
	
	double latToDoubleY(double lat);
	double lonToDoubleX(double lon);
	int latToIntY(double lat);
	int lonToIntX(double lon);
	void drawPoint(int x, int y, double c);
	int toCoordInt(double value);
	void swap(double &a, double &b);
	double fpart(double value);
	double rfpart(double value);
	double sign(double value);
	void drawLine(GeographicCoordinate from, GeographicCoordinate to, PrivacyZones* zones=nullptr);
	void xiaolinWu(GeographicCoordinate from, GeographicCoordinate to);

	long pascalValue(int row, int col);
	vector<long> getPascalTriangleRow(int length);
public:
	HeatMap(HeatMapConfiguration configuration);
	~HeatMap();

	double static latToWorldY(double lat);
	double static lonToWorldX(double lon);
	double static worldYToLat(double y);
	double static worldXToLon(double x);

	void addActivity(Activity &activity, PrivacyZones* zones=nullptr);
	void normalizeMap();
	Image* renderImage();
	Image* createGlowImage(Image* renderedImage, int diameter);
};
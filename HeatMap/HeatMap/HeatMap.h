#pragma once
#include "HeatMapCell.h"
#include "GeographicCoordinate.h"
#include "TrainingCenterXML.h"

#include <iostream>

class HeatMap {
private:
	int width;
	int height;

	GeographicCoordinate lowerLeft;
	GeographicCoordinate upperRight;

	HeatMapCell** cells;

	double latToDoubleY(double lat);
	double lonToDoubleX(double lon);
	int latToIntY(double lat);
	int lonToIntX(double lon);
	void drawPoint(int x, int y, double c);
	int toCoordInt(double value);
	void swap(double &a, double &b);
	double fpart(double value);
	double rfpart(double value);
	void drawLine(GeographicCoordinate from, GeographicCoordinate to);
public:
	HeatMap(int width, int height, GeographicCoordinate lowerLeft, GeographicCoordinate upperRight);
	~HeatMap();
	void addActivity(TrainingCenterXML &activity);

	void debugDrawCells();
};
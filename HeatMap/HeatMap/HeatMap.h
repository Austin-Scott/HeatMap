#pragma once
#include "HeatMapCell.h"
#include "GeographicCoordinate.h"
#include "TrainingCenterXML.h"
#include "Image.h"
#include "Color.h"

#include <iostream>
#include <algorithm>

class HeatMap {
private:
	int width;
	int height;

	GeographicCoordinate lowerLeft;
	GeographicCoordinate upperRight;

	HeatMapCell** cells;

	bool useAntiAliasing;

	bool useActivityFiltering;
	vector<ActivityType> activityFilters;

	bool useDateFiltering;
	bool includeUnknownDates;
	Date startDate;
	Date endDate;

	bool useAverageSpeedFiltering;
	bool includeUnknownSpeeds;
	Speed slowestSpeed;
	Speed fastestSpeed;

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
	void drawLine(GeographicCoordinate from, GeographicCoordinate to, bool smooth);
	void xiaolinWu(GeographicCoordinate from, GeographicCoordinate to);
	void bresenham(GeographicCoordinate from, GeographicCoordinate to);
	void plotLineLow(int x0, int y0, int x1, int y1);
	void plotLineHigh(int x0, int y0, int x1, int y1);
public:
	HeatMap(int width, int height, GeographicCoordinate lowerLeft, GeographicCoordinate upperRight, bool useAntiAliasing);
	~HeatMap();

	void setActivityTypeFilter(vector<ActivityType> activityFilters);
	void setDateFilter(Date startDate, Date endDate, bool includeUnknownDates = false);
	void setAverageSpeedFilter(Speed slowestSpeed, Speed fastestSpeed, bool includeUnknownSpeeds = false);

	void addActivity(Activity &activity);
	void normalizeMap();
	Image* renderImage(Color backgroundColor, Color minimumActivityColor, Color maximumActivityColor);

	void debugDrawCells();
};
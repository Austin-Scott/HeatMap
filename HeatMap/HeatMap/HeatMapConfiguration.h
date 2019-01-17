#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "Activity.h"
#include "Date.h"
#include "Speed.h"
#include "Color.h"

using namespace rapidxml;

struct HeatMapConfiguration {

	vector<ActivityType> activityFilters;

	bool useDateFilteringOne;
	bool useDateFilteringTwo;
	bool includeUnknownDates;
	Date startDate;
	Date endDate;

	bool useAverageSpeedFilteringOne;
	bool useAverageSpeedFilteringTwo;
	bool includeUnknownSpeeds;
	Speed slowestSpeed;
	Speed fastestSpeed;


	Color backgroundColor;
	Color minimumActivityColor;
	Color activity33Color;
	Color activity66Color;
	Color maximumActivityColor;

	GeographicCoordinate lowerLeft;
	GeographicCoordinate upperRight;

	int viewportMode;
	double highestLatitude;
	GeographicCoordinate bottomCenter;
	double radius;

	int width;
	int height;

	bool downloadMap;
	string mapType;
	unsigned char heatLayerTransparency;

	HeatMapConfiguration();
	HeatMapConfiguration(int width, int height);

	void removeFilters();

	bool operator==(HeatMapConfiguration other);

	void saveConfiguration(string filename);
	void loadConfiguration(string filename);

	void setRenderer(Color backgroundColor, Color minimumActivityColor, Color activity33Color, Color activity66Color, Color maximumActivityColor);
};
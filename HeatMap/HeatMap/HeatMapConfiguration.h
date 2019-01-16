#pragma once
#include <vector>
#include <string>

#include "Activity.h"
#include "Date.h"
#include "Speed.h"
#include "Color.h"

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

	int width;
	int height;

	bool downloadMap;
	string mapType;
	unsigned char heatLayerTransparency;

	HeatMapConfiguration();
	HeatMapConfiguration(int width, int height);

	void removeFilters();

	bool operator==(HeatMapConfiguration other);

	void setRenderer(Color backgroundColor, Color minimumActivityColor, Color activity33Color, Color activity66Color, Color maximumActivityColor);
};
#pragma once
#include <vector>
#include <string>

#include "Activity.h"
#include "Date.h"
#include "Speed.h"
#include "Color.h"

struct HeatMapConfiguration {
	bool useAntiAliasing;

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
	Color maximumActivityColor;

	GeographicCoordinate lowerLeft;
	GeographicCoordinate upperRight;

	int width;
	int height;

	HeatMapConfiguration();
	HeatMapConfiguration(int width, int height);

	void removeFilters();

	void setRenderer(bool useAntiAliasing, Color backgroundColor, Color minimumActivityColor, Color maximumActivityColor);
};
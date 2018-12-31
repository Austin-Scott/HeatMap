#pragma once
#include <vector>
#include <string>

#include "Activity.h"
#include "Date.h"
#include "Speed.h"
#include "Color.h"

struct HeatMapConfiguration {
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

	void setActivityTypeFilter(vector<ActivityType> activityFilters);
	void setDateFilter(Date startDate, Date endDate, bool includeUnknownDates = false);
	void setAverageSpeedFilter(Speed slowestSpeed, Speed fastestSpeed, bool includeUnknownSpeeds = false);
	void computeBoundingBox(GeographicCoordinate bottomCenter, double maxLatitude);

	void setRenderer(bool useAntiAliasing, Color backgroundColor, Color minimumActivityColor, Color maximumActivityColor);
};
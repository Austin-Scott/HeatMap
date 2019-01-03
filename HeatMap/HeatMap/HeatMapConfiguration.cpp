#include "HeatMapConfiguration.h"

HeatMapConfiguration::HeatMapConfiguration()
{
	removeFilters();
}

HeatMapConfiguration::HeatMapConfiguration(int width, int height)
{
	this->width = width;
	this->height = height;
	removeFilters();
}

void HeatMapConfiguration::removeFilters()
{
	useActivityFiltering = false;
	useDateFilteringOne = false;
	useDateFilteringTwo = false;
	useAverageSpeedFiltering = false;
}

void HeatMapConfiguration::setActivityTypeFilter(vector<ActivityType> activityFilters)
{
	if (activityFilters.size() > 0) {
		useActivityFiltering = true;
		this->activityFilters = activityFilters;
	}
}

void HeatMapConfiguration::setDateFilter(bool filterOne, Date startDate, bool filterTwo, Date endDate, bool includeUnknownDates)
{
	useDateFilteringOne = filterOne;
	useDateFilteringTwo = filterTwo;
	this->includeUnknownDates = includeUnknownDates;
	this->startDate = startDate;
	this->endDate = endDate;
}

void HeatMapConfiguration::setAverageSpeedFilter(Speed slowestSpeed, Speed fastestSpeed, bool includeUnknownSpeeds)
{
	useAverageSpeedFiltering = true;
	this->includeUnknownSpeeds = includeUnknownSpeeds;
	this->slowestSpeed = slowestSpeed;
	this->fastestSpeed = fastestSpeed;
}

void HeatMapConfiguration::computeBoundingBox(GeographicCoordinate bottomCenter, double maxLatitude)
{
	double latitudeDifference = maxLatitude - bottomCenter.getLat();
	double aspectRatio = (double)width / (double)height;
	double viewportWidth = abs(latitudeDifference) * aspectRatio;
	lowerLeft = geoCoord(bottomCenter.getLat(), bottomCenter.getLon() - (viewportWidth / 2.0));
	upperRight = geoCoord(maxLatitude, bottomCenter.getLon() + (viewportWidth / 2.0));
}

void HeatMapConfiguration::setRenderer(bool useAntiAliasing, Color backgroundColor, Color minimumActivityColor, Color maximumActivityColor)
{
	this->useAntiAliasing = useAntiAliasing;
	this->backgroundColor = backgroundColor;
	this->minimumActivityColor = minimumActivityColor;
	this->maximumActivityColor = maximumActivityColor;
}

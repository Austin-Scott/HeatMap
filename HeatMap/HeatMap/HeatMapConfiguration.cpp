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
	useDateFilteringOne = false;
	useDateFilteringTwo = false;
	useAverageSpeedFilteringOne = false;
	useAverageSpeedFilteringTwo = false;
}

bool HeatMapConfiguration::operator==(HeatMapConfiguration other)
{
	return activityFilters == other.activityFilters&&
		useDateFilteringOne == other.useDateFilteringOne &&
		useDateFilteringTwo == other.useDateFilteringTwo &&
		includeUnknownDates == other.includeUnknownDates &&
		startDate == other.startDate &&
		endDate == other.endDate &&
		useAverageSpeedFilteringOne == other.useAverageSpeedFilteringOne &&
		useAverageSpeedFilteringTwo == other.useAverageSpeedFilteringTwo &&
		includeUnknownSpeeds == other.includeUnknownSpeeds &&
		slowestSpeed == other.slowestSpeed &&
		fastestSpeed == other.fastestSpeed &&
		backgroundColor == other.backgroundColor &&
		minimumActivityColor == other.minimumActivityColor &&
		activity33Color == other.activity33Color &&
		activity66Color == other.activity66Color &&
		maximumActivityColor == other.maximumActivityColor &&
		lowerLeft == other.lowerLeft &&
		upperRight == other.upperRight &&
		width == other.width &&
		height == other.height &&
		downloadMap == other.downloadMap &&
		mapType == other.mapType &&
		heatLayerTransparency == other.heatLayerTransparency;

}

void HeatMapConfiguration::setRenderer(Color backgroundColor, Color minimumActivityColor, Color activity33Color, Color activity66Color, Color maximumActivityColor)
{
	this->backgroundColor = backgroundColor;
	this->minimumActivityColor = minimumActivityColor;
	this->activity33Color = activity33Color;
	this->activity66Color = activity66Color;
	this->maximumActivityColor = maximumActivityColor;
}

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

void HeatMapConfiguration::setRenderer(bool useAntiAliasing, Color backgroundColor, Color minimumActivityColor, Color maximumActivityColor)
{
	this->useAntiAliasing = useAntiAliasing;
	this->backgroundColor = backgroundColor;
	this->minimumActivityColor = minimumActivityColor;
	this->maximumActivityColor = maximumActivityColor;
}

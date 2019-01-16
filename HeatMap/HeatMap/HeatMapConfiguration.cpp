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

void HeatMapConfiguration::setRenderer(Color backgroundColor, Color minimumActivityColor, Color activity33Color, Color activity66Color, Color maximumActivityColor)
{
	this->backgroundColor = backgroundColor;
	this->minimumActivityColor = minimumActivityColor;
	this->activity33Color = activity33Color;
	this->activity66Color = activity66Color;
	this->maximumActivityColor = maximumActivityColor;
}

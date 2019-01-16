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

void HeatMapConfiguration::saveConfiguration(string filename)
{
	xml_document<> doc;
	xml_node<>* root = doc.allocate_node(node_element, "heatmap_config");
	xml_attribute<>* version = doc.allocate_attribute("version", "1.1");
	root->append_attribute(version);
	doc.append_node(root);
	xml_node<>* activityFiltersNode = doc.allocate_node(node_element, "activityFilters");
	for (ActivityType t : activityFilters) {
		xml_node<>* actFilter = doc.allocate_node(node_element, "filter");
		char* value = doc.allocate_string(to_string(static_cast<int>(t)).c_str());
		xml_attribute<>* actAtr = doc.allocate_attribute("value", value);
		actFilter->append_attribute(actAtr);
		activityFiltersNode->append_node(actFilter);
	}
	root->append_node(activityFiltersNode);

	xml_node<>* dateFilterNode = doc.allocate_node(node_element, "dateFilter");
	//TODO finish here
}

void HeatMapConfiguration::loadConfiguration(string filename)
{
}

void HeatMapConfiguration::setRenderer(Color backgroundColor, Color minimumActivityColor, Color activity33Color, Color activity66Color, Color maximumActivityColor)
{
	this->backgroundColor = backgroundColor;
	this->minimumActivityColor = minimumActivityColor;
	this->activity33Color = activity33Color;
	this->activity66Color = activity66Color;
	this->maximumActivityColor = maximumActivityColor;
}

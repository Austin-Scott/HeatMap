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
		viewportMode == other.viewportMode &&
		highestLatitude == other.highestLatitude &&
		bottomCenter == other.bottomCenter &&
		radius == other.radius &&
		width == other.width &&
		height == other.height &&
		downloadMap == other.downloadMap &&
		mapType == other.mapType &&
		heatLayerTransparency == other.heatLayerTransparency;

}

string to_string(Date value) {
	return value.toFormalString();
}

string to_string(string value) {
	return value;
}

string to_string(Speed value) {
	return to_string(value.getSpeed(value.getUnits()));
}

string to_string(SpeedUnits value) {
	return to_string(static_cast<int>(value));
}

string to_string(Color value) {
	return value.toHex();
}

string to_string(GeographicCoordinate value) {
	return to_string(value.getLat()) + "," + to_string(value.getLon());
}

GeographicCoordinate geoCoordFromString(string value) {
	stringstream buffer(value);
	vector<string> tokens;
	while (!buffer.eof()) {
		string token = "";
		getline(buffer, token, ',');
		tokens.push_back(token);
	}
	if (tokens.size() == 2) {
		return geoCoord(stod(tokens[0].c_str()), stod(tokens[1].c_str()));
	}
	return GeographicCoordinate();
}

template<class T> void addAttribute(xml_document<> &doc, xml_node<>* node, string name, T value) {
	char* valueStr = doc.allocate_string(to_string(value).c_str());
	char* nameStr = doc.allocate_string(name.c_str());
	xml_attribute<>* atr = doc.allocate_attribute(nameStr, valueStr);
	node->append_attribute(atr);
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
	addAttribute(doc, dateFilterNode, "useDateFilteringOne", useDateFilteringOne);
	addAttribute(doc, dateFilterNode, "useDateFilteringTwo", useDateFilteringTwo);
	addAttribute(doc, dateFilterNode, "includeUnknownDates", includeUnknownDates);
	addAttribute(doc, dateFilterNode, "startDate", startDate);
	addAttribute(doc, dateFilterNode, "endDate", endDate);
	root->append_node(dateFilterNode);

	xml_node<>* speedFilterNode = doc.allocate_node(node_element, "speedFilter");
	addAttribute(doc, speedFilterNode, "useAverageSpeedFilteringOne", useAverageSpeedFilteringOne);
	addAttribute(doc, speedFilterNode, "useAverageSpeedFilteringTwo", useAverageSpeedFilteringTwo);
	addAttribute(doc, speedFilterNode, "includeUnknownSpeeds", includeUnknownSpeeds);
	addAttribute(doc, speedFilterNode, "slowestSpeed", slowestSpeed);
	addAttribute(doc, speedFilterNode, "slowestSpeedUnits", slowestSpeed.getUnits());
	addAttribute(doc, speedFilterNode, "fastestSpeed", fastestSpeed);
	addAttribute(doc, speedFilterNode, "fastestSpeedUnits", fastestSpeed.getUnits());
	root->append_node(speedFilterNode);

	xml_node<>* viewportNode = doc.allocate_node(node_element, "viewport");
	addAttribute(doc, viewportNode, "viewportMode", viewportMode);
	if (viewportMode == 0) {
		addAttribute(doc, viewportNode, "lowerLeft", lowerLeft);
		addAttribute(doc, viewportNode, "upperRight", upperRight);
	}
	else {
		addAttribute(doc, viewportNode, "lowerLeft", GeographicCoordinate());
		addAttribute(doc, viewportNode, "upperRight", GeographicCoordinate());
	}
	
	addAttribute(doc, viewportNode, "highestLatitude", highestLatitude);
	addAttribute(doc, viewportNode, "bottomCenter", bottomCenter);
	addAttribute(doc, viewportNode, "radius", radius);
	root->append_node(viewportNode);

	xml_node<>* rendererNode = doc.allocate_node(node_element, "renderer");
	addAttribute(doc, rendererNode, "backgroundColor", backgroundColor);
	addAttribute(doc, rendererNode, "minimumActivityColor", minimumActivityColor);
	addAttribute(doc, rendererNode, "activity33Color", activity33Color);
	addAttribute(doc, rendererNode, "activity66Color", activity66Color);
	addAttribute(doc, rendererNode, "maximumActivityColor", maximumActivityColor);
	addAttribute(doc, rendererNode, "width", width);
	addAttribute(doc, rendererNode, "height", height);
	addAttribute(doc, rendererNode, "downloadMap", downloadMap);
	addAttribute(doc, rendererNode, "mapType", mapType);
	addAttribute(doc, rendererNode, "heatLayerTransparency", heatLayerTransparency);
	root->append_node(rendererNode);

	string document = "";
	print(back_inserter(document), doc, 0);

	fstream file(filename, fstream::out);
	if (file) {
		file << document;

		file.close();
	}
}

void HeatMapConfiguration::loadConfiguration(string filename)
{
	xml_document<> xmlDocument;
	xml_node<>* rootNode;
	ifstream file(filename, ios::binary);
	if (file) {

		filebuf* fbuf = file.rdbuf();
		size_t size = fbuf->pubseekoff(0, file.end, file.in);
		fbuf->pubseekpos(0, file.in);
		char* buffer = new char[size];
		fbuf->sgetn(buffer, size);
		file.close();

		string fileBuffer(buffer, size);
		delete[] buffer;

		char* cstring = new char[fileBuffer.length() + 1];
		for (int i = 0; i < fileBuffer.length(); i++) {
			cstring[i] = fileBuffer[i];
		}
		cstring[fileBuffer.length()] = '\0';

		xmlDocument.parse<0>(cstring);

		rootNode = xmlDocument.first_node("heatmap_config");

		if (rootNode != nullptr) {

			xml_attribute<>* version = rootNode->first_attribute("version");
			if (version != nullptr && strcmp(version->value(), "1.1") != 0) {
				//Possibly unsupported version
			}

			activityFilters.clear();
			xml_node<>* activityFilterNode = rootNode->first_node("activityFilters");
			if (activityFilterNode != nullptr) {
				for (xml_node<>* filter = activityFilterNode->first_node("filter"); filter; filter=filter->next_sibling()) {
					xml_attribute<>* value = filter->first_attribute("value");
					if (value != nullptr) {
						activityFilters.push_back(static_cast<ActivityType>(atoi(value->value())));
					}
				}
			}

			xml_node<>* dateFilterNode = rootNode->first_node("dateFilter");
			if (dateFilterNode != nullptr) {
				xml_attribute<>* atr = dateFilterNode->first_attribute("useDateFilteringOne");
				useDateFilteringOne = atr ? (bool)atoi(atr->value()) : false;

				atr = dateFilterNode->first_attribute("useDateFilteringTwo");
				useDateFilteringTwo = atr ? (bool)atoi(atr->value()) : false;

				atr = dateFilterNode->first_attribute("includeUnknownDates");
				includeUnknownDates = atr ? (bool)atoi(atr->value()) : true;

				atr = dateFilterNode->first_attribute("startDate");
				startDate = atr ? Date::fromFormalString(atr->value()) : Date();

				atr = dateFilterNode->first_attribute("endDate");
				endDate = atr ? Date::fromFormalString(atr->value()) : Date();
			}

			xml_node<>* speedFilterNode = rootNode->first_node("speedFilter");
			if (speedFilterNode != nullptr) {
				xml_attribute<>* atr = speedFilterNode->first_attribute("useAverageSpeedFilteringOne");
				useAverageSpeedFilteringOne = atr ? (bool)atoi(atr->value()) : false;

				atr = speedFilterNode->first_attribute("useAverageSpeedFilteringTwo");
				useAverageSpeedFilteringTwo = atr ? (bool)atoi(atr->value()) : false;

				atr = speedFilterNode->first_attribute("includeUnknownSpeeds");
				includeUnknownSpeeds = atr ? (bool)atoi(atr->value()) : false;

				atr = speedFilterNode->first_attribute("slowestSpeed");
				double speed = atr ? stod(atr->value()) : 0.0;
				atr = speedFilterNode->first_attribute("slowestSpeedUnits");
				SpeedUnits units = atr ? static_cast<SpeedUnits>(atoi(atr->value())) : SpeedUnits::MetersPerSecond;
				slowestSpeed = Speed(speed, units);

				atr = speedFilterNode->first_attribute("fastestSpeed");
				speed = atr ? stod(atr->value()) : 0.0;
				atr = speedFilterNode->first_attribute("fastestSpeedUnits");
				units = atr ? static_cast<SpeedUnits>(atoi(atr->value())) : SpeedUnits::MetersPerSecond;
				fastestSpeed = Speed(speed, units);

			}

			xml_node<>* viewportNode = rootNode->first_node("viewport");
			if (viewportNode != nullptr) {
				xml_attribute<>* atr = viewportNode->first_attribute("lowerLeft");
				lowerLeft = atr ? geoCoordFromString(atr->value()) : GeographicCoordinate();

				atr = viewportNode->first_attribute("upperRight");
				upperRight = atr ? geoCoordFromString(atr->value()) : GeographicCoordinate();
			
				atr = viewportNode->first_attribute("bottomCenter");
				bottomCenter = atr ? geoCoordFromString(atr->value()) : GeographicCoordinate();
			
				atr = viewportNode->first_attribute("viewportMode");
				viewportMode = atr ? atoi(atr->value()) : 2;

				atr = viewportNode->first_attribute("highestLatitude");
				highestLatitude = atr ? stod(atr->value()) : 0.0;

				atr = viewportNode->first_attribute("radius");
				radius = atr ? stod(atr->value()) : 10.0;
			}

			xml_node<>* rendererNode = rootNode->first_node("renderer");
			if (rendererNode != nullptr) {
				xml_attribute<>* atr = rendererNode->first_attribute("backgroundColor");
				backgroundColor = atr ? Color(atr->value()) : Color();

				atr = rendererNode->first_attribute("minimumActivityColor");
				minimumActivityColor = atr ? Color(atr->value()) : Color();

				atr = rendererNode->first_attribute("activity33Color");
				activity33Color = atr ? Color(atr->value()) : Color();

				atr = rendererNode->first_attribute("activity66Color");
				activity66Color = atr ? Color(atr->value()) : Color();

				atr = rendererNode->first_attribute("maximumActivityColor");
				maximumActivityColor = atr ? Color(atr->value()) : Color();

				atr = rendererNode->first_attribute("width");
				width = atr ? atoi(atr->value()) : 1920;

				atr = rendererNode->first_attribute("height");
				height = atr ? atoi(atr->value()) : 1080;

				atr = rendererNode->first_attribute("downloadMap");
				downloadMap = atr ? (bool)atoi(atr->value()) : true;

				atr = rendererNode->first_attribute("mapType");
				mapType = atr ? string(atr->value()) : "dark";

				atr = rendererNode->first_attribute("heatLayerTransparency");
				heatLayerTransparency = atr ? max(min(atoi(atr->value()), 255), 0) : 255;
			}

		}

	}
}

void HeatMapConfiguration::setRenderer(Color backgroundColor, Color minimumActivityColor, Color activity33Color, Color activity66Color, Color maximumActivityColor)
{
	this->backgroundColor = backgroundColor;
	this->minimumActivityColor = minimumActivityColor;
	this->activity33Color = activity33Color;
	this->activity66Color = activity66Color;
	this->maximumActivityColor = maximumActivityColor;
}

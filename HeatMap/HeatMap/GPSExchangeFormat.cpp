#include "GPSExchangeFormat.h"

GPSExchangeFormat::GPSExchangeFormat(string filename)
{
	this->filename = filename;

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

		rootNode = xmlDocument.first_node("gpx");
		
		if (rootNode != nullptr) {

			xml_node<>* metadata = rootNode->first_node("metadata");
			if (metadata != nullptr) {
				metadata = metadata->first_node("time");
				if (metadata != nullptr) {
					startDate = Date::parseDateString(metadata->value());
				}
			}

			rootNode = rootNode->first_node("trk");
			if (rootNode != nullptr) {

				int trackPointCount = 0;

				for (xml_node<>* lapNode = rootNode->first_node("trkseg"); lapNode; lapNode = lapNode->next_sibling()) {
					for (xml_node<>* pointNode = lapNode->first_node("trkpt"); pointNode; pointNode = pointNode->next_sibling()) {
						xml_attribute<>* latAtr = pointNode->first_attribute("lat");
						xml_attribute<>* lonAtr = pointNode->first_attribute("lon");
						if (latAtr != nullptr && lonAtr != nullptr) {
							double lat = stod(latAtr->value());
							double lon = stod(lonAtr->value());

							if (track.size() == 0) {
								maxLatitude = lat;
								minLatitude = lat;
								maxLongitude = lon;
								minLongitude = lon;
							}
							else {
								if (lat < minLatitude)
									minLatitude = lat;
								if (lat > maxLatitude)
									maxLatitude = lat;
								if (lon < minLongitude)
									minLatitude = lon;
								if (lon > maxLongitude)
									maxLongitude = lon;
							}

							track.emplace_back(lat, lon);
							trackPointCount++;
						}
					}
				}
			}
		}
		else {
			//cerr << "Error: couldn't find the gpx node in " << filename << "." << endl;
		}

		delete[] cstring;
	}
	else {
		//cerr << "Error: Couldn't open " << filename << " for reading." << endl;
	}
}

const vector<GeographicCoordinate>& GPSExchangeFormat::getTrack()
{
	return track;
}

ActivityType GPSExchangeFormat::getActivityType()
{
	return ActivityType::Unknown;
}

Date GPSExchangeFormat::getStartDate()
{
	return startDate;
}

Speed GPSExchangeFormat::getAverageSpeed()
{
	return Speed(); //For now we don't support average speeds in GPX files. May be implemented in the future.
}

string GPSExchangeFormat::getFilename()
{
	return filename;
}

GeographicCoordinate GPSExchangeFormat::upperRight()
{
	return geoCoord(maxLatitude, maxLongitude);
}

GeographicCoordinate GPSExchangeFormat::lowerLeft()
{
	return geoCoord(minLatitude, minLongitude);
}

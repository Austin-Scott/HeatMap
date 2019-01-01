#include "TrainingCenterXML.h"

TrainingCenterXML::TrainingCenterXML(string filename)
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
		
		char* cstring = new char[fileBuffer.length()+1];
		for (int i = 0; i < fileBuffer.length();i++) {
			cstring[i] = fileBuffer[i];
		}
		cstring[fileBuffer.length()] = '\0';

		xmlDocument.parse<0>(cstring);

		rootNode = xmlDocument.first_node("TrainingCenterDatabase");
		if (rootNode != nullptr) {
			rootNode = rootNode->first_node("Activities");
			if (rootNode != nullptr) {
				rootNode = rootNode->first_node("Activity");
				if (rootNode != nullptr) {

					activityType = ActivityType::Unknown;
					xml_attribute<>* sportAtr = rootNode->first_attribute("Sport");
					if (sportAtr != nullptr) {
						string sportName = sportAtr->value();

						if (sportName == "Running") {
							activityType = ActivityType::Running;
						}
						else if (sportName == "Walking") {
							activityType = ActivityType::Walking;
						}
						else if (sportName == "Cycling") {
							activityType = ActivityType::Cycling;
						}
						else if (sportName == "Swimming") {
							activityType = ActivityType::Swimming;
						}
					}

					bool foundAverageSpeed = true;
					double cumulativeDistanceMeters = 0.0;
					double cumulativeTimeSeconds = 0.0;

					for (xml_node<>* lapNode = rootNode->first_node("Lap"); lapNode; lapNode = lapNode->next_sibling()) {
						if (strcmp(lapNode->name(), "Lap")==0) {
							if (!startDate.isDateSet()) {
								xml_attribute<>* timeAtr = lapNode->first_attribute("StartTime");
								if (timeAtr != nullptr) {
									startDate = Date::parseDateString(timeAtr->value());
								}
							}
							if (foundAverageSpeed) {
								xml_node<>* timeNode = lapNode->first_node("TotalTimeSeconds");
								xml_node<>* distanceNode = lapNode->first_node("DistanceMeters");
								if (timeNode != nullptr && distanceNode != nullptr) {
									cumulativeTimeSeconds += stod(timeNode->value());
									cumulativeDistanceMeters += stod(distanceNode->value());
								}
								else {
									foundAverageSpeed = false;
									//Something went wrong with the calculation. Abort attempt.
								}
							}

							xml_node<>* currentTrack = lapNode->first_node("Track");
							if (currentTrack == nullptr) continue;

							for (xml_node<>* pointNode = currentTrack->first_node("Trackpoint"); pointNode; pointNode = pointNode->next_sibling()) {
								xml_node<>* currentPosition = pointNode->first_node("Position");
								if (currentPosition != nullptr) {
									xml_node<>* lat = currentPosition->first_node("LatitudeDegrees");
									xml_node<>* lon = currentPosition->first_node("LongitudeDegrees");
									if (lat != nullptr && lon != nullptr) {
										double lati = stod(lat->value());
										double longi = stod(lon->value());

										if (track.size() == 0) {
											maxLatitude = lati;
											minLatitude = lati;
											maxLongitude = longi;
											minLongitude = longi;
										}
										else {
											if (lati < minLatitude)
												minLatitude = lati;
											if (lati > maxLatitude)
												maxLatitude = lati;
											if (longi < minLongitude)
												minLongitude = longi;
											if (longi > maxLongitude)
												maxLongitude = longi;
										}

										track.emplace_back(lati, longi);
									}
								}
							}
						}
					}

					if (foundAverageSpeed && cumulativeTimeSeconds!=0.0) {
						double speed = cumulativeDistanceMeters / cumulativeTimeSeconds;
						averageSpeed = Speed(speed, SpeedUnits::MetersPerSecond);
					}
				}
			}
		}
		delete[] cstring;
	}
	else {
		//cerr << "Error: Couldn't open " << filename << " for reading." << endl;
	}
}

const vector<GeographicCoordinate>& TrainingCenterXML::getTrack()
{
	return track;
}

ActivityType TrainingCenterXML::getActivityType()
{
	return activityType;
}

Date TrainingCenterXML::getStartDate()
{
	return startDate;
}

Speed TrainingCenterXML::getAverageSpeed()
{
	return averageSpeed;
}

string TrainingCenterXML::getFilename()
{
	return filename;
}

GeographicCoordinate TrainingCenterXML::upperRight()
{
	return geoCoord(maxLatitude, maxLongitude);
}

GeographicCoordinate TrainingCenterXML::lowerLeft()
{
	return geoCoord(minLatitude, minLongitude);
}

#include "TrainingCenterXML.h"

TrainingCenterXML::TrainingCenterXML(string filename)
{
	xml_document<> xmlDocument;
	xml_node<>* rootNode;
	ifstream file(filename, ios::binary);
	if (file) {
		cout << "\t*Loading file into memory..." << endl;

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

		cout << "\t*Parsing file..." << endl;

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
							cout << "\t*Activity type: " << sportName << endl;
						}
						else if (sportName == "Walking") {
							activityType = ActivityType::Walking;
							cout << "\t*Activity type: " << sportName << endl;
						}
						else if (sportName == "Cycling") {
							activityType = ActivityType::Cycling;
							cout << "\t*Activity type: " << sportName << endl;
						}
						else if (sportName == "Swimming") {
							activityType = ActivityType::Swimming;
							cout << "\t*Activity type: " << sportName << endl;
						}
					}


					for (xml_node<>* lapNode = rootNode->first_node("Lap"); lapNode; lapNode = lapNode->next_sibling()) {
						if (!startDate.isDateSet()) {
							xml_attribute<>* timeAtr = lapNode->first_attribute("StartTime");
							if (timeAtr != nullptr) {
								startDate = Date::parseDateString(timeAtr->value());
								if (startDate.isDateSet()) {
									cout << "\t*Date of activity: " << startDate.toString() << endl;
								}
							}
						}
						xml_node<>* currentTrack = lapNode->first_node("Track");
						if (currentTrack==nullptr) continue;

						for (xml_node<>* pointNode = currentTrack->first_node("Trackpoint"); pointNode; pointNode = pointNode->next_sibling()) {
							xml_node<>* currentPosition = pointNode->first_node("Position");
							if (currentPosition != nullptr) {
								xml_node<>* lat = currentPosition->first_node("LatitudeDegrees");
								xml_node<>* lon = currentPosition->first_node("LongitudeDegrees");
								if (lat != nullptr && lon != nullptr) {
									track.emplace_back(stod(lat->value()), stod(lon->value()));
								}
							}
						}
					}
					cout << "\t*Successfully retrieved " << track.size() << " tracking points from file." << endl;
				}
			}
		}
		delete[] cstring;
	}
	else {
		cerr << "Error: Couldn't open " << filename << " for reading." << endl;
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

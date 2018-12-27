#include "TrainingCenterXML.h"

TrainingCenterXML::TrainingCenterXML(string filename, string activityFilter)
{
	xml_document<> xmlDocument;
	xml_node<>* rootNode;
	ifstream file(filename);
	if (file) {
		cout << "\t*Loading file into memory..." << endl;
		string fileBuffer;
		while (!file.eof()) {
			string line = "";
			getline(file, line);
			fileBuffer = fileBuffer + line + '\n';
		}

		char* cstring = new char[fileBuffer.length()+1];
		for (int i = 0; i < fileBuffer.length();i++) {
			cstring[i] = fileBuffer[i];
		}
		cstring[fileBuffer.length()] = '\0';

		cout << "\t*Parsing file..." << endl;

		xmlDocument.parse<0>(cstring);

		rootNode = xmlDocument.first_node("TrainingCenterDatabase")->first_node("Activities")->first_node("Activity");

		string sportName = rootNode->first_attribute("Sport")->value();
		cout << "\t*Activity type: " << sportName << endl;

		if (activityFilter != "" && activityFilter != sportName) {
			cout << "\t*Activity excluded from Heat Map due to filter." << endl;
			return;
		}

		int trackPointCount = 0;

		for (xml_node<>* lapNode = rootNode->first_node("Lap"); lapNode; lapNode = lapNode->next_sibling()) {
			xml_node<>* currentTrack = lapNode->first_node("Track");
			if (!currentTrack) continue;
			for (xml_node<>* pointNode = currentTrack->first_node("Trackpoint"); pointNode; pointNode = pointNode->next_sibling()) {
				xml_node<>* currentPosition = pointNode->first_node("Position");
				if (currentPosition != nullptr) {
					xml_node<>* lat = currentPosition->first_node("LatitudeDegrees");
					xml_node<>* lon = currentPosition->first_node("LongitudeDegrees");
					track.emplace_back(stod(lat->value()), stod(lon->value()));
					trackPointCount++;
				}
			}
		}
		delete[] cstring;

		cout << "\t*Successfully retrieved " << trackPointCount << " tracking points from file." << endl;
	}
	else {
		cerr << "Error: Couldn't open " << filename << " for reading." << endl;
	}
}

const vector<GeographicCoordinate>& TrainingCenterXML::getTrack()
{
	return track;
}

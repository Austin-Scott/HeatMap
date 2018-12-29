#include "FITProtocol.h"

double FITProtocol::convertSemicirclesToDegrees(double semicircles)
{
	return semicircles * (180.0 / pow(2, 31));
}

FITProtocol::FITProtocol(string filename, vector<string> activityFilters)
{
	isAcceptableActivityType = true;
	filters = activityFilters;
	fit::Decode decode;
	fit::MesgBroadcaster mesgBroadcaster;

	cout << "\t*Loading file into memory..." << endl;

	fstream file(filename, ios::in | ios::binary);
	if (file) {
		if (decode.CheckIntegrity(file)) {
			mesgBroadcaster.AddListener((fit::MesgListener &)*this);

			cout << "\t*Parsing file..." << endl;

			try {
				decode.Read(&file, &mesgBroadcaster, &mesgBroadcaster, this);
			}
			catch (const fit::RuntimeException& e) {
				cerr << "Exception when decoding " << filename << ": " << e.what() << endl;
			}
			if (isAcceptableActivityType) {
				cout << "\t*Successfully retrieved " << track.size() << " tracking points from file." << endl;
			}
		}
		else {
			cerr << filename << " failed to pass file integrity check." << endl;
		}
	}
	else {
		cerr << "Error opening " << filename << "." << endl;
	}
}

const vector<GeographicCoordinate>& FITProtocol::getTrack()
{
	if(isAcceptableActivityType)
		return track;
	else return vector<GeographicCoordinate>();
}

void FITProtocol::OnMesg(fit::Mesg & mesg)
{
	if (mesg.GetName() == "record") {
		double lat = 0.0;
		bool foundLat = false;
		double lon = 0.0;
		bool foundLon = false;
		for (FIT_UINT16 i = 0; i < (FIT_UINT16)mesg.GetNumFields(); i++) {
			fit::Field* field = mesg.GetFieldByIndex(i);
			if (field->GetName() == "position_lat") {
				lat = convertSemicirclesToDegrees(field->GetFLOAT64Value(0));
				foundLat = true;
			}
			else if (field->GetName() == "position_long") {
				lon = convertSemicirclesToDegrees(field->GetFLOAT64Value(0));
				foundLon = true;
			}
		}
		if (foundLon && foundLat) {
			track.emplace_back(lat, lon);
		}
	}
	else if (mesg.GetName() == "session") {
		for (FIT_UINT16 i = 0; i < (FIT_UINT16)mesg.GetNumFields(); i++) {
			fit::Field* field = mesg.GetFieldByIndex(i);
			if (field->GetName() == "sport") {
				int sportType = (int)field->GetFLOAT64Value(0);
				if (isAcceptableActivityType) {
					if (filters.size() == 0) return;
					bool foundActivity = false;
					for (string filter : filters) {
						if (filter == "Running" && sportType == FIT_ACTIVITY_TYPE_RUNNING) {
							foundActivity = true;
							break;
						}
						else if (filter == "Walking" && sportType == FIT_ACTIVITY_TYPE_WALKING) {
							foundActivity = true;
							break;
						}
						else if (filter == "Cycling" && sportType == FIT_ACTIVITY_TYPE_CYCLING) {
							foundActivity = true;
							break;
						}
						else if (filter == "Swimming" && sportType == FIT_ACTIVITY_TYPE_SWIMMING) {
							foundActivity = true;
							break;
						}
					}
					if (!foundActivity) {
						cout << "\t*Activity excluded from Heat Map due to filter." << endl;
						isAcceptableActivityType = false;
					}
				}
			}
		}
	}
}

void FITProtocol::OnDeveloperFieldDescription(const fit::DeveloperFieldDescription & desc)
{
	//Do nothing
}

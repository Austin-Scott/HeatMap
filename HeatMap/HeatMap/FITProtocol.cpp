#include "FITProtocol.h"

double FITProtocol::convertSemicirclesToDegrees(double semicircles)
{
	return semicircles * (180.0 / pow(2, 31));
}

FITProtocol::FITProtocol(string filename)
{
	fit::Decode decode;
	fit::MesgBroadcaster mesgBroadcaster;

	activityType = ActivityType::Unknown;

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

			cout << "\t*Successfully retrieved " << track.size() << " tracking points from file." << endl;
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
	return track;
}

ActivityType FITProtocol::getActivityType()
{
	return activityType;
}

Date FITProtocol::getStartDate()
{
	return startDate;
}

Speed FITProtocol::getAverageSpeed()
{
	return averageSpeed;
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
				int sportType = (int)field->GetENUMValue(0);
				if (sportType == FIT_ACTIVITY_TYPE_RUNNING) {
					activityType = ActivityType::Running;
					cout << "\t*Activity type: Running" << endl;
				}
				else if (sportType == FIT_ACTIVITY_TYPE_WALKING) {
					activityType = ActivityType::Walking;
					cout << "\t*Activity type: Walking" << endl;
				}
				else if (sportType == FIT_ACTIVITY_TYPE_CYCLING) {
					activityType = ActivityType::Cycling;
					cout << "\t*Activity type: Cycling" << endl;
				}
				else if (sportType == FIT_ACTIVITY_TYPE_SWIMMING) {
					activityType = ActivityType::Swimming;
					cout << "\t*Activity type: Swimming" << endl;
				}
			}
			else if (field->GetName() == "start_time") {
				//UINT32 data field giving seconds since UTC 00:00 Dec 31 1989 as measured when the activity started
				startDate = Date::createDate(field->GetUINT32Value(0));
				cout << "\t*Date of activity: " << startDate.toString() << endl;
			}
			else if (field->GetName() == "avg_speed") {
				string units = field->GetUnits();
				if (units == "m/s") {
					double speed = field->GetFLOAT64Value(0);
					cout << "\t*Average speed: " << speed << " m/s" << endl;
					averageSpeed.setSpeed(speed, SpeedUnits::MetersPerSecond);
				}
			}
		}
	}
}

void FITProtocol::OnDeveloperFieldDescription(const fit::DeveloperFieldDescription & desc)
{
	//Do nothing
}

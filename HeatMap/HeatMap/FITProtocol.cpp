#include "FITProtocol.h"

double FITProtocol::convertSemicirclesToDegrees(double semicircles)
{
	return semicircles * (180.0 / pow(2, 31));
}

FITProtocol::FITProtocol(string filename)
{
	this->filename = filename;

	fit::Decode decode;
	fit::MesgBroadcaster mesgBroadcaster;

	activityType = ActivityType::Unknown;

	fstream file(filename, ios::in | ios::binary);
	if (file) {
		if (decode.CheckIntegrity(file)) {
			mesgBroadcaster.AddListener((fit::MesgListener &)*this);

			try {
				decode.Read(&file, &mesgBroadcaster, &mesgBroadcaster, this);
			}
			catch (const fit::RuntimeException& e) {
				//cerr << "Exception when decoding " << filename << ": " << e.what() << endl;
			}

		}
		else {
			//cerr << filename << " failed to pass file integrity check." << endl;
		}
	}
	else {
		//cerr << "Error opening " << filename << "." << endl;
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

string FITProtocol::getFilename()
{
	return filename;
}

GeographicCoordinate FITProtocol::upperRight()
{
	return geoCoord(maxLatitude, maxLongitude);
}

GeographicCoordinate FITProtocol::lowerLeft()
{
	return geoCoord(minLatitude, minLongitude);
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
			if (track.size() == 0) {
				maxLatitude = lat;
				minLatitude = lat;
				maxLongitude = lon;
				minLongitude = lon;
			}
			else {
				if (lat > maxLatitude)
					maxLatitude = lat;
				if (lat < minLatitude)
					minLatitude = lat;
				if (lon > maxLongitude)
					maxLongitude = lon;
				if (lon < minLongitude)
					minLongitude = lon;
			}
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
				}
				else if (sportType == FIT_ACTIVITY_TYPE_WALKING) {
					activityType = ActivityType::Walking;
				}
				else if (sportType == FIT_ACTIVITY_TYPE_CYCLING) {
					activityType = ActivityType::Cycling;
				}
				else if (sportType == FIT_ACTIVITY_TYPE_SWIMMING) {
					activityType = ActivityType::Swimming;
				}
			}
			else if (field->GetName() == "start_time") {
				//UINT32 data field giving seconds since UTC 00:00 Dec 31 1989 as measured when the activity started
				startDate = Date::createDate(field->GetUINT32Value(0));
			}
			else if (field->GetName() == "avg_speed") {
				string units = field->GetUnits();
				if (units == "m/s") {
					double speed = field->GetFLOAT64Value(0);
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

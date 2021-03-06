#pragma once
#include <vector>
#include <fstream>
#include <string.h>
#include <string>
#include <iostream>

#include "fit_decode.hpp"
#include "fit_mesg_broadcaster.hpp"
#include "fit_developer_field_description.hpp"

#include "GeographicCoordinate.h"
#include "Activity.h"

using namespace std;

class FITProtocol : public Activity, public fit::MesgListener, public fit::DeveloperFieldDescriptionListener {
private:
	string filename;

	vector<GeographicCoordinate> track;
	ActivityType activityType;
	Date startDate;
	Speed averageSpeed;

	double maxLatitude;
	double minLatitude;
	double maxLongitude;
	double minLongitude;

	double convertSemicirclesToDegrees(double semicircles);
public:
	FITProtocol(string filename);

	const vector<GeographicCoordinate>& getTrack();
	ActivityType getActivityType();
	Date getStartDate();
	Speed getAverageSpeed();
	string getFilename();
	GeographicCoordinate upperRight();
	GeographicCoordinate lowerLeft();

	void OnMesg(fit::Mesg& mesg);
	void OnDeveloperFieldDescription(const fit::DeveloperFieldDescription& desc);
};
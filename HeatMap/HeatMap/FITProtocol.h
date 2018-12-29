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
	bool isAcceptableActivityType;
	vector<string> filters;
	vector<GeographicCoordinate> track;
	double convertSemicirclesToDegrees(double semicircles);
public:
	FITProtocol(string filename, vector<string> activityFilters);
	const vector<GeographicCoordinate>& getTrack();
	void OnMesg(fit::Mesg& mesg);
	void OnDeveloperFieldDescription(const fit::DeveloperFieldDescription& desc);
};
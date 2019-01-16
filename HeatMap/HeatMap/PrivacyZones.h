#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "GeographicCoordinate.h"

using namespace std;


struct Zone {
	bool rectangle;
	GeographicCoordinate lowerLeft;
	GeographicCoordinate upperRight;
	GeographicCoordinate center;
	double radius;
	Zone(GeographicCoordinate lowerLeft, GeographicCoordinate upperRight);
	Zone(GeographicCoordinate center, double radius);

	bool isPointInZone(GeographicCoordinate point);
};

class PrivacyZones {
private:
	vector<Zone> zones;
public:
	PrivacyZones(string filename);
	PrivacyZones();

	bool isPointInZones(GeographicCoordinate point);
};
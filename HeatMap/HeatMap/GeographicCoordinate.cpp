#include "GeographicCoordinate.h"

#include <math.h>
#include <iostream>

using namespace std;

GeographicCoordinate::GeographicCoordinate()
{
	isSet = false;
	latitude = 0.0;
	longitude = 0.0;
}

GeographicCoordinate::GeographicCoordinate(double latitude, double longitude)
{
	isSet = true;
	this->latitude = latitude;
	this->longitude = longitude;
}

double GeographicCoordinate::getLat() const
{
	return latitude;
}

double GeographicCoordinate::getLon() const
{
	return longitude;
}

string GeographicCoordinate::toString()
{
	string result = "Latitude: " + to_string(latitude) + " Longitude: " + to_string(longitude);
	return result;
}

bool GeographicCoordinate::getIsSet()
{
	return isSet;
}

double GeographicCoordinate::getKilometersTo(GeographicCoordinate other)
{
	if (this->latitude==other.getLat() && this->longitude==other.getLon())
		return 0.0;

	const double radius = 6371.0; //Average radius of Earth
	double dLat = (latitude - other.getLat())*(M_PI / 180.0);
	double dLon = (longitude - other.getLon())*(M_PI / 180.0);
	double a = sin(dLat / 2.0)*sin(dLat / 2.0) + cos(latitude*(M_PI / 180.0))*cos(other.getLat()*(M_PI / 180.0))*sin(dLon / 2.0)*sin(dLon / 2.0);
	double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
	return radius * c;
}

bool GeographicCoordinate::operator==(GeographicCoordinate other)
{
	return latitude == other.getLat() && longitude == other.getLon() && isSet == other.getIsSet();
}

GeographicCoordinate geoCoord(double latitude, double longitude)
{
	return GeographicCoordinate(latitude, longitude);
}

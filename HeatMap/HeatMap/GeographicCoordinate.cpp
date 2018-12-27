#include "GeographicCoordinate.h"

GeographicCoordinate::GeographicCoordinate()
{
	latitude = 0.0;
	longitude = 0.0;
}

GeographicCoordinate::GeographicCoordinate(double latitude, double longitude)
{
	this->latitude = latitude;
	this->longitude = longitude;
}

double GeographicCoordinate::getLat()
{
	return latitude;
}

double GeographicCoordinate::getLon()
{
	return longitude;
}

GeographicCoordinate geoCoord(double latitude, double longitude)
{
	return GeographicCoordinate(latitude, longitude);
}

#include "GeographicCoordinate.h"

#include <math.h>
#include <iostream>

using namespace std;

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

void GeographicCoordinate::printCoordinate()
{
	cout << "Latitude: " << latitude << " Longitude: " << longitude;
}

GeographicCoordinate geoCoord(double latitude, double longitude)
{
	return GeographicCoordinate(latitude, longitude);
}

GeographicCoordinate * computeBoundingBox(GeographicCoordinate bottomCenter, double maxLatitude, int width, int height)
{
	GeographicCoordinate* result = new GeographicCoordinate[2];
	double latitudeDifference = maxLatitude - bottomCenter.getLat();
	double aspectRatio = (double)width / (double)height;
	double viewportWidth = abs(latitudeDifference) * aspectRatio;
	result[0] = geoCoord(bottomCenter.getLat(), bottomCenter.getLon() - (viewportWidth / 2.0));
	result[1] = geoCoord(maxLatitude, bottomCenter.getLon() + (viewportWidth / 2.0));
	return result;
}

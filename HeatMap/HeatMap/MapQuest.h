#pragma once
#include "GeographicCoordinate.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
struct MapQuestConfig {
	GeographicCoordinate center;
	GeographicCoordinate lowerLeft;
	GeographicCoordinate upperRight;
	int zoom;
};

double getMetersPerOneDegreeLat();

double getMetersPerOneDegreeLon(double latitude);

double getMetersPerPixel(GeographicCoordinate referencePoint, int zoom);

MapQuestConfig getMapConfig(GeographicCoordinate lowerLeft, GeographicCoordinate upperRight, int width, int height);
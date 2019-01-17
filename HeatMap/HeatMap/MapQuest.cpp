#include "MapQuest.h"

double getMetersPerOneDegreeLat()
{
	return 111.32;
}

double getMetersPerOneDegreeLon(double latitude)
{
	return 40075.017 * cos(latitude*(M_PI / 180.0)) / 360.0;
}

double getMetersPerPixel(GeographicCoordinate referencePoint, int zoom)
{
	return 156543.03392 * (cos(referencePoint.getLat() * (M_PI / 180.0)) / (double)pow(2, zoom));
}

MapQuestConfig getMapConfig(GeographicCoordinate lowerLeft, GeographicCoordinate upperRight, int width, int height)
{
	MapQuestConfig result;
	result.center = geoCoord((lowerLeft.getLat() + upperRight.getLat()) / 2.0, (lowerLeft.getLon() + upperRight.getLon()) / 2.0);
	double dX = (double)width / 2.0;
	double dY = (double)height / 2.0;
	double thresholdDistance = lowerLeft.getKilometersTo(upperRight);

	for (result.zoom = 20; result.zoom >= 1; result.zoom--) {
		double metersPerPixel = getMetersPerPixel(result.center, result.zoom);
		double dXMeters = dX * metersPerPixel;
		double dYMeters = dY * metersPerPixel;
		double dLat = dYMeters / (getMetersPerOneDegreeLat()*1000.0);
		double dLon = dXMeters / (getMetersPerOneDegreeLon(result.center.getLat())*1000.0);

		result.lowerLeft = geoCoord(result.center.getLat() - dLat, result.center.getLon() - dLon);
		result.upperRight = geoCoord(result.center.getLat() + dLat, result.center.getLon() + dLon);

		if (result.lowerLeft.getKilometersTo(result.upperRight) >= thresholdDistance) {
			break;
		}
	}

	return result;
}

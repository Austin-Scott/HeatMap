#pragma once

enum SpeedUnits { MetersPerSecond, MilesPerHour, KilometersPerHour, MinutesPerMile, MinutesPerKilometer };

class Speed {
private:
	double speedInMetersPerSecond;
	SpeedUnits setWithUnits;

	bool isSet;
public:
	Speed(double speed, SpeedUnits units = SpeedUnits::MetersPerSecond);
	Speed();
	void setSpeed(double speed, SpeedUnits units = SpeedUnits::MetersPerSecond);
	double getSpeed(SpeedUnits units = SpeedUnits::MetersPerSecond);
	SpeedUnits getUnits();

	bool operator==(Speed other);

	bool isSpeedSet();
};
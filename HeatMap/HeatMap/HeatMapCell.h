#pragma once
class HeatMapCell {
private:
	bool setThisCycle;
	int activities;
	double value;
	double deltaValue;
	double normalizedValue;
public:
	HeatMapCell();
	double getValue();
	void addToValue(double amount);
	double getNormalizedValue();
	void setNormalizedValue(double amount);
	void incrementCounter();
	int getActivities();
};
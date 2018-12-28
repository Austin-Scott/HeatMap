#pragma once
class HeatMapCell {
private:
	double value;
	double normalizedValue;
public:
	HeatMapCell();
	double getValue();
	void addToValue(double amount);
	double getNormalizedValue();
	void setNormalizedValue(double amount);
};
#pragma once
#include <string>

using std::string;

enum Month { January=1, February, March, April, May, June, July, August, September, October, November, December };
enum Weekday { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

class Date {
private:
	int year=0;
	Month month=Month::January;
	int day=0;
	int hour=0;
	int minute=0;
	int second=0;
public:
	Date();
	Date(int year, Month month, int day, int hour, int minute, int second);

	string toString();
	bool isMoreRecentThan(Date other);
	bool isOlderThan(Date other);
	bool isConcurrentWith(Date other);

	bool operator==(Date other) const;

	int getYear();
	Month getMonth();
	int getDay();
	int getHour();
	int getMinute();
	int getSecond();

	Weekday getDayOfWeek();
	
	bool isValidDate();

	Date offsetByNumOfDays(int numOfDays);

	static Date now();
	static Date createDate(int year, Month month, int day, int hour, int minute, int second);
	static Date parseDateString(string dateString);
	
	static int getDaysInMonth(Month month, int year);
};
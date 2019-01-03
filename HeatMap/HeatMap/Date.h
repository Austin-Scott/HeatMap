#pragma once
#include <string>
#include <regex>

using namespace std;

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

	bool isSet;
	template<class T>string padZeros(int width, T value) {
		string result = to_string(value);
		if (width < result.length()) {
			result = "";
		}
		while (result.length() < width) {
			result = "0" + result;
		}
		return result;
	}
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

	bool isDateSet();

	string toFormalString(); //form YYYY-MM-DD

	static Date now();
	static Date createDate(int year, Month month, int day, int hour, int minute, int second);
	static Date createDate(time_t seconds); //Seconds as reported by Garmin's .fit date stamps
	static Date parseDateString(string dateString); //dateString in Zulu time. Function creates a Date object in local time.

	static Date fromFormalString(string dateString); //form YYYY-MM-DD
	
	static int getDaysInMonth(Month month, int year);
};
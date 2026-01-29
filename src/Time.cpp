#include "Time.h"

int timeInMin(std::string& time)
{
	if (time.length() != 5 || time[2] != ':') return -1;
	int h = std::stoi(time.substr(0, 2));
	int m = std::stoi(time.substr(3, 2));
	return h * 60 + m;
}

std::string minInTime(int mins)
{
	if (mins < 0) return std::string();
	std::string time = mins / 60 < 10 ? "0" + std::to_string(mins / 60) : std::to_string(mins / 60);
	time += ':';
	time += mins % 60 < 10 ? "0" + std::to_string(mins % 60) : std::to_string(mins % 60);

	return time;
}


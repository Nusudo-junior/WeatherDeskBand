#pragma once
#include<string>
#include<vector>
#include<iostream>
class Weather
{
private:
	struct data {
		std::wstring Max_temperature,
			min_temperature;
		std::vector<std::wstring> Current_temperature;
		std::vector<int> weathercode;
	};
public:
	void GetWeather();
	data TodaysWeather;
private:
	LPCWSTR base = L"https://api.open-meteo.com/v1/forecast?latitude=34.69&longitude=135.50&hourly=temperature_2m,weathercode&daily=weathercode,temperature_2m_max,temperature_2m_min,sunrise,sunset&timeformat=unixtime&timezone=Asia%2FTokyo";
	inline bool night(int weathercode,int hour, int sunrisehour, int sunsethour);
};

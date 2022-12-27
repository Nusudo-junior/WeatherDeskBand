#pragma once
#include<string>
#include<vector>
#include<iostream>

class Weather
{
private:
	struct data {
		int date=-1;
		std::wstring Max_temperature,
			min_temperature;
		std::vector<std::wstring> Current_temperature;
		std::vector<int> weathercode;
	};
public:
	void SetURL(std::wstring url);
	void GetWeather();
	data TodaysWeather;
private:
	std::wstring base;
	inline bool night(int weathercode,int hour, int sunrisehour, int sunsethour);
};

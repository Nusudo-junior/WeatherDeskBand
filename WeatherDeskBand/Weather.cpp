#include"pch.h"
#include"Weather.h"
#include<format>
#include <cpprest/http_client.h>
#include <nlohmann/json.hpp>
#include"OutputLog.h"

using json = nlohmann::json;

void Weather::SetURL(std::wstring url) {
	base = url;
}

void Weather::GetWeather() {
    try {
		web::http::client::http_client_config config;
		config.set_timeout(utility::seconds(5));
		web::http::client::http_client client(base.c_str(), config);
		auto response = client.request(web::http::methods::GET).get();
		if (response.status_code() == web::http::status_codes::OK)
		{
			auto body = response.extract_string().get();
			auto json = json::parse(body);
			time_t unix_date = json["daily"]["time"][0];
			struct tm local_date;
			localtime_s(&local_date, &unix_date);
			TodaysWeather.date = local_date.tm_yday;
			TCHAR buf[8];
			swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"%4.1f", (float)json["daily"]["temperature_2m_max"][0]);
			TodaysWeather.Max_temperature = buf;
			swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"%4.1f", (float)json["daily"]["temperature_2m_min"][0]);
			TodaysWeather.min_temperature = buf;

			struct tm sunrise, sunset;
			time_t unixsunrise = json["daily"]["sunrise"][0], unixsunset = json["daily"]["sunset"][0];
			localtime_s(&sunrise, &unixsunrise);
			localtime_s(&sunset, &unixsunset);

			TodaysWeather.Current_temperature.clear();
			TodaysWeather.weathercode.clear();
			for (int i = 0; i < 24; i++) {
				if (json["hourly"]["temperature_2m"][i].is_null() == false) {
					swprintf_s(buf, sizeof(buf) / sizeof(wchar_t), L"%4.1fŽ", (float)json["hourly"]["temperature_2m"][i]);
					TodaysWeather.Current_temperature.emplace_back(buf);
				}
				else TodaysWeather.Current_temperature.emplace_back(L"ERR");

				if (json["hourly"]["weathercode"][i].is_null() == false) {
					if (night(json["hourly"]["weathercode"][i], i, sunrise.tm_hour, sunset.tm_hour))
						json["hourly"]["weathercode"][i] = json["hourly"]["weathercode"][i].get<int>() + 100;
					TodaysWeather.weathercode.emplace_back(json["hourly"]["weathercode"][i]);
				}
				else TodaysWeather.weathercode.emplace_back(-1);
			}
		}
		else throw web::http::http_exception(response.status_code());
	}
	catch (const web::http::http_exception& e)
    {
		time_t t = time(NULL);
		struct tm localTime;
		localtime_s(&localTime, &t);
		if (localTime.tm_yday == TodaysWeather.date)return;

		OutputLog(e);
    }
}

inline bool Weather::night(int weathercode,int hour, int sunrisehour, int sunsethour) {
    if(
        (weathercode>=0 && weathercode<=2) 
        && 
        (hour<sunrisehour || hour>sunsethour)
        )return true;
       else return false;

}
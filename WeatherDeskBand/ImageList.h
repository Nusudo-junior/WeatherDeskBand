#pragma once
#include <windows.h>
#include<commctrl.h>
#include<uxtheme.h>
#include<vector>
#include<map>

extern HINSTANCE g_hinstDll;

class Images {
public:
	Images();
	HIMAGELIST ImageList;
	std::map<int, int> codetoindex;
private:
	struct codeandfile {
		int weathercode;
		LPCWSTR filename;
	};
	std::vector<codeandfile>files = 
	{
		{0,L"sunny"},{1,L"sunny"},{2,L"partlycloudy"},{3,L"cloudy"},
		{45,L"fog"},{48,L"fog"},
		{51,L"drizzle1"},{53,L"drizzle2"},{55,L"drizzle3"},{56,L"freezing_drizzle"},{57,L"freezing_drizzle"},
		{61,L"rainy1"},{63,L"rainy2"},{65,L"rainy3"},{66,L"freezing_rain"},{67,L"freezing_rain"},
		{71,L"snow1"},{73,L"snow2"},{75,L"snow3"},{77,L"snowgrain"},
		{80,L"rainy1"},{81,L"rainy2"},{82,L"rainy3"},{85,L"snow1"},{86,L"snow3"},
		{95,L"thunderstorm"},{96,L"thunderstorm_hail"},{99,L"thunderstorm_hail"},
		{100,L"moon"},{101,L"moon"},{102,L"moonpartly"},
		{-1,L"None"}
	};
};

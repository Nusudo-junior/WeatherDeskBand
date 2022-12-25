#pragma once
#include<windows.h>
#include<format>
#include<time.h>
#include<fstream>

extern HINSTANCE g_hinstDll;

inline void OutputLog(auto e) {
	time_t t = time(NULL);
	struct tm localTime;
	localtime_s(&localTime, &t);
	char path[MAX_PATH + 1];
	GetModuleFileNameA(g_hinstDll, path, MAX_PATH);
	char drive[MAX_PATH + 1]
		, dir[MAX_PATH + 1]
		, fname[MAX_PATH + 1]
		, ext[MAX_PATH + 1];
	_splitpath_s(path, drive, dir, fname, ext);
	std::ofstream writing_file;
	std::string filename = std::format("{}{}/images/log.txt", drive, dir);
	writing_file.open(filename, std::ios::app);
	std::string errlog = std::format("{}-{}-{} {}:{}:{} Error:{}",
		localTime.tm_year + 1900,
		localTime.tm_mon + 1,
		localTime.tm_mday,
		localTime.tm_hour,
		localTime.tm_min,
		localTime.tm_sec,
		e.what());
	writing_file << errlog << std::endl;
	writing_file.close();
}
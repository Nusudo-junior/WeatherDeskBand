#pragma once
#include<windows.h>
#include<string>
#include<vector>
#include<utility>

extern HINSTANCE g_hinstDll;

class ImportConfig {
public:
	std::wstring URL;
	std::vector<std::pair<int, std::wstring>> filenames;
	ImportConfig();
};
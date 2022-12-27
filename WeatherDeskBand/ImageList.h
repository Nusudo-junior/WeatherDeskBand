#pragma once
#include <windows.h>
#include<commctrl.h>
#include<uxtheme.h>
#include<vector>
#include<map>
#include<utility>
#include<string>

extern HINSTANCE g_hinstDll;

class Images {
public:
	void MakeImageList();
	HIMAGELIST ImageList;
	void SetFileList(std::vector<std::pair<int,std::wstring>> filenames);
	std::map<int, int> codetoindex;
private:
	std::vector<std::pair<int, std::wstring>>files;
};

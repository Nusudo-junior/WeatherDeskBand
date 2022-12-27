#include"pch.h"
#include"ImageList.h"
#include<format>
#pragma comment (lib, "uxtheme.lib")
#pragma comment (lib, "Comctl32.lib")

void Images::SetFileList(std::vector<std::pair<int, std::wstring>> _files) {
	files = _files;
}

void Images::MakeImageList() {
	SetProcessDPIAware();
	int zoom = GetDpiForSystem() / 96;
	ImageList = ImageList_Create(32*zoom,32*zoom, ILC_COLOR32, 21, 1);
	TCHAR path[MAX_PATH + 1];
	GetModuleFileName(g_hinstDll, path, MAX_PATH);
	TCHAR drive[MAX_PATH + 1]
		, dir[MAX_PATH + 1]
		, fname[MAX_PATH + 1]
		, ext[MAX_PATH + 1];
	_wsplitpath_s(path, drive, dir, fname, ext);
	std::map<std::wstring, int> Iconindex;
	int indexnum = 0;
	for (auto file:files) {
		if (Iconindex.find(file.second) != Iconindex.end()) {
			codetoindex[file.first] = Iconindex[file.second];
			continue;
		}
		std::wstring filepath = std::format(L"{}{}/images/{}.ico",drive,dir, file.second);
		HICON _image = (HICON)LoadImage(NULL, filepath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		ImageList_AddIcon(ImageList, _image,(HICON)NULL);
		codetoindex[file.first] = indexnum;
		Iconindex[file.second] = indexnum;
		indexnum++;
		DestroyIcon(_image);
	}
}

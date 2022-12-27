#include"ImportConfig.h"
#include <atlbase.h>  // CComPtrを使用するため
#include <atlstr.h>   // CStringを使用するため
#include<sstream>
#include <xmllite.h>
#include<format>
#pragma comment(lib, "xmllite.lib")

#pragma comment(lib,"User32.lib")

ImportConfig::ImportConfig() {
	CComPtr<IXmlReader> pReader;
	CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&pReader), 0);
	TCHAR path[MAX_PATH + 1];
	GetModuleFileName(g_hinstDll, path, MAX_PATH);
	TCHAR drive[MAX_PATH + 1]
		, dir[MAX_PATH + 1]
		, fname[MAX_PATH + 1]
		, ext[MAX_PATH + 1];
	_wsplitpath_s(path, drive, dir, fname, ext);
	std::wstring cfgpath = std::format(L"{}{}/WeatherDeskBand.xml", drive, dir);
	CComPtr<IStream> pStream;
	SHCreateStreamOnFile(cfgpath.c_str(), STGM_READ, &pStream);
	pReader->SetInput(pStream);
	XmlNodeType nodeType;

	while (S_OK == pReader->Read(&nodeType)) {
		LPCWSTR locname = NULL;
		switch (nodeType) {
		case XmlNodeType_Element: {
			pReader->GetLocalName(&locname, NULL);
			if (lstrcmp(locname, L"URL") == 0) {
				HRESULT hr = pReader->MoveToFirstAttribute();
				if (S_FALSE == hr) {
					break;
				}
				LPCWSTR pwszAttributeName;
				pReader->GetLocalName(&pwszAttributeName, NULL);
				LPCWSTR pwszAttributeValue;
				pReader->GetValue(&pwszAttributeValue, NULL);
				if (lstrcmp(pwszAttributeName, L"src") == 0) {
					URL = pwszAttributeValue;
				}
			}
			else if (lstrcmp(locname, L"Icon") == 0) {
				HRESULT hr = pReader->MoveToFirstAttribute();

				if (S_FALSE == hr) {
					break;
				}
				LPCWSTR pwszAttributeName;
				LPCWSTR pwszAttributeValue;
				int key;
				std::wstring filename;
				do {
					pReader->GetLocalName(&pwszAttributeName, NULL);
					pReader->GetValue(&pwszAttributeValue, NULL);
					if (lstrcmp(pwszAttributeName, L"key") == 0) {
						key = std::stoi(pwszAttributeValue);
					}
					if (lstrcmp(pwszAttributeName, L"filename") == 0) {
						filename = pwszAttributeValue;
					}
				} while (pReader->MoveToNextAttribute() == S_OK);
				filenames.push_back({key, filename});
			}
			break;
		}
		default:
			break;
		}

	}

}
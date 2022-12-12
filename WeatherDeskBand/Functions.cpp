#include"pch.h"
#include <windows.h>
#include <ShlObj.h>
#include <shlwapi.h>
#include "CClassFactory.h"
#include"ClsId.h"

#pragma comment (lib, "Shlwapi.lib")

LONG g_lLocks = 0;
extern HINSTANCE g_hinstDll;

void LockModule(BOOL bLock)
{
	if (bLock)
		InterlockedIncrement(&g_lLocks);
	else
		InterlockedDecrement(&g_lLocks);
}

BOOL CreateRegistryKey(HKEY hKeyRoot, LPCWSTR lpszKey, LPCWSTR lpszValue, LPCWSTR lpszData)
{
	HKEY  hKey;
	LONG  lResult;
	DWORD dwSize;

	lResult = RegCreateKeyEx(hKeyRoot, lpszKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
	if (lResult != ERROR_SUCCESS)
		return FALSE;

	if (lpszData != NULL)
		dwSize = (lstrlen(lpszData) + 1) * sizeof(TCHAR);
	else
		dwSize = 0;

	RegSetValueEx(hKey, lpszValue, 0, REG_SZ, (LPBYTE)lpszData, dwSize);
	RegCloseKey(hKey);

	return TRUE;
}


BOOL RegisterCategoryManager(BOOL bRegister)
{
	HRESULT      hr;
	ICatRegister* pCatRegister;
	CATID        catid = CATID_DeskBand;

	CoInitialize(NULL);

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pCatRegister));
	if (SUCCEEDED(hr)) {
		if (bRegister)
			hr = pCatRegister->RegisterClassImplCategories(CLSID_Weather_DeskBand, 1, &catid);
		else
			hr = pCatRegister->UnRegisterClassImplCategories(CLSID_Weather_DeskBand, 1, &catid);
		pCatRegister->Release();
	}

	CoUninitialize();

	return hr;
}

STDAPI DllCanUnloadNow()
{
	return g_lLocks == 0 ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	static CClassFactory serverFactory;
	HRESULT hr;

	*ppv = NULL;

	if (IsEqualCLSID(rclsid, CLSID_Weather_DeskBand))
		hr = serverFactory.QueryInterface(riid, ppv);
	else
		hr = CLASS_E_CLASSNOTAVAILABLE;

	return hr;
}

STDAPI DllRegisterServer(void)
{
	TCHAR szModulePath[256];
	TCHAR szKey[256];

	wsprintf(szKey, TEXT("CLSID\\%s"), g_szClsid);
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, NULL, TEXT("Weather DeskBand")))
		return E_FAIL;

	GetModuleFileName(g_hinstDll, szModulePath, sizeof(szModulePath) / sizeof(TCHAR));
	wsprintf(szKey, TEXT("CLSID\\%s\\InprocServer32"), g_szClsid);
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, NULL, szModulePath))
		return E_FAIL;

	wsprintf(szKey, TEXT("CLSID\\%s\\InprocServer32"), g_szClsid);
	if (!CreateRegistryKey(HKEY_CLASSES_ROOT, szKey, TEXT("ThreadingModel"), TEXT("Apartment")))
		return E_FAIL;

	RegisterCategoryManager(TRUE);

	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
	TCHAR szKey[256];

	wsprintf(szKey, TEXT("CLSID\\%s"), g_szClsid);
	SHDeleteKey(HKEY_CLASSES_ROOT, szKey);

	RegisterCategoryManager(FALSE);

	return S_OK;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {

	case DLL_PROCESS_ATTACH:
		g_hinstDll = hinstDll;
		DisableThreadLibraryCalls(hinstDll);
		return TRUE;

	}

	return TRUE;
}


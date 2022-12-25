#pragma once
#include<ShlObj.h>
#include"ImageList.h"
#include"Weather.h"

class CDeskBand :public IDeskBand2, public IObjectWithSite, public IPersistStream {
public:
	/*AllInterface*/
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	/*IDeskBand2*/
	STDMETHODIMP GetWindow(HWND* phwnd);
	STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);
	STDMETHODIMP ShowDW(BOOL bShow);
	STDMETHODIMP CloseDW(DWORD dwReserved);
	STDMETHODIMP ResizeBorderDW(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);
	STDMETHODIMP GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi);
	STDMETHODIMP CanRenderComposited(BOOL* pfCanRenderComposited);
	STDMETHODIMP SetCompositionState(BOOL fCompositionEnabled);
	STDMETHODIMP GetCompositionState(BOOL* pfCompositionEnabled);
	/*IObjectWithSite*/
	STDMETHODIMP SetSite(IUnknown* pUnkSite);
	STDMETHODIMP GetSite(REFIID riid, void** ppvSite);
	/*IPersirtStream*/
	STDMETHODIMP GetClassID(CLSID* pClassID);
	STDMETHODIMP IsDirty();
	STDMETHODIMP Load(IStream* pStm);
	STDMETHODIMP Save(IStream* pStm, BOOL fClearDirty);
	STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pcbSize);

	CDeskBand();
	~CDeskBand();
	BOOL RegisterAndCreateWindow(HWND hwndParent);
	BOOL OnPaint(HWND hwnd);
	BOOL UpdateBand(HWND hwnd);
	BOOL SetUpdateTimer(HWND hwnd);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:

	LONG     m_cRef;
	HWND     m_hwnd;
	int zoom;
	BOOL     m_bCompositionEnabled;
	IUnknown* m_pSite;
	int Current_Hour;
	Weather _weather;
	Images weathericons;
};
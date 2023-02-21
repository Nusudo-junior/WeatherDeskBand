#include "pch.h"
#include <algorithm>
#include <uxtheme.h>
#include <shlwapi.h>
#include<Vsstyle.h>
#include<Vssym32.h>
#include<time.h>
#include "DeskBand.h"
#include "ClsId.h"
#include"ImageList.h"
#include"Weather.h"
#include<iostream>
#include <fstream>
#include"OutputLog.h"
#include"ImportConfig.h"

#define ID_BANDWINDOW 100
#define TIMER_ID 1
#define RECTWIDTH(x)   ((x).right - (x).left)
#define RECTHEIGHT(x)  ((x).bottom - (x).top)

#pragma comment (lib, "shlwapi.lib")
#pragma comment (lib, "uxtheme.lib")

HINSTANCE g_hinstDll = NULL;
extern void LockModule(BOOL bLock);

#pragma region IUnknown
STDMETHODIMP CDeskBand::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = NULL;

    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleWindow) || IsEqualIID(riid, IID_IDockingWindow) || IsEqualIID(riid, IID_IDeskBand) || IsEqualIID(riid, IID_IDeskBand2))
        *ppvObject = static_cast<IDeskBand*>(this);
    else if (IsEqualIID(riid, IID_IPersist) || IsEqualIID(riid, IID_IPersistStream))
        *ppvObject = static_cast<IPersistStream*>(this);
    else if (IsEqualIID(riid, IID_IObjectWithSite))
        *ppvObject = static_cast<IObjectWithSite*>(this);
    else
        return E_NOINTERFACE;

    AddRef();

    return S_OK;
}

STDMETHODIMP_(ULONG) CDeskBand::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CDeskBand::Release()
{
    if (InterlockedDecrement(&m_cRef) == 0) {
        delete this;
        return 0;
    }

    return m_cRef;
}
#pragma endregion

#pragma region IOleWindow
STDMETHODIMP CDeskBand::ContextSensitiveHelp(BOOL fEnterMode)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDeskBand::GetWindow(HWND* phwnd)
{
    *phwnd = m_hwnd;

    return S_OK;
}

STDMETHODIMP CDeskBand::ShowDW(BOOL bShow)
{
    if (m_hwnd != NULL)
        ShowWindow(m_hwnd, bShow ? SW_SHOW : SW_HIDE);
    return S_OK;
}

STDMETHODIMP CDeskBand::CloseDW(DWORD dwReserved)
{
    if (m_hwnd != NULL) {
        ShowWindow(m_hwnd, SW_HIDE);
        DestroyWindow(m_hwnd);
    }

    return S_OK;
}


STDMETHODIMP CDeskBand::ResizeBorderDW(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved)
{
    return E_NOTIMPL;
}
#pragma endregion

#pragma region IDeskBand
STDMETHODIMP CDeskBand::GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi)
{
    if (pdbi == NULL)
        return E_INVALIDARG;

    SetProcessDPIAware();
    zoom = GetDpiForSystem() / 96;

    if (pdbi->dwMask & DBIM_MINSIZE) {
        pdbi->ptMinSize.x = 122*zoom;
        pdbi->ptMinSize.y = 30;
    }

    if (pdbi->dwMask & DBIM_MAXSIZE)
        pdbi->ptMaxSize.x = -1;
        pdbi->ptMaxSize.y = -1;

    if (pdbi->dwMask & DBIM_INTEGRAL)
        pdbi->ptIntegral.y = 1;

    if (pdbi->dwMask & DBIM_ACTUAL) {
        pdbi->ptActual.x = 122*zoom;
        pdbi->ptActual.y = 30;
    }

    if (pdbi->dwMask & DBIM_TITLE)
        pdbi->dwMask &= ~DBIM_TITLE;

    if (pdbi->dwMask & DBIM_MODEFLAGS)
        pdbi->dwModeFlags = DBIMF_VARIABLEHEIGHT| DBIMF_FIXED;

    if (pdbi->dwMask & DBIM_BKCOLOR)
        pdbi->dwMask &= ~DBIM_BKCOLOR;

    return S_OK;
}

#pragma endregion

#pragma region IDeskBand2 
STDMETHODIMP CDeskBand::CanRenderComposited(BOOL* pfCanRenderComposited)
{
    *pfCanRenderComposited = TRUE;

    return S_OK;
}

STDMETHODIMP CDeskBand::SetCompositionState(BOOL fCompositionEnabled)
{
    m_bCompositionEnabled = fCompositionEnabled;

    InvalidateRect(m_hwnd, NULL, FALSE);
    UpdateWindow(m_hwnd);

    return S_OK;
}

STDMETHODIMP CDeskBand::GetCompositionState(BOOL* pfCompositionEnabled)
{
    *pfCompositionEnabled = m_bCompositionEnabled;

    return S_OK;
}
#pragma endregion

#pragma region IObjectWithSite
STDMETHODIMP CDeskBand::SetSite(IUnknown* pUnkSite)
{
    if (m_pSite != NULL) {
        m_pSite->Release();
        m_pSite = NULL;
    }

    if (pUnkSite != NULL) {
        HWND       hwndParent;
        IOleWindow* pOleWindow;

        pUnkSite->QueryInterface(IID_PPV_ARGS(&pOleWindow));
        pOleWindow->GetWindow(&hwndParent);
        pOleWindow->Release();

        RegisterAndCreateWindow(hwndParent);

        pUnkSite->QueryInterface(IID_PPV_ARGS(&m_pSite));
    }

    return S_OK;
}

STDMETHODIMP CDeskBand::GetSite(REFIID riid, void** ppvSite)
{
    HRESULT hr;

    *ppvSite = NULL;

    if (m_pSite != NULL)
        hr = m_pSite->QueryInterface(riid, ppvSite);
    else
        hr = E_FAIL;

    return hr;
}
#pragma endregion

#pragma region IPersist
STDMETHODIMP CDeskBand::GetClassID(CLSID* pClassID)
{
    *pClassID = CLSID_Weather_DeskBand;

    return S_OK;
}
#pragma endregion

#pragma region IPersistStream
STDMETHODIMP CDeskBand::IsDirty()
{
    return E_NOTIMPL;
}

STDMETHODIMP CDeskBand::Load(IStream* pStm)
{
    return S_OK;
}

STDMETHODIMP CDeskBand::Save(IStream* pStm, BOOL fClearDirty)
{
    return S_OK;
}

STDMETHODIMP CDeskBand::GetSizeMax(ULARGE_INTEGER* pcbSize)
{
    return E_NOTIMPL;
}
#pragma endregion

CDeskBand::CDeskBand()
{
    m_cRef = 1;
    m_hwnd = NULL;
    m_bCompositionEnabled = FALSE;
    m_pSite = NULL;
    ImportConfig Importconfig;
    weathericons.SetFileList(Importconfig.filenames);
    weathericons.MakeImageList();
    _weather.SetURL(Importconfig.URL);
    LockModule(TRUE);
}

CDeskBand::~CDeskBand()
{
    LockModule(FALSE);
}


BOOL CDeskBand::RegisterAndCreateWindow(HWND hwndParent)
{
    TCHAR      szClassName[] = TEXT("Weather deskband");
    WNDCLASSEX wc;

    if (!GetClassInfoEx(g_hinstDll, szClassName, &wc)) {
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = 0;
        wc.lpfnWndProc = WindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = g_hinstDll;
        wc.hIcon = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);
        wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
        wc.hbrBackground = GetSysColorBrush(COLOR_MENU);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = szClassName;
        wc.hIconSm = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_SHARED);

        RegisterClassEx(&wc);
    }
    m_hwnd = CreateWindowEx(0, szClassName, TEXT(""), WS_CHILD | WS_CLIPSIBLINGS, 0, 0, 0, 0, hwndParent, (HMENU)ID_BANDWINDOW, g_hinstDll, this);
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);

    return TRUE;
}

LRESULT CALLBACK CDeskBand::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static CDeskBand* p = NULL;

    switch (uMsg) {

    case WM_CREATE: {
        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
        p = (CDeskBand*)(lpcs->lpCreateParams);
        return 0;
    }
    case WM_PAINT: {
        p->SetUpdateTimer(hwnd);
        p->OnPaint(hwnd);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        p->_weather.GetWeather();
        p->UpdateBand(hwnd);
    }
    case WM_TIMER: {
        p->UpdateBand(hwnd);
        return 0;
    }
    case WM_THEMECHANGED:
        p->m_bCompositionEnabled = !p->m_bCompositionEnabled;
        break;
    case WM_SIZE:
        return 0;
    case WM_ERASEBKGND:
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CDeskBand::OnPaint(HWND hwnd) {
    HDC         hdc;
    RECT        rc;
    PAINTSTRUCT ps;
    hdc = BeginPaint(hwnd, &ps);
    GetClientRect(hwnd, &rc);
    if (m_bCompositionEnabled) {
        HTHEME       htheme;
        HDC          hdcPaint;
        HPAINTBUFFER hBufferedPaint;
        DTTOPTS      dttOpts;
        htheme = OpenThemeData(NULL, L"STARTPANEL");
        hBufferedPaint = BeginBufferedPaint(hdc, &rc, BPBF_TOPDOWNDIB, NULL, &hdcPaint);
        DrawThemeParentBackground(hwnd, hdcPaint, &rc);
        ZeroMemory(&dttOpts, sizeof(DTTOPTS));
        dttOpts.dwSize = sizeof(DTTOPTS);
        dttOpts.dwFlags = DTT_COMPOSITED | DTT_TEXTCOLOR;
        dttOpts.crText = RGB(255,255,255);

        RECT Current_temperature = { (RECTWIDTH(rc) / 4) ,0,RECTWIDTH(rc) * 3 / 4 ,RECTHEIGHT(rc) };//left,top,right,bottom
        RECT MAXtemperature = { (RECTWIDTH(rc)*3/4) ,0,RECTWIDTH(rc) ,RECTHEIGHT(rc)/2 };//left,top,right,bottom
        RECT mintemperature = { (RECTWIDTH(rc)*3/4) ,RECTHEIGHT(rc)/2,RECTWIDTH(rc) ,RECTHEIGHT(rc) };//left,top,right,bottom
        int iconsize = 32 * zoom;
        RECT Current_weather= {(RECTWIDTH(rc)/4-iconsize)/2, (RECTHEIGHT(rc)-iconsize)/2, (RECTWIDTH(rc) / 4 - iconsize) / 2+iconsize, (RECTHEIGHT(rc) - iconsize) / 2 +iconsize}; 
        try {
            int Index = min(Current_Hour, _weather.TodaysWeather.Current_temperature.size()-1);
            DrawThemeIcon(htheme, hdcPaint, SPP_USERPANE, 0, &Current_weather, weathericons.ImageList, weathericons.codetoindex[_weather.TodaysWeather.weathercode[Index]]);
            DrawThemeTextEx(htheme, hdcPaint, SPP_USERPANE, 0, _weather.TodaysWeather.Current_temperature[Index].c_str(), -1, DT_SINGLELINE | DT_CENTER | DT_VCENTER, &Current_temperature, &dttOpts);
        }
        catch (std::out_of_range& oor) {
            DrawThemeTextEx(htheme, hdcPaint, SPP_USERPANE, 0, L"ERR", -1, DT_SINGLELINE | DT_CENTER | DT_VCENTER, &Current_temperature, &dttOpts);
            OutputLog(oor);
        }
        CloseThemeData(htheme);

        htheme = OpenThemeData(NULL, L"TASKBAND");
        dttOpts.crText = RGB(255, 0, 0);
        DrawThemeTextEx(htheme, hdcPaint, TDP_GROUPCOUNT, 0, _weather.TodaysWeather.Max_temperature.c_str(), -1, DT_SINGLELINE | DT_CENTER | DT_VCENTER, &MAXtemperature, &dttOpts);
        dttOpts.crText = RGB(0, 178, 238);
        DrawThemeTextEx(htheme, hdcPaint, TDP_GROUPCOUNT, 0, _weather.TodaysWeather.min_temperature.c_str(), -1, DT_SINGLELINE | DT_CENTER | DT_VCENTER, &mintemperature, &dttOpts);
        
        EndBufferedPaint(hBufferedPaint, TRUE);
        CloseThemeData(htheme);

    }
    else {
        FillRect(hdc, &rc, GetSysColorBrush(COLOR_MENU));
        SetBkMode(hdc, TRANSPARENT);
    }

    EndPaint(hwnd, &ps);
    return 0;
}

BOOL CDeskBand::UpdateBand(HWND hwnd) {
    InvalidateRect(hwnd, NULL, FALSE);
    UpdateWindow(hwnd);
    return 0;
}

BOOL CDeskBand::SetUpdateTimer(HWND hwnd) {
    time_t t = time(NULL);
    struct tm localTime;
    localtime_s(&localTime, &t);
    int nextupdate = 60 - localTime.tm_min;
    Current_Hour = localTime.tm_hour;
    if(_weather.TodaysWeather.date!=localTime.tm_yday||Current_Hour%6==0)
    _weather.GetWeather();
    SetTimer(hwnd, TIMER_ID, nextupdate*60*1000, NULL);
    return true;
}
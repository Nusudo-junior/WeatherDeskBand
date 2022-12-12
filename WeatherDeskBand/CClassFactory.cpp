#include"pch.h"
#include"DeskBand.h"
#include "CClassFactory.h"

extern void LockModule(BOOL bLock);

STDMETHODIMP CClassFactory::QueryInterface(REFIID riid, void** ppvObject)
{
	*ppvObject = NULL;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
		*ppvObject = static_cast<IClassFactory*>(this);
	else
		return E_NOINTERFACE;

	AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) CClassFactory::AddRef()
{
	LockModule(TRUE);

	return 2;
}

STDMETHODIMP_(ULONG) CClassFactory::Release()
{
	LockModule(FALSE);

	return 1;
}

STDMETHODIMP CClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject)
{
	CDeskBand* p;
	HRESULT   hr;

	*ppvObject = NULL;

	if (pUnkOuter != NULL)
		return CLASS_E_NOAGGREGATION;

	p = new CDeskBand();
	if (p == NULL)
		return E_OUTOFMEMORY;

	hr = p->QueryInterface(riid, ppvObject);
	p->Release();

	return hr;
}

STDMETHODIMP CClassFactory::LockServer(BOOL fLock)
{
	LockModule(fLock);

	return S_OK;
}
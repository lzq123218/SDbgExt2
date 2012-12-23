#pragma once

#include <DbgEng.h>
#include <vector>
#include <clrdata.h>
#include "IXCLRDataProcess3.h"
#include "IDacMemoryAccess.h"

typedef HRESULT (__stdcall *CLRDataCreateInstancePtr)(REFIID iid, ICLRDataTarget* target, void** iface);

#define RETURN_IF_FAILED(exp) if (FAILED(hr = (exp))) return hr;

inline LPWSTR AllocString(size_t len)
{
	return (LPWSTR)malloc(sizeof(WCHAR) * len);
}

inline LPWSTR AllocString(LPWSTR str)
{
	if (str == NULL) return NULL;

	size_t len = wcslen(str);
	LPWSTR newStr = (LPWSTR)malloc(sizeof(WCHAR) * (len+1));

	wcscpy_s(newStr, len+1, str);

	return newStr;
}

inline void FreeString(LPWSTR str)
{
	free(str);
}

inline void FreeString(LPCWSTR str)
{
	free((void*)str);
}

template <class T>
inline void DeleteVector(std::vector<T*> *x)
{
	for (std::vector<T*>::iterator a = x->begin(); a != x->end(); a++)
	{
		delete *a;
	}
	delete x;
}

HRESULT InitRemoteProcess(DWORD dwProcessId, IXCLRDataProcess3 **ppDac, IDacMemoryAccess **ppDcma);

/*
struct DebugInterfaces
{
	IXCLRDataProcess *pDac;
	IDebugClient *client;
	IDebugControl4 *ctrl;
	IDebugSymbols3 *sym;
	IDebugSystemObjects *sysobjs;
	IDebugDataSpaces *data;
	IDacpCoreMemoryAccess *dcma;
	
	BOOL DetachOnClose;

	DebugInterfaces()
	{
		pDac = NULL;
		client = NULL;
		ctrl = NULL;
		sym = NULL;
		sysobjs = NULL;
		data = NULL;
		dcma = NULL;
		DetachOnClose = FALSE;
	}

	~DebugInterfaces()
	{
		if (ctrl)	
			ctrl->Release();

		if (sym)
			sym->Release();
	
		if (sysobjs)
			sysobjs->Release();

		if (data)
			data->Release();

		if (pDac)
			pDac->Release();

		if (dcma)
			dcma->Release();

		if (DetachOnClose && client)
		{
			client->DetachProcesses();
			client->Release();
		}
	}
};
*/

//HRESULT CreateDebugInterfaces(IDebugClient *cli, DebugInterfaces *interfaces, BOOL waitForEvent);
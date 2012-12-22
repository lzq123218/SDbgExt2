#include "stdafx.h"
#include "..\inc\SDbgCore.h"
#include "..\inc\DbgEngCLRDataTarget.h"
#include "..\inc\IXCLRDataProcess3.h"
#include "..\inc\IDacMemoryAccess.h"
#include "..\inc\DbgEngMemoryAccess.h"
#include <DbgHelp.h>
#ifndef NO_WDBG_EXT
#include <wdbgexts.h>
#endif

#pragma unmanaged

#define CORDAC_FORMAT L"%s\\Microsoft.NET\\Framework%s\\v%s\\mscordacwks.dll"

#ifdef _M_IX86
	#define CORDAC_BITNESS L""
#else
	#define CORDAC_BITNESS L"64"
#endif

#ifdef CLR2
	#define CORDAC_CLRVER L"2.0.50727"
#else
	#define CORDAC_CLRVER L"4.0.30319"
#endif

#define SYMBOL_PATH "srv*c:\\symcache*http://msdl.microsoft.com/download/symbols"


//IID IID_IXCLRDataProcess = {0x5c552ab6, 0xfc09, 0x4cb3, {0x8e,0x36,0x22,0xfa,0x03,0xc7,0x98,0xb7} };
//HRESULT InitIXCLRDataFromWinDBG(IXCLRDataProcess3 **ppDac);

HRESULT InitIXCLRData(IDebugClient *cli, IXCLRDataProcess3 **ppDac)
{
	ComHolder<IDebugSymbols3> dSym;
	ComHolder<IDebugDataSpaces> dds;
	ComHolder<IDebugSystemObjects> dso;

	RETURN_IF_FAILED(dSym.QueryInterface(cli));
	RETURN_IF_FAILED(dds.QueryInterface(cli));
	RETURN_IF_FAILED(dso.QueryInterface(cli));

	// Init CORDAC
	WCHAR winDirBuffer[MAX_PATH] = { 0 };
	WCHAR corDacBuffer[MAX_PATH] = { 0 };

	GetWindowsDirectory(winDirBuffer, ARRAYSIZE(winDirBuffer));
	swprintf_s(corDacBuffer, CORDAC_FORMAT, winDirBuffer, CORDAC_BITNESS, CORDAC_CLRVER);

	HMODULE hCorDac = LoadLibrary(corDacBuffer);
	if (hCorDac == NULL)
	{
		return FALSE;
	}
	CLRDataCreateInstancePtr clrData = (CLRDataCreateInstancePtr)GetProcAddress(hCorDac, "CLRDataCreateInstance");
	DbgEngCLRDataTarget *dataTarget = new DbgEngCLRDataTarget(dSym.GetObject_NoRef(), dds.GetObject_NoRef(), dso.GetObject_NoRef());
	
	RETURN_IF_FAILED(clrData(__uuidof(IXCLRDataProcess3), dataTarget, (PVOID*)ppDac));
		
	return S_OK;
}

HRESULT InitRemoteProcess(DWORD dwProcessId, IXCLRDataProcess3 **ppDac, IDacMemoryAccess **ppDcma)
{
	IDebugClient *cli;
	ComHolder<IDebugControl> ctrl;
	
	RETURN_IF_FAILED(DebugCreate(__uuidof(IDebugClient), (PVOID*)&cli));
	RETURN_IF_FAILED(cli->AttachProcess(NULL, dwProcessId, DEBUG_ATTACH_NONINVASIVE | DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND));
	RETURN_IF_FAILED(ctrl.QueryInterface(cli));
	RETURN_IF_FAILED(ctrl->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE));	

	IXCLRDataProcess3 *pcdp;
	RETURN_IF_FAILED(InitIXCLRData(cli, &pcdp));
	*ppDac = pcdp;

	ComHolder<IDebugDataSpaces> dds;
	dds.QueryInterface(cli);

	*ppDcma = new DbgEngMemoryAccess(dds.GetObject_NoRef());

	return S_OK;
}
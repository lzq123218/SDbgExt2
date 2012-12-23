#include "stdafx.h"
#include "..\inc\Tests.h"

#define RUN_STANDARD_TEST(name) (name(p)) ? (numSuccess++) : (numFailures++);

int RunAllTests(ClrProcess *p)
{
	int numFailures = 0; int numSuccess = 0;

	RUN_STANDARD_TEST(ClrThreadStoreData_Basic);
	RUN_STANDARD_TEST(ClrThreadData_Basic);
	RUN_STANDARD_TEST(ClrThreadData_Iterator);

	RUN_STANDARD_TEST(ClrAppDomainStoreData_Basic);
	RUN_STANDARD_TEST(ClrAppDomainList_Basic);
	RUN_STANDARD_TEST(ClrAppDomainData_Basic);
	RUN_STANDARD_TEST(ClrAssemblyList_Basic);


	printf("\r\n%d succeeded, %d failed\r\n", numSuccess, numFailures);

	return numFailures;
}

int _tmain(int argc, _TCHAR* argv[])
{
	const DWORD dwProcessId = 7032;

	CoInitialize(NULL);

	CComPtr<IXCLRDataProcess3> chDac; CComPtr<IDacMemoryAccess> chDcma;
	InitRemoteProcess(dwProcessId, &chDac, &chDcma);

	CComPtr<ClrProcess> p = new ClrProcess(chDac, chDcma);
	
	return RunAllTests(p);
}
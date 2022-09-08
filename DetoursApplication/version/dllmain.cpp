#include "Proxy.h"
#include "detours.h"

#pragma comment( lib, "..\\libs\\detours.lib")

typedef BOOL(WINAPI* pShellExecuteExW)(SHELLEXECUTEINFOW* pExecInfo);

bool isPartOf(LPCTSTR w1, LPCTSTR w2)
{
	int i = 0;
	int j = 0;


	while (w1[i] != '\0') {
		if (w1[i] == w2[j])
		{
			int init = i;
			while (w1[i] == w2[j] && w2[j] != '\0')
			{
				j++;
				i++;
			}
			if (w2[j] == '\0') {
				return true;
			}
			j = 0;
		}
		i++;
	}
	return false;
}

pShellExecuteExW oExec;
BOOL WINAPI newShellExecuteExW(SHELLEXECUTEINFOW* pExecInfo)
{

	// runas; open
	//MessageBoxW(NULL, pExecInfo->lpVerb, NULL, NULL);
	//if (isPartOf(pExecInfo->lpFile, TEXT("xx.exe"))) {
	//	MessageBoxW(NULL, pExecInfo->lpFile, NULL, NULL);
	//	return 0;
	//}
	MessageBoxW(NULL, pExecInfo->lpVerb, NULL, NULL);
	MessageBoxW(NULL, pExecInfo->lpFile, NULL, NULL);
	return oExec(pExecInfo);
}

VOID StartHook()
{
	oExec = ShellExecuteExW;
	DetourRestoreAfterWith();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oExec, newShellExecuteExW);
	DetourTransactionCommit();
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxW(NULL, L"ATTACH version.dll", NULL, NULL);
		StartHook();
		ProxyDll();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		FreeProxyDll();
		break;
	}
	return TRUE;
}


#include "Proxy.h"

VOID StartHook()
{
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WriteTable, NULL, NULL, NULL);

	// �ο� ͬ���� hook ��Ŀ�Ĵ���
	MessageBoxW(NULL, TEXT("dinput8.dll"), TEXT("hook"), NULL);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
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


#include <Windows.h>
#include "detours.h"

#pragma comment( lib, "..\\libs\\detours.lib")
// 需要加上不然调用 dll 报错
VOID __declspec(dllexport) MyFunc() {}

typedef int (WINAPI* pMsgBox)(
	HWND hWnd,
	LPCWSTR lpText,
	LPCWSTR lpCaption,
	UINT uType);
pMsgBox originMsgBox;
int WINAPI newMsgBox(
	HWND hWnd,
	LPCWSTR lpText,
	LPCWSTR lpCaption,
	UINT uType)

{
	lpText = L"my hook";
	return originMsgBox(hWnd, lpText, lpCaption, uType);
}

VOID StartHook()
{
	originMsgBox = MessageBoxW;
	DetourRestoreAfterWith();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)originMsgBox, newMsgBox);
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
		StartHook();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


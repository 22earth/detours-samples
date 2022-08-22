#include <Windows.h>
#include "detours.h"

#pragma comment( lib, "..\\libs\\detours.lib")

STARTUPINFOW si;
PROCESS_INFORMATION pi;

// https://github.com/microsoft/Detours/blob/master/samples/withdll/withdll.cpp
int main()
{
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	LPCSTR dllNames[] = { "hook.dll", "hook2.dll"};
	//LPCSTR dllName = { "hook.dll" };
	DetourCreateProcessWithDllsW(L"Test.exe", NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi, 1, dllNames, NULL);
	ResumeThread(pi.hThread);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return 0;
}


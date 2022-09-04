#pragma once

#include <windows.h>
#include <Shlwapi.h>

#pragma comment( lib, "Shlwapi.lib")

#pragma comment(linker, "/EXPORT:DirectInput8Create=_AheadLib_DirectInput8Create,@1")
#pragma comment(linker, "/EXPORT:DllCanUnloadNow=_AheadLib_DllCanUnloadNow,@2")
#pragma comment(linker, "/EXPORT:DllGetClassObject=_AheadLib_DllGetClassObject,@3")
#pragma comment(linker, "/EXPORT:DllRegisterServer=_AheadLib_DllRegisterServer,@4")
#pragma comment(linker, "/EXPORT:DllUnregisterServer=_AheadLib_DllUnregisterServer,@5")
#pragma comment(linker, "/EXPORT:GetdfDIJoystick=_AheadLib_GetdfDIJoystick,@6")


PVOID pfnAheadLib_DirectInput8Create;
PVOID pfnAheadLib_DllCanUnloadNow;
PVOID pfnAheadLib_DllGetClassObject;
PVOID pfnAheadLib_DllRegisterServer;
PVOID pfnAheadLib_DllUnregisterServer;
PVOID pfnAheadLib_GetdfDIJoystick;


HMODULE g_OldModule = NULL;

VOID WINAPI FreeProxyDll()
{
	if (g_OldModule)
	{
		FreeLibrary(g_OldModule);
	}
}

FARPROC WINAPI GetAddress(PCSTR pszProcName)
{
	FARPROC fpAddress = GetProcAddress(g_OldModule, pszProcName);
	if (fpAddress == NULL)
	{
		MessageBoxW(NULL, TEXT("Load dll error"), TEXT("AheadLib"), MB_OK);
		ExitProcess(1);
	}

	return fpAddress;
}

VOID WINAPI ProxyDll()
{

	TCHAR tzPath[MAX_PATH];

	GetSystemDirectoryW(tzPath, MAX_PATH);
	// dll name
	lstrcat(tzPath, L"\\dinput8.dll");

	g_OldModule = LoadLibrary(tzPath);
	if (g_OldModule == NULL)
	{
		MessageBoxW(NULL, TEXT("Load dll error"), TEXT("AheadLib"), MB_OK);
		ExitProcess(1);
	}
	pfnAheadLib_DirectInput8Create = GetAddress("DirectInput8Create");
	pfnAheadLib_DllCanUnloadNow = GetAddress("DllCanUnloadNow");
	pfnAheadLib_DllGetClassObject = GetAddress("DllGetClassObject");
	pfnAheadLib_DllRegisterServer = GetAddress("DllRegisterServer");
	pfnAheadLib_DllUnregisterServer = GetAddress("DllUnregisterServer");
	pfnAheadLib_GetdfDIJoystick = GetAddress("GetdfDIJoystick");
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_DirectInput8Create(void)
{
	__asm jmp pfnAheadLib_DirectInput8Create;
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_DllCanUnloadNow(void)
{
	__asm jmp pfnAheadLib_DllCanUnloadNow;
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_DllGetClassObject(void)
{
	__asm jmp pfnAheadLib_DllGetClassObject;
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_DllRegisterServer(void)
{
	__asm jmp pfnAheadLib_DllRegisterServer;
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_DllUnregisterServer(void)
{
	__asm jmp pfnAheadLib_DllUnregisterServer;
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_GetdfDIJoystick(void)
{
	__asm jmp pfnAheadLib_GetdfDIJoystick;
}

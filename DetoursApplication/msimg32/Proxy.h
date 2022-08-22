#pragma once

#include <windows.h>
#include <Shlwapi.h>

#pragma comment( lib, "Shlwapi.lib")

#pragma comment(linker, "/EXPORT:vSetDdrawflag=_AheadLib_vSetDdrawflag,@1")
#pragma comment(linker, "/EXPORT:AlphaBlend=_AheadLib_AlphaBlend,@2")
#pragma comment(linker, "/EXPORT:DllInitialize=_AheadLib_DllInitialize,@3")
#pragma comment(linker, "/EXPORT:GradientFill=_AheadLib_GradientFill,@4")
#pragma comment(linker, "/EXPORT:TransparentBlt=_AheadLib_TransparentBlt,@5")


PVOID pfnAheadLib_vSetDdrawflag;
PVOID pfnAheadLib_AlphaBlend;
PVOID pfnAheadLib_DllInitialize;
PVOID pfnAheadLib_GradientFill;
PVOID pfnAheadLib_TransparentBlt;


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
	lstrcat(tzPath, L"\\msimg32.dll");

	g_OldModule = LoadLibrary(tzPath);
	if (g_OldModule == NULL)
	{
		MessageBoxW(NULL, TEXT("Load dll error"), TEXT("AheadLib"), MB_OK);
		ExitProcess(1);
	}

	// copy from AheadLib
	pfnAheadLib_vSetDdrawflag = GetAddress("vSetDdrawflag");
	pfnAheadLib_AlphaBlend = GetAddress("AlphaBlend");
	pfnAheadLib_DllInitialize = GetAddress("DllInitialize");
	pfnAheadLib_GradientFill = GetAddress("GradientFill");
	pfnAheadLib_TransparentBlt = GetAddress("TransparentBlt");
}


EXTERN_C __declspec(naked) void __cdecl AheadLib_vSetDdrawflag(void)
{
	__asm jmp pfnAheadLib_vSetDdrawflag;
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_AlphaBlend(void)
{
	__asm jmp pfnAheadLib_AlphaBlend;
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_DllInitialize(void)
{
	__asm jmp pfnAheadLib_DllInitialize;
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_GradientFill(void)
{
	__asm jmp pfnAheadLib_GradientFill;
}

EXTERN_C __declspec(naked) void __cdecl AheadLib_TransparentBlt(void)
{
	__asm jmp pfnAheadLib_TransparentBlt;
}


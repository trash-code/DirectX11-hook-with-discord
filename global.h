#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

uintptr_t dwdiscord = 0;

using namespace std::chrono_literals;

#include "xorstr.h"
#include "lazyimporter.h"
#include "memory.h"
#include "gui/imgui.h"
#include "gui/imgui_impl_dx11.h"



typedef int(__stdcall* createhook_fn)(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal);
createhook_fn CreateHook = nullptr;

typedef int(__stdcall* enablehook_fn)(LPVOID pTarget,BOOL enable);
enablehook_fn EnableHook = nullptr;

typedef int (__stdcall* applyqueued_fn)(VOID);
applyqueued_fn EnableHookQue = nullptr;

typedef long(__stdcall *present_fn) (IDXGISwapChain* p_swapchain, UINT syncintreval, UINT flags);
present_fn o_present = nullptr;

typedef SHORT(__stdcall* getasynckeystate_fn)(int vKey);
getasynckeystate_fn o_getasynckeystate;

WNDPROC	o_wndproc = nullptr;

template<typename t>
inline t vfunc(DWORD_PTR* pTable, int index)
{
	DWORD_PTR* VTableFunctionBase = *(DWORD_PTR**)pTable;
	DWORD_PTR dwAddress = VTableFunctionBase[index];
	return (t)(dwAddress);
}
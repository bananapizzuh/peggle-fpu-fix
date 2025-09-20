#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <MinHook.h>
#include <float.h>
#include "sdk/SexySDK.hpp"
#include "sdk/HaggleSDK.hpp"

static void(__thiscall* Set3DAcclerated)(void*, bool, bool);

void __fastcall HookSet3DAcclerated(void* this_, int edx, bool is3D, bool reinit)
{
    Set3DAcclerated(this_, is3D, reinit);

    unsigned int current_word;
    unsigned int new_precision;
    int err;

    if (is3D)
    {
        new_precision = _PC_24;
    }
    else
    {
        new_precision = _PC_53;
    }

    err = _controlfp_s(&current_word, new_precision, _MCW_PC);
    
    if (err)
    {
        std::printf("Error setting FPU precision!\n");
    } 
	else {
		std::printf("Successfully set new FPU precision to %d bits!\n", is3D ? 24 : 53);
	}
}


void init()
{
    Haggle::PeggleVersion game_version = Haggle::get_game_version();

    std::uint32_t target_address = 0;

    switch (game_version)
    {
    case Haggle::PeggleVersion::Deluxe101:
        target_address = 0x00538a60;
        break;
    case Haggle::PeggleVersion::NightsDeluxe10:
        target_address = 0x0054fe20;
        break;
    default:
        std::printf("Unknown Peggle version.\n");
        return;
    }

    MH_Initialize();

    MH_CreateHook(
        (void*)target_address,
        &HookSet3DAcclerated,
        (void**)&Set3DAcclerated
    );

    MH_EnableHook(MH_ALL_HOOKS);
}

DWORD WINAPI OnAttachImpl(LPVOID lpParameter)
{
	init();
	return 0;
}

DWORD WINAPI OnAttach(LPVOID lpParameter)
{
	__try
	{
		return OnAttachImpl(lpParameter);
	}
	__except (0)
	{
		FreeLibraryAndExitThread((HMODULE)lpParameter, 0xDECEA5ED);
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, OnAttach, hModule, 0, nullptr);
		return true;
	}

	return false;
}
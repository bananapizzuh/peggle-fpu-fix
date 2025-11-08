#include "sdk/HaggleSDK.hpp"

static void(__thiscall* Set3DAccelerated)(void*, bool, bool);

void __fastcall HookSet3DAccelerated(void* this_, int edx, bool is3D, bool reinit)
{
    Set3DAccelerated(this_, is3D, reinit);

    unsigned int current_word;
    int err = _controlfp_s(&current_word, is3D ? _PC_24 : _PC_53, _MCW_PC);

    if (err)
    {
        std::printf("[ ERROR ]: Error setting FPU precision!\n");
    } 
	else {
		std::printf("[ INFO ]: Successfully set new FPU precision to %d bits!\n", is3D ? 24 : 53);
	}
}


void init()
{
    std::uint32_t addr_Set3DAccelerated = 0;

    switch (Haggle::get_game_version())
    {
    case Haggle::PeggleVersion::Deluxe101:
        addr_Set3DAccelerated = 0x00538a60;
        break;
    case Haggle::PeggleVersion::NightsDeluxe10:
        addr_Set3DAccelerated = 0x0054fe20;
        break;
    default:
        std::printf("[ ERROR ]: Unknown Peggle version.\n");
        return;
    }

    MH_Initialize();

    MH_CreateHook(
        (void*)addr_Set3DAccelerated,
        &HookSet3DAccelerated,
        (void**)&Set3DAccelerated
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
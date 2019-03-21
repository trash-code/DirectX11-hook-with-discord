#include "global.h"

ID3D11Device *g_pdevice = nullptr;
ID3D11DeviceContext *g_pcontext = nullptr;

std::once_flag present;
bool b_showmenu = true;

int h = 1440, w = 2560;

uintptr_t curtime = 0;	

#define windowname "Zombs Royale" // game window 
#define windowclass 0 //game class, unused in most situations

long __stdcall hk_present(IDXGISwapChain* p_swapchain, unsigned int syncintreval, unsigned int flags) {

	std::call_once(present, [&] {
		p_swapchain->GetDevice(__uuidof(g_pdevice), reinterpret_cast<void**>(&g_pdevice));
		g_pdevice->GetImmediateContext(&g_pcontext);
		ID3D11Texture2D *pBackBuffer;
		p_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

		ID3D11RenderTargetView *pRTV;
		g_pdevice->CreateRenderTargetView(pBackBuffer, NULL, &pRTV);

		D3D11_TEXTURE2D_DESC bd;
		pBackBuffer->GetDesc(&bd);
		pBackBuffer->Release();
		w = static_cast<int>(bd.Width);
		h = static_cast<int>(bd.Height);
		ImGui_ImplDX11_Init(iat(FindWindowA).get()(windowclass, xorstr_(windowname)), g_pdevice, g_pcontext);
	});

	if (o_getasynckeystate(VK_INSERT) && (iat(GetTickCount64).get()() - curtime) > 1500) {//wndproc for loosers, big coders(not me, so i added only 1 button) create their own handler
		b_showmenu = !b_showmenu;
		curtime = iat(GetTickCount64).get()();
	}


	if (b_showmenu) {
		ImGui_ImplDX11_NewFrame();
		if (ImGui::Begin(xorstr_("easy hook thru discord"), 0, ImVec2(800, 400))) {

			ImGui::End();
		}
		ImGui::Render();
	}
	return o_present(p_swapchain, syncintreval, flags);
}


bool __stdcall DllMain(void* module, unsigned long reason, void* buffer) {
	if (reason == 1) {//since we hacked thread, no reason to create our own
		while (!dwdiscord) {
		#ifdef _WIN64
			dwdiscord = (uintptr_t)iat(GetModuleHandleA).get()(xorstr_("DiscordHook64.dll"));
		#else
			dwdiscord = (uintptr_t)iat(GetModuleHandleA).get()(xorstr_("DiscordHook.dll"));
		#endif
		}

	#ifdef _WIN64
		CreateHook = (createhook_fn)(memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("40 53 55 56 57 41 54 41 56 41 57 48 83 EC 60")));
		EnableHook = (enablehook_fn)(memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 20 33 F6 8B FA")));
		EnableHookQue = (applyqueued_fn)(memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 57")));
		o_getasynckeystate = (getasynckeystate_fn)memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("40 53 48 83 EC 20 8B D9 FF 15 ? ? ? ?"));
	#else
		CreateHook = (createhook_fn)(memory::occurence(xorstr_("DiscordHook.dll"), xorstr_("55 8B EC 83 EC 34 A1 ? ? ? ? 33 C5 89 45 FC 8B 45 10")));
		EnableHook = (enablehook_fn)(memory::occurence(xorstr_("DiscordHook.dll"), xorstr_("55 8B EC 53 56 57 33 DB")));
		EnableHookQue = (applyqueued_fn)(memory::occurence(xorstr_("DiscordHook.dll"), xorstr_("55 8B EC 83 EC 0C 53 56 57 33 DB")));
		o_getasynckeystate = (getasynckeystate_fn)memory::occurence(xorstr_("DiscordHook.dll"), xorstr_("55 8B EC FF 75 08 FF 15 ? ? ? ? 0F B7 C0"));
	#endif

		uintptr_t dwpresent = 0;

	#ifdef _WIN64
		dwpresent = memory::occurence(xorstr_("DiscordHook64.dll"), xorstr_("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 8B F2 48 8B D9 48 8B D1"));
	#else
		//in discordhook.dll there are 2 present which are really similar, so sig them = gayasss, but you can try modify occurence and you will be able to select which one do you need
		//sig wich finds both of them - 55 8B EC FF 75 08 B9 ? ? ? ? E8 ? ? ? ? 84 C0 74 48 B9 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 8B 15 ? ? ? ? 85 D2 75 0D 50 B9 ? ? ? ?, 
		//also you can try dwdiscord + offset like i did but it changes really often 
		//dwdiscord + 0x5AD50
		dwpresent = (dwdiscord + 0x5AD50);// for present number 1(i think only this is used, number two for unusual situations)
		//dwpresent = (dwdiscord + 0x5ACD0);// for present number 2
	#endif
		CreateHook((void*)dwpresent, (void*)hk_present, (void**)&o_present);
		EnableHook((void*)dwpresent, 1);
		EnableHookQue();
		return true;
	}
	return false;
}
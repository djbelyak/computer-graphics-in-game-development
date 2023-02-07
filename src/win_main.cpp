#include "renderer/renderer.h"
#include "settings.h"
#include "utils/window.h"

#include <Windows.h>

#include <exception>

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	try
	{
		auto settings = cg::settings::parse_settings(__argc, __argv);
		auto renderer = cg::renderer::make_renderer(settings);
		return cg::utils::window::run(renderer.get(), hInstance, nShowCmd);
	}
	catch (std::exception& e)
	{
#ifdef DEBUG
		OutputDebugStringA(e.what());
#endif// DEBUG
		MessageBoxA(NULL, e.what(), "Runtime failure", MB_OK);
		return 1;
	}
}

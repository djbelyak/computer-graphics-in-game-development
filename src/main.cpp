#include "renderer/renderer.h"
#include "settings.h"

#include <iostream>

int main(int argc, char** argv)
{
	try
	{
		auto settings = cg::settings::parse_settings(argc, argv);
		auto renderer = cg::renderer::make_renderer(settings);

		renderer->init();

		renderer->render();

		renderer->destroy();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
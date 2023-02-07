//#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "resource_utils.h"

#include "utils/error_handler.h"

#include <stb_image_write.h>


using namespace cg::utils;

void cg::utils::save_resource(
		cg::resource<cg::unsigned_color>& render_target, std::filesystem::path filepath)
{
	int width = static_cast<int>(render_target.get_stride());
	int height = static_cast<int>(render_target.get_number_of_elements()) / width;

	int result = stbi_write_png(
			filepath.string().c_str(), width, height, 3, render_target.get_data(),
			width * sizeof(cg::unsigned_color));

	if (result != 1)
		THROW_ERROR("Can't save the resource");

	std::string view_command("start ");
	view_command.append(filepath.string());

	std::system(view_command.c_str());
}

#include "settings.h"

#include "utils/error_handler.h"

#include <cxxopts.hpp>

using namespace cg;

std::shared_ptr<settings> cg::settings::parse_settings(int argc, char** argv)
{
	std::shared_ptr<cg::settings> settings = std::make_shared<cg::settings>();

	cxxopts::Options options(argv[0], "Computer graphics in Game development");

	auto add_options = options.add_options();
	add_options("height", "Render target height", cxxopts::value<unsigned>()->default_value("1080"));
	add_options("width", "Render target width", cxxopts::value<unsigned>()->default_value("1920"));
	add_options("model_path", "Path to OBJ model", cxxopts::value<std::filesystem::path>()->default_value("models/cube.obj"));
	add_options("camera_position", "Camera position", cxxopts::value<std::vector<float>>()->default_value("0.0,1.0,5.0"));
	add_options("camera_theta", "Camera polar angle", cxxopts::value<float>()->default_value("0.0"));
	add_options("camera_phi", "Camera azimuth angle", cxxopts::value<float>()->default_value("0.0"));
	add_options("camera_angle_of_view", "Camera angle of view", cxxopts::value<float>()->default_value("60.0"));
	add_options("camera_z_near", "Minimum expected depth", cxxopts::value<float>()->default_value("0.001"));
	add_options("camera_z_far", "Maximum expected depth", cxxopts::value<float>()->default_value("100.0"));
	add_options("result_path", "Path to resulted image", cxxopts::value<std::filesystem::path>()->default_value("result.png"));
	add_options("raytracing_depth", "Maximum number of traces rays", cxxopts::value<unsigned>()->default_value("1"));
	add_options("accumulation_num", "Number of accumulated frames", cxxopts::value<unsigned>()->default_value("1"));
	add_options("shader_path", "Path to a shader file", cxxopts::value<std::filesystem::path>()->default_value("shaders/shaders.hlsl"));
	add_options("h,help", "Print usage");

	auto result = options.parse(argc, argv);

	if (result.count("help"))
	{
		THROW_ERROR(options.help());
	}

	settings->height = result["height"].as<unsigned>();
	settings->width = result["width"].as<unsigned>();
	settings->model_path = result["model_path"].as<std::filesystem::path>();
	settings->camera_position = result["camera_position"].as<std::vector<float>>();
	settings->camera_theta = result["camera_theta"].as<float>();
	settings->camera_phi = result["camera_phi"].as<float>();
	settings->camera_angle_of_view = result["camera_angle_of_view"].as<float>();
	settings->camera_z_near = result["camera_z_near"].as<float>();
	settings->camera_z_far = result["camera_z_far"].as<float>();
	settings->result_path = result["result_path"].as<std::filesystem::path>();
	settings->raytracing_depth = result["raytracing_depth"].as<unsigned>();
	settings->accumulation_num = result["accumulation_num"].as<unsigned>();
	settings->shader_path = result["shader_path"].as<std::filesystem::path>();

	return settings;
}

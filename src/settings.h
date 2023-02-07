#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace cg
{
	struct settings
	{
		static std::shared_ptr<settings> parse_settings(int argc, char** argv);

		unsigned height;
		unsigned width;

		std::filesystem::path model_path;

		std::vector<float> camera_position;
		float camera_theta;
		float camera_phi;
		float camera_angle_of_view;
		float camera_z_near;
		float camera_z_far;

		std::filesystem::path result_path;

		unsigned raytracing_depth;
		unsigned accumulation_num;
	};

}// namespace cg
#pragma once

#include "resource.h"

#include <filesystem>
#include <linalg.h>
#include <tiny_obj_loader.h>


using namespace linalg::aliases;

namespace cg::world
{
	class model
	{
	public:
		model();
		virtual ~model();

		void load_obj(const std::filesystem::path& model_path);

		const std::vector<std::shared_ptr<cg::resource<cg::vertex>>>& get_vertex_buffers() const;
		const std::vector<std::shared_ptr<cg::resource<unsigned int>>>& get_index_buffers() const;
		const std::vector<std::filesystem::path>& get_per_shape_texture_files() const;

		const float4x4 get_world_matrix() const;

	protected:

		std::vector<std::shared_ptr<cg::resource<cg::vertex>>> vertex_buffers;
		std::vector<std::shared_ptr<cg::resource<unsigned int>>> index_buffers;
		std::vector<std::filesystem::path> textures;

		void allocate_buffers(const std::vector<tinyobj::shape_t>& shapes);
		static float3 compute_normal(const tinyobj::attrib_t& attrib, const tinyobj::mesh_t& mesh, size_t index_offset);
		static void fill_vertex_data(cg::vertex& vertex, const tinyobj::attrib_t& attrib, tinyobj::index_t idx, float3 computed_normal, tinyobj::material_t material);
		void fill_buffers(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attrib, const std::vector<tinyobj::material_t>& materials, const std::filesystem::path& base_folder);
	};
}// namespace cg::world
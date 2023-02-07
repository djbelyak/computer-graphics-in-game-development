#define TINYOBJLOADER_IMPLEMENTATION

#include "model.h"

#include "utils/error_handler.h"

#include <linalg.h>


using namespace linalg::aliases;
using namespace cg::world;

cg::world::model::model() {}

cg::world::model::~model() {}

void cg::world::model::load_obj(const std::filesystem::path& model_path)
{
	// TODO Lab: 1.03 Using `tinyobjloader` implement `load_obj`, `allocate_buffers`, `compute_normal`, `fill_vertex_data`, `fill_buffers`, `get_vertex_buffers`, `get_index_buffers` methods of `cg::world::model` class
}

void model::allocate_buffers(const std::vector<tinyobj::shape_t>& shapes)
{
	// TODO Lab: 1.03 Using `tinyobjloader` implement `load_obj`, `allocate_buffers`, `compute_normal`, `fill_vertex_data`, `fill_buffers`, `get_vertex_buffers`, `get_index_buffers` methods of `cg::world::model` class
}

float3 cg::world::model::compute_normal(const tinyobj::attrib_t& attrib, const tinyobj::mesh_t& mesh, size_t index_offset)
{
	// TODO Lab: 1.03 Using `tinyobjloader` implement `load_obj`, `allocate_buffers`, `compute_normal`, `fill_vertex_data`, `fill_buffers`, `get_vertex_buffers`, `get_index_buffers` methods of `cg::world::model` class
	return float3{};
}

void model::fill_vertex_data(cg::vertex& vertex, const tinyobj::attrib_t& attrib, const tinyobj::index_t idx, const float3 computed_normal, const tinyobj::material_t material)
{
	// TODO Lab: 1.03 Using `tinyobjloader` implement `load_obj`, `allocate_buffers`, `compute_normal`, `fill_vertex_data`, `fill_buffers`, `get_vertex_buffers`, `get_index_buffers` methods of `cg::world::model` class
}

void model::fill_buffers(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attrib, const std::vector<tinyobj::material_t>& materials, const std::filesystem::path& base_folder)
{
	// TODO Lab: 1.03 Using `tinyobjloader` implement `load_obj`, `allocate_buffers`, `compute_normal`, `fill_vertex_data`, `fill_buffers`, `get_vertex_buffers`, `get_index_buffers` methods of `cg::world::model` class
}


const std::vector<std::shared_ptr<cg::resource<cg::vertex>>>&
cg::world::model::get_vertex_buffers() const
{
	// TODO Lab: 1.03 Using `tinyobjloader` implement `load_obj`, `allocate_buffers`, `compute_normal`, `fill_vertex_data`, `fill_buffers`, `get_vertex_buffers`, `get_index_buffers` methods of `cg::world::model` class
	return vertex_buffers;
}

const std::vector<std::shared_ptr<cg::resource<unsigned int>>>&
cg::world::model::get_index_buffers() const
{
	// TODO Lab: 1.03 Using `tinyobjloader` implement `load_obj`, `allocate_buffers`, `compute_normal`, `fill_vertex_data`, `fill_buffers`, `get_vertex_buffers`, `get_index_buffers` methods of `cg::world::model` class
	return index_buffers;
}

const std::vector<std::filesystem::path>& cg::world::model::get_per_shape_texture_files() const
{
	return textures;
}


const float4x4 cg::world::model::get_world_matrix() const
{
	return float4x4{
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}};
}

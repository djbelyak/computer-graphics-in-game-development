#pragma once

#include "resource.h"

#include <functional>
#include <iostream>
#include <linalg.h>
#include <limits>
#include <memory>


using namespace linalg::aliases;

static constexpr float DEFAULT_DEPTH = std::numeric_limits<float>::max();

namespace cg::renderer
{
	template<typename VB, typename RT>
	class rasterizer
	{
	public:
		rasterizer(){};
		~rasterizer(){};
		void set_render_target(
				std::shared_ptr<resource<RT>> in_render_target,
				std::shared_ptr<resource<float>> in_depth_buffer = nullptr);
		void clear_render_target(
				const RT& in_clear_value, const float in_depth = DEFAULT_DEPTH);

		void set_vertex_buffer(std::shared_ptr<resource<VB>> in_vertex_buffer);
		void set_index_buffer(std::shared_ptr<resource<unsigned int>> in_index_buffer);

		void set_viewport(size_t in_width, size_t in_height);

		void draw(size_t num_vertexes, size_t vertex_offset);

		std::function<std::pair<float4, VB>(float4 vertex, VB vertex_data)> vertex_shader;
		std::function<cg::color(const VB& vertex_data, const float z)> pixel_shader;

	protected:
		std::shared_ptr<cg::resource<VB>> vertex_buffer;
		std::shared_ptr<cg::resource<unsigned int>> index_buffer;
		std::shared_ptr<cg::resource<RT>> render_target;
		std::shared_ptr<cg::resource<float>> depth_buffer;

		size_t width = 1920;
		size_t height = 1080;

		int edge_function(int2 a, int2 b, int2 c);
		bool depth_test(float z, size_t x, size_t y);
	};

	template<typename VB, typename RT>
	inline void rasterizer<VB, RT>::set_render_target(
			std::shared_ptr<resource<RT>> in_render_target,
			std::shared_ptr<resource<float>> in_depth_buffer)
	{
		// Lab: 1.02 Implement `set_render_target`, `set_viewport`, `clear_render_target` methods of `cg::renderer::rasterizer` class
		if (in_render_target)
			render_target = in_render_target;
		// Lab: 1.06 Adjust `set_render_target`, and `clear_render_target` methods of `cg::renderer::rasterizer` class to consume a depth buffer
		if (in_depth_buffer) 
			depth_buffer = in_depth_buffer;
	}

	template<typename VB, typename RT>
	inline void rasterizer<VB, RT>::set_viewport(size_t in_width, size_t in_height)
	{
		// Lab: 1.02 Implement `set_render_target`, `set_viewport`, `clear_render_target` methods of `cg::renderer::rasterizer` class
		width = in_width;
		height = in_height;
	}

	template<typename VB, typename RT>
	inline void rasterizer<VB, RT>::clear_render_target(
			const RT& in_clear_value, const float in_depth)
	{
		// Lab: 1.02 Implement `set_render_target`, `set_viewport`, `clear_render_target` methods of `cg::renderer::rasterizer` class
		if (render_target) {
			for (size_t i = 0; i < render_target->count(); ++i) {
				render_target->item(i) = in_clear_value;
			}
		}
		// TODO Lab: 1.06 Adjust `set_render_target`, and `clear_render_target` methods of `cg::renderer::rasterizer` class to consume a depth buffer
		if (depth_buffer) {
			for (size_t i = 0; i < depth_buffer->count(); i++) {
				depth_buffer->item(i) = in_depth;
			}
		}
	}

	template<typename VB, typename RT>
	inline void rasterizer<VB, RT>::set_vertex_buffer(
			std::shared_ptr<resource<VB>> in_vertex_buffer)
	{
		vertex_buffer = in_vertex_buffer;
	}

	template<typename VB, typename RT>
	inline void rasterizer<VB, RT>::set_index_buffer(
			std::shared_ptr<resource<unsigned int>> in_index_buffer)
	{
		index_buffer = in_index_buffer;
	}

	template<typename VB, typename RT>
	inline void rasterizer<VB, RT>::draw(size_t num_vertexes, size_t vertex_offset)
	{
		// Lab: 1.04 Implement `cg::world::camera` class

		size_t vertex_id = vertex_offset;

		while (vertex_id < vertex_offset + num_vertexes) {
			std::vector<VB> vertices(3);
			vertices[0] = vertex_buffer->item(index_buffer->item(vertex_id++));
			vertices[1] = vertex_buffer->item(index_buffer->item(vertex_id++));
			vertices[2] = vertex_buffer->item(index_buffer->item(vertex_id++));

			for (auto& vertex: vertices) {
				float4 coords{vertex.position.x, vertex.position.y, vertex.position.z, 1.f};
				auto processed_vertex = vertex_shader(coords, vertex);

				vertex.position = processed_vertex.first.xyz() / processed_vertex.first.w;

				vertex.position.x = (vertex.position.x + 1.f) * width / 2.f;
				vertex.position.y = (-vertex.position.y + 1.f) * height / 2.f;
				
			}

			// Lab 1.05 segment

			int2 vertex_a = int2(vertices[0].position.xy());
			int2 vertex_b = int2(vertices[1].position.xy());
			int2 vertex_c = int2(vertices[2].position.xy());

			int2 min_vertex = min(vertex_a, min(vertex_b, vertex_c));
			int2 max_vertex = max(vertex_a, max(vertex_b, vertex_c));
			int2 min_viewport = int2{0,0};
			int2 max_viewport = int2{static_cast<int>(width - 1), static_cast<int>(height - 1)};

			int2 begin = clamp(min_vertex, min_viewport, max_viewport);
			int2 end = clamp(max_vertex, min_viewport, max_viewport);

			// Lab: 1.06 Add `Depth test` stage to `draw` method of `cg::renderer::rasterizer`
			// also lab 1.05 is here
			float edge = static_cast<float>(edge_function(vertex_a, vertex_b, vertex_c));

			for (int x = begin.x; x <= end.x; x++) {
				for (int y = begin.y; y <= end.y; y++) {
					int2 point{x, y};

					int edge0 = edge_function(vertex_a, vertex_b, point);
					int edge1 = edge_function(vertex_b, vertex_c, point);
					int edge2 = edge_function(vertex_c, vertex_a, point);

					if (edge0 >= 0 && edge1 >= 0 && edge2 >= 0) {
						float w = static_cast<float>(edge0) / edge;
						float u = static_cast<float>(edge1) / edge;
						float v = static_cast<float>(edge2) / edge;
						
						float depth = 	u*vertices[0].position.z + 
										v*vertices[1].position.z + 
										w*vertices[2].position.z;

						if (depth_test(depth, x, y)) {
							auto pixel_result = pixel_shader(vertices[0], depth);
							render_target->item(x, y) = RT::from_color(pixel_result);
							depth_buffer->item(x, y) = depth;
						}

					}
				}
			}
		}
		// Lab: 1.05 Add `Rasterization` and `Pixel shader` stages to `draw` method of `cg::renderer::rasterizer`

		

	}

	template<typename VB, typename RT>
	inline int
	rasterizer<VB, RT>::edge_function(int2 a, int2 b, int2 c)
	{
		// Lab: 1.05 Implement `cg::renderer::rasterizer::edge_function` method
		return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
	}

	template<typename VB, typename RT>
	inline bool rasterizer<VB, RT>::depth_test(float z, size_t x, size_t y)
	{
		if (!depth_buffer)
		{
			return true;
		}
		return depth_buffer->item(x, y) > z;
	}

}// namespace cg::renderer
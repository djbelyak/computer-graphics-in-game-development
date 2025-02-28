#pragma once

#include "resource.h"

#include <iostream>
#include <linalg.h>
#include <memory>
#include <omp.h>
#include <random>

using namespace linalg::aliases;

namespace cg::renderer
{
	struct ray
	{
		ray(float3 position, float3 direction) : position(position)
		{
			this->direction = normalize(direction);
		}
		float3 position;
		float3 direction;
	};

	struct payload
	{
		float t;
		float3 bary;
		cg::color color;
	};

	template<typename VB>
	struct triangle
	{
		triangle(const VB& vertex_a, const VB& vertex_b, const VB& vertex_c);

		float3 a;
		float3 b;
		float3 c;

		float3 ba;
		float3 ca;

		float3 na;
		float3 nb;
		float3 nc;

		float3 ambient;
		float3 diffuse;
		float3 emissive;
	};

	template<typename VB>
	inline triangle<VB>::triangle(
			const VB& vertex_a, const VB& vertex_b, const VB& vertex_c)
	{
		// Lab: 2.02 Implement a constructor of `triangle` struct
		a = vertex_a.position;
		b = vertex_b.position;
		c = vertex_c.position;

		ba = b-a;
		ca = c-a;

		na = vertex_a.normal;
		nb = vertex_b.normal;
		nc = vertex_c.normal;

		ambient = vertex_a.ambient;
		diffuse = vertex_a.diffuse;
		emissive = vertex_a.emissive;
	}

	template<typename VB>
	class aabb
	{
	public:
		void add_triangle(const triangle<VB> triangle);
		const std::vector<triangle<VB>>& get_triangles() const;
		bool aabb_test(const ray& ray) const;

	protected:
		std::vector<triangle<VB>> triangles;

		float3 aabb_min;
		float3 aabb_max;
	};

	struct light
	{
		float3 position;
		float3 color;
	};

	template<typename VB, typename RT>
	class raytracer
	{
	public:
		raytracer(){};
		~raytracer(){};

		void set_render_target(std::shared_ptr<resource<RT>> in_render_target);
		void clear_render_target(const RT& in_clear_value);
		void set_viewport(size_t in_width, size_t in_height);

		void set_vertex_buffers(std::vector<std::shared_ptr<cg::resource<VB>>> in_vertex_buffers);
		void set_index_buffers(std::vector<std::shared_ptr<cg::resource<unsigned int>>> in_index_buffers);
		void build_acceleration_structure();
		std::vector<aabb<VB>> acceleration_structures;

		void ray_generation(float3 position, float3 direction, float3 right, float3 up, size_t depth, size_t accumulation_num);

		payload trace_ray(const ray& ray, size_t depth, float max_t = 1000.f, float min_t = 0.001f) const;
		payload intersection_shader(const triangle<VB>& triangle, const ray& ray) const;

		std::function<payload(const ray& ray)> miss_shader = nullptr;
		std::function<payload(const ray& ray, payload& payload, const triangle<VB>& triangle, size_t depth)>
				closest_hit_shader = nullptr;
		std::function<payload(const ray& ray, payload& payload, const triangle<VB>& triangle)> any_hit_shader =
				nullptr;

		float2 get_jitter(int frame_id);

	protected:
		std::shared_ptr<cg::resource<RT>> render_target;
		std::shared_ptr<cg::resource<float3>> history;
		std::vector<std::shared_ptr<cg::resource<unsigned int>>> index_buffers;
		std::vector<std::shared_ptr<cg::resource<VB>>> vertex_buffers;
		std::vector<triangle<VB>> triangles;

		size_t width = 1920;
		size_t height = 1080;
	};

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::set_render_target(
			std::shared_ptr<resource<RT>> in_render_target)
	{
		// Lab: 2.01 Implement `set_render_target`, `set_viewport`, and `clear_render_target` methods of `raytracer` class
		render_target = in_render_target;
	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::set_viewport(size_t in_width,
												size_t in_height)
	{
		// Lab: 2.01 Implement `set_render_target`, `set_viewport`, and `clear_render_target` methods of `raytracer` class
		width = in_width;
		height = in_height;
		// Lab: 2.06 Add `history` resource in `raytracer` class
		history = std::make_shared<cg::resource<float3>>(width, height);

	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::clear_render_target(
			const RT& in_clear_value)
	{
		// Lab: 2.01 Implement `set_render_target`, `set_viewport`, and `clear_render_target` methods of `raytracer` class
		for (size_t i = 0; i < render_target->count(); i++) {
			render_target->item(i) = in_clear_value;
			history->item(i) = float3{0.f, 0.f, 0.f};
		}
		// Lab: 2.06 Add `history` resource in `raytracer` class
	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::set_vertex_buffers(std::vector<std::shared_ptr<cg::resource<VB>>> in_vertex_buffers)
	{
		// TODO Lab: 2.02 Implement `set_vertex_buffers` and `set_index_buffers` of `raytracer` class
		vertex_buffers = in_vertex_buffers;
	}

	template<typename VB, typename RT>
	void raytracer<VB, RT>::set_index_buffers(std::vector<std::shared_ptr<cg::resource<unsigned int>>> in_index_buffers)
	{
		// TODO Lab: 2.02 Implement `set_vertex_buffers` and `set_index_buffers` of `raytracer` class
		index_buffers = in_index_buffers;
	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::build_acceleration_structure()
	{
		// TODO Lab: 2.02 Fill `triangles` vector in `build_acceleration_structure` of `raytracer` class
		for (size_t shape_id=0; shape_id<index_buffers.size(); shape_id++) {
			auto& index_buffer = index_buffers[shape_id];
			auto& vertex_buffer = vertex_buffers[shape_id];
			size_t index_id = 0;
			aabb<VB> aabb;
			while (index_id < index_buffer->count()) {
				triangle<VB> triangle(
						vertex_buffer->item(index_buffer->item(index_id++)),
						vertex_buffer->item(index_buffer->item(index_id++)),
						vertex_buffer->item(index_buffer->item(index_id++))
					);
				aabb.add_triangle(triangle);

				//triangles.push_back(triangle);
			}
			acceleration_structures.push_back(aabb);
		}
		// Lab: 2.05 Implement `build_acceleration_structure` method of `raytracer` class
	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::ray_generation(
			float3 position, float3 direction,
			float3 right, float3 up, size_t depth, size_t accumulation_num)
	{
		// Lab: 2.01 Implement `ray_generation` and `trace_ray` method of `raytracer` class
		float frame_weight = 1.f / static_cast<float>(accumulation_num);
		for (int frame_id = 0; frame_id < accumulation_num; ++frame_id) {
			std::cout << "Tracing frame # " << frame_id + 1 << "\n";
			float2 jitter = get_jitter(frame_id);

		
			#pragma omp parallel for
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					float u = (2.f * x) / static_cast<float>(width - 1) - 1.f;
					float v = (2.f * x) / static_cast<float>(height - 1) - 1.f;

					u *= static_cast<float>(width) / static_cast<float>(height); //aspect ratio

					float3 ray_direction = direction + u * right - v * up;
					ray ray(position, ray_direction);

					payload payload = trace_ray(ray, depth);

					auto& history_pixel = history->item(x, y);
					history_pixel += payload.color.to_float3() * frame_weight;

					if (frame_id == accumulation_num - 1)
						render_target->item(x, y) = RT::from_float3(history_pixel);
				}
			}
		}
		// TODO Lab: 2.06 Implement TAA in `ray_generation` method of `raytracer` class
	}

	template<typename VB, typename RT>
	inline payload raytracer<VB, RT>::trace_ray(
			const ray& ray, size_t depth, float max_t, float min_t) const
	{
		// TODO Lab: 2.01 Implement `ray_generation` and `trace_ray` method of `raytracer` class
		if (depth == 0)
			return miss_shader(ray);

		depth--;
		// TODO Lab: 2.02 Adjust `trace_ray` method of `raytracer` class to traverse geometry and call a closest hit shader
		
		payload closest_hit_payload{};
		closest_hit_payload.t = max_t;
		const triangle<VB>* closest_triangle = nullptr;

		for (auto& aabb: acceleration_structures) {

			if (!aabb.aabb_test(ray)) 
				continue;
			for (auto& triangle : aabb.get_triangles()) {
				payload payload = intersection_shader(triangle, ray);
				if (payload.t > min_t && payload.t < closest_hit_payload.t){
					closest_hit_payload = payload;
					closest_triangle = &triangle;
				}
			}

		}

		if (closest_hit_payload.t < max_t) {
			if (closest_hit_shader) {
				return closest_hit_shader(ray, closest_hit_payload, *closest_triangle, depth);
			}
		}


		// TODO Lab: 2.04 Adjust `trace_ray` method of `raytracer` to use `any_hit_shader`
		// TODO Lab: 2.05 Adjust `trace_ray` method of `raytracer` class to traverse the acceleration structure
		return miss_shader(ray);
	}

	template<typename VB, typename RT>
	inline payload raytracer<VB, RT>::intersection_shader(
			const triangle<VB>& triangle, const ray& ray) const
	{
		// Lab: 2.02 Implement an `intersection_shader` method of `raytracer` class
		payload payload{};
		payload.t = -1.f;

		float3 pvec = cross(ray.direction, triangle.ca);
		float det = dot(triangle.ba, pvec);
		if (det > -1e-8 && det < 1e-8) 
			return payload;
		float inv_det = 1.f / det;

		float3 tvec = ray.position - triangle.a;
		float u = dot(tvec, pvec) * inv_det;
		if (u < 0.f || u > 1.f)
			return payload;

		float3 qvec = cross(tvec, triangle.ba);
		float v = dot(ray.direction, qvec) * inv_det;
		if (v < 0.f || u + v > 1.f)
			return payload;

		payload.t = dot(triangle.ca, qvec) * inv_det;
		payload.bary = float3{1.f - v - u, u, v};
		return payload;
	}

	template<typename VB, typename RT>
	float2 raytracer<VB, RT>::get_jitter(int frame_id)
	{
		// Lab: 2.06 Implement `get_jitter` method of `raytracer` class
		float2 results(0.f, 0.f);
		constexpr int base_x = 2;
		int index = frame_id + 1;
		float inv_base = 1.f / base_x;
		float fraction = inv_base;
		while (index > 0){
			results.x += (index % base_x) * fraction;
			index /= base_x;
			fraction *= inv_base;
		}

		constexpr int base_y = 3;
		int index = frame_id + 1;
		float inv_base = 1.f / base_y;
		float fraction = inv_base;
		while (index > 0) {
			results.y += (index % base_y) * fraction;
			index /= base_y;
			fraction *= inv_base;
		}

		return results - 0.5f;
	}


	template<typename VB>
	inline void aabb<VB>::add_triangle(const triangle<VB> triangle)
	{
		// Lab: 2.05 Implement `aabb` class
		if (triangles.empty()){
			aabb_max = aabb_min = triangle.a;
		}

		aabb_max = max(aabb_max, triangle.a);
		aabb_max = max(aabb_max, triangle.b);
		aabb_max = max(aabb_max, triangle.c);

		aabb_min = min(aabb_min, triangle.a);
		aabb_min = min(aabb_min, triangle.b);
		aabb_min = min(aabb_min, triangle.c);

		triangles.push_back(triangle);
	}

	template<typename VB>
	inline const std::vector<triangle<VB>>& aabb<VB>::get_triangles() const
	{
		// Lab: 2.05 Implement `aabb` class
		return triangles;
	}

	template<typename VB>
	inline bool aabb<VB>::aabb_test(const ray& ray) const
	{
		// Lab: 2.05 Implement `aabb` class
		float3 inv_ray_direction = float3(1.f) / ray.direction();
		float3 t0 = (aabb_min - ray.position) * inv_ray_direction;
		float3 t1 = (aabb_max - ray.position) * inv_ray_direction;
		float3 tmax = max(t0, t1);
		float3 tmin = min(t0, t1);

		return maxelem(tmin) <= minelem(tmax);
	}

}// namespace cg::renderer
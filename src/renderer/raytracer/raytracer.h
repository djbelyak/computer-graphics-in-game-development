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
		a = float3{vertex_a.x, vertex_a.y, vertex_a.z};
		b = float3{vertex_b.x, vertex_b.y, vertex_b.z};
		c = float3{vertex_c.x, vertex_c.y, vertex_c.z};

		ba = b - a;
		ca = c - a;

		na = float3{vertex_a.nx, vertex_a.ny, vertex_a.nz};
		nb = float3{vertex_b.nx, vertex_b.ny, vertex_b.nz};
		nc = float3{vertex_c.nx, vertex_c.ny, vertex_c.nz};

		ambient = float3{vertex_a.ambient_r, vertex_a.ambient_g, vertex_a.ambient_b};
		diffuse = float3{vertex_a.diffuse_r, vertex_a.diffuse_g, vertex_a.diffuse_b};
		emissive = float3{vertex_a.emissive_r, vertex_a.emissive_g, vertex_a.emissive_b};
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
		render_target = in_render_target;
	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::set_viewport(size_t in_width,
												size_t in_height)
	{
		width = in_width;
		height = in_height;

		history = std::make_shared<cg::resource<float3>>(width, height);
	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::clear_render_target(
			const RT& in_clear_value)
	{
		for (size_t i = 0; i < render_target->get_number_of_elements(); ++i) {
			render_target->item(i) = in_clear_value;
			history->item(i) = float3{0.f, 0.f, 0.f};
		}
	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::set_vertex_buffers(std::vector<std::shared_ptr<cg::resource<VB>>> in_vertex_buffers)
	{
		vertex_buffers = in_vertex_buffers;
	}

	template<typename VB, typename RT>
	void raytracer<VB, RT>::set_index_buffers(std::vector<std::shared_ptr<cg::resource<unsigned int>>> in_index_buffers)
	{
		index_buffers = in_index_buffers;
	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::build_acceleration_structure()
	{
		for (size_t i = 0; i < index_buffers.size(); ++i) {
			auto& index_buffer = index_buffers[i];
			auto& vertex_buffer = vertex_buffers[i];
			size_t index = 0;
			aabb<VB> aabb;
			while (index < index_buffer->get_number_of_elements()) {
				triangle<VB> triangle(
						vertex_buffer->item(index_buffer->item(index++)),
						vertex_buffer->item(index_buffer->item(index++)),
						vertex_buffer->item(index_buffer->item(index++)));
				aabb.add_triangle(triangle);
			}
			acceleration_structures.push_back(aabb);
		}
	}

	template<typename VB, typename RT>
	inline void raytracer<VB, RT>::ray_generation(
			float3 position, float3 direction,
			float3 right, float3 up, size_t depth, size_t accumulation_num)
	{
		for (size_t frame_id = 0; frame_id < accumulation_num; frame_id++) {
			std::cout << "Tracing " << frame_id << "/" << accumulation_num << " frame\n";
			float2 jitter = get_jitter(frame_id);
#pragma omp parallel for
			for (int x = 0; x < width; ++x) {
				for (int y = 0; y < height; ++y) {
					float u = (2.f * x + jitter.x) / static_cast<float>(width) - 1.f;
					float v = (2.f * y + jitter.y) / static_cast<float>(height) - 1.f;
					u *= static_cast<float>(width) / static_cast<float>(height);
					float3 ray_direction = direction + right * u - up * v;

					ray primary_ray(position, ray_direction);
					payload payload_var = trace_ray(primary_ray, depth);

					history->item(x, y) += sqrt(payload_var.color.to_float3() / accumulation_num);
					if (frame_id + 1 == accumulation_num) {
						render_target->item(x, y) = RT::from_float3(history->item(x, y));
					}
				}
			}
		}
	}

	template<typename VB, typename RT>
	inline payload raytracer<VB, RT>::trace_ray(
			const ray& ray, size_t depth, float max_t, float min_t) const
	{
		if (depth == 0) {
			return miss_shader(ray);
		}
		--depth;

		payload closest_hit_payload{};
		closest_hit_payload.t = max_t;
		const triangle<VB>* closest_triangle = nullptr;

		for (auto& aabb: acceleration_structures) {
			if (!aabb.aabb_test(ray)) {
				continue;
			}
			for (auto& triangle: aabb.get_triangles()) {
				payload payload_var = intersection_shader(triangle, ray);

				if (payload_var.t > min_t && payload_var.t < closest_hit_payload.t) {
					closest_hit_payload = payload_var;
					closest_triangle = &triangle;
					if (any_hit_shader) {
						return any_hit_shader(ray, payload_var, triangle);
					}
				}
			}
		}

		if (closest_hit_payload.t < max_t) {
			if (closest_hit_shader) {
				return closest_hit_shader(ray, closest_hit_payload, *closest_triangle, depth);
			}
		}

		return miss_shader(ray);
	}

	template<typename VB, typename RT>
	inline payload raytracer<VB, RT>::intersection_shader(
			const triangle<VB>& triangle, const ray& ray) const
	{
		payload payload_var{};
		payload_var.t = -1.f;

		float3 pvec = cross(ray.direction, triangle.ca);
		float det = dot(triangle.ba, pvec);
		if (det > -1e-8 && det < 1e-8) {
			return payload_var;
		}

		float inv_det = 1.f / det;
		float3 tvec = ray.position - triangle.a;
		float u = dot(tvec, pvec) * inv_det;
		if (u < 0.f || u > 1.f) {
			return payload_var;
		}

		float3 qvec = cross(tvec, triangle.ba);
		float v = dot(ray.direction, qvec) * inv_det;
		if (v < 0.f || u + v > 1.f) {
			return payload_var;
		}

		payload_var.t = dot(triangle.ca, qvec) * inv_det;
		payload_var.bary = float3{1.f - u - v, u, v};
		return payload_var;
	}

	template<typename VB, typename RT>
	float2 raytracer<VB, RT>::get_jitter(int frame_id)
	{
		float2 result{0.f, 0.f};

		constexpr int base_x = 2;
		int index = frame_id + 1;
		float inv_base = 1.f / base_x;
		float fraction = inv_base;
		while (index > 0) {
			result.x += static_cast<float>(index % base_x) * fraction;
			index /= base_x;
			fraction *= inv_base;
		}

		constexpr int base_y = 3;
		index = frame_id + 1;
		inv_base = 1.f / base_y;
		fraction = inv_base;
		while (index > 0) {
			result.y += static_cast<float>(index % base_y) * fraction;
			index /= base_y;
			fraction *= inv_base;
		}

		return result - 0.5f;
	}


	template<typename VB>
	inline void aabb<VB>::add_triangle(const triangle<VB> triangle)
	{
		if (triangles.empty()) {
			aabb_min = aabb_max = triangle.a;
		}

		triangles.push_back(triangle);

		aabb_max = max(aabb_max, triangle.a);
		aabb_max = max(aabb_max, triangle.b);
		aabb_max = max(aabb_max, triangle.c);

		aabb_min = max(aabb_min, triangle.a);
		aabb_min = max(aabb_min, triangle.b);
		aabb_min = max(aabb_min, triangle.c);
	}

	template<typename VB>
	inline const std::vector<triangle<VB>>& aabb<VB>::get_triangles() const
	{
		return triangles;
	}

	template<typename VB>
	inline bool aabb<VB>::aabb_test(const ray& ray) const
	{
		float3 inv_ray_dir = 1.f / ray.direction;
		float3 t0 = (aabb_max - ray.position) * inv_ray_dir;
		float3 t1 = (aabb_min - ray.position) * inv_ray_dir;
		float3 t_min = min(t0, t1);
		float3 t_max = max(t0, t1);
		return maxelem(t_min) <= maxelem(t_max);
	}

}// namespace cg::renderer
#pragma once

#include "utils/error_handler.h"

#include <algorithm>
#include <linalg.h>
#include <vector>


using namespace linalg::aliases;

namespace cg
{
	template<typename T>
	class resource
	{
	public:
		resource(size_t size);
		resource(size_t x_size, size_t y_size);
		~resource();

		const T* get_data();
		T& item(size_t item);
		T& item(size_t x, size_t y);

		size_t size_bytes() const;
		size_t count() const;
		size_t get_stride() const;

	private:
		std::vector<T> data;
		size_t item_size = sizeof(T);
		size_t stride;
	};

	template<typename T>
	inline resource<T>::resource(size_t size)
	{
		//  Lab: 1.02 Implement `cg::resource` class
		data.resize(size);
		stride = 0;
	}
	template<typename T>
	inline resource<T>::resource(size_t x_size, size_t y_size)
	{
		//  Lab: 1.02 Implement `cg::resource` class
		data.resize(x_size * y_size);
		stride = x_size;
	}
	template<typename T>
	inline resource<T>::~resource()
	{
	}
	template<typename T>
	inline const T* resource<T>::get_data()
	{
		//  Lab: 1.02 Implement `cg::resource` class
		return data.data();
	}
	template<typename T>
	inline T& resource<T>::item(size_t item)
	{
		//  Lab: 1.02 Implement `cg::resource` class
		return data.at(item);
	}
	template<typename T>
	inline T& resource<T>::item(size_t x, size_t y)
	{
		//  Lab: 1.02 Implement `cg::resource` class
		return data.at(y * stride + x);
	}
	template<typename T>
	inline size_t resource<T>::size_bytes() const
	{
		//  Lab: 1.02 Implement `cg::resource` class
		return item_size * count();
	}
	template<typename T>
	inline size_t resource<T>::count() const
	{
		//  Lab: 1.02 Implement `cg::resource` class
		return data.size();
	}

	template<typename T>
	inline size_t resource<T>::get_stride() const
	{
		//  Lab: 1.02 Implement `cg::resource` class
		return stride;
	}

	struct color
	{
		static color from_float3(const float3& in)
		{
			//  Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			return color{in.x, in.y, in.z};
		};
		float3 to_float3() const
		{
			//  Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			return float3{r, g, b};
		}
		float r;
		float g;
		float b;
	};

	struct unsigned_color
	{
		static unsigned_color from_color(const color& color)
		{
			//  Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			unsigned_color out{};
			out.r = std::clamp(static_cast<int>(color.r * 255.f), 0, 255);
			out.g = std::clamp(static_cast<int>(color.g * 255.f), 0, 255);
			out.b = std::clamp(static_cast<int>(color.b * 255.f), 0, 255);
			return out;
		};
		static unsigned_color from_float3(const float3& color)
		{
			//  Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			float3 preprocessed = clamp(255.f * color, 0.f, 255.f);
			return unsigned_color{
				static_cast<uint8_t>(preprocessed.x),
				static_cast<uint8_t>(preprocessed.y),
				static_cast<uint8_t>(preprocessed.z)
			};
		};
		float3 to_float3() const
		{
			//  Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			return float3{
				static_cast<float>(r),
				static_cast<float>(g),
				static_cast<float>(b),
			} / 255.f;
		};
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};


	struct vertex
	{
		//  Lab: 1.03 Implement `cg::vertex` struct
		float3 position;
		float3 normal;
		float2 texture;
		float3 ambient;
		float3 diffuse;
		float3 emissive;
	};

}// namespace cg
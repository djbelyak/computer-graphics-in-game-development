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

		size_t get_size_in_bytes() const;
		size_t get_number_of_elements() const;
		size_t get_stride() const;

	private:
		std::vector<T> data;
		size_t item_size = sizeof(T);
		size_t stride;
	};

	template<typename T>
	inline resource<T>::resource(size_t size)
	{
		// TODO Lab: 1.02 Implement `cg::resource` class
	}
	template<typename T>
	inline resource<T>::resource(size_t x_size, size_t y_size)
	{
		// TODO Lab: 1.02 Implement `cg::resource` class
	}
	template<typename T>
	inline resource<T>::~resource()
	{
	}
	template<typename T>
	inline const T* resource<T>::get_data()
	{
		// TODO Lab: 1.02 Implement `cg::resource` class
		return nullptr;
	}
	template<typename T>
	inline T& resource<T>::item(size_t item)
	{
		// TODO Lab: 1.02 Implement `cg::resource` class
		return T();
	}
	template<typename T>
	inline T& resource<T>::item(size_t x, size_t y)
	{
		// TODO Lab: 1.02 Implement `cg::resource` class
		return T();
	}
	template<typename T>
	inline size_t resource<T>::get_size_in_bytes() const
	{
		// TODO Lab: 1.02 Implement `cg::resource` class
		return 0;
	}
	template<typename T>
	inline size_t resource<T>::get_number_of_elements() const
	{
		// TODO Lab: 1.02 Implement `cg::resource` class
		return 0;
	}

	template<typename T>
	inline size_t resource<T>::get_stride() const
	{
		// TODO Lab: 1.02 Implement `cg::resource` class
		return 0;
	}

	struct color
	{
		static color from_float3(const float3& in)
		{
			// TODO Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			return color{};
		};
		float3 to_float3() const
		{
			// TODO Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			return float3{};
		}
		float r;
		float g;
		float b;
	};

	struct unsigned_color
	{
		static unsigned_color from_color(const color& color)
		{
			// TODO Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			return unsigned_color{};
		};
		static unsigned_color from_float3(const float3& color)
		{
			// TODO Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			return unsigned_color{};
		};
		float3 to_float3() const
		{
			// TODO Lab: 1.02 Implement `cg::color` and `cg::unsigned_color` structs
			return float3{};
		};
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};


	struct vertex
	{
		// TODO Lab: 1.03 Implement `cg::vertex` struct
	};

}// namespace cg
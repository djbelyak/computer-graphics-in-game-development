#define _USE_MATH_DEFINES

#include "camera.h"

#include "utils/error_handler.h"

#include <math.h>


using namespace cg::world;

cg::world::camera::camera() : theta(0.f), phi(0.f), height(1080.f), width(1920.f),
							  aspect_ratio(1920.f / 1080.f), angle_of_view(1.04719f),
							  z_near(0.001f), z_far(100.f), position(float3{0.f, 0.f, 0.f})
{
}

cg::world::camera::~camera() {}

void cg::world::camera::set_position(float3 in_position)
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
}

void cg::world::camera::set_theta(float in_theta)
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
}

void cg::world::camera::set_phi(float in_phi)
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
}

void cg::world::camera::set_angle_of_view(float in_aov)
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
}

void cg::world::camera::set_height(float in_height)
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
}

void cg::world::camera::set_width(float in_width)
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
}

void cg::world::camera::set_z_near(float in_z_near)
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
}

void cg::world::camera::set_z_far(float in_z_far)
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
}

const float4x4 cg::world::camera::get_view_matrix() const
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
	return float4x4{};
}

#ifdef DX12
const DirectX::XMMATRIX cg::world::camera::get_dxm_view_matrix() const
{
	// TODO Lab: 3.08 Implement `get_dxm_view_matrix`, `get_dxm_projection_matrix`, and `get_dxm_mvp_matrix` methods of `camera`
	return  DirectX::XMMatrixIdentity();
}

const DirectX::XMMATRIX cg::world::camera::get_dxm_projection_matrix() const
{
	// TODO Lab: 3.08 Implement `get_dxm_view_matrix`, `get_dxm_projection_matrix`, and `get_dxm_mvp_matrix` methods of `camera`
	return  DirectX::XMMatrixIdentity();
}

const DirectX::XMMATRIX camera::get_dxm_mvp_matrix() const
{
	// TODO Lab: 3.08 Implement `get_dxm_view_matrix`, `get_dxm_projection_matrix`, and `get_dxm_mvp_matrix` methods of `camera`
	return  DirectX::XMMatrixIdentity();
}
#endif

const float4x4 cg::world::camera::get_projection_matrix() const
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
	return float4x4{};
}

const float3 cg::world::camera::get_position() const
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
	return float3{};
}

const float3 cg::world::camera::get_direction() const
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
	return float3{};
}

const float3 cg::world::camera::get_right() const
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
	return float3{};
}

const float3 cg::world::camera::get_up() const
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
	return float3{};
}
const float camera::get_theta() const
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
	return 0.f;
}
const float camera::get_phi() const
{
	// TODO Lab: 1.04 Implement `cg::world::camera` class
	return 0.f;
}

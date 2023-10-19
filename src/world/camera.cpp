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

	position = in_position;
}

void cg::world::camera::set_theta(float in_theta)
{
	theta = in_theta * static_cast<float>(M_PI) / 180.f;
}

void cg::world::camera::set_phi(float in_phi)
{
	phi = in_phi * static_cast<float>(M_PI) / 180.f;
}

void cg::world::camera::set_angle_of_view(float in_aov)
{
	angle_of_view = in_aov * static_cast<float>(M_PI) / 180.f;
}

void cg::world::camera::set_height(float in_height)
{
	height = in_height;
	aspect_ratio = width / height;
}

void cg::world::camera::set_width(float in_width)
{
	width = in_width;
	aspect_ratio = width / height;
}

void cg::world::camera::set_z_near(float in_z_near)
{
	z_near = in_z_near;
}

void cg::world::camera::set_z_far(float in_z_far)
{
	z_far = in_z_far;
}

const float4x4 cg::world::camera::get_view_matrix() const
{
	float3 up{0.f, 1.f, 0.f};
	float3 eye = position + get_direction();

	float3 z_axis = normalize(position - eye);
	float3 x_axis = normalize(cross(up, z_axis));
	float3 y_axis = cross(z_axis, x_axis);
	return float4x4{
			{x_axis.x, y_axis.x, z_axis.x, 0},
			{x_axis.y, y_axis.y, z_axis.y, 0},
			{x_axis.z, y_axis.z, z_axis.z, 0},
			{-dot(x_axis, position), -dot(y_axis, position), -dot(z_axis, position), 1}};
}

#ifdef DX12
const DirectX::XMMATRIX cg::world::camera::get_dxm_view_matrix() const
{
	return DirectX::XMMatrixIdentity();
}

const DirectX::XMMATRIX cg::world::camera::get_dxm_projection_matrix() const
{
	return DirectX::XMMatrixIdentity();
}

const DirectX::XMMATRIX camera::get_dxm_mvp_matrix() const
{

	return DirectX::XMMatrixIdentity();
}
#endif

const float4x4 cg::world::camera::get_projection_matrix() const
{
	float f = 1.f / tanf(angle_of_view / 2.f);
	return float4x4{
			{f / aspect_ratio, 0, 0, 0},
			{0, f, 0, 0},
			{0, 0, z_far / (z_near - z_far), -1},
			{0, 0, (z_far * z_near) / (z_near - z_far), 0}};
}

const float3 cg::world::camera::get_position() const
{
	return position;
}

const float3 cg::world::camera::get_direction() const
{

	return float3{
			sin(theta) * cos(phi),
			sin(phi),
			-cos(theta) * cos(phi)};
}

const float3 cg::world::camera::get_right() const
{
	return cross(get_direction(), float3(0.f, 1.f, 0.f));
}

const float3 cg::world::camera::get_up() const
{

	return cross(get_right(), get_direction());
}

const float camera::get_theta() const
{
	return theta;
}

const float camera::get_phi() const
{
	return phi;
}

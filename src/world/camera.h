#pragma once

#include <linalg.h>
#ifdef DX12
#include <DirectXMath.h>
#endif


using namespace linalg::aliases;

namespace cg::world
{
	class camera
	{
	public:
		camera();
		virtual ~camera();

		void set_position(float3 in_position);
		void set_theta(float in_theta);
		void set_phi(float in_phi);

		void set_angle_of_view(float in_aov);
		void set_height(float in_height);
		void set_width(float in_width);
		void set_z_near(float in_z_near);
		void set_z_far(float in_z_far);

		const float4x4 get_view_matrix() const;
		const float4x4 get_projection_matrix() const;

#ifdef DX12
		const DirectX::XMMATRIX get_dxm_view_matrix() const;
		const DirectX::XMMATRIX get_dxm_projection_matrix() const;
		const DirectX::XMMATRIX get_dxm_mvp_matrix() const;
#endif
		const float3 get_position() const;
		const float3 get_direction() const;
		const float3 get_right() const;
		const float3 get_up() const;
		const float get_theta() const;
		const float get_phi() const;

	protected:
		float3 position;
		float theta;
		float phi;

		float height;
		float width;
		float aspect_ratio;
		float angle_of_view;
		float z_near;
		float z_far;
	};
}// namespace cg::world
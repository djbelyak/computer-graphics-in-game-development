#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include "renderer/renderer.h"

#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_4.h>
#include <exception>
#include <initguid.h>
#include <iostream>
#include <wrl.h>


using namespace Microsoft::WRL;

namespace cg::renderer
{
	struct light
	{
		float4 position;
		float4 color;
	};
	struct constant_buffer
	{
		DirectX::XMMATRIX mwpMatrix;
		DirectX::XMMATRIX shadowMatrix;
		light light;
	};

	class descriptor_heap
	{
	public:
		void create_heap(ComPtr<ID3D12Device>& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT number = 1, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		D3D12_CPU_DESCRIPTOR_HANDLE get_cpu_descriptor_handle(UINT index = 0) const;
		D3D12_GPU_DESCRIPTOR_HANDLE get_gpu_descriptor_handle(UINT index = 0) const;
		ID3D12DescriptorHeap* get() const;

	private:
		ComPtr<ID3D12DescriptorHeap> heap;
		UINT descriptor_size;
	};

	class dx12_renderer : public renderer
	{
	public:
		void init() override;
		void destroy() override;

		void update() override;
		void render() override;

	protected:
		static const UINT frame_number = 2;

		// Pipeline objects.
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> command_queue;
		ComPtr<IDXGISwapChain3> swap_chain;
		descriptor_heap rtv_heap;
		descriptor_heap cbv_srv_heap;
		descriptor_heap dsv_heap;
		ComPtr<ID3D12Resource> render_targets[frame_number];
		ComPtr<ID3D12CommandAllocator> command_allocators[frame_number];
		ComPtr<ID3D12PipelineState> pipeline_state;
		ComPtr<ID3D12PipelineState> pipeline_state_texture;
		ComPtr<ID3D12PipelineState> pipeline_state_shadow;
		ComPtr<ID3D12GraphicsCommandList> command_list;

		ComPtr<ID3D12RootSignature> root_signature;
		CD3DX12_VIEWPORT view_port;
		CD3DX12_RECT scissor_rect;

		// Resources
		std::vector<ComPtr<ID3D12Resource>> vertex_buffers;
		std::vector<ComPtr<ID3D12Resource>> upload_vertex_buffers;
		std::vector<D3D12_VERTEX_BUFFER_VIEW> vertex_buffer_views;

		std::vector<ComPtr<ID3D12Resource>> index_buffers;
		std::vector<ComPtr<ID3D12Resource>> upload_index_buffers;
		std::vector<D3D12_INDEX_BUFFER_VIEW> index_buffer_views;

		std::vector<ComPtr<ID3D12Resource>> textures;
		std::vector<ComPtr<ID3D12Resource>> upload_textures;

		ComPtr<ID3D12Resource> depth_buffer;

		ComPtr<ID3D12Resource> shadow_map;

		ComPtr<ID3D12Resource> constant_buffer;
		UINT8* constant_buffer_data_begin;

		cg::renderer::constant_buffer cb{};

		std::shared_ptr<cg::world::camera> shadow_light;

		// Synchronization objects.
		UINT frame_index;
		HANDLE fence_event;
		ComPtr<ID3D12Fence> fence;
		UINT64 fence_values[frame_number];

		void load_pipeline();
		void load_assets();
		void populate_command_list();

		void move_to_next_frame();
		void wait_for_gpu();

		static ComPtr<IDXGIFactory4> get_dxgi_factory() ;
		void initialize_device(ComPtr<IDXGIFactory4>& dxgi_factory);
		void create_direct_command_queue();
		void create_swap_chain(ComPtr<IDXGIFactory4>& dxgi_factory);

		void create_resource_on_upload_heap(ComPtr<ID3D12Resource>& resource, UINT size = 0, const std::wstring& name = L"");
		static void copy_data(const void* buffer_data, UINT buffer_size, ComPtr<ID3D12Resource>& destination_resource);

		void create_render_target_views();
		static D3D12_VERTEX_BUFFER_VIEW create_vertex_buffer_view(const ComPtr<ID3D12Resource>& vertex_buffer, UINT vertex_buffer_size);
		static D3D12_INDEX_BUFFER_VIEW create_index_buffer_view(const ComPtr<ID3D12Resource>& index_buffer, UINT index_buffer_size);
		void create_constant_buffer_view(const ComPtr<ID3D12Resource>& buffer, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handler);

		void create_root_signature(const D3D12_STATIC_SAMPLER_DESC* sampler_descriptors, UINT num_sampler_descriptors);
		static std::filesystem::path get_shader_path(const std::string& shader_name = "shaders.hlsl") ;
		static ComPtr<ID3DBlob> compile_shader(const std::filesystem::path& shader_path, const std::string& entrypoint, const std::string& target) ;
		void create_pso(const std::string& shader_name);

		void create_command_allocators();
		void create_command_list();

		void create_depth_buffer();

		static D3D12_STATIC_SAMPLER_DESC get_sampler_descriptor() ;


		void create_resource_on_default_heap(ComPtr<ID3D12Resource>& resource, UINT size = 0, const std::wstring& name = L"", D3D12_RESOURCE_DESC* resource_descriptor = nullptr);

		void copy_data(const void* buffer_data, UINT buffer_size, ComPtr<ID3D12Resource>& destination_resource, ComPtr<ID3D12Resource>& intermediate_resource, D3D12_RESOURCE_STATES state_after, int row_pitch = 0, int slice_pitch = 0);

		void create_shader_resource_view(const ComPtr<ID3D12Resource>& texture, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handler);

	};
}// namespace cg::renderer
#include "dx12_renderer.h"

#include "utils/com_error_handler.h"
#include "utils/window.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>


void cg::renderer::dx12_renderer::init()
{
	// TODO Lab: 3.01 Add `model` and `camera` creation code into `init` method of `dx12_renderer` class
}

void cg::renderer::dx12_renderer::destroy()
{
	wait_for_gpu();
	CloseHandle(fence_event);
}

void cg::renderer::dx12_renderer::update()
{
	// TODO Lab: 3.08 Implement `update` method of `dx12_renderer`
}

void cg::renderer::dx12_renderer::render()
{
	// TODO Lab: 3.06 Implement `render` method
}

ComPtr<IDXGIFactory4> cg::renderer::dx12_renderer::get_dxgi_factory()
{
	// TODO Lab: 3.02 Enable a validation layer
	return nullptr;
}

void cg::renderer::dx12_renderer::initialize_device(ComPtr<IDXGIFactory4>& dxgi_factory)
{
	// TODO Lab: 3.02 Enumerate hardware adapters
	// TODO Lab: 3.02 Create a device object
}

void cg::renderer::dx12_renderer::create_direct_command_queue()
{
	// TODO Lab: 3.02 Create a command queue
}

void cg::renderer::dx12_renderer::create_swap_chain(ComPtr<IDXGIFactory4>& dxgi_factory)
{
	// TODO Lab: 3.02 Create a swap chain and bind it to window
}

void cg::renderer::dx12_renderer::create_render_target_views()
{
	// TODO Lab: 3.04 Create a descriptor heap for render targets
	// TODO Lab: 3.04 Create render target views
}

void cg::renderer::dx12_renderer::create_depth_buffer()
{
}

void cg::renderer::dx12_renderer::create_command_allocators()
{
	// TODO Lab: 3.06 Create command allocators and a command list
}

void cg::renderer::dx12_renderer::create_command_list()
{
	// TODO Lab: 3.06 Create command allocators and a command list
}


void cg::renderer::dx12_renderer::load_pipeline()
{
	// TODO Lab: 3.02 Bring everything together in `load_pipeline` method
	// TODO Lab: 3.04 Create render target views
}

D3D12_STATIC_SAMPLER_DESC cg::renderer::dx12_renderer::get_sampler_descriptor()
{
	D3D12_STATIC_SAMPLER_DESC sampler_desc{};
	return sampler_desc;
}

void cg::renderer::dx12_renderer::create_root_signature(const D3D12_STATIC_SAMPLER_DESC* sampler_descriptors, UINT num_sampler_descriptors)
{
	// TODO Lab: 3.05 Create a descriptor table and a root signature
}

std::filesystem::path cg::renderer::dx12_renderer::get_shader_path(const std::string& shader_name)
{
	// TODO Lab: 3.05 Compile shaders
	return "";
}

ComPtr<ID3DBlob> cg::renderer::dx12_renderer::compile_shader(const std::filesystem::path& shader_path, const std::string& entrypoint, const std::string& target)
{
	// TODO Lab: 3.05 Compile shaders
	return nullptr;
}

void cg::renderer::dx12_renderer::create_pso(const std::string& shader_name)
{
	// TODO Lab: 3.05 Compile shaders
	// TODO Lab: 3.05 Setup a PSO descriptor and create a PSO
}

void cg::renderer::dx12_renderer::create_resource_on_upload_heap(ComPtr<ID3D12Resource>& resource, UINT size, const std::wstring& name)
{
	// TODO Lab: 3.03 Implement resource creation on upload heap
}

void cg::renderer::dx12_renderer::create_resource_on_default_heap(ComPtr<ID3D12Resource>& resource, UINT size, const std::wstring& name, D3D12_RESOURCE_DESC* resource_descriptor)
{
}

void cg::renderer::dx12_renderer::copy_data(const void* buffer_data, UINT buffer_size, ComPtr<ID3D12Resource>& destination_resource)
{
	// TODO Lab: 3.03 Implement map, unmap, and copying data to the resource
}

void cg::renderer::dx12_renderer::copy_data(const void* buffer_data, const UINT buffer_size, ComPtr<ID3D12Resource>& destination_resource, ComPtr<ID3D12Resource>& intermediate_resource, D3D12_RESOURCE_STATES state_after, int row_pitch, int slice_pitch)
{
}

D3D12_VERTEX_BUFFER_VIEW cg::renderer::dx12_renderer::create_vertex_buffer_view(const ComPtr<ID3D12Resource>& vertex_buffer, const UINT vertex_buffer_size)
{
	// TODO Lab: 3.04 Create vertex buffer views
	return D3D12_VERTEX_BUFFER_VIEW{};
}

D3D12_INDEX_BUFFER_VIEW cg::renderer::dx12_renderer::create_index_buffer_view(const ComPtr<ID3D12Resource>& index_buffer, const UINT index_buffer_size)
{
	// TODO Lab: 3.04 Create index buffer views
	return D3D12_INDEX_BUFFER_VIEW{};
}

void cg::renderer::dx12_renderer::create_shader_resource_view(const ComPtr<ID3D12Resource>& texture, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handler)
{
}

void cg::renderer::dx12_renderer::create_constant_buffer_view(const ComPtr<ID3D12Resource>& buffer, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handler)
{
	// TODO Lab: 3.04 Create a constant buffer view
}

void cg::renderer::dx12_renderer::load_assets()
{
	// TODO Lab: 3.05 Create a descriptor table and a root signature
	// TODO Lab: 3.05 Setup a PSO descriptor and create a PSO
	// TODO Lab: 3.06 Create command allocators and a command list

	// TODO Lab: 3.04 Create a descriptor heap for a constant buffer

	// TODO Lab: 3.03 Allocate memory for vertex and index buffers
	// TODO Lab: 3.03 Create committed resources for vertex, index and constant buffers on upload heap
	// TODO Lab: 3.03 Copy resource data to suitable resources
	// TODO Lab: 3.04 Create vertex buffer views
	// TODO Lab: 3.04 Create index buffer views

	// TODO Lab: 3.04 Create a constant buffer view

	// TODO Lab: 3.07 Create a fence and fence event
}


void cg::renderer::dx12_renderer::populate_command_list()
{
	// TODO Lab: 3.06 Implement `populate_command_list` method
}


void cg::renderer::dx12_renderer::move_to_next_frame()
{
	// TODO Lab: 3.07 Implement `move_to_next_frame` method
}

void cg::renderer::dx12_renderer::wait_for_gpu()
{
	// TODO Lab: 3.07 Implement `wait_for_gpu` method
}


void cg::renderer::descriptor_heap::create_heap(ComPtr<ID3D12Device>& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT number, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
{
	// TODO Lab: 3.04 Implement `create_heap`, `get_cpu_descriptor_handle`, `get_gpu_descriptor_handle`, and `get` methods of `cg::renderer::descriptor_heap`
}

D3D12_CPU_DESCRIPTOR_HANDLE cg::renderer::descriptor_heap::get_cpu_descriptor_handle(UINT index) const
{
	// TODO Lab: 3.04 Implement `create_heap`, `get_cpu_descriptor_handle`, `get_gpu_descriptor_handle`, and `get` methods of `cg::renderer::descriptor_heap`
	return D3D12_CPU_DESCRIPTOR_HANDLE{};
}

D3D12_GPU_DESCRIPTOR_HANDLE cg::renderer::descriptor_heap::get_gpu_descriptor_handle(UINT index) const
{
	// TODO Lab: 3.04 Implement `create_heap`, `get_cpu_descriptor_handle`, `get_gpu_descriptor_handle`, and `get` methods of `cg::renderer::descriptor_heap`
	return D3D12_GPU_DESCRIPTOR_HANDLE{};
}
ID3D12DescriptorHeap* cg::renderer::descriptor_heap::get() const
{
	// TODO Lab: 3.04 Implement `create_heap`, `get_cpu_descriptor_handle`, `get_gpu_descriptor_handle`, and `get` methods of `cg::renderer::descriptor_heap`
	return nullptr;
}

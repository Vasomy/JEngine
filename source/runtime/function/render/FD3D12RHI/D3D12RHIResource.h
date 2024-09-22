#pragma once
#ifndef D3D12RHIRESOURCE_H
#define D3D12RHIRESOURCE_H

#include"../interface/RHI_Resource.h"
#include"D3D12Resource.h"
#include"D3D12Util.h"
#include"../RenderMeshNodeTree.h"
#include<runtime/core/JCores.h>
#include<unordered_map>
class D3D12RHIBuffer : public RHIBuffer
{
public:
	~D3D12RHIBuffer()override
	{
		Resource.Release();
	}
	JD3D12Resource Resource;

};
class D3D12RHIVertexBuffer : public RHIVertexBuffer
{
public:
	D3D12RHIVertexBuffer()
		:RHIVertexBuffer()
	{

	}
	D3D12RHIVertexBuffer(int32 InVertexByteSize, int32 InStride, int32 InOffset = 0)
		:RHIVertexBuffer(InVertexByteSize, InStride, InOffset)
	{
		


	}
	JD3D12Resource Resource;
	D3D12_GPU_VIRTUAL_ADDRESS ResourceAddress{0};
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = Resource.Resource->GetGPUVirtualAddress();
		vbv.SizeInBytes = Size;
		vbv.StrideInBytes = Stride;
		return vbv;
	}

};
class D3D12RHIIndexBuffer : public RHIIndexBuffer
{
public:
	D3D12RHIIndexBuffer()
		:RHIIndexBuffer()
	{

	}
	D3D12RHIIndexBuffer(int32 InIndexCount)
		:RHIIndexBuffer(InIndexCount)
	{

	}
	JD3D12Resource Resource;
	D3D12_GPU_VIRTUAL_ADDRESS ResourceAddress{0};
	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = Resource.Resource->GetGPUVirtualAddress();
		ibv.Format = DXGI_FORMAT_R32_UINT;
		ibv.SizeInBytes = Size;
		return ibv;
	}
};
class D3D12ShaderUniformDesc
{
public:
	D3D12ShaderUniformDesc(ShaderUniformType type,
		ID3D12Device*Device,uint32 Count,uint32 Size,void* Data = nullptr,bool isConstant = false)
	{
		Buffer = std::make_unique<UploadBuffer>(Device, Count, Size, isConstant);
		if (Data)
			Buffer->CopyData(0, Data);
	}
	ShaderUniformType ValType;
	uint32 slot = 0;
	std::unique_ptr<UploadBuffer> Buffer;
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress()
	{
		return Buffer->Resource()->GetGPUVirtualAddress();
	}
};
class D3D12RHIShaderUniformBuffer : public RHIShaderUniformBuffer
{
public:
	D3D12RHIShaderUniformBuffer()
		:RHIShaderUniformBuffer(0)
	{}
	D3D12RHIShaderUniformBuffer(int32 InUniformCount)
		:RHIShaderUniformBuffer(InUniformCount)
	{}
	using UniformDescArray = std::vector<D3D12ShaderUniformDesc*>;
	UniformDescArray UniformDescs;
	void Add(D3D12ShaderUniformDesc* desc)
	{
		UniformDescs.push_back(nullptr);
		UniformDescs[0] = std::move(desc);
	}
	virtual void Release()override
	{
		UniformDescs.clear();
		UniformDescs.shrink_to_fit();
	}
	virtual void Destory()override
	{

	}
	virtual ~D3D12RHIShaderUniformBuffer()override
	{

	}
};
class D3D12RHIShader : public RHIShader
{
public:
	D3D12RHIShader()
		:RHIShader()
	{

	}
	using ShadersMap = std::unordered_map<JString,ComPtr<ID3DBlob>>;
	ShadersMap Shaders;
	virtual void Release()override
	{

	}
	virtual void Destory()override
	{

	}
	virtual ~D3D12RHIShader()override
	{

	}
};
class D3D12RHITexture : public RHITexture
{
public:
	JD3D12Resource Resource;
	void Release()override {
		Resource.Release();
	}
	void Destory()override
	{
	}
	ID3D12Resource* GetResource()
	{
		return Resource.Resource.Get();
	}

	void Reset()
	{
		Resource.Resource.Reset();
		Resource.UploadHeap.Reset();
	}
};

class D3D12RHIRenderTarget : public RHIRenderTarget
{
public:
	D3D12RHIRenderTarget(int32 rt_nums = 1)
		:RHIRenderTarget() {
		RTVTexture = std::vector<D3D12RHITexture>(rt_nums);
	}
	std::vector<D3D12RHITexture>RTVTexture;
	D3D12RHITexture DSVTexture;

	ComPtr<ID3D12DescriptorHeap> RTVHeap;
	ComPtr<ID3D12DescriptorHeap> DSVHeap;

	D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle;// RTVTexture 在RenderTarge SRV Heap中的GPU地址
										  // RenderTarget SRV Heap 在 OfflineDescriptorManager中管理
	JString RTName;
	D3D12_GPU_DESCRIPTOR_HANDLE GetRtvGpuHandle() {
		return RTVHeap->GetGPUDescriptorHandleForHeapStart();
	}
	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvCpuHandle()
	{
		return RTVHeap->GetCPUDescriptorHandleForHeapStart();
	}
	void CreateHeaps(ID3D12Device*device,int32 RTVNums = 1,bool isSwapToScreen = false)
	{
	
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;
		IfFailedHResultFile(device->CreateDescriptorHeap(
			&dsvHeapDesc, IID_PPV_ARGS(&DSVHeap)));

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.NumDescriptors = RTVNums;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		IfFailedHResultFile(device->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(&RTVHeap)));

		

		auto rtvHeapHandle = RTVHeap->GetCPUDescriptorHandleForHeapStart();
		uint32 rtvByteSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		for (int32 i = 0; i < RTVNums; ++i) {
			D3D12RHITexture& tex = RTVTexture[i];
			CD3DX12_CPU_DESCRIPTOR_HANDLE handle(rtvHeapHandle);
			handle.Offset(i, rtvByteSize);
			device->CreateRenderTargetView(tex.Resource.Resource.Get(), nullptr, handle);
		}
		D3D12_DEPTH_STENCIL_VIEW_DESC stDepthStencilDesc = {};
		//stDepthStencilDesc.Format = CastFormat(DSVTexture.GetDesc().TextureFormat); //should be that
		stDepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		stDepthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		stDepthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

		device->CreateDepthStencilView(DSVTexture.Resource.Resource.Get()
			, &stDepthStencilDesc
			, DSVHeap->GetCPUDescriptorHandleForHeapStart());
	}
	void CreateSwapToScreenHeaps(ID3D12Device* device, IDXGISwapChain* swap_chain)
	{
		RTVTexture = std::vector<D3D12RHITexture>(2);
		for (int32 i = 0; i < 2; ++i)
		{
			RTVTexture[i].SetDesc(RHITextureDesc::RenderTargetTexture());
			RTVTexture[i].SetClearColorValue({1.0f,0.0f,0.0f,1.0f});
		}
		DSVTexture.SetDesc(RHITextureDesc::RenderTargetDepthStencil());
		DSVTexture.SetClearDepthStencilValue(1.0f, 0u);
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.NumDescriptors = 2;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		IfFailedHResultFile(device->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;
		IfFailedHResultFile(device->CreateDescriptorHeap(
			&dsvHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));

		
	}
	virtual void Release()override
	{}
	virtual void Destory()override
	{}

};

#endif // !D3D12RHIRESOURCE_H

#pragma once
#ifndef D3D12RHIDYNAMIC_H
#define D3D12RHIDYNAMIC_H

#include"../interface/RHI.h"
#include"d3dx12.h"
#include"D3D12TypeTraits.h"
#include"D3D12OfflineDescriptorManager.h"
#include"D3D12Resource.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include<iostream>
class UIContextBase;
class UIContextImGui;
class D3D12RHIDynamic : public RHIDynamic
{
public:
	friend class UIContextBase;
	friend class UIContextImGui;
public:
	D3D12RHIDynamic()
	{
		this->type = RHIInterfaceType::DirectX12;
	}
	~D3D12RHIDynamic()override
	{
		
	}
	

	virtual void Init()override;
	virtual void CreateVertexBuffer(RHIVertexBuffer** VertexBuffer,
		uint32 InitDataSize, uint32 Stride, uint32 offset)override;
	virtual void CreateVertexBufferAndInitialize(RHIVertexBuffer** VertexBuffer,
		const void* InitData, uint32 InitDataSize, uint32 Strid, uint32 offset)override;
	virtual void CreateIndexBuffer(RHIIndexBuffer** IndexBuffer, uint32 IndexCount)override;
	virtual void CreateIndexBufferAndInitialize(RHIIndexBuffer** IndexBuffer, 
		const void* Indexdata,
		uint32 IndexCount)override;
	virtual void CreateUniformBuffer(RHIShaderUniformBuffer** ShaderUniformBuffer) override;// init
	virtual void AddUniformBufferPartView(RHIShaderUniformBuffer* ShaderUniformBuffer, 
		void* data, 
		uint32 size/*unit data size*/, 
		uint32 count,
		ShaderUniformType type,
		bool isConstant = false) override;
	virtual void UpdateUniformBuffer(RHIShaderUniformBuffer* ShaderUniformBuffer, uint32 index, void* data, uint32 UpdateStart) override;

	virtual void CreateRootSignature(RHISampler& Samplers,
		ShaderUniformInfo UniformInfo,
		JString rootSigName) override;
	virtual void CreatePipeLineState(const RHIGraphicsPipeLineState& PipeLineState, 
		JString CreatedPipeLineName) override;
	virtual void PushSamplerInitializer(RHISampler& sampler,
		const RHISamplerInitializer& samplerInitializer) override;

	virtual void LoadShader(JString file_name, JString shader_name, int32 flags)override;
	virtual void CompileShader(RHIShader* Shader)override;

	virtual void LoadTexture(JString file_name, RHITexture* out_texture, RHITextureDesc* desc,TextureLoadType type) override;

	// should alloc new RTV heap for RenderTarget
	virtual void CreateRenderTarget(JString RenderTargetName,RHIRenderTarget* render_target,
		RHITextureDesc rtv_texture, RHITextureDesc dsv_texture,int32 rt_nums)override;
	virtual void CreateRenderTargetTexture(RHITexture* out_rtv_texture, RHITextureDesc rtv_texture_desc,
		RHITexture* out_dsv_texture, RHITextureDesc dsv_texture_desc)override;
	virtual void SetRenderTarget(JString render_target)override;
	virtual void SetGraphicsPipeLine(JString pipeline_name)override; // reset cmdListAlloc 的功能隐藏在设置渲染管线中
	virtual void SetRootSignature(JString root_sig_name)override;

	virtual void OnResizeWindow(int32 width,int32 height)override;


public:
	virtual void PrepareForRender()override;
	virtual void ResetRenderState() override;//reset pipeline, rootsig ... something else

	virtual void SubmitRenderData(RHIVertexBuffer* VertexBuffer, 
		RHIIndexBuffer* IndexBuffer,
		RHIShaderUniformBuffer*UniformBuffer,
		PrimitiveTopology TopologyType)override;

	virtual void SubmitRender()override;
	virtual void DrawInstance()override;

	virtual void SwapBackBufferToScreen() override;// 在所有的渲染结束后，将UI渲染和渲染到材质的画面 交换上去

	virtual void BeginRecordCommandList()override;

	virtual void CloseCommandList()override;//end record command list

	virtual void ExecuteCommandList() override;

public:
	virtual void* GetTextureDataForUI(TextureUsage flags, uint32 offset) override;// 主要是 为 ImGui提供 void*作为渲染材质,offset 为所要材质在 堆中的偏移量

private:
	
	D3D12_FILTER GetDX12Filter(RHISamplerFilter Filter);
	D3D12_TEXTURE_ADDRESS_MODE GetDX12AddressMode(RHISamplerAddress Address);
	CD3DX12_STATIC_SAMPLER_DESC GetSamplerDesc(RHISamplerInitializer& sampler, int32 registerSlot);
	D3D12_INPUT_ELEMENT_DESC GetShaderInputLayout(const ShaderInputLayOutPart& layout);
	D3D12_RASTERIZER_DESC GetRasterizerDesc(const RasterizerState& state);
	D3D12_BLEND_DESC GetBlendDesc(const BlendState& desc);
	D3D12_BLEND CastBlendEnum(Blend target);
	D3D12_BLEND_OP CastBlendOp(BlendOp op);
	DXGI_FORMAT CastFormat(Format format);
	D3D12_DEPTH_STENCIL_DESC CastDepthStencilDesc(const DepthStencilDesc& DSD);
	D3D12_STENCIL_OP CastStencilOp(StencilOP op);
	D3D12_DEPTH_STENCILOP_DESC CastDSDesc(const DepthStencilOpDesc& DSOD);
	D3D12_COMPARISON_FUNC CastComparison(ComparisonFunc cf);

	void AllocTextureHeap(int32 texture_nums = 1000);
	D3D12OfflineDescriptorManager OfflineDescriptorHeapManager;

	bool InitDirectX12();
	bool InitSwapToScreenRTVandDSV();
	void LogAdapters();
	void CreateCommandObjects();
	void CreateSwapChain();
	void FlushCommand();
	uint64 mCurrentFence = 0;

	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

	template<typename RHIType>
	typename D3D12TypeTraits<RHIType>::traits_type* ResourceCast(RHIType* rhi_resource);

	using DrawInstancedViewArray = std::vector<D3D12DrawInstancedView>;
	using PassDataViewArray = std::vector<D3D12UniformSlotDesc>;
	PassDataViewArray PassDataViews;
	DrawInstancedViewArray DrawInstancedViews;

	JD3D12RenderSettingContext RenderSettingContext;

	ComPtr<IDXGIFactory4> DxFactory;
	ComPtr<IDXGISwapChain> DxSwapChain;
	ComPtr<ID3D12Device> DxDevice;

	ComPtr<ID3D12GraphicsCommandList>DxCMDList;
	ComPtr<ID3D12CommandAllocator>DxCMDAlloc;
	ComPtr<ID3D12CommandQueue>DxCMDQueue;

	static const int32 SwapChainBufferCount = 2;
	int32 iCurrentBackBuffer = 0;
	int32 SwapChainBufferWidth= 1920, SwapChainBufferHeight =1080;


	// for Main Camera
	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mRect;
	int32 m4xMsaaQuality = 0;
	bool m4xMsaaState = 0;

	//以下内容可以被简化为一个D3D12RHIRenderTarget
	//D3D12RHIRenderTarget* SwapToScreenRT; // named SwapToScreen as key in map
	/*
	int32 CurrentSwapToScreenBufferIndex = 0;
	ComPtr<ID3D12Resource>mSwapToScreenBuffer[SwapChainBufferCount];
	ComPtr<ID3D12Resource>mSwapToScreenDSBuffer;// depth stencil
	ComPtr<ID3D12DescriptorHeap>mSwapToScreenRTVHeap;
	ComPtr<ID3D12DescriptorHeap>mSwapToScreenDSVHeap;
	*/
	ComPtr<ID3D12Fence>DxFence;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	using RootSignatureMap = std::unordered_map<JString,ComPtr<ID3D12RootSignature>>;
	RootSignatureMap mRootSignatures;
	using GraphicsPipeLineMap = std::unordered_map<JString, ComPtr<ID3D12PipelineState>>;
	GraphicsPipeLineMap mPSOs;
	using RenderTargetMap = std::unordered_map<JString, std::shared_ptr<D3D12RHIRenderTarget>>;
	RenderTargetMap mRenderTargets;
	using ShadersMap = std::unordered_map<JString, std::shared_ptr<D3D12RHIShader>>;
	ShadersMap mShaders;


	ClearValueBinding mBackBufferClearVal = ClearValueBinding({ 1.0f,0.0f,0.0f,1.0f });
};

#endif // !RHIDYNAMIC_H



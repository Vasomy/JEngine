#pragma once
#ifndef RHIDYNAMIC_H
#define RHIDYNAMIC_H
#include"RHI_Resource.h"
#include"RHI_Definitions.h"



class RHIDynamic : public std::enable_shared_from_this<RHIDynamic>
{
public:
	RHIDynamic(){}
	RHIDynamic(const RHIDynamic& rhs) = delete;
	virtual ~RHIDynamic() {
		
	}

	virtual void Init() = 0;

	 

	virtual void CreateVertexBuffer(RHIVertexBuffer** VertexBuffer,
		uint32 InitDataSize,
		uint32 Stride,
		uint32 offset) = 0;
	virtual void CreateVertexBufferAndInitialize(RHIVertexBuffer** VertexBuffer,
		const void* InitData,
		uint32 InitDataSize,
		uint32 Strid,
		uint32 offset) = 0;
	virtual void CreateIndexBuffer(RHIIndexBuffer** IndexBuffer,
		uint32 IndexCount) = 0;
	virtual void CreateIndexBufferAndInitialize(RHIIndexBuffer** IndexBuffer,
		const void* Indexdata, uint32 IndexCount) = 0;
	virtual void CreateUniformBuffer(RHIShaderUniformBuffer** ShaderUniformBuffer) = 0;// init
	virtual void AddUniformBufferPartView(RHIShaderUniformBuffer* ShaderUniformBuffer,
		void* data,
		uint32 size/*unit data size*/,
		uint32 count,
		ShaderUniformType type,
		bool isConstant = false) = 0;
	virtual void UpdateUniformBuffer(RHIShaderUniformBuffer* ShaderUniformBuffer, uint32 index, void* data, uint32 UpdateStart) = 0;

	virtual void CreateRootSignature(RHISampler& Samplers,
		ShaderUniformInfo UniformInfo,
		JString rootSigName) = 0;
	virtual void CreatePipeLineState(const RHIGraphicsPipeLineState& PipeLineState,
		JString PipeLineName) = 0;
	virtual void PushSamplerInitializer(RHISampler&sampler,
		const RHISamplerInitializer& samplerInitializer) = 0;

	virtual void LoadShader(JString FileName, JString ShaderName, int32 flags) = 0;
	virtual void CompileShader(RHIShader* Shader) = 0;

	virtual void LoadTexture(JString file_name,RHITexture*out_texture ,RHITextureDesc* desc,TextureLoadType type) = 0;

	virtual void CreateRenderTarget(JString RenderTargetName,RHIRenderTarget* render_target, RHITextureDesc rtv_texture, RHITextureDesc dsv_texture,int32 rt_nums = 1) = 0;
	virtual void CreateRenderTargetTexture(RHITexture* out_rtv_texture, RHITextureDesc rtv_texture_desc,
		RHITexture* out_dsv_texture,RHITextureDesc dsv_texture_desc) = 0;
	virtual void SetRenderTarget(JString render_target) = 0;
	virtual void SetGraphicsPipeLine(JString pipeline_name) = 0;
	virtual void SetRootSignature(JString root_sig_name) = 0;

	virtual void OnResizeWindow(int32 width,int32 height) = 0;//should be called when crab window's edge

	virtual void BeginRecordCommandList() = 0;

	virtual void CloseCommandList() = 0;

	virtual void ExecuteCommandList() = 0;

protected:

public:
	virtual void PrepareForRender() = 0;
	virtual void ResetRenderState() = 0;//reset pipeline, rootsig ... something else

	virtual void SubmitRenderData(RHIVertexBuffer* VertexBuffer, 
		RHIIndexBuffer* IndexBuffer,
		RHIShaderUniformBuffer*UniformBuffer,
		PrimitiveTopology TopologyType) = 0;
	virtual void SubmitRender() = 0;
	virtual void DrawInstance() = 0;

	virtual void SwapBackBufferToScreen() = 0;// 在所有的渲染结束后，将UI渲染和渲染到材质的画面 交换上去

public:																
	virtual void* GetTextureDataForUI(TextureUsage flags,uint32 offset) = 0;// 主要是 为 ImGui提供 void*作为渲染材质,offset 为所要材质在 堆中的偏移量

public:

	RHIInterfaceType type = RHIInterfaceType::Unknow;
};

#endif // !RHIDYNAMIC_H

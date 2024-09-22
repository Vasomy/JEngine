#pragma once

#ifndef RHI_RESOURCE
#define RHI_RESOURCE
#include<runtime/core/JCores.h>
#include"RHI_Definitions.h"
#include"RHICommon.h"

class RHIResource;
class RHISamplerInitializer;
class RHISampler;

class RHIBuffer;

class RHIResource : public std::enable_shared_from_this<RHIResource>
{
public:
	RHIResource(RHIResourceType type)
		:Type(type)
	{

	}
	virtual ~RHIResource() {}

	virtual void Release() = 0; // 等同于 Reset 清空数据 但是保留类本身
	virtual void Destory() = 0; // 销毁类本身


public:
	const RHIResourceType GetType() const { return Type; }

	RHIResourceType Type;
};



class RHISamplerInitializer
{
public:
	RHISamplerInitializer(){}
	RHISamplerInitializer(
		RHISamplerFilter InFilter,
		RHISamplerAddress InUAddress,
		RHISamplerAddress InVAddress,
		RHISamplerAddress InWAddress,
		float32 InMipBias,
		float32 InMinMipLevel,
		float32 InMaxMipLevel,
		int32 InMaxAnisotropy)
		:Filter(InFilter),
		UAddress(InUAddress),
		VAddress(InVAddress),
		WAddress(InWAddress),
		MipBias(InMipBias),
		MinMipLevel(InMinMipLevel),
		MaxMipLevel(InMaxMipLevel),
		MaxAnisotropy(InMaxAnisotropy)
	{
	}
	RHISamplerFilter Filter = RHISamplerFilter::SF_Point;
	RHISamplerAddress UAddress = RHISamplerAddress::SA_Wrap,
		VAddress = RHISamplerAddress::SA_Wrap,
		WAddress = RHISamplerAddress::SA_Wrap;
	float32 MipBias = 0.0f;
	float32 MinMipLevel = 0.0f;
	float32 MaxMipLevel = FLT_MAX;
	int32	MaxAnisotropy = 0;
};

class RHISampler : public RHIResource
{
public:
	RHISampler()
		:RHIResource(RHIResourceType::RT_SamplerState)
	{
	}
	void AddSamplerInitializer(const RHISamplerInitializer&initializer)
	{
		Initializers.push_back(initializer);
	}
	using SamplersArry = Array<RHISamplerInitializer>;
	SamplersArry Initializers;
	virtual void Destory()override{}
	virtual void Release()override{}
};

class RHIBuffer : public RHIResource
{
public:
	RHIBuffer()
		:RHIResource(RHIResourceType::RT_Buffer)
	{}
	RHIBuffer(RHIBufferUsage usage)
		:Usage(usage),RHIResource(RT_Buffer)
	{

	}
	RHIBuffer(uint32 InSize,const RHIBufferUsage&InUsage,uint32 InStride/*sizeof(T)*/)
		:RHIResource(RHIResourceType::RT_Buffer),
		Size(InSize), Stride(InStride), Usage(InUsage)
	{
	}
	~RHIBuffer()override{}
	uint32 GetSize()const { return Size; }
	uint32 GetStride()const { return Stride; }
	RHIBufferUsage GetUsage()const { return Usage; }
protected:
	uint32 Size{0};// Count
	uint32 Stride{0};
	RHIBufferUsage Usage{ 0 };
	
};
struct DepthStencilVal
{
	float32 Depth = 1.0f;
	uint32 Stencil = 0;
};
struct ClearValueBinding
{
	ClearValueBinding():ClearBindingType(ClearBinding::CB_Color){}
	ClearValueBinding(const Vec4f& color, ClearBinding binding = ClearBinding::CB_Color)
		:ClearBindingType(binding)
	{
		Val.Color = color;
	}
	ClearValueBinding(const DepthStencilVal& dsv,ClearBinding binding = ClearBinding::CB_DepthStencil)
		:ClearBindingType(binding)
	{
		Val.DepthStencil = dsv;
	}
	ClearValueBinding(float32 InDepth, uint32 InStencil, ClearBinding binding = ClearBinding::CB_DepthStencil)
		:ClearBindingType(binding)
	{
		Val.DepthStencil = { InDepth,InStencil };
	}
	const Vec4f& GetColor() { 
		IfFailedLogTextErrorAssert(ClearBindingType == ClearBinding::CB_Color, "Get invaild clear binding type!")
		return Val.Color;
	}
	const DepthStencilVal& GetDSV()
	{
		IfFailedLogTextErrorAssert(ClearBindingType == ClearBinding::CB_DepthStencil, "Get invaild clear binding type!");
		return Val.DepthStencil;
	}
	union ClearValType
	{
		Vec4f Color{};
		DepthStencilVal DepthStencil;
	}Val;

	ClearBinding ClearBindingType = ClearBinding::CB_Color;

};

struct RHITextureDesc
{
	RHITextureDesc(){}
	// Texture width and height;
	Vec2Int Size;
	Format TextureFormat = F_Unknow;
	// if UAVFormat is unknow,UAVFormat use as TextureFormat
	Format UAVFormat = F_Unknow;
	TextureCreateFlags Flags;//
	//size of elements in texture
	uint32 Arraysize = 1;
	// if texture is 3d,else Depth = 1
	int32 Depth = 1;

	uint8 NumMips = 1;

	uint8 NumSamplers : 5;

	ClearValueBinding CVB;

	Vec4f GetClearColor() {
		return CVB.GetColor();
	}
	float32 GetDepth() {
		return CVB.GetDSV().Depth;
	}
	uint32 GetStencil() {
		return CVB.GetDSV().Stencil;
	}


	static RHITextureDesc RenderTargetTexture(int32 width =1920,int32 height = 1080)
	{
		RHITextureDesc desc;
		desc.TextureFormat = F_R8G8B8A8_Unorm;
		desc.Size = { width,height };
		desc.CVB.ClearBindingType = ClearBinding::CB_Color;
		return desc;
	}
	static RHITextureDesc RenderTargetDepthStencil(int32 width = 1920,int32 height = 1080)
	{
		RHITextureDesc desc;
		desc.TextureFormat = F_D24_Unorm_S8_Uint;
		desc.Size = { width,height };
		desc.CVB.ClearBindingType = ClearBinding::CB_DepthStencil;
		desc.CVB.Val.DepthStencil = { 1.0f,0 };
		return desc;
	}
};

class RHITexture : public RHIResource
{
public:
	RHITexture(const RHITextureDesc& InDesc)
		:RHIResource(RHIResourceType::RT_Texture2D)
	{
		desc = InDesc;
	}
	RHITexture()
		:RHIResource(RHIResourceType::RT_Texture2D)
	{
		
	}

	virtual void Release()override{}
	virtual void Destory()override{}

	void SetDesc(const RHITextureDesc& idesc)
	{
		desc = idesc;
	}
	RHITextureDesc& GetDesc() { return desc; }

	Vec2Int GetSize() { return desc.Size; }
	Vec3Int GetSizeXYZ() { return { desc.Size.x,desc.Size.y,desc.Depth }; }
	int32 GetSizeX() { return desc.Size.x; }
	int32 GetSizeY() { return desc.Size.y; }

	Vec4f ClearColorValue() { return desc.GetClearColor(); }
	float32 ClearDepthValue() { return desc.GetDepth(); }
	uint32 ClearStencilValue() { return desc.GetStencil(); }

	bool isMultiplySampler() { return desc.NumSamplers > 1; }

	void SetClearColorValue(Vec4f color)
	{
		desc.CVB.ClearBindingType = ClearBinding::CB_Color;
		desc.CVB.Val.Color = color;
	}

	void SetClearDepthStencilValue(DepthStencilVal val)
	{
		desc.CVB.ClearBindingType = ClearBinding::CB_DepthStencil;
		desc.CVB.Val.DepthStencil = val;
	}
	void SetClearDepthStencilValue(float32 depth, uint32 stencil)
	{
		desc.CVB.ClearBindingType = ClearBinding::CB_DepthStencil;
		desc.CVB.Val.DepthStencil = { depth,stencil };
	}
	// dimension?

private:
	RHITextureDesc desc;
};

class RHIVertexBuffer : public RHIBuffer
{
public:
	RHIVertexBuffer()
		:RHIBuffer(RHIBufferUsage::VertexBuffer)
	{}
	RHIVertexBuffer(int32 InVertexCount, int32 InStride, int32 InOffset = 0)
		:RHIBuffer(InVertexCount, RHIBufferUsage::VertexBuffer, InStride),
		Offset(InOffset)
	{

	}

	virtual void Release()override{}
	virtual void Destory()override{}

	int32 GetVertexCount() { return Size; }
	int32 GetVertexOffset() { return Offset; }
	int32 GetVertexStride() { return Stride; }
	void Init(int32 InVertexCount, int32 InStride, int32 InOffset = 0)
	{
		Size = InVertexCount;
		Stride = InStride;
		Offset = InOffset;
	}
private:
	int32 Offset;
};
class RHIIndexBuffer : public RHIBuffer
{
public:
	using index_type = uint16;
	RHIIndexBuffer()
	: RHIBuffer(0,RHIBufferUsage::IndexBuffer,sizeof(index_type))
	{}
	RHIIndexBuffer(int32 InIndexCount)
		:RHIBuffer(InIndexCount,RHIBufferUsage::IndexBuffer,sizeof(index_type))
	{}
	index_type GetIndexCount() { return IndexCount; }
	void Init(index_type InIndexCount)
	{
		Size = InIndexCount*sizeof(index_type);
		IndexCount = InIndexCount;
	}
	index_type IndexCount;
	virtual void Release()override{}
	virtual void Destory()override{}
};
class RHIShaderUniformBuffer : public RHIBuffer
{
public:
	RHIShaderUniformBuffer(int32 InBufferCount/*nums*/)
		:RHIBuffer(InBufferCount, RHIBufferUsage::UniformBuffer, 1)
	{}
	
};
class RHIShader : public RHIResource
{
public:
	RHIShader()
		:RHIResource(RHIResourceType::RT_Shader){}
	Array<ShaderDesc>ShaderDescs;
	void AddShaderDesc(const ShaderDesc& desc)
	{
		ShaderDescs.push_back(desc);
	}
};

class GraphicsPipeLineInitializer
{
public:
	GraphicsPipeLineInitializer(JString shader,ShaderInputLayOut inputlayout)
	{
		ShaderName = shader;
		ShaderLayout = inputlayout;
	}
	GraphicsPipeLineInitializer(
		const JString& shader,
		const ShaderInputLayOut&Layout,
		const RasterizerState&InRasterizerState,
		const BlendState&InBlendState,
		const uint32& SampleMask,
		PrimitiveTopologyType InTopologyType,
		const uint32& InNumRenderTargets,
		Format InRTVFormat,
		const SampleDesc& InsampleDesc,
		Format InDSVFormat,
		const DepthStencilDesc&InDSDesc)
		:
		ShaderName(shader),ShaderLayout(Layout),RasterizerState(InRasterizerState),
		BlendStateDesc(InBlendState),Samplemask(SampleMask),
		TopologyType(InTopologyType),NumRenderTargets(InNumRenderTargets),
		RtvFormat(InRTVFormat),Sampledesc(InsampleDesc),
		DsvFormat(InDSVFormat),DSDesc(InDSDesc)
	{

	}
	JString ShaderName;
	ShaderInputLayOut ShaderLayout;
	RasterizerState RasterizerState;
	BlendState BlendStateDesc;
	uint32 Samplemask = UINT_MAX;
	PrimitiveTopologyType TopologyType = PTT_Triangle;
	uint32 NumRenderTargets;
	Format RtvFormat;
	SampleDesc Sampledesc;
	Format DsvFormat;
	DepthStencilDesc DSDesc;
};


class RHIGraphicsPipeLineState : public RHIResource
{
public:
	RHIGraphicsPipeLineState(const GraphicsPipeLineInitializer& initializer,
		const JString& pipeline_name,const JString& root_sig_name)
		:RHIResource(RHIResourceType::RT_GraphicsPipeLineState),
		Initializer(initializer),name(pipeline_name),RootSigName(root_sig_name)
	{}
	GraphicsPipeLineInitializer Initializer;
	JString name;
	JString RootSigName;// 声明该渲染管线所需要的根签名的名字 用于在无序表中查询
	void Destory()override{}
	void Release()override{}
};

class RHIRootSignature : public RHIResource
{
public:
	RHIRootSignature()
		:RHIResource(RT_RootSinature)
	{

	}
	JString name;
};

class RHIRenderTarget : public RHIResource
{
public:
	RHIRenderTarget()
		:RHIResource(RT_RenderTarget)
	{}
};

#endif // !RHI_RESOURCE

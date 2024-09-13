#pragma once
#ifndef RHICOMMON_H
#define RHICOMMON_H
#include"RHI_Definitions.h"

class BlendDesc;
class ShaderDesc
{
public:
	JString FileName;
	ShaderUsage Usage;
};
class ShaderInputLayOutPart
{
public:
	JString ParamterName;// 如果是在Opengl模式下 无变量名内容
	uint32 SemanticIndex = 0;
	Format Format = Format::F_R32G32B32_Float;
	uint32 InputSlot = 0;
	uint32 AlignedByteOffset = 0;
	uint32 InstanceStepData = 0;
};
class ShaderInputLayOut
{
public:
	std::vector<ShaderInputLayOutPart> layout;
};
class RasterizerState
{
public:
	FillMode Fillmode = FillMode::FM_Default;
	CullMode Cullmode = CullMode::CM_Default;
	bool FrontCounterClockWise = false;
	int32 DepthBias = 0;
	float32 DepthBiasClamp = 0.0f;
	float32 SlopeScaledDepthBias = 0.0f;
	bool DepthClipEnable = true;
	bool MultiSampleEnable = false;
	bool AntialiasedLineEnable = false;
	uint32 ForceSampleCount = 0;
};
class BlendDesc
{
public:
	bool BlendEnable = false;
	Blend SrcBlend = Blend::B_One;
	Blend DestBlend = Blend::B_Zero;
	BlendOp BlendOperator = BlendOp::BO_Add;
	Blend SrcBlendAlpha = Blend::B_One;
	Blend DestBlendAlpha = Blend::B_One;
	BlendOp BlendAlphaOperator = BlendOp::BO_Add;
	uint8 RenderTargetWriteMask = WCE_All;
};
class BlendState
{
public:
	bool AlphaToCoverageEnable;
	bool IndependentBlendEnable;
	BlendDesc Desc;
};

class DepthStencilOpDesc
{
public:
	StencilOP StencilFailOp;
	StencilOP StencilDepthFailOp;
	StencilOP StencilPassOp;
	ComparisonFunc StencilFunc;
};
class DepthStencilDesc
{
public:
	
	bool DepthEnable=true;
	DepthMask DepthWriteMask = DepthMask::DM_All;
	ComparisonFunc DepthFunc = ComparisonFunc ::COMP_Less;
	bool StencilEnable = false;
	uint8	StencilReadMask = 0xff;
	uint8 StencilWriteMask = 0xff;
	DepthStencilOpDesc FrontFaceDesc=
	{SO_Keep,SO_Keep,SO_Keep,COMP_Always};
	DepthStencilOpDesc BackFaceDesc=
	{ SO_Keep,SO_Keep,SO_Keep,COMP_Always };

};
class SampleDesc
{
public:
	uint32 Count =1;
	uint32 Quality = 0;
};
class ShaderUniformInfo
{
public:
	uint32 ConstantsBufferCount = 0;
	uint32 TextureBufferCount = 0;
	uint32 SamplerCount = 0;
};
#endif // !RHICOMMON_H

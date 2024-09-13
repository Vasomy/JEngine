#include "RenderSystem.h"
#include"GlobalSystemContext.h"
void RenderSystem::Init(RHIInterfaceType type)
{
	rhi = std::make_shared<D3D12RHIDynamic>();
	rhi->Init();

	/// 以下内容应当放在 一个 renderpass 中，其固有的着色器，根签名以及图形渲染管线
	rhi->BeginRecordCommandList();
	JString shaderName = "SimpleShader";
	JString pipelineName = "SwapToScreen";
	JString rootSigName = "SwapToScreen";
	rhi->LoadShader("source/shaders/SimpleShader.hlsl", shaderName, VertexShader | PixelShader);
	ShaderInputLayOut inputLayout;
	inputLayout.layout = {
		{"POSITION",0,F_R32G32B32_Float,0,0,0},
		{"NORMAL",0,F_R32G32B32_Float,0,12,0},
		{"TEXCOORD",0,F_R32G32_Float,0,24,0}
	};
	RHISampler samplers;
	ShaderUniformInfo uniformInfo;
	uniformInfo.ConstantsBufferCount = 1;
	rhi->CreateRootSignature(samplers, uniformInfo, rootSigName);

	GraphicsPipeLineInitializer initializer(
		shaderName,
		inputLayout,
		RasterizerState(),
		BlendState(),
		UINT_MAX,
		PTT_Triangle,
		1u,
		F_R8G8B8A8_Unorm,
		SampleDesc(),
		F_D24_Unorm_S8_Uint,
		DepthStencilDesc()
	);
	rhi->CreatePipeLineState(
		RHIGraphicsPipeLineState(initializer, pipelineName, rootSigName),
		pipelineName
	);
	rhi->CloseCommandList();
	rhi->ExecuteCommandList();
	///
	mGlobalSystemContext.mWindowSystem->IsRenderSytemRun = true;
}

void RenderSystem::Tick(float32 dt)
{
	
	rhi->SetGraphicsPipeLine("SwapToScreen");
	rhi->SetRootSignature("SwapToScreen");
	rhi->SetRenderTarget("SwapToScreen");
	rhi->SwapBackBufferToScreen();
	rhi->ResetRenderState();
}

void RenderSystem::ChangeRHIType()
{
	if (RenderHardwareInterfaceType != rhi->type)
	{

	}
	else
	{
		return;
	}
}

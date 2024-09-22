#include "RenderSystem.h"
#include"GlobalSystemContext.h"
#include"../render/Tools/GeometryGenerator.h"

Vec4 color = { 1,0,0,0 };
RawMesh mesh;

void RenderSystem::Init(RHIInterfaceType type)
{
	rhi = std::make_shared<D3D12RHIDynamic>();
	rhi->Init();

	/// 以下内容应当放在 一个 renderpass 中，其固有的着色器，根签名以及图形渲染管线
	
	//rhi->
	__RHIDebug();
	///
	mGlobalSystemContext.mWindowSystem->IsRenderSytemRun = true;

	UIContext = std::make_shared<UIContextImGui>();
	UIContext->Init();
}

void RenderSystem::Tick(float32 dt)
{

	rhi->SetGraphicsPipeLine("MainScene");
	rhi->SetRootSignature("MainScene");
	rhi->SetRenderTarget("MainScene");
	rhi->PrepareForRender();
	rhi->SubmitRender();
	rhi->SwapBackBufferToScreen();
	rhi->ResetRenderState();
	// close
}

void RenderSystem::__RHIDebug()
{
	rhi->BeginRecordCommandList();
	JString shaderName = "SimpleShader";
	JString pipelineName = "SwapToScreen";
	JString rootSigName = "SwapToScreen";
	JString renderTargetNameMainScene = "MainScene";
	JString RootSigNameMainScene = "MainScene";
	JString PipelineNameMainScene = "MainScene";

	rhi->LoadShader("source/shaders/SimpleShader.hlsl", shaderName, VertexShader | PixelShader);
	RHITextureDesc rtvDesc = RHITextureDesc::RenderTargetTexture();
	RHITextureDesc dsvDesc = RHITextureDesc::RenderTargetDepthStencil();

	rhi->CreateRenderTarget(renderTargetNameMainScene, nullptr, rtvDesc, dsvDesc, 1);

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
	/**/rhi->CreateRootSignature(samplers, uniformInfo, RootSigNameMainScene);

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
	rhi->CreatePipeLineState(
		RHIGraphicsPipeLineState(initializer, pipelineName, rootSigName),
		PipelineNameMainScene
	);

	GeometryGenerator geo;
	auto data = geo.CreateBox(1, 1, 1, 1);
	std::vector<Vertex> vbv(data.Vertices.size());
	for (int i = 0; i < vbv.size(); ++i)
	{
		Vertex v;
		v.Position = data.Vertices[i].Position;
		v.Normal = data.Vertices[i].Normal;
		v.Texcoord = data.Vertices[i].TexC;
		vbv[i] = v;
	}

	rhi->CreateUniformBuffer(&mesh.UniformBuffer);
	rhi->AddUniformBufferPartView(mesh.UniformBuffer, (void*)color.data(), sizeof(float32), 4, SUT_ConstantsBuffer, false);

	rhi->CreateVertexBufferAndInitialize(&mesh.VertexBuffer, vbv.data(), (uint32)sizeof(Vertex) * (uint32)vbv.size(), (uint32)sizeof(Vertex), 0u);

	rhi->CreateIndexBufferAndInitialize(&mesh.IndexBuffer, data.GetIndices16().data(), (uint32)data.Indices32.size());

	rhi->SubmitRenderData(mesh.VertexBuffer, mesh.IndexBuffer, mesh.UniformBuffer, PT_Triangle);

	rhi->CloseCommandList();
	rhi->ExecuteCommandList();
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

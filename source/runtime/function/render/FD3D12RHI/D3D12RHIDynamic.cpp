#include "D3D12RHIDynamic.h"
#include<array>
#include"D3D12Util.h"
#include"../../../core/exception/ExceptionMarco.h"
#include"d3dx12.h"
#include<d3d12.h>
#include"../../system/GlobalSystemContext.h"
#include"tools/DDSTextureLoader.h"
#include<DirectXColors.h>
#define ReleaseCom(X) {if(X){X->Release();X=0;}}

void D3D12RHIDynamic::Init()
{
	InitDirectX12();
	OfflineDescriptorHeapManager.Init(DxDevice.Get());
}

void D3D12RHIDynamic::CreateVertexBuffer(RHIVertexBuffer* VertexBuffer,
	uint32 InitDataSize,
	uint32 Stride,
	uint32 offset)
{
	D3D12RHIVertexBuffer* D3dVertexBuffer = ResourceCast(VertexBuffer);
	D3dVertexBuffer->Init(InitDataSize, Stride, offset);
}

void D3D12RHIDynamic::CreateVertexBufferAndInitialize(RHIVertexBuffer* VertexBuffer,
	void** InitData,
	uint32 InitDataSize,
	uint32 Stride,
	uint32 offset)
{
	D3D12RHIVertexBuffer* D3dVertexBuffer = ResourceCast(VertexBuffer);
	D3dVertexBuffer->Init(InitDataSize, Stride, offset);
	D3dVertexBuffer->Resource.Resource=D3DUtil::CreateDefaultBuffer(DxDevice.Get(), DxCMDList.Get(), 
		InitData, InitDataSize, D3dVertexBuffer->Resource.UploadHeap);
	D3dVertexBuffer->ResourceAddress = D3dVertexBuffer->Resource.Resource->GetGPUVirtualAddress();

}

void D3D12RHIDynamic::CreateIndexBuffer(RHIIndexBuffer* IndexBuffer, uint32 IndexCount)
{
	D3D12RHIIndexBuffer* D3dIndexBuffer = ResourceCast(IndexBuffer);
	D3dIndexBuffer->Init(IndexCount);
}

void D3D12RHIDynamic::CreateIndexBufferAndInitialize(RHIIndexBuffer* IndexBuffer, void** Indexdata, uint32 IndexCount)
{
	D3D12RHIIndexBuffer* D3dIndexBuffer = ResourceCast(IndexBuffer);
	D3dIndexBuffer->Init(IndexCount);
	D3dIndexBuffer->Resource.Resource = D3DUtil::CreateDefaultBuffer(DxDevice.Get(),
		DxCMDList.Get(), Indexdata, IndexCount, D3dIndexBuffer->Resource.UploadHeap);
	D3dIndexBuffer->ResourceAddress = D3dIndexBuffer->Resource.Resource->GetGPUVirtualAddress();
}

void D3D12RHIDynamic::CreateRootSignature(RHISampler& Samplers, ShaderUniformInfo UniformInfo,JString rootSigName)
{
	std::vector<CD3DX12_STATIC_SAMPLER_DESC>SamplerDesc(Samplers.Initializers.size());
	for (int32 SamplerIndex = 0; SamplerIndex < Samplers.Initializers.size(); ++SamplerIndex)
	{
		auto& desc = Samplers.Initializers[SamplerIndex];
		uint32 ShaderRegister = SamplerIndex;
		D3D12_FILTER filter;
		D3D12_TEXTURE_ADDRESS_MODE addressMode;
		CD3DX12_STATIC_SAMPLER_DESC ret_desc;
		if (desc.Filter == SF_Point)
			filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		else if (desc.Filter == SF_Linear)
			filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		else if(desc.Filter == SF_Anisotropic)
			filter = D3D12_FILTER_ANISOTROPIC;

		ret_desc.Filter = filter;
		if (desc.UAddress == RHISamplerAddress::SA_Clamp)
			addressMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		else if (desc.UAddress == RHISamplerAddress::SA_Wrap)
			addressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		ret_desc.AddressU = addressMode;
		
		if (desc.VAddress == RHISamplerAddress::SA_Clamp)
			addressMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		else if (desc.VAddress == RHISamplerAddress::SA_Wrap)
			addressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		ret_desc.AddressV = addressMode;

		if (desc.WAddress == RHISamplerAddress::SA_Clamp)
			addressMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		else if (desc.WAddress == RHISamplerAddress::SA_Wrap)
			addressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		ret_desc.AddressW = addressMode;

		ret_desc.MipLODBias = desc.MipBias;
		if (filter == D3D12_FILTER_ANISOTROPIC)
			ret_desc.MaxAnisotropy = 8;
		else
			ret_desc.MaxAnisotropy = 0;
		ret_desc.ShaderRegister = ShaderRegister;
		SamplerDesc.push_back(ret_desc);
	}
	CD3DX12_ROOT_PARAMETER* slotParameter = new CD3DX12_ROOT_PARAMETER[UniformInfo.TextureBufferCount + UniformInfo.ConstantsBufferCount];
	int32 InitIndex = 0;
	if (UniformInfo.TextureBufferCount) {
		CD3DX12_DESCRIPTOR_RANGE texTable;
		texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, UniformInfo.TextureBufferCount, 0);
		slotParameter[InitIndex++].InitAsDescriptorTable(UniformInfo.TextureBufferCount, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	}
	for (uint32 i = 0; i < UniformInfo.ConstantsBufferCount; ++i)
	{
		slotParameter[InitIndex++].InitAsConstantBufferView(i);
	}
	int32 slotCounts = UniformInfo.TextureBufferCount + UniformInfo.ConstantsBufferCount;
	int32 samplerCounts = (int32)Samplers.Initializers.size();
	CD3DX12_STATIC_SAMPLER_DESC* samplerDescs = nullptr;
	if (samplerCounts) {
		samplerDescs = new CD3DX12_STATIC_SAMPLER_DESC[samplerCounts];
		for (int32 i = 0; i < samplerCounts; ++i)
		{
			SamplerDesc[i] = GetSamplerDesc(Samplers.Initializers[i], i);
		}
	}
	CD3DX12_ROOT_SIGNATURE_DESC	rootSigDesc(slotCounts, slotParameter, (UINT)samplerCounts, samplerDescs,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob>SerializedRootSig = nullptr;
	ComPtr<ID3DBlob>ErrorMsgBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, SerializedRootSig.GetAddressOf(), ErrorMsgBlob.GetAddressOf());
	if (ErrorMsgBlob != nullptr)
	{
		LogTextError((char*)ErrorMsgBlob->GetBufferPointer());
	}
	IfFailedHResultFile(hr);
	IfFailedHResultFile(
		DxDevice->CreateRootSignature(0,SerializedRootSig->GetBufferPointer(),
			SerializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(mRootSignatures[rootSigName].GetAddressOf()))
	);
}

void D3D12RHIDynamic::CreatePipeLineState(const RHIGraphicsPipeLineState& PipeLineState, JString PipeLineName)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc;
	ZeroMemory(&pipelineDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	auto& initializer = PipeLineState.Initializer;
	std::vector<D3D12_INPUT_ELEMENT_DESC>layouts;
	for(int32 i =0;i<initializer.ShaderLayout.layout.size();++i)
		layouts.push_back(GetShaderInputLayout(initializer.ShaderLayout.layout[i]));
	for (int i = 0; i < layouts.size(); ++i)
	{
		std::cout << layouts[i].SemanticName << '\n';
	}
	pipelineDesc.InputLayout = { layouts.data(),(UINT)layouts.size() };
	pipelineDesc.pRootSignature = mRootSignatures[PipeLineState.RootSigName].Get();
	pipelineDesc.BlendState = GetBlendDesc(initializer.BlendStateDesc);
	pipelineDesc.RasterizerState = GetRasterizerDesc(initializer.RasterizerState);
	pipelineDesc.PrimitiveTopologyType = static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(initializer.TopologyType);
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.SampleMask = UINT_MAX;
	pipelineDesc.RTVFormats[0] = CastFormat(initializer.RtvFormat);
	pipelineDesc.SampleDesc.Count = PipeLineState.Initializer.Sampledesc.Count;
	pipelineDesc.SampleDesc.Quality = PipeLineState.Initializer.Sampledesc.Quality;
	pipelineDesc.DepthStencilState = CastDepthStencilDesc(initializer.DSDesc);
	pipelineDesc.DSVFormat = CastFormat(initializer.DsvFormat);
	D3D12RHIShader* shaders = mShaders[initializer.ShaderName].get();
	pipelineDesc.VS = 
	{
		reinterpret_cast<BYTE*>(shaders->Shaders["VS"]->GetBufferPointer()),
		shaders->Shaders["VS"]->GetBufferSize()
	};
	pipelineDesc.PS =
	{
		reinterpret_cast<BYTE*>(shaders->Shaders["PS"]->GetBufferPointer()),
		shaders->Shaders["PS"]->GetBufferSize()
	};
	HRESULT hr = DxDevice->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[PipeLineName]));
	IfFailedHResultFile(hr);

}

void D3D12RHIDynamic::PushSamplerInitializer(RHISampler& sampler, const RHISamplerInitializer& samplerInitializer)
{
	sampler.Initializers.push_back(samplerInitializer);
}

void D3D12RHIDynamic::LoadShader(JString file_name, JString shader_name, int32 flags)
{
	std::shared_ptr<D3D12RHIShader>shader = std::make_shared<D3D12RHIShader>();
	if ((flags & VertexShader))
	{
		ShaderDesc desc;
		desc.FileName = file_name;
		desc.Usage = SU_Vertex;
		shader->AddShaderDesc(desc);
	}
	if ((flags & PixelShader))
	{
		ShaderDesc desc;
		desc.FileName = file_name;
		desc.Usage = SU_Pixel;
		shader->AddShaderDesc(desc);
	}
	CompileShader(shader.get());
	mShaders[shader_name] = std::move(shader);
}

void D3D12RHIDynamic::CompileShader(RHIShader* Shader)
{
	D3D12RHIShader* D3dShader = ResourceCast(Shader);
	for (uint8 i = 0; i < D3dShader->ShaderDescs.size(); ++i)
	{
		auto& shaderDesc = D3dShader->ShaderDescs[i];
		switch (D3dShader->ShaderDescs[i].Usage)
		{
		case ShaderUsage::SU_Unknow:

			break;
		case ShaderUsage::SU_Vertex:
			D3dShader->Shaders["VS"] = D3DUtil::CompileShader(
				to_wide_string(shaderDesc.FileName),nullptr,"VS","vs_5_0"
			);
			break;
		case ShaderUsage::SU_Pixel:
			D3dShader->Shaders["PS"] = D3DUtil::CompileShader(
				to_wide_string(shaderDesc.FileName),nullptr,"PS","ps_5_0"
			);
			break;
		default:
			break;
		}
	}
}

void D3D12RHIDynamic::LoadTexture(JString file_name, RHITexture* out_texture, RHITextureDesc* desc,TextureLoadType type)
{
	std::wstring FileName = ConvertToWideString(file_name);
	D3D12RHITexture* texture = ResourceCast(out_texture);
	switch (type)
	{
	case TEXTURE_DDS:
		IfFailedHResultFile(
			CreateDDSTextureFromFile12(DxDevice.Get(), DxCMDList.Get(),
				FileName.c_str(), texture->Resource.Resource, texture->Resource.UploadHeap)
		);
		break;
	case TEXTURE_JPG:
		break;
	case TEXTURE_PNG:
		break;
	default:
		break;
	}

}

void D3D12RHIDynamic::CreateRenderTarget(JString render_target_name,RHIRenderTarget* render_target,
	RHITextureDesc rtv_texture, RHITextureDesc dsv_texture,int32 rt_nums)/////////////////////// 目前只能自定义创建一个只有一个RtvTex的RenderTarget
{
	D3D12RHIRenderTarget* D3dRT = ResourceCast(render_target);
	rt_nums = 1;
	for(int i =0;i<1;++i)//***************************************//
		CreateRenderTargetTexture(&D3dRT->RTVTexture[i], rtv_texture, &D3dRT->DSVTexture, dsv_texture);
	D3dRT->CreateHeaps(DxDevice.Get(), rt_nums);

	mRenderTargets[render_target_name] = std::shared_ptr<D3D12RHIRenderTarget>(D3dRT);
}

void D3D12RHIDynamic::CreateRenderTargetTexture(RHITexture* out_rtv_texture, RHITextureDesc rtv_texture_desc, RHITexture* out_dsv_texture, RHITextureDesc dsv_texture_desc)
{
	D3D12RHITexture* dsvTex = ResourceCast(out_dsv_texture);
	D3D12RHITexture* rtvTex = ResourceCast(out_rtv_texture);

	auto ds_desc = CD3DX12_RESOURCE_DESC::Tex2D(
		CastFormat(dsv_texture_desc.TextureFormat),// D24_Unorm_S8_Uint
		dsv_texture_desc.Size.x,
		dsv_texture_desc.Size.y,
		dsv_texture_desc.Arraysize,
		dsv_texture_desc.NumMips,
		1,
		0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	auto heap_properties_default = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	IfFailedHResultFile(DxDevice->CreateCommittedResource(
		&heap_properties_default,
		D3D12_HEAP_FLAG_NONE,
		&ds_desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optClear,
		IID_PPV_ARGS(dsvTex->Resource.Resource.GetAddressOf())
	));

	D3D12_CLEAR_VALUE stClear;
	auto color = Colors::LightSteelBlue;
	memcpy(&stClear.Color, &color, 4 * sizeof(float));
	stClear.Format = mBackBufferFormat;

	D3D12_RESOURCE_DESC stTexDepthsDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		CastFormat(rtv_texture_desc.TextureFormat)
		, rtv_texture_desc.Size.x
		, rtv_texture_desc.Size.y
		, rtv_texture_desc.Arraysize
		, rtv_texture_desc.NumMips
		, 1
		, 0
		, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET		//注意这个标志是纹理可以作为渲染目标使用的关键
	);

	IfFailedHResultFile(DxDevice->CreateCommittedResource(
		&heap_properties_default
		, D3D12_HEAP_FLAG_NONE
		, &stTexDepthsDesc
		, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE |
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_GENERIC_READ
		, &stClear
		, IID_PPV_ARGS(rtvTex->Resource.Resource.GetAddressOf())
	));

	OfflineDescriptorHeapManager.AllocHeapForFrameTexture(rtvTex);

}

void D3D12RHIDynamic::SetRenderTarget(JString render_target)
{
	RenderSettingContext.RenderTargetName = render_target;
}

void D3D12RHIDynamic::SetGraphicsPipeLine(JString pipeline_name)
{
	RenderSettingContext.GraphicsPipelineName = pipeline_name;
}

void D3D12RHIDynamic::SetRootSignature(JString root_sig_name)
{
	RenderSettingContext.RootSigName = root_sig_name;
}

void D3D12RHIDynamic::OnResizeWindow(int32 width,int32 height)
{
	assert(DxDevice);
	assert(DxSwapChain);
	assert(DxCMDAlloc);

	// Flush before changing any resources.
	FlushCommand();

	IfFailedHResultFile(DxCMDList->Reset(DxCMDAlloc.Get(), nullptr));
	auto& rt = mRenderTargets["SwapToScreen"];
	// Release the previous resources we will be recreating.
	for (int i = 0; i < SwapChainBufferCount; ++i)
		rt->RTVTexture[i].Resource.Resource.Reset();
	rt->DSVTexture.Resource.Resource.Reset();

	// Resize the swap chain.
	IfFailedHResultFile(DxSwapChain->ResizeBuffers(
		SwapChainBufferCount,
		width, height,
		mBackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	iCurrentBackBuffer = 0;


	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(rt->RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SwapChainBufferCount; i++)
	{
		IfFailedHResultFile(DxSwapChain->GetBuffer(i, IID_PPV_ARGS(&rt->RTVTexture[i].Resource.Resource)));
		DxDevice->CreateRenderTargetView(rt->RTVTexture[i].Resource.Resource.Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}
	auto td = rt->RTVTexture[0].Resource.Resource->GetDesc();

	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;

	// Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from 
	// the depth buffer.  Therefore, because we need to create two views to the same resource:
	//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
	//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
	// we need to create the depth buffer resource with a typeless format.  
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	IfFailedHResultFile(DxDevice->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(rt->DSVTexture.Resource.Resource.GetAddressOf())));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = mDepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	DxDevice->CreateDepthStencilView(rt->DSVTexture.Resource.Resource.Get(), &dsvDesc, rt->DSVHeap->GetCPUDescriptorHandleForHeapStart());
	// Transition the resource from its initial state to be used as a depth buffer.
	auto resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(rt->DSVTexture.Resource.Resource.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	DxCMDList->ResourceBarrier(1, &resource_barrier);

	// Execute the resize commands.
	IfFailedHResultFile(DxCMDList->Close());
	ID3D12CommandList* cmdsLists[] = { DxCMDList.Get() };
	DxCMDQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until resize is complete.
	FlushCommand();

	// Update the viewport transform to cover the client area.
	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	mViewPort.Width = static_cast<float>(width);
	mViewPort.Height = static_cast<float>(height);
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;

	mRect = { 0, 0, width, height };
	SwapChainBufferWidth = width;
	SwapChainBufferHeight = height;
}

void D3D12RHIDynamic::SetRootSignature_v(ID3D12RootSignature* root_sig)// travial
{
	
}

void D3D12RHIDynamic::SetGraphicsPipeLine_v(ID3D12PipelineState* pipeline_state)// travial
{
}

void D3D12RHIDynamic::ResetRenderState()
{
	IfFailedHResultFile(DxCMDList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { DxCMDList.Get() };
	DxCMDQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	// Swap the back and front buffers
	IfFailedHResultFile(DxSwapChain->Present(0, 0));
	//mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
	++mCurrentFence;

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	DxCMDQueue->Signal(DxFence.Get(), mCurrentFence);
	iCurrentBackBuffer++;
	iCurrentBackBuffer %= SwapChainBufferCount;
	FlushCommand();
}

void D3D12RHIDynamic::SubmitRenderData(
	RHIVertexBuffer* InVertexBuffer,
	RHIIndexBuffer* InIndexBuffer,
	RHIShaderUniformBuffer*InShaderUniformBuffer)
{
	D3D12RHIVertexBuffer* VertexBuffer = ResourceCast(InVertexBuffer);
	D3D12RHIIndexBuffer* IndexBuffer = ResourceCast(InIndexBuffer);
	D3D12RHIShaderUniformBuffer* ShaderUniformBuffer = ResourceCast(InShaderUniformBuffer);
	D3D12_VERTEX_BUFFER_VIEW vbv = { VertexBuffer->ResourceAddress,
		VertexBuffer->GetSize(),
		VertexBuffer->GetStride() };
	D3D12_INDEX_BUFFER_VIEW ibv = {
		IndexBuffer->ResourceAddress,
		IndexBuffer->GetSize(),
		DXGI_FORMAT_R32_UINT
	};
	VertexBufferViews.push_back(vbv);
	IndexBufferViews.push_back(ibv);
	D3D12RHIShaderUniformBuffer InstShaderUniformBuffer = *ShaderUniformBuffer;
	UniformBufferViews.push_back(InstShaderUniformBuffer);
}
#include<DirectXColors.h>
void D3D12RHIDynamic::SubmitRender() // should Render To Texture
{

	RenderSettingContext.Check();
	assert(mPSOs.count(RenderSettingContext.GraphicsPipelineName));
	auto& pipeline = mPSOs.at(RenderSettingContext.GraphicsPipelineName);

	assert(mRootSignatures.count(RenderSettingContext.RootSigName));
	auto& rootsig = mRootSignatures.at(RenderSettingContext.RootSigName);

	assert(mRenderTargets.count(RenderSettingContext.RenderTargetName));
	auto& rendertarget = mRenderTargets.at(RenderSettingContext.RenderTargetName);

	auto cmdAlloc = DxCMDAlloc;

	IfFailedHResultFile(cmdAlloc->Reset());

	IfFailedHResultFile(DxCMDList->Reset(cmdAlloc.Get(), pipeline.Get()));

	auto RTVHandle = rendertarget->RTVHeap->GetCPUDescriptorHandleForHeapStart();
	auto DSVHandle = rendertarget->DSVHeap->GetCPUDescriptorHandleForHeapStart();
	// 或许可以记录 一下 Resource的State 以扩展泛型
	auto before_barrier = CD3DX12_RESOURCE_BARRIER::Transition(rendertarget->RTVTexture[0].Resource.Resource.Get(),
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);
	DxCMDList->ResourceBarrier(1, &before_barrier);
	Vec4f clear_color = rendertarget->RTVTexture[0].ClearColorValue();
	DxCMDList->OMSetRenderTargets(1, &RTVHandle, true, &DSVHandle);
	DxCMDList->ClearRenderTargetView(RTVHandle, Colors::Red, 0, nullptr);
	DxCMDList->ClearDepthStencilView(DSVHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 
		rendertarget->DSVTexture.ClearDepthValue(),
		rendertarget->DSVTexture.ClearStencilValue()
		, 0, nullptr);
	DxCMDList->RSSetScissorRects(1, &mRect);
	DxCMDList->RSSetViewports(1, &mViewPort);
	ID3D12DescriptorHeap* descHeap[] = { OfflineDescriptorHeapManager.OffScreenRenderDescHeap.Get() };
	DxCMDList->SetDescriptorHeaps(_countof(descHeap), descHeap);
	DxCMDList->SetGraphicsRootSignature(rootsig.Get());

	DrawInstance();
	auto after_barrier = CD3DX12_RESOURCE_BARRIER::Transition(rendertarget->RTVTexture[0].Resource.Resource.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_GENERIC_READ);
	DxCMDList->ResourceBarrier(1, &after_barrier);

	
}

void D3D12RHIDynamic::DrawInstance()
{
	
}

void D3D12RHIDynamic::SwapBackBufferToScreen()
{
	auto cmdListAlloc = DxCMDAlloc;
	auto rt = mRenderTargets["SwapToScreen"];

	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	IfFailedHResultFile(cmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	IfFailedHResultFile(DxCMDList->Reset(cmdListAlloc.Get(), mPSOs["SwapToScreen"].Get()));

	DxCMDList->RSSetViewports(1, &mViewPort);
	DxCMDList->RSSetScissorRects(1, &mRect);

	// Indicate a state transition on the resource usage.
	auto b_barrier = CD3DX12_RESOURCE_BARRIER::Transition(rt->RTVTexture[iCurrentBackBuffer].Resource.Resource.Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	DxCMDList->ResourceBarrier(1, &b_barrier);

	// Clear the back buffer and depth buffer.
	auto rtvHandle = rt->RTVHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += iCurrentBackBuffer * mRtvDescriptorSize;
	auto dsvHandle = rt->DSVHeap->GetCPUDescriptorHandleForHeapStart();
	DxCMDList->ClearRenderTargetView(rtvHandle, Colors::LightSteelBlue, 0, nullptr);
	DxCMDList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
	// Specify the buffers we are going to render to.
	DxCMDList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

	ID3D12DescriptorHeap* descriptorHeaps[] = { OfflineDescriptorHeapManager.OffScreenRenderDescHeap.Get() };
	DxCMDList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	DxCMDList->SetGraphicsRootSignature(mRootSignatures["SwapToScreen"].Get());


	// Indicate a state transition on the resource usage.
	auto a_barrier = CD3DX12_RESOURCE_BARRIER::Transition(rt->RTVTexture[iCurrentBackBuffer].Resource.Resource.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	DxCMDList->ResourceBarrier(1, &a_barrier);

	//std::cout << iCurrentBackBuffer << '\n';

	IfFailedHResultFile(DxDevice->GetDeviceRemovedReason());
}

void D3D12RHIDynamic::BeginRecordCommandList()
{
	DxCMDList->Reset(DxCMDAlloc.Get(), nullptr);
}

void D3D12RHIDynamic::CloseCommandList()
{
	DxCMDList->Close();
}

void D3D12RHIDynamic::ExecuteCommandList()
{
	ID3D12CommandList* cmdsLists[] = { DxCMDList.Get() };
	DxCMDQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommand();
}

D3D12_FILTER D3D12RHIDynamic::GetDX12Filter(RHISamplerFilter Filter)
{
	D3D12_FILTER ret;
	switch (Filter)
	{
	case SF_Point:
		ret = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		break;
	case SF_Linear:
		ret = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		break;
	case SF_Anisotropic:
		ret = D3D12_FILTER_ANISOTROPIC;
		break;
	default:
		ret = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		break;
	}
	return ret;
}

D3D12_TEXTURE_ADDRESS_MODE D3D12RHIDynamic::GetDX12AddressMode(RHISamplerAddress Address)
{
	D3D12_TEXTURE_ADDRESS_MODE ret;
	switch (Address)
	{
	case RHISamplerAddress::SA_Clamp:
		ret = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		break;
	case RHISamplerAddress::SA_Wrap:
		ret = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		break;
	default:
		ret = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		break;
	}
	return ret;
}


CD3DX12_STATIC_SAMPLER_DESC D3D12RHIDynamic::GetSamplerDesc(RHISamplerInitializer& sampler,int32 registerSlot)
{
	auto& initializer = sampler;
	auto uAddress = GetDX12AddressMode(sampler.UAddress);
	auto vAddress = GetDX12AddressMode(sampler.VAddress);
	auto wAddress = GetDX12AddressMode(sampler.WAddress);
	auto Filter = GetDX12Filter(sampler.Filter);
	CD3DX12_STATIC_SAMPLER_DESC desc
	{
		static_cast<uint32>(registerSlot),
		Filter,
		uAddress,
		vAddress,
		wAddress,
		sampler.MipBias,
		(UINT)sampler.MaxAnisotropy
	};
	return desc;
}

D3D12_INPUT_ELEMENT_DESC D3D12RHIDynamic::GetShaderInputLayout(const ShaderInputLayOutPart& layout)
{
	D3D12_INPUT_ELEMENT_DESC ret = {
		layout.ParamterName.data(),layout.SemanticIndex,CastFormat(layout.Format),
		layout.InputSlot,layout.AlignedByteOffset,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,layout.InstanceStepData
	};
	return ret;
}

D3D12_RASTERIZER_DESC D3D12RHIDynamic::GetRasterizerDesc(const RasterizerState& state)
{
	D3D12_RASTERIZER_DESC ret{
		static_cast<D3D12_FILL_MODE>(state.Fillmode),
		static_cast<D3D12_CULL_MODE>(state.Cullmode),
		state.FrontCounterClockWise,
		state.DepthBias,
		state.DepthBiasClamp,
		state.SlopeScaledDepthBias,
		state.DepthClipEnable,
		state.MultiSampleEnable,
		state.AntialiasedLineEnable,
		state.ForceSampleCount,
		static_cast<D3D12_CONSERVATIVE_RASTERIZATION_MODE>(0)
	};
	return ret;
}

D3D12_BLEND_DESC D3D12RHIDynamic::GetBlendDesc(const BlendState& desc)
{
	D3D12_BLEND_DESC ret{
		desc.AlphaToCoverageEnable,
		desc.IndependentBlendEnable
	};
	auto& Desc = desc.Desc;
	D3D12_RENDER_TARGET_BLEND_DESC defaultRTBlendDesc
	{
		desc.Desc.BlendEnable,FALSE,
		CastBlendEnum(Desc.SrcBlend),CastBlendEnum(Desc.DestBlend),CastBlendOp(Desc.BlendOperator),
		CastBlendEnum(Desc.SrcBlendAlpha),CastBlendEnum(Desc.DestBlendAlpha),CastBlendOp(Desc.BlendAlphaOperator),
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL,
	};
	for (int i = 0; i < 8; ++i)
	{
		ret.RenderTarget[i] = defaultRTBlendDesc;
	}
	return ret;
}

D3D12_BLEND D3D12RHIDynamic::CastBlendEnum(Blend target)
{
	return static_cast<D3D12_BLEND>(target);
}

D3D12_BLEND_OP D3D12RHIDynamic::CastBlendOp(BlendOp op)
{
	return static_cast<D3D12_BLEND_OP>(op);
}

DXGI_FORMAT D3D12RHIDynamic::CastFormat(Format format)
{
	DXGI_FORMAT ret = static_cast<DXGI_FORMAT>(format);
	return ret;
}

D3D12_DEPTH_STENCIL_DESC D3D12RHIDynamic::CastDepthStencilDesc(const DepthStencilDesc& DSD)
{
	D3D12_DEPTH_STENCIL_DESC ret;
	ret.DepthEnable = DSD.DepthEnable;
	ret.DepthWriteMask = static_cast<D3D12_DEPTH_WRITE_MASK>(DSD.DepthWriteMask);
	ret.DepthFunc = CastComparison(DSD.DepthFunc);
	ret.StencilEnable = DSD.StencilEnable;
	ret.StencilReadMask = DSD.StencilReadMask;
	ret.StencilWriteMask = DSD.StencilWriteMask;
	ret.FrontFace = CastDSDesc(DSD.FrontFaceDesc);
	ret.BackFace = CastDSDesc(DSD.BackFaceDesc);
	return ret;
}

D3D12_STENCIL_OP D3D12RHIDynamic::CastStencilOp(StencilOP op)
{
	return static_cast<D3D12_STENCIL_OP>(op);
}

D3D12_DEPTH_STENCILOP_DESC D3D12RHIDynamic::CastDSDesc(const DepthStencilOpDesc& DSOD)
{
	D3D12_DEPTH_STENCILOP_DESC ret;
	ret.StencilDepthFailOp = CastStencilOp(DSOD.StencilDepthFailOp);
	ret.StencilFailOp = CastStencilOp(DSOD.StencilFailOp);
	ret.StencilPassOp = CastStencilOp(DSOD.StencilPassOp);
	ret.StencilFunc = CastComparison(DSOD.StencilFunc);
	return ret;
}

D3D12_COMPARISON_FUNC D3D12RHIDynamic::CastComparison(ComparisonFunc cf)
{
	return static_cast<D3D12_COMPARISON_FUNC>(cf);
}

void D3D12RHIDynamic::AllocTextureHeap(int32 texture_nums)
{
}
#define _DEBUG
bool D3D12RHIDynamic::InitDirectX12()
{
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
	{
		ComPtr<ID3D12Debug> debugController;
		IfFailedHResultFile(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}
#endif

	IfFailedHResultFile(
		CreateDXGIFactory1(IID_PPV_ARGS(&DxFactory))
	);

	HRESULT hardware = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&DxDevice)
	);
	if (FAILED(hardware))
	{
		ComPtr<IDXGIAdapter>pEnumAdapter;
		IfFailedHResultFile(
			DxFactory->EnumWarpAdapter(IID_PPV_ARGS(&pEnumAdapter))
		);

		IfFailedHResultFile(
			D3D12CreateDevice(
				pEnumAdapter.Get(),
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&DxDevice)
			)
		);
	}
	IfFailedHResultFile(
		DxDevice->CreateFence(0,D3D12_FENCE_FLAG_NONE,IID_PPV_ARGS(&DxFence))
	);

	mRtvDescriptorSize = DxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = DxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = DxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	IfFailedHResultFile(DxDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");


#ifdef _DEBUG
	LogAdapters();
#endif // _DEBUG

	CreateCommandObjects();
	CreateSwapChain();
	InitSwapToScreenRTVandDSV();
	return false;
}

bool D3D12RHIDynamic::InitSwapToScreenRTVandDSV()
{
	auto rt = std::make_shared<D3D12RHIRenderTarget>();
	rt->RTName = "SwapToScreen";

	rt->CreateSwapToScreenHeaps(DxDevice.Get(), DxSwapChain.Get());
	
	mRenderTargets[rt->RTName] = std::move(rt);
	OnResizeWindow(SwapChainBufferWidth, SwapChainBufferHeight);
	return true;
}

void D3D12RHIDynamic::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (DxFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		LogAdapterOutputs(adapterList[i]);
		ReleaseCom(adapterList[i]);
	}
}

void D3D12RHIDynamic::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	IfFailedHResultFile(DxDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&DxCMDQueue)));

	IfFailedHResultFile(DxDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(DxCMDAlloc.GetAddressOf())));

	IfFailedHResultFile(DxDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		DxCMDAlloc.Get(), // Associated command allocator
		nullptr,                   // Initial PipelineStateObject
		IID_PPV_ARGS(DxCMDList.GetAddressOf())));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	DxCMDList->Close();
}

void D3D12RHIDynamic::CreateSwapChain()
{
	// Release the previous swapchain we will be recreating.
	DxSwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = SwapChainBufferWidth;
	sd.BufferDesc.Height = SwapChainBufferHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SwapChainBufferCount;
	sd.OutputWindow = mGlobalSystemContext.mWindowSystem->MainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	IfFailedHResultFile(DxFactory->CreateSwapChain(
		DxCMDQueue.Get(),
		&sd,
		DxSwapChain.GetAddressOf()));
}


void D3D12RHIDynamic::FlushCommand()
{
	mCurrentFence++;

	IfFailedHResultFile(DxCMDQueue->Signal(DxFence.Get(),mCurrentFence));

	if (DxFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, 0, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		IfFailedHResultFile(DxFence->SetEventOnCompletion(mCurrentFence, eventHandle));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void D3D12RHIDynamic::LogAdapterOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, mBackBufferFormat);

		ReleaseCom(output);

		++i;
	}
}

void D3D12RHIDynamic::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}

template<typename RHIType>
inline typename D3D12TypeTraits<RHIType>::traits_type* D3D12RHIDynamic::ResourceCast(RHIType* rhi_resource)
{
	return static_cast<typename D3D12TypeTraits<RHIType>::traits_type*>(rhi_resource);
}
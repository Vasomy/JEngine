#pragma once
#ifndef D3D12RESOURCE_H
#define D3D12RESOURCE_H
#include<runtime/core/JCores.h>
#include<Windows.h>
#include<direct.h>
#include<d3d12.h>
#include<windowsx.h>
#include<dxgi1_6.h>
#include"d3dx12.h"
class JD3D12Resource
{
public:
	ComPtr<ID3D12Resource> Resource;
	ComPtr<ID3D12Resource> UploadHeap;

	D3D12_GPU_VIRTUAL_ADDRESS Address;
	void Release()
	{}
};
class JD3D12RenderSettingContext
{
public:
	JString RootSigName = InvaildName;
	JString GraphicsPipelineName = InvaildName;
	JString RenderTargetName = InvaildName;
	inline static const JString InvaildName = "__NULL__PTR!";
	void Reset()
	{
		RootSigName = InvaildName;
		GraphicsPipelineName = InvaildName;
		RenderTargetName = InvaildName;
	}
	void Check()
	{
		assert((RootSigName != InvaildName));
		assert(GraphicsPipelineName!=InvaildName);
		assert(RenderTargetName!=InvaildName);
	}
};
#endif // !D3D12RESOURCE_H

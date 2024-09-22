#pragma once
#ifndef OFFLINEDESCMANAGER_H
#define OFFLINEDESCMANAGER_H
#include"D3D12Util.h"
#include"d3dx12.h"
#include"D3D12AllocHeap.h"
#include"D3D12RHIResource.h"

struct DescriptorHandleRange
{
	uint64 Start;
	uint64 End;
	bool isUsed = false;
};

struct D3D12HeapNode
{
	ComPtr<ID3D12DescriptorHeap>Heap;
	using DescriptorHandleList = std::vector<DescriptorHandleRange>;
	DescriptorHandleList List;
};


class D3D12OfflineDescriptorManager
{
public:
	
	void Init(ID3D12Device*InDevice);
	using HeapNodeList = std::vector<D3D12HeapNode>;
	HeapNodeList HeapMap;
	void AllocEmptyHeap();
	void AllocHeapForTexture(D3D12RHITexture* texture);//return the texture offset to heap start
	void AllocHeapForFrameTexture(D3D12RHITexture* texture);

	ComPtr<ID3D12DescriptorHeap>TextureDescHeap;
	int32 CurrentTextureNum = 0;
	ComPtr<ID3D12DescriptorHeap>OffScreenRenderDescHeap; // 用于存储离屏渲染后的材质
	int32 CurrentOffScreenTextureNum = 1;// 0为ui的渲染材质，例如 imgui中可以把 ui的渲染结果放在第一位
	int32 NumsOffScreenTexture = 16;

	D3D12_DESCRIPTOR_HEAP_DESC TextureHeapDesc;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc = {};

	UINT mCbvSrvUavDescriptorSize = 0;
	uint32 NumsDescriptors = 1001;

	ID3D12Device* DxDevice;

	D3D12_DESCRIPTOR_HEAP_DESC CreateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32 NumDescriptors);

};
#endif // !OFFLINEDESCMANAGER_H

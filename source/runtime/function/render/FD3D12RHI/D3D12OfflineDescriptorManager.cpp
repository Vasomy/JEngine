#include "D3D12OfflineDescriptorManager.h"

void D3D12OfflineDescriptorManager::Init(ID3D12Device* InDevice)
{
	DxDevice = InDevice;
	TextureHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	TextureHeapDesc.NumDescriptors = NumsDescriptors;
	TextureHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	TextureHeapDesc.NodeMask = 0;
	IfFailedHResultFile(DxDevice->CreateDescriptorHeap(&TextureHeapDesc, IID_PPV_ARGS(&TextureDescHeap)));

	D3D12_DESCRIPTOR_HEAP_DESC offscreentexturedesc = TextureHeapDesc;
	offscreentexturedesc.NumDescriptors = NumsOffScreenTexture;
	IfFailedHResultFile(DxDevice->CreateDescriptorHeap(&offscreentexturedesc, IID_PPV_ARGS(&OffScreenRenderDescHeap)));
}

void D3D12OfflineDescriptorManager::AllocEmptyHeap()
{
	ComPtr<ID3D12DescriptorHeap>Heap;
	IfFailedHResultFile(DxDevice->CreateDescriptorHeap(&TextureHeapDesc, IID_PPV_ARGS(&Heap)));

	D3D12_CPU_DESCRIPTOR_HANDLE handle = Heap->GetCPUDescriptorHandleForHeapStart();

	D3D12HeapNode node;
	node.Heap = Heap;
	node.List.push_back({ handle.ptr,handle.ptr + (size_t)TextureHeapDesc.NumDescriptors * mCbvSrvUavDescriptorSize });// in common , NumDescriptors = 1

	HeapMap.push_back(node);

}

void D3D12OfflineDescriptorManager::AllocHeapForTexture(D3D12RHITexture* texture)
{
	/*int32 EntryIndex = -1;
	for (int32 i = 0; i < HeapMap.size(); ++i)
	{
		if (HeapMap[i].List.front().isUsed == false)
		{
			EntryIndex = i;
			break;
		}
	}
		
	if (EntryIndex == -1)
	{
		AllocEmptyHeap();
		EntryIndex = HeapMap.size();
	}
	
	auto& heap = HeapMap[EntryIndex].Heap;
	auto& range = HeapMap[EntryIndex].List.front().Start;
	HeapMap[EntryIndex].List.front().isUsed = true;
	DxDevice->CreateShaderResourceView(texture->Resource.Resource.Get(), &srvDesc, {range});
	texture->Resource.Address = range;*/
	uint32 offset = CurrentTextureNum;
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(TextureDescHeap->GetCPUDescriptorHandleForHeapStart());
	cpuHandle.Offset(CurrentTextureNum, mCbvSrvUavDescriptorSize);
	DxDevice->CreateShaderResourceView(texture->Resource.Resource.Get(), &srvDesc, cpuHandle);
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(TextureDescHeap->GetGPUDescriptorHandleForHeapStart());
	gpuHandle.Offset(CurrentTextureNum, mCbvSrvUavDescriptorSize);
	texture->Resource.Address = gpuHandle.ptr;
	CurrentTextureNum++;

}

void D3D12OfflineDescriptorManager::AllocHeapForFrameTexture(D3D12RHITexture* texture)
{
	IfFailedLogTextErrorAssert(CurrentOffScreenTextureNum < NumsOffScreenTexture, "Nums of ScreenTexture over max OffScreenTexture Nums!");
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(OffScreenRenderDescHeap->GetCPUDescriptorHandleForHeapStart());
	cpuHandle.Offset(CurrentOffScreenTextureNum, mCbvSrvUavDescriptorSize);
	DxDevice->CreateShaderResourceView(texture->Resource.Resource.Get(), &srvDesc, cpuHandle);
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(OffScreenRenderDescHeap->GetGPUDescriptorHandleForHeapStart());
	gpuHandle.Offset(CurrentOffScreenTextureNum, mCbvSrvUavDescriptorSize);
	texture->Resource.Address = gpuHandle.ptr;
	CurrentOffScreenTextureNum++;
}

D3D12_DESCRIPTOR_HEAP_DESC D3D12OfflineDescriptorManager::CreateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32 NumDescriptors)
{
	D3D12_DESCRIPTOR_HEAP_DESC Desc = {};
	Desc.Type = type;
	Desc.NumDescriptors = NumDescriptors;
	Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // This heap will not be bound to the shader
	Desc.NodeMask = 0;

	return Desc;
}

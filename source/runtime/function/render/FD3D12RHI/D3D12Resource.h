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
#include"D3D12Util.h"
class JD3D12Resource
{
public:
	ComPtr<ID3D12Resource> Resource = nullptr;
	ComPtr<ID3D12Resource> UploadHeap = nullptr;

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
class D3D12UniformSlotDesc
{
public:
	uint32 Slot = 0;
	uint64 Address;
};
class D3D12DrawInstancedView
{
public:
	D3D12_VERTEX_BUFFER_VIEW vbv{};
	D3D12_INDEX_BUFFER_VIEW ibv{};
	uint32 IndexCount = 0;
	uint32 StartIndexLocation = 0;
	uint32 BaseVertexLocation = 0;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//uniform desc...
	std::vector<D3D12UniformSlotDesc>ConstantsUniformDesc;
	std::vector<D3D12UniformSlotDesc>TexturesUniformDesc;
};



class UploadBuffer
{
public:
	UploadBuffer(ID3D12Device* device, UINT elementCount,UINT size, bool isConstantBuffer) :
		mIsConstantBuffer(isConstantBuffer)
	{
		mElementByteSize = size;

		// Constant buffer elements need to be multiples of 256 bytes.
		// This is because the hardware can only view constant data 
		// at m*256 byte offsets and of n*256 byte lengths. 
		// typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
		// UINT64 OffsetInBytes; // multiple of 256
		// UINT   SizeInBytes;   // multiple of 256
		// } D3D12_CONSTANT_BUFFER_VIEW_DESC;
		if (isConstantBuffer)
			mElementByteSize = D3DUtil::CalcConstantBufferByteSize(size);

		auto HeapTypeUpload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount);

		IfFailedHResultFile(device->CreateCommittedResource(
			&HeapTypeUpload,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBuffer)));

		IfFailedHResultFile(mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));

		// We do not need to unmap until we are done with the resource.  However, we must not write to
		// the resource while it is in use by the GPU (so we must use synchronization techniques).
	}

	UploadBuffer(const UploadBuffer& rhs) = delete;
	UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
	~UploadBuffer()
	{
		if (mUploadBuffer != nullptr)
			mUploadBuffer->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	ID3D12Resource* Resource()const
	{
		return mUploadBuffer.Get();
	}

	void CopyData(int elementIndex, const void* data)
	{
		memcpy(&mMappedData[elementIndex * mElementByteSize], data, mElementByteSize);
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
	BYTE* mMappedData = nullptr;

	UINT mElementByteSize = 0;
	bool mIsConstantBuffer = false;
};
#endif // !D3D12RESOURCE_H

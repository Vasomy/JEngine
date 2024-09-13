#pragma once
#include"d3dx12.h"
#include"D3D12Util.h"

#include<d3d12.h>

#ifndef HEAPALLOC_H
#define HEAPALLOC_H

class D3D12HeapAlloc
{
public:
	ComPtr<ID3D12DescriptorHeap>SrvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>UavDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>RtvDescriptorHeap;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	uint32 MaxCbvSrvUavDescriptorCount = 1000;

		
};

#endif
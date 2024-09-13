#pragma once
#ifndef D3D12TYPETRAITS_H
#define D3D12TYPETRAITS_H
#include"D3D12RHIResource.h"

template<typename T>
struct D3D12TypeTraits;
template<>
struct D3D12TypeTraits<RHIBuffer>
{
	using traits_type = D3D12RHIBuffer;
};
template<>
struct D3D12TypeTraits<RHIVertexBuffer>
{
	using traits_type = D3D12RHIVertexBuffer;
};
template<>
struct D3D12TypeTraits<RHIIndexBuffer>
{
	using traits_type = D3D12RHIIndexBuffer;
};
template<>
struct D3D12TypeTraits<RHIShaderUniformBuffer>
{
	using traits_type = D3D12RHIShaderUniformBuffer;
};
template<>
struct D3D12TypeTraits<RHIShader>
{
	using traits_type = D3D12RHIShader;
};
template <>
struct D3D12TypeTraits<RHIRenderTarget>
{
	using traits_type = D3D12RHIRenderTarget;
};
template<>
struct D3D12TypeTraits<RHITexture>
{
	using traits_type = D3D12RHITexture;
};
#endif // !D3D12TYPETRAITS_H

#pragma once
#ifndef D3D12UTIL_H
#define D3D12UTIL_H

#include"D3D12Resource.h"
#include"d3dx12.h"
#include<D3Dcompiler.h>
#include<iostream>

class D3DUtil {
public:
    static Microsoft::WRL::ComPtr<ID3D12Resource>CreateDefaultBuffer(ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList,
        const void* initData,
        UINT64 byteSize,
        ComPtr<ID3D12Resource>& uploadBuffer);
    inline static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
        const std::wstring& filename,
        const D3D_SHADER_MACRO* defines,
        const std::string& entrypoint,
        const std::string& target
    )
    {
        UINT compileFlags = 0;//Debug flag
#if defined(DEBUG)||defined(_DEBUG)
        compileFlags = D3DCOMPILE_DEBUG || D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // 
        HRESULT hr = S_OK;
        ComPtr<ID3DBlob>byteCode = nullptr;
        ComPtr<ID3DBlob>errors;
        hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);
        

        if (errors != nullptr)
        {
            OutputDebugStringA((char*)errors->GetBufferPointer());
            std::cout << (char*)errors->GetBufferPointer() << '\n';
        }

        IfFailedHResultFile(hr);

        /// example
        ///
        /// ComPtr<ID3DBlob>mVsByteCode = nullptr;
        /// ComPtr<ID3DBlob>mPsByteCode = nullptr;
        /// mVsByteCode = d3dUtil::CompileShader(path,nullptr,"VS","vs_5_0");
        /// mVsByteCode = d3dUtil::CompileShader(path,nullptr,"PS","ps_5_0");
        ///

        return byteCode;
    }
};
#endif

#pragma once
#ifndef EXCEPTION_MARCO_H
#define EXCEPTION_MARCO_H

#include"../log/LogText.h"
#include<comdef.h>
#include<Windows.h>
class DxException
{
public:
    DxException() = default;
    DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
        ErrorCode(hr),
        FunctionName(functionName),
        Filename(filename),
        LineNumber(lineNumber)
    {
    }

    std::wstring ToString()const
    {
        // Get the string description of the error code.
        _com_error err(ErrorCode);
        std::wstring msg = err.ErrorMessage();

        return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + msg;
    }

    HRESULT ErrorCode = S_OK;
    std::wstring FunctionName;
    std::wstring Filename;
    int LineNumber = -1;
};
#define IfFailedAssert(x) {if(!(x)){assert(0);}}
#define IfFailedThrowFile(x){if(!(x)){LogTextError(__FILE__);assert(0);}}
#define IfFailedHResultFile(x) \
{                                                                      \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = to_wide_string(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#define __LogError__ {ShowError();}
#define IfFailedLogTextErrorAssert(x,content) {if(!(x)){LogTextError(content);__LogError__;assert(0);}}
#define IfFailedLogTextWarningAssert(x,content) {if(!(x)){LogTextWarning(content);assert(0);}}

#endif // !EXCEPTION_MARCO_H

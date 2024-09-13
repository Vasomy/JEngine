#include "SmartPtr.h"
#include<Windows.h>

inline std::wstring ConvertToWideString(std::string& input)
{
    wchar_t* ret;
    ret = (wchar_t*)malloc(input.size() * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, input.c_str(), -1,ret, 0);
    std::wstring tar(ret);
    free(ret);
    return tar;
}

#pragma once
#ifndef SMART_PTR_H
#define SMART_PTR_H

#include<memory>
#include<wrl.h>
using namespace Microsoft;
using namespace WRL;

#include <locale>
#include <codecvt>
#include<string>

extern inline std::wstring ConvertToWideString(std::string& input);

template<typename T>
void ResetPtr(T** ptr)
{
    if (*ptr)
        delete *ptr;
    *ptr = new T();
}
template<typename ptr_type,typename ...Args>
void ResetPtr(ptr_type** ptr, Args... args)
{
    if (*ptr)
        delete ptr;
    *ptr = new ptr_type(args...);
}

#endif // !SMART_PTR_H


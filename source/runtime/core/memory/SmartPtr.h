#pragma once
#include<memory>
#include<wrl.h>
using namespace Microsoft;
using namespace WRL;

#include <locale>
#include <codecvt>
#include<string>

extern inline std::wstring ConvertToWideString(std::string& input);

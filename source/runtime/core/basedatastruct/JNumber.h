#pragma once
#ifndef JNUMBER_H
#define JNUMBER_H


class NumType
{
public:
#if 1
	using d_uint8 = unsigned char;
	using d_uint16 = unsigned short;
	using d_uint32 = unsigned int;
	using d_uint64 = unsigned long long;
	using d_int8 = char;
	using d_int16 = short;
	using d_int32 = int;
	using d_int64 = long long;

	using d_float32 = float;
	using d_float64 = double;

#endif // 1

};

using uint8 = NumType::d_uint8;
using uint16 = NumType::d_uint16;
using uint32 = NumType::d_uint32;
using uint64 = NumType::d_uint64;
using int8 = NumType::d_int8;
using int16 = NumType::d_int16;
using int32 = NumType::d_int32;
using int64 = NumType::d_int64;

using float32 = NumType::d_float32;
using float64 = NumType::d_float64;

class JNumber
{
public:
	inline static float32 PI = 3.1415926f;
	inline static float32 PI_Div2 = 3.1415926f / 2.0f;
};

#endif // !JNUMBER_H

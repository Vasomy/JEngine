#pragma once
#ifndef MATH_H
#define MATH_H

#include<cmath>
#include"basedatastruct/JNumber.h"

template<typename T>
inline T Clamp(const T& _src, const T& _min, const T& _max)
{
	return _src < _min ? _min : (_src > _max ? _max : _src);
}
inline float32 Radius(float32 degree)//角度转弧度
{
	return JNumber::PI * (degree / 180.0f);
}
inline float32 Degree(float32 radius)
{
	return radius / JNumber::PI * 180.0f;
}
inline float32 RadiusFromXY(float32 x, float32 y)
{
	float32 radius;
	if (x >= 0.0f)
	{
		radius = atanf(y / x);
		if (y <= 0)
			radius += 2.0f * JNumber::PI;
	}
	else
		radius = atanf(y / x) + JNumber::PI;
	return radius;
}
inline float32 DegreeFromXY(float32 x, float32 y)
{
	return Degree(RadiusFromXY(x, y));
}
inline float32 Sin(float32 radius)//传入弧度制
{
	return sinf(radius);
}
inline float32 Cos(float32 radius)
{
	return cosf(radius);
}
inline float32 ASin(float32 radius)
{
	return asinf(radius);
}
inline float32 ACos(float32 radius)
{
	return acosf(radius);
}
#endif // !MATH_H

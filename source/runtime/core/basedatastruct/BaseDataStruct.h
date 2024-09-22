#pragma once
#ifndef BASEDATASTRUCT_H
#define BASEDATASTRUCT_H

#ifndef Template_M
#define Template_M
#define Template(T) template<typename T>
#endif // !Template_M

#include"JNumber.h"

#ifndef JMatrix
#include<DirectXMath.h>
using namespace DirectX;
#endif



template<typename T,int size>
struct Vector
{
};

template<typename T>
struct Vector<T,4>
{
	using val_type = T;
	using vec_type = Vector<T, 4>;

	Vector<T, 4>(T x, T y, T z, T w)
		:x(x),y(y),z(z),w(w)
	{

	}
	Vector<T, 4>()
		:x(0),y(0),z(0),w(0)
	{

	}

	T& operator[](int idx)
	{
		switch (idx)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			break;
		}
	}
	friend vec_type operator+(const vec_type& left, const vec_type& right)
	{
		return { left.x + right.x
			,left.y + right.y
			,left.z + right.z
			,left.w + right.w };
	}
	friend vec_type operator*(const val_type left, const vec_type& right)
	{
		return {
			right.x*left,
			right.y*left,
			right.z*left,
			right.w*left
		};
	}
	friend vec_type operator*(const vec_type& left, const val_type right)
	{
		return{
			left.x*right,
			left.y*right,
			left.z*right,
			left.w*right
		};
	}
	template<typename _Ty>
	vec_type& operator*(_Ty right)
	{
		this->x*=right;
		this->y*=right;
		this->z*=right;
		this->w*=right;
		return *this;
	}
	vec_type& operator-(vec_type& right)
	{
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;
		this->w -= right.w;
		return *this;
	}
	vec_type& operator*(vec_type& right)
	{
		this->x *= right.x;
		this->y *= right.y;
		this->z *= right.z;
		this->w *= right.w;
		return *this;
	}
	template<typename T>
	friend vec_type& operator*(T right, vec_type& vec)
	{
		return vec * right;
	}
	const T* data() { return(T*)this; }

	T x, y, z, w;
};

template<typename T>
struct Vector<T, 3>
{
	using val_type = T;
	using vec_type = Vector<T, 3>;
	Vector<T, 3>(T x, T y, T z)
		:x(x), y(y), z(z)
	{

	}
	Vector<T, 3>()
		: x(0), y(0), z(0)
	{

	}

	T& operator[](int idx)
	{
		switch (idx)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			break;
		}
	}
	friend vec_type operator+(const vec_type& left, const vec_type& right)
	{
		return { left.x + right.x
			,left.y + right.y
			,left.z + right.z };
	}
	friend vec_type operator*(const val_type left, const vec_type& right)
	{
		return {
			right.x * left,
			right.y * left,
			right.z * left
		};
	}
	friend vec_type operator*(const vec_type& left, const val_type right)
	{
		return{
			left.x * right,
			left.y * right,
			left.z * right
		};
	}
	vec_type& operator-(vec_type& right)
	{
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;
		return *this;
	}
	vec_type& operator*(vec_type& right)
	{
		this->x *= right.x;
		this->y *= right.y;
		this->z *= right.z;
		return *this;
	}
	template<typename _Ty>
	vec_type& operator*(_Ty right)
	{
		this->x *= right;
		this->y *= right;
		this->z *= right;
		return *this;
	}
	template<typename T>
	friend vec_type& operator*(T right,vec_type&vec)
	{
		return vec*right;
	}
	T* data() {return(T*)this; }
	
	T x, y, z;
};

template<typename T>
struct Vector<T, 2>
{
	using val_type = T;
	using vec_type = Vector<T, 2>;
	Vector<T, 2>(T x, T y)
		:x(x), y(y)
	{

	}
	Vector<T, 2>()
		: x(0), y(0)
	{

	}
	friend vec_type operator*(const val_type left, const vec_type& right)
	{
		return {
			right.x * left,
			right.y * left
		};
	}
	friend vec_type operator*(const vec_type& left, const val_type right)
	{
		return{
			left.x * right,
			left.y * right
		};
	}

	T& operator[](int idx)
	{
		switch (idx)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			break;
		}
	}
	friend vec_type operator+(const vec_type& left, const vec_type& right)
	{
		return { left.x + right.x
			,left.y + right.y };
	}
	vec_type& operator-(vec_type& right)
	{
		this->x -= right.x;
		this->y -= right.y;
		return *this;
	}
	vec_type& operator*(vec_type& right)
	{
		this->x *= right.x;
		this->y *= right.y;
		return *this;
	}
	template<typename _Ty>
	vec_type& operator*(_Ty right)
	{
		this->x *= right;
		this->y *= right;
		return *this;
	}
	template<typename T>
	friend vec_type& operator*(T right, vec_type& vec)
	{
		return vec * right;
	}
	T* data() { return(T*)this; }

	T x, y;
};

template<typename T>
struct Vector<T, 1>
{
	using val_type = T;
	using vec_type = Vector<T, 1>;
	Vector<T, 1>(T x)
		:x(x)
	{

	}
	Vector<T, 1>()
		: x(0)
	{

	}

	T& operator[](int idx)
	{
		switch (idx)
		{
		case 0:
			return x;
		default:
			break;
		}
	}
	friend vec_type operator+(const vec_type& left, const vec_type& right)
	{
		return { left.x + right.x};
	}
	vec_type& operator-(vec_type& right)
	{
		this->x -= right.x;
		return *this;
	}
	vec_type& operator*(vec_type& right)
	{
		this->x *= right.x;
		return *this;
	}
	template<typename _Ty>
	vec_type& operator*(_Ty right)
	{
		this->x *= right;
		return *this;
	}
	T* data() { return(T*)this; }

	T x;
};

using Vec4f = Vector<float32, 4>;
using Vec3f = Vector<float32, 3>;
using Vec2f = Vector<float32, 2>;
using Vec1f = Vector<float32, 1>;

using Vec4 = Vec4f;
using Vec3 = Vec3f;
using Vec2 = Vec2f;
using Vec1 = Vec1f;

using Vec4Int = Vector<int32, 4>;
using Vec3Int = Vector<int32, 3>;
using Vec2Int = Vector<int32, 2>;
using Vec1Int = Vector<int32, 1>;

using Vec4UInt = Vector<uint32, 4>;
using Vec3UInt = Vector<uint32, 3>;
using Vec2UInt = Vector<uint32, 2>;
using Vec1UInt = Vector<uint32, 1>;


inline Vec4 __vectorcall Normalize(const Vec4& vec1) noexcept
{
	float32 d = sqrtf(vec1.x * vec1.x + vec1.y * vec1.y + vec1.z * vec1.z + vec1.w * vec1.w);
	return { vec1.x / d,vec1.y / d,vec1.z / d,vec1.w / d };
}

inline Vec3 __vectorcall Normalize(const Vec3& vec1) noexcept
{
	float32 d = sqrtf(vec1.x * vec1.x + vec1.y * vec1.y + vec1.z * vec1.z);
	return { vec1.x / d,vec1.y / d,vec1.z / d };
}

template<typename T>
float32 Dot(const Vector<T,4>&vec1,const Vector<T,4>&vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z+vec1.w*vec2.w;
}

template<typename T>
float32 Dot(const Vector<T, 3>& vec1, const Vector<T, 3>& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

template<typename T>
float32 Dot(const Vector<T, 2>& vec1, const Vector<T, 2>& vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

Vector<float32, 3> Cross(const Vector<float32, 3>& v1, const Vector<float32, 3>& v2);


Vector<float32, 2>Cross(const Vector<float32, 2>& v1, const Vector<float32, 2>& v2);



template<typename T,int size>
struct Matrix
{
	using val_type = T;
	using mat_type = Matrix<T,size>;
};

template<typename T>
struct Matrix<T,4>
{
	using val_type = T;
	using mat_type = Matrix<T,4>;
	T& operator()(uint32 idx1,uint32 idx2)
	{
		return elements[idx1][idx2];
	}
	T operator()(uint32 idx1, uint32 idx2)
	{
		return elements[idx1][idx2];
	}
private:
	T elements[4][4];
};

#ifdef JMatrix
using Matrix4x4f = Matrix<float32,4>;
using Matrix4x4  = Matrix4x4f;
#else
using Matrix4x4  = XMFLOAT4X4;
using VECTOR = XMVECTOR;
#endif



#ifdef JMatrix //my matrix algorith

#else // DirectxMath Matrix algorithm
Matrix4x4 Transpose(const Matrix4x4& tar);
XMMATRIX Transpose(const XMMATRIX& tar);
Matrix4x4 Inverse(const Matrix4x4& tar);
VECTOR Determiant(const Matrix4x4& tar);//
Matrix4x4 MatrixMultiply(const Matrix4x4& m1, const Matrix4x4& m2);
XMMATRIX MatrixMultiply(const XMMATRIX& m1, const XMMATRIX& m2);
Matrix4x4 LookAt(const Vector<float32, 3>& pos, const Vector<float32, 3>& target, const Vector<float32, 3>& up);
Matrix4x4 Perspective(float32 fov, float32 aspect, float32 zNear, float32 zFar);

#endif



#ifdef JString

#else
#include<string>
#include<xstring>
using JString = std::string;
using JStringView = std::string_view;

#include <locale>
#include <codecvt>

// convert string to wstring
std::wstring to_wide_string(const std::string& input);


#endif


#endif // !BASEDATASTRUCT_H

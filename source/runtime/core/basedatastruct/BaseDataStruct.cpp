#include "BaseDataStruct.h"
Vector<float32,3>Cross(const Vector<float32,3>&v1,const Vector<float32,3>&v2)
{
	static Vector<float32, 3>i = { 1.0f,0.0f,0.0f };
	static Vector<float32, 3>j = { 0.0f,1.0f,0.0f };
	static Vector<float32, 3>k = { 0.0f,0.0f,1.0f };
	using vec_type = Vector<float32, 3>;
	return ((v1.y * v2.z - v1.z * v2.y) * i +
		(v1.z * v2.x - v1.x * v2.z) * j +
		(v1.x * v2.y - v1.y * v2.x) * k);
}
Vector<float32, 2>Cross(const Vector<float32, 2>& v1, const Vector<float32, 2>& v2)
{
	static Vector<float32, 2>i = { 1.0f,0.0f };
	static Vector<float32, 2>j = { 0.0f,1.0f };
	return (
		(v1.y - v2.x) * i + (v1.x - v2.y) * j
		);
}



#ifdef JMatrix

#else // !

Matrix4x4 Transpose(const Matrix4x4& tar)
{
	XMMATRIX m = XMLoadFloat4x4(&tar);
	XMMatrixTranspose(m);
	Matrix4x4 res;
	XMStoreFloat4x4(&res, m);
	return res;
}
XMMATRIX Transpose(const XMMATRIX& tar)
{
	return XMMatrixTranspose(tar);
}
Matrix4x4 Inverse(const Matrix4x4& tar)
{
	XMMATRIX src = XMLoadFloat4x4(&tar);
	VECTOR v = Determiant(tar);
	XMMATRIX m = XMMatrixInverse(&v,src);
	m = Transpose(m);
	Matrix4x4 res;
	XMStoreFloat4x4(&res, m);
	return res;
}

VECTOR Determiant(const Matrix4x4& tar)
{
	XMMATRIX m = XMLoadFloat4x4(&tar);
	return XMMatrixDeterminant(m);
}

Matrix4x4 MatrixMultiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	XMMATRIX mat1 = XMLoadFloat4x4(&m1);
	XMMATRIX mat2 = XMLoadFloat4x4(&m2);
	XMMATRIX res_mat = XMMatrixMultiply(mat1, mat2);
	res_mat = Transpose(res_mat);
	Matrix4x4 res;
	XMStoreFloat4x4(&res, res_mat);
	return res;
}

XMMATRIX MatrixMultiply(const XMMATRIX& m1, const XMMATRIX& m2)
{
	return XMMatrixMultiply(m1,m2);
}

Matrix4x4 LookAt(const Vector<float32, 3>& pos, const Vector<float32, 3>& target, const Vector<float32, 3>& up)
{
	XMVECTOR Pos = { pos.x,pos.y,pos.z,1.0f };
	XMVECTOR Target = { target.x,target.y,target.z,0.0f };
	XMVECTOR Up = { up.x,up.y,up.z,0.0f };
	XMMATRIX res = XMMatrixLookAtLH(Pos,Target,Up);
	Matrix4x4 ans;
	XMStoreFloat4x4(&ans, res);
	return ans;
}
Matrix4x4 Perspective(float32 fov, float32 aspect, float32 zNear, float32 zFar)
{
	Matrix4x4 ret;
	XMMATRIX mat = XMMatrixPerspectiveFovLH(fov, aspect, zNear, zFar);
	XMStoreFloat4x4(&ret, mat);
	return ret;
}
#endif

std:: wstring to_wide_string(const std::string& input)
{
	return std::wstring(input.begin(), input.end());
}

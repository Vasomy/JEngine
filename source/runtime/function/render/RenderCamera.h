#pragma once
#include<runtime/core/JCores.h>

class RenderCamera
{
public:
	float32 Pitch;
	float32 Yaw;
	float32 Zoom;
	Vec3f   Pos;
	Vec3f   Front;
	Vec3f	Up = {0.0f,1.0f,0.0f};
	Vec3f   Right;
	float32 MoveSpeed;
	float32 MouseSensitivity;

	float32 Aspect;
	float32 Fov;
	float32 ZNear;
	float32 FNear;

	Matrix4x4 GetView()const {
		return LookAt(Pos, Pos + Front, Up);
	}
	Matrix4x4 GetPerspective()const
	{
		return Perspective(Fov, Aspect, ZNear, FNear);
	}
};
#pragma once
#ifndef RENDERCOMMON_H
#define RENDERCOMMON_H
#include<runtime/core/JCores.h>
#include<runtime/function/render/interface/RHI.h>
struct Vertex
{
	Vec3f Position;
	Vec3f Normal;

	Vec2f Texcoord;
	
};


struct RawMesh
{
	RHIVertexBuffer* VertexBuffer = nullptr;
	uint32 VertexCounts;
	RHIIndexBuffer*  IndexBuffer = nullptr;
	uint32 IndexCounts;

	RHIShaderUniformBuffer* UniformBuffer = nullptr;

};
struct SubRawMesh
{
	uint32 Offset;//offset to begin
	uint32 Count;
};

struct RenderMeshNodePart
{
	uint32 P_id;
	SubRawMesh SubMesh;
};
struct RenderMeshNode
{
	uint32 UObjId;//unique id
	Array<RenderMeshNodePart>Parts;
	void Add(const RenderMeshNodePart desc)
	{
		Parts.push_back(desc);
	}
};

struct AmbientLight
{
	Vec4f AmbientLightStrength = { 0.0f,0.0f,0.0f,1.0f };
};
struct Light
{
	Vec3f Strength = { 0.5f,0.5f,0.5f };
	float32 FallOffStart = 0.0f;
	Vec3f Direction = { 0.0f,0.0f,0.0f };
	float32 FallOffEnd = 10.0f;
	Vec3f Position = { 0.0f,0.0f,0.0f };
	float32 SpotPower = 64.0f;
};
#endif // !RENDERCOMMON_H

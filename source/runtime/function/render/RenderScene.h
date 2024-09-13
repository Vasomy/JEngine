#pragma once
#include"interface/RHI.h"
#include"RenderCommon.h"
#include"RenderCamera.h"
#include"RenderEntity.h"

extern const int32 __max_lights = 16;
struct MainCameraPassConstants
{
	Matrix4x4 View;
	Matrix4x4 Proj;
	Matrix4x4 ViewProj;
	Matrix4x4 InvView;
	Matrix4x4 InvProj;
	Matrix4x4 InvViewProj;
	
	AmbientLight AmbientLights;

	Light lights[__max_lights];
};

class MainCameraRenderScene
{
public:
	void Init();
	void Update();
	void ProcessRenderData();//from RenderEntity to RenderMeshNode and Push in RenderMeshNodeTree for LOD...
	void SubmitRenderDataToRenderPass();// after ProcessRenderData call 
	
	MainCameraPassConstants& GetPassData() { return SceneConstants; }

private:
	MainCameraPassConstants SceneConstants;
	void UpdateMainCamera(const RenderCamera* camera);
	void UpdateVisableObject(const Array<RenderEntity>& entities);


};
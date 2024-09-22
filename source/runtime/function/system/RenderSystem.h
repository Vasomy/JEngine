#pragma once
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include"../render/interface/RHI.h"
#include"../render/FD3D12RHI/D3D12RHI.h"

class RenderSystem
{
public:
	void Init(RHIInterfaceType type);

	void Tick(float32 dt);

public:

	void __RHIDebug();

	void ChangeRHIType();
	std::shared_ptr<RHIDynamic>rhi;
	RHIInterfaceType RenderHardwareInterfaceType;

	std::shared_ptr<UIContextImGui>UIContext;

};

#endif // !RENDERSYSTEM

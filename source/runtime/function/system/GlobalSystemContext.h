#pragma once
#ifndef GLOBALSYSTEMCONTEXT_H
#include"WindowSystem.h"
#include"RenderSystem.h"
#include"../render/uirender/ImGui/ImGuiContext.h"
class GlobalSystemContext
{
public:
	void Init();
	std::shared_ptr<WindowSystem>mWindowSystem;
	std::shared_ptr<RenderSystem>mRenderSystem;

	

	std::shared_ptr<RHIDynamic> GetRHI() { return mRenderSystem->rhi; }
};

extern GlobalSystemContext mGlobalSystemContext;

std::shared_ptr<RHIDynamic> RHI();
#endif // !GLOBALSYSTEMCONTEXT_H

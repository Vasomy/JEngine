#include "GlobalSystemContext.h"

GlobalSystemContext mGlobalSystemContext;


void GlobalSystemContext::Init()
{
	mWindowSystem = std::make_shared<WindowSystem>();
	mWindowSystem->Init();

	mRenderSystem = std::make_shared<RenderSystem>();
	mRenderSystem->Init(RHIInterfaceType::DirectX12);

}

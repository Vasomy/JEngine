#include "Engine.h"
#include"../system/GlobalSystemContext.h"
#include<iostream>
void Engine::Init()
{
	InitSystem();
	
}
void Engine::Start()
{
}
void Engine::Run()
{
	MSG& msg = mGlobalSystemContext.mWindowSystem->msg;
	while (!msg.message !=WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mGlobalSystemContext.mWindowSystem->Tick();
			Tick();
		}
	}
}

void Engine::Tick()
{
	mGlobalSystemContext.mRenderSystem->Tick(0.12f);
}

void Engine::ShutDown()
{
}

void Engine::SetRenderOption(RHIInterfaceType type)
{

	mGlobalSystemContext.mRenderSystem->RenderHardwareInterfaceType = type;
	
}

void Engine::InitSystem()
{
	mGlobalSystemContext.Init();
}

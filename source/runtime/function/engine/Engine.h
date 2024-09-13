#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include"../render/interface/RHI.h"
class Engine
{
public:
	void Init();
	void Start();
	void Run();
	void Tick();
	void ShutDown();

	void SetRenderOption(RHIInterfaceType type);
private:
	void InitSystem();

	RHIInterfaceType mRHIInterfaceType;
};
#endif
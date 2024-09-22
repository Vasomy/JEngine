#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include"../render/interface/RHI.h"
#include"../../../editor/Editor.h"
class Engine
{
public:

	Engine(Editor* bind_editor)
	{
		editor = bind_editor;
	}

	void Init();
	void Start();
	void Run();
	void Tick();
	void ShutDown();

	void SetRenderOption(RHIInterfaceType type);
private:
	void InitSystem();

	Editor* editor = nullptr;

	RHIInterfaceType mRHIInterfaceType;
};
#endif
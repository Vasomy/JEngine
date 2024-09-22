#pragma once
#ifndef EDITOR_H
#define EDITOR_H
#include"EditorUIContext.h"
class Editor
{
public:
	Editor()
	{

	}

	void Init()
	{
	
	}

	void Tick()
	{
		UIContext.Tick();
	}

public:
	EditorUIContext UIContext;
};
#endif // !EDITOR_H

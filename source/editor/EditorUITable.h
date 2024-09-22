#pragma once
#include"../runtime/function/render/uirender/ImGui/ImGuiContext.h"

#ifndef EditorUITable_h
#define EditorUITable_h
class EditorUITable // 像填表一样 设立UI
{
public:
	EditorUITable()
	{
		
	}

	virtual void FillUITable() = 0;

};

class EditorWindowUI : public EditorUITable
{
public:
	EditorWindowUI(){}

	virtual void FillUITable();
};

#endif // !EditorTable_h

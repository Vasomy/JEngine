#pragma once
#ifndef UIContext

enum UIInterface
{
	UI_Unknow,
	UI_ImGui,
};

class UIContextBase
{
public:

	UIContextBase(UIInterface type)
		:InterfaceType(type)
	{}
	virtual ~UIContextBase()
	{

	}

	virtual void Init() = 0;
	virtual void BeginUI() = 0;// ׼��UI �����ģ����ΪIMGUI ����BeginFrame...

	virtual void RenderDrawData() = 0; //Ӧ���� SwapToScreen ��RenderPass �� ��ΪDrawInstanced ����

	virtual void EndUI() = 0;// ����UI������

	UIInterface InterfaceType;
};
#endif // !UIContext

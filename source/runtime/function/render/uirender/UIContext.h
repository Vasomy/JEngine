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
	virtual void BeginUI() = 0;// 准备UI 上下文，如果为IMGUI 就是BeginFrame...

	virtual void RenderDrawData() = 0; //应当在 SwapToScreen 的RenderPass 中 作为DrawInstanced 调用

	virtual void EndUI() = 0;// 结束UI上下文

	UIInterface InterfaceType;
};
#endif // !UIContext

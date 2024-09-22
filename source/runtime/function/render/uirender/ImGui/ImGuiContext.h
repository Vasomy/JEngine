#pragma once
#include"../UIContext.h"

#include"../../FD3D12RHI/D3D12RHI.h"

class UIContextImGui : public UIContextBase
{
public:
	UIContextImGui()
		:UIContextBase(UI_ImGui)
	{
		
	}


	// Í¨¹ý UIContextBase ¼Ì³Ð
	virtual void Init() override;

	virtual void BeginUI() override;

	virtual void RenderDrawData() override;

	virtual void EndUI() override;// submit render data (ImGui::Render())

	static LRESULT WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	std::shared_ptr<RHIDynamic> rhi;
};
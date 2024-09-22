#include "ImGuiContext.h"
#include"../../../system/GlobalSystemContext.h"

#include<3rdParty/imgui-docking/imgui.h>
#include<3rdParty/imgui-docking/backends/imgui_impl_dx12.h>
#include<3rdParty/imgui-docking/backends/imgui_impl_win32.h>

void UIContextImGui::Init()
{
	rhi = mGlobalSystemContext.GetRHI();
	if (rhi->type == RHIInterfaceType::DirectX12)
	{
		D3D12RHIDynamic*d3dRhi = (D3D12RHIDynamic*)(rhi.get());
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		ImGui_ImplWin32_Init(mGlobalSystemContext.mWindowSystem->MainWnd);
		ImGui_ImplDX12_Init(d3dRhi->DxDevice.Get(), 1,
			DXGI_FORMAT_R8G8B8A8_UNORM, d3dRhi->OfflineDescriptorHeapManager.OffScreenRenderDescHeap.Get(),
			d3dRhi->OfflineDescriptorHeapManager.OffScreenRenderDescHeap->GetCPUDescriptorHandleForHeapStart(),
			d3dRhi->OfflineDescriptorHeapManager.OffScreenRenderDescHeap->GetGPUDescriptorHandleForHeapStart());
	}
}
/// <summary>
/// 以下内容应该在 RHI中抽象出来，在子类中实现，以便于供Opengl和D3D12 两种图形库使用
/// 目前暂时不做实现
void UIContextImGui::BeginUI()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGui::DockSpaceOverViewport();
}

void UIContextImGui::RenderDrawData()
{
	D3D12RHIDynamic* d3dRhi = (D3D12RHIDynamic*)(rhi.get());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), d3dRhi->DxCMDList.Get());
}

void UIContextImGui::EndUI()
{
	ImGui::Render();
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT UIContextImGui::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd,msg,wParam,lParam);
}
/// </summary>

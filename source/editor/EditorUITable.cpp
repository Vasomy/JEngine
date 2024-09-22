#include "EditorUITable.h"
#include"../runtime/function/system/GlobalSystemContext.h"
#include<3rdParty/imgui-docking/imgui.h>
#include<3rdParty/imgui-docking/backends/imgui_impl_dx12.h>
#include<3rdParty/imgui-docking/backends/imgui_impl_win32.h>
void EditorWindowUI::FillUITable()
{
	if (ImGui::Begin("A"))
	{
		ImGui::Image(mGlobalSystemContext.GetRHI()->GetTextureDataForUI(TU_RenderTarget,1),{500,500});
		ImGui::End();
	}
}

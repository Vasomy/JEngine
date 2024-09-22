#include "EditorUIContext.h"
#include"../runtime/function/system/GlobalSystemContext.h"
void EditorUIContext::Tick()
{
	mGlobalSystemContext.mRenderSystem->UIContext->BeginUI();
	WindowUI.FillUITable();

	mGlobalSystemContext.mRenderSystem->UIContext->EndUI();
}

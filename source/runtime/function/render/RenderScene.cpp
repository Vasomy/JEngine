#include "RenderScene.h"

void MainCameraRenderScene::UpdateMainCamera(const RenderCamera* camera)
{
	SceneConstants.View = camera->GetView();
	SceneConstants.Proj = camera->GetPerspective();
	SceneConstants.ViewProj = MatrixMultiply(SceneConstants.View, SceneConstants.Proj);
	SceneConstants.InvView = Inverse(SceneConstants.View);
	SceneConstants.InvProj = Inverse(SceneConstants.Proj);
	SceneConstants.InvViewProj = Inverse(SceneConstants.InvViewProj);



}

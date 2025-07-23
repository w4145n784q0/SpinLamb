#include "ImGuiViewer.h"

ImGuiViewer::ImGuiViewer(GameObject* parent)
	:GameObject(parent,"ImGuiViewer")
{
}

ImGuiViewer::~ImGuiViewer()
{
}

void ImGuiViewer::DrawImGuiPlaying()
{
	if (pGround_ != nullptr)
	{
		pGround_->DrawImGui();
	}
	if (pFence_ != nullptr)
	{

	}
}

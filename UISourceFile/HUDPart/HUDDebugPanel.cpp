#include "HUDDebugPanel.h"
#include"../HUD.h"

HUDDebugPanel::HUDDebugPanel(GameObject* parent)
	:GameObject(parent, "HUDDebugPanel"), pHUD_(nullptr)
{
}

void HUDDebugPanel::ImGuiExplanationDraw()
{
}

void HUDDebugPanel::ImGuiStartLogoDraw()
{
}

void HUDDebugPanel::ImGuiFinishLogoDraw()
{
}

void HUDDebugPanel::ImGuiPracticeLogoDraw()
{
}

void HUDDebugPanel::ImGuiScoreDraw()
{
}

void HUDDebugPanel::ImGuiTimerDraw()
{
}

void HUDDebugPanel::ImGuiMiniMapDraw()
{
}

void HUDDebugPanel::ImGuiPauseDraw()
{
}

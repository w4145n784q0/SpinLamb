#pragma once
#include "../../Engine/GameObject.h"

class HUD;

//HUDクラスのImGui描画を行うモジュール

class HUDDebugPanel :
    public GameObject
{
public:
    HUDDebugPanel() = default;
    HUDDebugPanel(GameObject* parent);
    virtual ~HUDDebugPanel() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //ImGui描画全体を管理
    void ImGuiDrawer(HUD* _hud);

    //描画タスクImGui描画
    void ImGuiRenderTaskDraw(HUD* _hud);

    //操作説明のImGui描画
    void ImGuiExplanationDraw(HUD* _hud);

    //開始時のロゴのImGui描画
    void ImGuiStartLogoDraw(HUD* _hud);

    //終了時のロゴのImGui描画
    void ImGuiFinishLogoDraw(HUD* _hud);

    //練習モードのロゴのImGui描画
    void ImGuiPracticeLogoDraw(HUD* _hud);

    //スコア表示のImGui描画
    void ImGuiScoreDraw(HUD* _hud);

    //タイマー表示のImGui描画
    void ImGuiTimerDraw(HUD* _hud);

    //ミニマップ表示のImGui描画
    void ImGuiMiniMapDraw(HUD* _hud);

    //ポーズ画面のUIのImGui描画
    void ImGuiPauseDraw(HUD* _hud);
};


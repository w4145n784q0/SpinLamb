#pragma once
#include "../../Engine/GameObject.h"

class HUD;

//HUDクラスのImGui描画を行うモジュール
class HUDDebugPanel :
    public GameObject
{
private:
    //親クラス(HUD)のポインタ
    HUD* pHUD_;
public:
    HUDDebugPanel(GameObject* parent);
    virtual ~HUDDebugPanel() = default;

    //操作説明のImGui描画
    void ImGuiExplanationDraw();

    //開始時のロゴのImGui描画
    void ImGuiStartLogoDraw();

    //終了時のロゴのImGui描画
    void ImGuiFinishLogoDraw();

    //練習モードのロゴのImGui描画
    void ImGuiPracticeLogoDraw();

    //スコア表示のImGui描画
    void ImGuiScoreDraw();

    //タイマー表示のImGui描画
    void ImGuiTimerDraw();

    //ミニマップ表示のImGui描画
    void ImGuiMiniMapDraw();

    //ポーズ画面のUIのImGui描画
    void ImGuiPauseDraw();
};


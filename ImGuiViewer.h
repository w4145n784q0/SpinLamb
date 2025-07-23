#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"

#include"Fence.h"
#include"OutStageThing.h"
#include"Player.h"
#include"Enemy.h"
#include"HUD.h"

//ImGuiを描画する用のクラス
//画面分割時に一度だけ描画する(多重に呼ぶことを防ぐ)ため、各クラスではなくこのクラスで処理を書く
//GameViewから一度だけ呼ぶ


class ImGuiViewer :
    public GameObject
{
private:
    Ground* pGround_ = nullptr;
    Fence* pFence_ = nullptr;
    OutStageThing* pOutStageThing_ = nullptr;
    Player* pPlayer_ = nullptr;
    Enemy* pEnemy_ = nullptr;
    HUD* pHUD_ = nullptr;
public:
    ImGuiViewer(GameObject* parent);
    ~ImGuiViewer();

    //Battle,Practice中に表示する用
    void DrawImGuiPlaying();
};


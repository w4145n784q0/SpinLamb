#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"

#include"Fence.h"
#include"OutStageThing.h"
#include"Player.h"
#include"Enemy.h"
#include"HUD.h"

//ImGui��`�悷��p�̃N���X
//��ʕ������Ɉ�x�����`�悷��(���d�ɌĂԂ��Ƃ�h��)���߁A�e�N���X�ł͂Ȃ����̃N���X�ŏ���������
//GameView�����x�����Ă�


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

    //Battle,Practice���ɕ\������p
    void DrawImGuiPlaying();
};


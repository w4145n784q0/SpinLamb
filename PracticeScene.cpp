#include "PracticeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

#include"Player.h"
#include"Ground.h"
#include"Enemy.h"
#include"EnemyManager.h"
#include"Fence.h"
#include"MiniMap.h"
#include"HUD.h"
#include"StageManager.h"

namespace
{
	const int BackTitleValue = 120;//�������Ń^�C�g���ɖ߂�܂ł̎���
}

PracticeScene::PracticeScene(GameObject* parent)
	:GameObject(parent,"PracticeScene"),Press_(0)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	//Instantiate<Ground>(this);
	//Instantiate<Fence>(this);

	Instantiate<StageManager>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	Instantiate<MiniMap>(this);
	Instantiate<HUD>(this);

	StageManager* pS = (StageManager*)FindObject("StageManager");
	float north = pS->GetNorthEnd();
	float south = pS->GetSouthEnd();
	float west = pS->GetWestEnd();
	float east = pS->GetEastEnd();

	Player* pPlayer_ = (Player*)FindObject("Player");
	pPlayer_->PlayerStart();
	pPlayer_->SetEnd(north, south, west, east);

	Enemy* pEnemy = (Enemy*)FindObject("Enemy");
	pEnemy->EnemyStart();
	pEnemy->SetEnd(north, south, west, east);

	HUD* pHUD = (HUD*)FindObject("HUD");
	pHUD->SetStatePractice();
}

void PracticeScene::Update()
{
	if (Input::IsKey(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))//�{�^���������Ń^�C�g���ɖ߂�
	{
		Press_++;
	}
	else
	{
		Press_ = 0;
	}

	if (Press_ >= BackTitleValue)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void PracticeScene::Draw()
{
}

void PracticeScene::Release()
{
}

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
	const int BackTitleValue = 120;//長押しでタイトルに戻るまでの時間
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
	//Instantiate<EnemyManager>(this);
	Instantiate<MiniMap>(this);
	Instantiate<HUD>(this);

	//EnemyManager* pEnemyManager = (EnemyManager*)FindObject("EnemyManager");
	//pEnemyManager->EnemyInitialize();

	Player* pPlayer_ = (Player*)FindObject("Player");
	pPlayer_->PlayerStart();

	Enemy* pEnemy = (Enemy*)FindObject("Enemy");
	pEnemy->EnemyStart();

	HUD* pHUD = (HUD*)FindObject("HUD");
	pHUD->SetStatePractice();
}

void PracticeScene::Update()
{
	if (Input::IsKey(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))//ボタン長押しでタイトルに戻る
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

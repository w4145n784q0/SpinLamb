#include "BattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

#include"Player.h"
#include"Enemy.h"
#include"MiniMap.h"
#include"HUD.h"
#include"StageManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	//ŽžŠÔŒv‘ª
	int Timecounter = 0;
}

BattleScene::BattleScene(GameObject* parent)
	:GameObject(parent,"BattleScene") ,BattleState(BEFORE),
	 hBattleSound_(-1),hWhistle_(-1),
	PlayerScore_(0),EnemyScore_(0),GameTime_(GameTimeLimit),pPlayerScore_(0),pEnemyScore_(0)
{
}

void BattleScene::Initialize()
{
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
	pPlayer_->SetEnd(north, south, west, east);

	Enemy* pEnemy = (Enemy*)FindObject("Enemy");
	pEnemy->SetEnd(north, south, west, east);


	HUD* pHUD = (HUD*)FindObject("HUD");
	pHUD->SetStateBattle();
	pHUD->SetTime(GameTime_);

	hBattleSound_ = Audio::Load("Sound\\maou_game_rock51.wav");
	assert(hBattleSound_>= 0);

	hWhistle_ = Audio::Load("Sound\\maou_se_sound_whistle01.wav");
	assert(hWhistle_ >= 0);

	pPlayerScore_ = new Text;
	pPlayerScore_->Initialize();

	pEnemyScore_ = new Text;
	pEnemyScore_->Initialize();

}

void BattleScene::Update()
{
	switch (BattleState)
	{
	case BattleScene::BEFORE:
		UpdateBattleBefore();
		break;
	case BattleScene::NOW:
		UpdateBattle();
		break;
	case BattleScene::AFTER:
		UpdateBattleAfter();
		break;
	case BattleScene::MAX:
		break;
	default:
		break;
	}
}

void BattleScene::Draw()
{
	pPlayerScore_->Draw(30, 30, PlayerScore_);
	pEnemyScore_->Draw(1250, 30, EnemyScore_);
}

void BattleScene::Release()
{
}

void BattleScene::UpdateBattleBefore()
{
	if (++Timecounter > SceneTransition)
	{
		Timecounter = 0;

		BattleState = NOW;
		Player* pPlayer_ = (Player*)FindObject("Player");
		pPlayer_->PlayerStart();

		Enemy* pEnemy = (Enemy*)FindObject("Enemy");
		pEnemy->EnemyStart();

		HUD* pHUD = (HUD*)FindObject("HUD");
		pHUD->SetStateBattleInProgress();
	}
}

void BattleScene::UpdateBattle()
{
	if (GameTime_ <= 0)
	{
		BattleState = AFTER;
		Player* pPlayer_ = (Player*)FindObject("Player");
		pPlayer_->PlayerStop();

		Enemy* pEnemy = (Enemy*)FindObject("Enemy");
		pEnemy->EnemyStop();

		HUD* pHUD = (HUD*)FindObject("HUD");
		pHUD->SetStateBattleEnd();
	}

	Audio::Play(hBattleSound_);
	if (++Timecounter > oneSecond)
	{
		Timecounter = 0;
		if(GameTime_ > 0)
		{
			GameTime_--;
			HUD* pHUD = (HUD*)FindObject("HUD");
			pHUD->SetTime(GameTime_);
		}
	}
}

void BattleScene::UpdateBattleAfter()
{
	static bool IsSound = true;
	if (IsSound)
	{
		Audio::Play(hWhistle_);
		IsSound = false;
	}


	if (++Timecounter > SceneTransition)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		if (PlayerScore_ > EnemyScore_)
		{
			pSceneManager->ChangeScene(SCENE_ID_CLEAR);
		}
		else
		{
			pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
		}
		Timecounter = 0;
	}
}
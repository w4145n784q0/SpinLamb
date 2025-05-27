#include "BattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

#include"Player.h"
#include"Ground.h"
#include"Enemy.h"
#include"EnemyManager.h"
#include"MiniMap.h"
#include"Fence.h"
#include"HUD.h"
#include"StageManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	int Timecounter = 0;
	const int oneSecond = 60;
	const int GameTimeLimit = 60;
}

BattleScene::BattleScene(GameObject* parent)
	:GameObject(parent,"BattleScene") ,BattleState(BEFORE),
	 hBattleSound_(-1),hWhistle_(-1),
	PlayerScore_(0),EnemyScore_(0),GameTime_(GameTimeLimit)
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
	//pHUD->SetNumber(StartCount_);


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
	
	//ImGui::Text("count :%1f", (float)Timecounter);

	pPlayerScore_->Draw(30, 30, PlayerScore_);
	pEnemyScore_->Draw(1250, 30, EnemyScore_);


	/*for (int i = 0; i < PlayerLife_; i++)
	{
		HUD_Trans_[0].position_.x = PlayerLifeStart + i * LifeWidth;
		Image::SetTransform(hPlayerLife_, HUD_Trans_[0]);
		Image::Draw(hPlayerLife_);
	}

	for (int i = 0; i < EnemyLife_; i++)
	{
		HUD_Trans_[1].position_.x = PlayerLifeStart + i * LifeWidth;
		Image::SetTransform(hEnemyLife_, HUD_Trans_[1]);
		Image::Draw(hEnemyLife_);
	}*/

	switch (BattleState)
	{
	case BattleScene::BEFORE:
		DrawBattleBefore();
		break;
	case BattleScene::NOW:
		DrawBattle();
		break;
	case BattleScene::AFTER:
		DrawBattleAfter();
		break;
	case BattleScene::MAX:
		break;
	default:
		break;
	}
}

void BattleScene::Release()
{
}

void BattleScene::UpdateBattleBefore()
{
	if (++Timecounter > 120)
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
	static bool Issound = true;
	if (Issound)
	{
		Audio::Play(hWhistle_);
		Issound = false;
	}


	static int time = 120;
	if (--time < 0)
	{
		
		if (PlayerScore_ > EnemyScore_)
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_CLEAR);
		}
		else
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
		}
		time = 120;
	}
}

void BattleScene::DrawBattleBefore()
{
}

void BattleScene::DrawBattle()
{
}

void BattleScene::DrawBattleAfter()
{	
}

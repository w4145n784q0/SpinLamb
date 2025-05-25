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
	//const float PlayerLifeStart = -0.9;//HPの描画位置の初期地点(プレイヤー)
	//const float EnemyLifeStart = 0.9;//HPの描画位置の初期地点(敵)
	//const float LifeWidth = 0.1;//HPの描画の間隔

	int Timecounter = 0;
	const int oneSecond = 60;
	const int GameTimeLimit = 60;
}

BattleScene::BattleScene(GameObject* parent)
	:GameObject(parent,"BattleScene") ,BattleState(BEFORE),
	 hBattleSound_(-1),hWhistle_(-1),
	PlayerScore_(0),EnemyScore_(0),GameTime_(GameTimeLimit),StartCount_(3)
{
	//HUD_Trans_[0].position_ = { -0.7,0.8,0 };
	//HUD_Trans_[1].position_ = { 0.7,0.8,0 };
}

void BattleScene::Initialize()
{

	Instantiate<StageManager>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	//Instantiate<EnemyManager>(this);
	Instantiate<MiniMap>(this);
	Instantiate<HUD>(this);

	HUD* pHUD = (HUD*)FindObject("HUD");
	pHUD->SetStateBattle();
	pHUD->SetTime(GameTime_);
	//pHUD->SetNumber(StartCount_);

	//EnemyManager* pEnemyManager = (EnemyManager*)FindObject("EnemyManager");
	//pEnemyManager->EnemyInitialize();

	hBattleSound_ = Audio::Load("Sound\\maou_game_rock51.wav");
	assert(hBattleSound_>= 0);

	hWhistle_ = Audio::Load("Sound\\maou_se_sound_whistle01.wav");
	assert(hWhistle_ >= 0);

	pTime_ = new Text;
	pTime_->Initialize();

	pPlayerScore_ = new Text;
	pPlayerScore_->Initialize();

	pEnemyScore_ = new Text;
	pEnemyScore_->Initialize();

	//pPlayer_ = (Player*)FindObject("Player");
	

	//PlayerLife_ = pPlayer_->GetCharacterLife();
	//EnemyLife_ = pEnemy_->GetCharacterLife();
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

	//pTime_->Draw(640, 30, GameTime_);
	pPlayerScore_->Draw(30, 30, PlayerScore_);
	pEnemyScore_->Draw(1250, 30, EnemyScore_);

	/*pText_->Draw(140, 30, Phase_);
	pText_->Draw(30, 30, "PHASE:");

	pText2_->Draw(140, 60, deadCount_);
	pText2_->Draw(30, 60, "LIFE :");*/

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
//	if (StartCount_ <= 0)
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

	/*if (++Timecounter > oneSecond)
	{
		Timecounter = 0;
		if (StartCount_ > 0)
		{
			StartCount_--;
		}
	}*/
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

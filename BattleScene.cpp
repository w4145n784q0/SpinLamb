#include "BattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

#include"Player.h"
#include"Ground.h"
#include"Enemy.h"
#include"EnemyManager.h"
#include"Fence.h"
#include"Logo.h"

namespace
{
	const float LifeStartPosition = -0.9;//HPÇÃï`âÊà íuÇÃèâä˙ínì_
	const float LifeWidth = 0.1;//HPÇÃï`âÊÇÃä‘äu
}

BattleScene::BattleScene(GameObject* parent)
	:GameObject(parent,"BattleScene") ,BattleState(NOW),
	hWin_(-1),hLose_(-1),hFinish_(-1),hLife_(-1), hBattleSound_(-1),hWhistle_(-1),
	PlayerLife_(0),EnemyLife_(0),pPlayer_(nullptr)
{
	HUD_Trans_[0].position_ = { -0.7,0.8,0 };
	HUD_Trans_[1].position_ = { 0.7,0.8,0 };
}

void BattleScene::Initialize()
{
	Instantiate<Ground>(this);
	Instantiate<Fence>(this);
	Instantiate<Player>(this);
	Instantiate<EnemyManager>(this);

	EnemyManager* pEnemyManager = (EnemyManager*)FindObject("EnemyManager");
	pEnemyManager->EnemyInitialize();

	hWin_ = Image::Load("YouWin.png");
	hLose_ = Image::Load("YouLose.png");
	hFinish_ = Image::Load("finish.png");
	hLife_ = Image::Load("arrow.png");

	hBattleSound_ = Audio::Load("maou_game_rock51.wav");
	hWhistle_ = Audio::Load("maou_se_sound_whistle01.wav");

	pPlayer_ = (Player*)FindObject("Player");

	PlayerLife_ = pPlayer_->GetCharacterLife();
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
	/*pText_->Draw(140, 30, Phase_);
	pText_->Draw(30, 30, "PHASE:");

	pText2_->Draw(140, 60, deadCount_);
	pText2_->Draw(30, 60, "LIFE :");*/

	//Image::SetTransform(hLife_,HUD_Trans_[0]);
	//Image::Draw(hLife_);

	for (int i = 0; i < PlayerLife_; i++)
	{
		HUD_Trans_[0].position_.x = LifeStartPosition + i * LifeWidth;
		Image::SetTransform(hLife_, HUD_Trans_[0]);
		Image::Draw(hLife_);
	}

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

}

void BattleScene::UpdateBattle()
{

	Audio::Play(hBattleSound_);

	//if (FindObject("Enemy") == nullptr && Phase_ < 3)
	//{
	//	//Phase_++;
	//	EnemyManager* pEnemyManager = (EnemyManager*)FindObject("EnemyManager");
	//	pEnemyManager->EnemyInitialize();
	//}
	//if (Phase_ == 3)
	//{
	//	Player* pPlayer = (Player*)FindObject("Player");
	//	pPlayer->PlayerStop();

	//	IsWin_ = true;
	//	BattleState = AFTER;
	//	Instantiate<Logo>(this);
	//	/*SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	//	pSceneManager->ChangeScene(SCENE_ID_CLEAR);*/
	//}
	//if (deadCount_ == 0)
	//{
	//	Enemy* pEnemy = (Enemy*)FindObject("Enemy");
	//	pEnemy->SetStateStop();
	//	Player* pPlayer = (Player*)FindObject("Player");
	//	pPlayer->PlayerStop();

	//	BattleState = AFTER;
	//	IsWin_ = false;
	//	Instantiate<Logo>(this);
	//	/*SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	//	pSceneManager->ChangeScene(SCENE_ID_TITLE);*/
	//}


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
		Logo* pLogo = (Logo*)FindObject("Logo");
		pLogo->SetMax();
		
		if (IsWin_)
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_CLEAR);
			time = 120;
			
		}
		else
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
			time = 120;
		}
		
	}
}

void BattleScene::DrawBattleBefore()
{
	//Image::SetTransform(hLife_,transform_);
	//Image::Draw(hLife_);
}

void BattleScene::DrawBattle()
{
}

void BattleScene::DrawBattleAfter()
{
}

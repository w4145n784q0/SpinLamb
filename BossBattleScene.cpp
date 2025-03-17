#include "BossBattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

#include"Player.h"
#include"Ground.h"
#include"Enemy.h"
#include"EnemyManager.h"
#include"Logo.h"

BossBattleScene::BossBattleScene(GameObject* parent)
	:GameObject(parent,"BossBattleScene") ,pText_(nullptr),pText2_(nullptr), Phase_(0),deadCount_(3),
	hWin_(-1),hLose_(-1),hBattleSound_(-1),hWhistle_(-1)
{
}

void BossBattleScene::Initialize()
{
	Instantiate<Ground>(this);
	Instantiate<Player>(this);
	Instantiate<EnemyManager>(this);
	
	pText_ = new Text;
	pText_->Initialize();
	pText2_ = new Text;
	pText2_->Initialize();

	hWin_ = Image::Load("YouWin.png");
	hLose_ = Image::Load("YouLose.png");
	hFinish_ = Image::Load("finish.png");

	hBattleSound_ = Audio::Load("maou_game_rock51.wav");
	hWhistle_ = Audio::Load("maou_se_sound_whistle01.wav");

	BattleState = NOW;
}

void BossBattleScene::Update()
{
	switch (BattleState)
	{
	case BossBattleScene::BEFORE:
		UpdateBattleBefore();
		break;
	case BossBattleScene::NOW:
		UpdateBattle();
		break;
	case BossBattleScene::AFTER:
		UpdateBattleAfter();
		break;
	case BossBattleScene::MAX:
		break;
	default:
		break;
	}
}

void BossBattleScene::Draw()
{
	pText_->Draw(140, 30, Phase_);
	pText_->Draw(30, 30, "PHASE:");

	pText2_->Draw(140, 60, deadCount_);
	pText2_->Draw(30, 60, "LIFE :");

	switch (BattleState)
	{
	case BossBattleScene::BEFORE:
		break;
	case BossBattleScene::NOW:
		break;
	case BossBattleScene::AFTER:
		break;
	case BossBattleScene::MAX:
		break;
	default:
		break;
	}
}

void BossBattleScene::Release()
{
}

void BossBattleScene::UpdateBattleBefore()
{
}

void BossBattleScene::UpdateBattle()
{
	Audio::Play(hBattleSound_);

	if (FindObject("Enemy") == nullptr && Phase_ < 3)
	{
		//Phase_++;
		EnemyManager* pEnemyManager = (EnemyManager*)FindObject("EnemyManager");
		pEnemyManager->EnemyInitialize();
	}
	if (Phase_ == 3)
	{
		Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->PlayerStop();

		IsWin_ = true;
		BattleState = AFTER;
		Instantiate<Logo>(this);
		/*SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);*/
	}
	if (deadCount_ == 0)
	{
		Enemy* pEnemy = (Enemy*)FindObject("Enemy");
		pEnemy->SetStateStop();
		Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->PlayerStop();

		BattleState = AFTER;
		IsWin_ = false;
		Instantiate<Logo>(this);
		/*SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);*/
	}
}

void BossBattleScene::UpdateBattleAfter()
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
			
		}
		else
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
		}
		
	}
}

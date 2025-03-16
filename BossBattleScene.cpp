#include "BossBattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

#include"Player.h"
#include"Ground.h"
#include"Enemy.h"
#include"EnemyManager.h"

BossBattleScene::BossBattleScene(GameObject* parent)
	:GameObject(parent,"BossBattleScene") ,pText_(nullptr),pText2_(nullptr), Phase_(-1),deadCount_(3)
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
}

void BossBattleScene::Update()
{
	if (FindObject("Enemy") == nullptr)
	{
		Phase_++;
		EnemyManager* pEnemyManager = (EnemyManager*)FindObject("EnemyManager");
		pEnemyManager->EnemyInitialize();
	}
	if (Phase_ == 3)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
	}
	if (deadCount_ == 0)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void BossBattleScene::Draw()
{
	pText_->Draw(140, 30, Phase_);
	pText_->Draw(30, 30, "PHASE:");

	pText2_->Draw(140, 60, deadCount_);
	pText2_->Draw(30, 60, "LIFE:");
}

void BossBattleScene::Release()
{
}

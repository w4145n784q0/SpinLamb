#include "BossBattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

#include"Player.h"
#include"Ground.h"
#include"Enemy.h"
#include"EnemyManager.h"

BossBattleScene::BossBattleScene(GameObject* parent)
	:GameObject(parent,"BossBattleScene") ,pText_(nullptr),pText2_(nullptr), Phase_(0),deadCount_(3),
	hWin_(-1),hLose_(-1)
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
}

void BossBattleScene::Update()
{
	if (FindObject("Enemy") == nullptr && Phase_ < 3)
	{
		//Phase_++;
		EnemyManager* pEnemyManager = (EnemyManager*)FindObject("EnemyManager");
		pEnemyManager->EnemyInitialize();
	}
	if (Phase_ == 3)
	{
		/*Player* pPlayer = (Player*)FindObject("Player");
		pPlayer->PlayerStop();*/
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
	}
	if (deadCount_ == 0)
	{
		/*Enemy* pEnemy = (Enemy*)FindObject("Enemy");
		pEnemy->SetStateStop();*/
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void BossBattleScene::Draw()
{
	pText_->Draw(140, 30, Phase_);
	pText_->Draw(30, 30, "PHASE:");

	pText2_->Draw(140, 60, deadCount_);
	pText2_->Draw(30, 60, "LIFE :");
}

void BossBattleScene::Release()
{
}

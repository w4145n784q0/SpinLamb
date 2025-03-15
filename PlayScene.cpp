#include "PlayScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

#include"Player.h"
#include"Ground.h"
#include"Enemy.h"
#include"EnemyManager.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene"), hImage_(-1),Phase_(-1),pText_(nullptr)
{
}

void PlayScene::Initialize()
{
	Instantiate<Ground>(this);
	Instantiate<Player>(this);
	Instantiate<EnemyManager>(this);

	pText_ = new Text;
	pText_->Initialize();
}

void PlayScene::Update()
{
	/*if (Input::IsKeyDown(DIK_P))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
	}*/
	if (FindObject("Enemy") == nullptr)
	{
		/*SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);*/
		Phase_++;
		EnemyManager* pEnemyManager = (EnemyManager*)FindObject("EnemyManager");
		pEnemyManager->EnemyInitialize();
	}
	if (Phase_ == 3)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
	}
}

void PlayScene::Draw()
{
	pText_->Draw(120, 30, Phase_);
	pText_->Draw(30, 30, "Phase");
}

void PlayScene::Release()
{
}

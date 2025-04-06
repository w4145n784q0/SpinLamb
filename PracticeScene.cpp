#include "PracticeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

#include"Player.h"
#include"Ground.h"
#include"Enemy.h"
#include"EnemyManager.h"

PracticeScene::PracticeScene(GameObject* parent)
	:GameObject(parent,"PracticeScene"),Press_(0)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	Instantiate<Ground>(this);
	Instantiate<Player>(this);
	Instantiate<EnemyManager>(this);
}

void PracticeScene::Update()
{
	if (Input::IsKey(DIK_P))
	{
		Press_++;
	}
	else
	{
		Press_ = 0;
	}

	if (Press_ >= 120)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
	}
}

void PracticeScene::Draw()
{
}

void PracticeScene::Release()
{
}

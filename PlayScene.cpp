#include "PlayScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

#include"Player.h"
#include"Ground.h"
#include"Enemy.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene"), hImage_(-1)
{
}

void PlayScene::Initialize()
{
	Instantiate<Ground>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);

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
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}

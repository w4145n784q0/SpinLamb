#include "GameOverScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene"), hImage_(-1),hGameOverSound_(-1)
{
}

void GameOverScene::Initialize()
{
	hImage_ = Image::Load("GameOverScreen.png");
	hGameOverSound_ = Audio::Load("maou_game_jingle10.wav");
}

void GameOverScene::Update()
{
	Audio::Play(hGameOverSound_);
	if (Input::IsKeyDown(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void GameOverScene::Release()
{
}

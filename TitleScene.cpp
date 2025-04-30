#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene"),hImage_(-1),hTitleSound_(-1)
{
}

void TitleScene::Initialize()
{
	hImage_ = Image::Load("TitleScreen.png");
	hTitleSound_ = Audio::Load("maou_game_rock45.wav"); 
	assert(hTitleSound_ >= 0);
}

void TitleScene::Update()
{
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		//pSceneManager->ChangeScene(SCENE_ID_PRACTICE);//デバッグ用
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);
		Audio::Stop(hTitleSound_);
	}

	Audio::Play(hTitleSound_);
}

void TitleScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void TitleScene::Release()
{
}

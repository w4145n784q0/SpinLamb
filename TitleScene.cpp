#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

TitleScene::TitleScene(GameObject* parent)
	:SceneData(parent,"TitleScene"), hBackScreen_(-1),hTitleSound_(-1)
{
}

void TitleScene::Initialize()
{
	hBackScreen_ = Image::Load("Image\\Title\\TitleScreen.jpg");
	assert(hBackScreen_ >= 0);
	hTitleSound_ = Audio::Load("Sound\\maou_game_rock45.wav"); 
	assert(hTitleSound_ >= 0);
	SetCSVScene();
}

void TitleScene::Update()
{
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);
		Audio::Stop(hTitleSound_);
	}

	Audio::Play(hTitleSound_);
}

void TitleScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);
}

void TitleScene::Release()
{
}

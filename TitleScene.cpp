#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene"), hBackScreen_(-1),hSoundTitle_(-1)
{
}

void TitleScene::Initialize()
{
	hBackScreen_ = Image::Load("Image\\Title\\TitleScreen.jpg");
	assert(hBackScreen_ >= 0);
	hSoundTitle_ = Audio::Load("Sound\\BGM\\title.wav",true); 
	assert(hSoundTitle_ >= 0);
}

void TitleScene::Update()
{
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);
		Audio::Stop(hSoundTitle_);
	}

	Audio::Play(hSoundTitle_);
}

void TitleScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);
}

void TitleScene::Release()
{
}

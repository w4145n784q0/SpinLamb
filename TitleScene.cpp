#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene"),hImage_(-1)
{
}

void TitleScene::Initialize()
{
	hImage_ = Image::Load("title.png");
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_B) || Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);
	}
}

void TitleScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void TitleScene::Release()
{
}

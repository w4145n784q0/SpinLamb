#include "ClearScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

ClearScene::ClearScene(GameObject* parent)
	:GameObject(parent,"ClearScene"), hImage_(-1)
{
}

void ClearScene::Initialize()
{
	hImage_ = Image::Load("gameclear.png");
}

void ClearScene::Update()
{
	if (Input::IsKeyDown(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_B) || Input::IsPadButton(XINPUT_GAMEPAD_START))//スペースキーを押したらゲームスタート
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void ClearScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void ClearScene::Release()
{
}

#include "ClearScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"

namespace
{
	Transform LogoTitle;
	XMFLOAT3 LogoTitlePosition = { 0,-0.8,0 };
}

ClearScene::ClearScene(GameObject* parent)
	:GameObject(parent,"ClearScene"), hImage_(-1), hlogoResult_(-1),hlogoTitle_(-1), hClearSound_(-1)
{
}

void ClearScene::Initialize()
{
	hImage_ = Image::Load("Image\\GameMode\\back_mode2.jpg");
	assert(hImage_ >= 0);

	hlogoResult_ = Image::Load("Image\\YouWin.png");
	assert(hlogoResult_ >= 0);

	hlogoTitle_ = Image::Load("Image\\PushToTitle.png");
	assert(hlogoTitle_ >= 0);

	hClearSound_ = Audio::Load("Sound\\maou_game_jingle05.wav");
	assert(hClearSound_ >= 0);

	LogoTitle.position_ = LogoTitlePosition;
}

void ClearScene::Update()
{
	Audio::Play(hClearSound_);
	if (Input::IsKeyDown(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))//スペースキーを押したらゲームスタート
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void ClearScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);

	Image::SetTransform(hlogoResult_, transform_);
	Image::Draw(hlogoResult_);

	Image::SetTransform(hlogoTitle_, LogoTitle);
	Image::Draw(hlogoTitle_);
}

void ClearScene::Release()
{
}

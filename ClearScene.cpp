#include "ClearScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"

namespace
{
	Transform LogoResult;
	Transform LogoTitle;
	XMFLOAT3 LogoResultPosition = { 0,0,0 };
	XMFLOAT3 LogoTitlePosition = { 0,-0.8,0 };
}

ClearScene::ClearScene(GameObject* parent)
	:GameObject(parent,"ClearScene"), hBackScreen_(-1), hlogoResult_(-1),hlogoTitle_(-1), hClearSound_(-1)
{
}

void ClearScene::Initialize()
{
	hBackScreen_ = Image::Load("Image\\GameMode\\back_mode.jpg");
	assert(hBackScreen_ >= 0);

	hlogoResult_ = Image::Load("Image\\Result\\YouWin.png");
	assert(hlogoResult_ >= 0);

	hlogoTitle_ = Image::Load("Image\\Result\\PushToTitle.png");
	assert(hlogoTitle_ >= 0);

	hClearSound_ = Audio::Load("Sound\\maou_game_jingle05.wav");
	assert(hClearSound_ >= 0);

	LogoResult.position_ = LogoResultPosition;
	LogoTitle.position_ = LogoTitlePosition;
}

void ClearScene::Update()
{
	Audio::Play(hClearSound_);
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void ClearScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

	Image::SetTransform(hlogoResult_, LogoResult);
	Image::Draw(hlogoResult_);

	Image::SetTransform(hlogoTitle_, LogoTitle);
	Image::Draw(hlogoTitle_);
}

void ClearScene::Release()
{
}

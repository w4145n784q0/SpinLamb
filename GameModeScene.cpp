#include "GameModeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

GameModeScene::GameModeScene(GameObject* parent)
	:GameObject(parent,"GameModeScene"),SelectMode_(Max)
{
}

void GameModeScene::Initialize()
{
	hBack_ = Image::Load("Black.png");
	hBossMode_ = Image::Load("BossMode.png");
	hRushMode_ = Image::Load("RushMode.png");
	hBackTitle_ = Image::Load("backtitle.png");
	hArrow_ = Image::Load("arrow.png");
	hModeSelect_ = Image::Load("ModeSelect.png");

	hModeSound_ = Audio::Load("maou_game_rock54.wav");

	Trans_BossMode_.position_ = { 0.5,0.5,0 };
	Trans_RushMode_.position_ = { 0.5,0.0,0 };
	Trans_Title_.position_ = {0.5, -0.5, 0 };
	Trans_Arrow_.position_ = { 0.0,0.5,0 };
	Trans_Mode_.position_ = { -0.5,0.5,0 };
}

void GameModeScene::Update()
{
	if (Input::IsKeyDown(DIK_UP) || Input::GetPadStickL().y >= 0.5)
	{
		if (Trans_Arrow_.position_.y == 0.5)
		{
			Trans_Arrow_.position_.y = 0.5;
		}
		else
		{
			Trans_Arrow_.position_.y += 0.5;
		}
	}
	if (Input::IsKeyDown(DIK_DOWN) || Input::GetPadStickL().y <= -0.5)
	{
		if (Trans_Arrow_.position_.y == -0.5)
		{
			Trans_Arrow_.position_.y = -0.5;
		}
		else
		{
			Trans_Arrow_.position_.y -= 0.5;
		}
	}
	
	if (Trans_Arrow_.position_.y == 0.5)
	{
		SelectMode_ = Boss;
	}
	else if (Trans_Arrow_.position_.y == 0.0)
	{
		SelectMode_ = Rush;
	}
	else if (Trans_Arrow_.position_.y == -0.5)
	{
		SelectMode_ = Title;
	}

	if (Input::IsKeyDown(DIK_P) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B) || Input::IsPadButtonDown(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		switch (SelectMode_)
		{
		case GameModeScene::Boss:
			pSceneManager->ChangeScene(SCENE_ID_BOSSBATTLE);
			Audio::Stop(hModeSound_);
			break;
		case GameModeScene::Rush:
			break;
		case GameModeScene::Title:
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
			Audio::Stop(hModeSound_);
			break;
		default:
			break;
		}
		
	}

	Audio::Play(hModeSound_);
}

void GameModeScene::Draw()
{
	Image::SetTransform(hBossMode_, Trans_BossMode_);
	Image::Draw(hBossMode_);

	/*Image::SetTransform(hRushMode_, Trans_RushMode_);
	Image::Draw(hRushMode_);*/

	Image::SetTransform(hBackTitle_, Trans_Title_);
	Image::Draw(hBackTitle_);

	Image::SetTransform(hArrow_, Trans_Arrow_);
	Image::Draw(hArrow_);

	Image::SetTransform(hModeSelect_, Trans_Mode_);
	Image::Draw(hModeSelect_);

}

void GameModeScene::Release()
{
}

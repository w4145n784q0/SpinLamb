#include "GameModeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

GameModeScene::GameModeScene(GameObject* parent)
	:GameObject(parent,"GameModeScene"),SelectMode_(Battle)
{
}

void GameModeScene::Initialize()
{
	std::string path = "Image\\GameMode\\";

	hBattle_ = Image::Load(path + "BattleButton.png");
	assert(hBattle_ >= 0);

	hPractice_ = Image::Load(path + "FreeplayButton.png");
	assert(hPractice_ >= 0);

	hHowtoPlay_ = Image::Load(path + "HowToPlayButton.png");
	assert(hHowtoPlay_ >= 0);

	hBackTitle_ = Image::Load(path + "TitleButton.png");
	assert(hBackTitle_ >= 0);

	//hFrameLine_ = Image::Load(path + "LineFrame.png");
	//assert(hFrameLine_ >= 0);


	hArrow_ = Image::Load("Image\\arrow.png");
	assert(hArrow_ >= 0);

	hModeSelect_ = Image::Load("Image\\ModeSelect.png");
	assert(hModeSelect_>= 0);

	hModeSound_ = Audio::Load("Sound\\maou_game_rock54.wav");
	assert(hModeSound_ >= 0);

	ModeSetTrans[0].position_ = { -0.5,0.5,0 };//モードセレクトの位置
	ModeSetTrans[1].position_ = { 0.5,0.6,0 };//プレイの位置
	ModeSetTrans[2].position_ = { 0.5,0.2,0 };//練習の位置
	ModeSetTrans[3].position_ = { 0.5,-0.2,0 };//遊び方の位置
	ModeSetTrans[4].position_ = { 0.5,-0.6,0 };//タイトルの位置
	
	Trans_Arrow_.position_ = { 0.0,0.6,0 };

	ModeList_ = { Battle,Practice,HowToPlay,Title };
	itr = ModeList_.begin();
}

void GameModeScene::Update()
{
	if (Input::IsKeyDown(DIK_UP) || Input::GetPadStickL().y >= sticktilt)
	{
		if (itr == ModeList_.begin())
		{
			itr = --ModeList_.end();
		}
		else
		{
			--itr;
		}
		SelectMode_ = *itr;
	}
	if (Input::IsKeyDown(DIK_DOWN) || Input::GetPadStickL().y <= -sticktilt)
	{
		if (itr == --ModeList_.end())
		{
			itr = ModeList_.begin();
		}
		else
		{
			++itr;
		}
		SelectMode_ = *itr;
	}

	switch (SelectMode_)
	{
	case GameModeScene::Battle:
		Trans_Arrow_.position_.y = 0.6f;
		break;
	case GameModeScene::Practice:
		Trans_Arrow_.position_.y = 0.2f;
		break;
	case GameModeScene::HowToPlay:
		Trans_Arrow_.position_.y = -0.2f;
		break;
	case GameModeScene::Title:
		Trans_Arrow_.position_.y = -0.6f;
		break;
	default:
		break;
	}


	if (Input::IsKeyDown(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		switch (SelectMode_)
		{
		case GameModeScene::Battle:
			pSceneManager->ChangeScene(SCENE_ID_BATTLE);
			Audio::Stop(hModeSound_);
			break;
		case GameModeScene::Practice:
			pSceneManager->ChangeScene(SCENE_ID_PRACTICE);
			Audio::Stop(hModeSound_);
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
	Image::SetTransform(hModeSelect_, ModeSetTrans[0]);
	Image::Draw(hModeSelect_);

	Image::SetTransform(hBattle_, ModeSetTrans[1]);
	Image::Draw(hBattle_);

	Image::SetTransform(hPractice_, ModeSetTrans[2]);
	Image::Draw(hPractice_);

	Image::SetTransform(hHowtoPlay_, ModeSetTrans[3]);
	Image::Draw(hHowtoPlay_);

	Image::SetTransform(hBackTitle_, ModeSetTrans[4]);
	Image::Draw(hBackTitle_);

	Image::SetTransform(hArrow_, Trans_Arrow_);
	Image::Draw(hArrow_);

#ifdef _DEBUG
	ImGui::SliderFloat("battle", &ModeSetTrans[1].position_.y, 1.0, -1.0);
	ImGui::SliderFloat("free", &ModeSetTrans[2].position_.y, 1.0, -1.0);
	ImGui::SliderFloat("how", &ModeSetTrans[3].position_.y, 1.0, -1.0);
	ImGui::SliderFloat("title", &ModeSetTrans[4].position_.y, 1.0, -1.0);
#endif
}

void GameModeScene::Release()
{
}

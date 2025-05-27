#include "GameModeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"Engine/CsvReader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	const float sheepRotate = 5.0f;
}

GameModeScene::GameModeScene(GameObject* parent)
	:GameObject(parent, "GameModeScene"), 
	hBackScreen_(-1), hBackChara_(-1),hExplanation_(-1),
	hBattle_(-1),hPractice_(-1), hHowtoPlay_(-1),hBackTitle_(-1), hFrameLine_(-1),
	hModeSelect_(-1), hBattleText_(-1), hFreePlayText_(-1), hHowtoPlayText_(-1),hTitleText_(-1),
	TextArray_({}),hModeSound_(-1),SelectMode_(Battle)
{
}

void GameModeScene::Initialize()
{
	SetSCV();

	std::string path = "Image\\GameMode\\";

	hBackScreen_ = Image::Load(path + "back_mode.jpg");
	assert(hBackScreen_ >= 0);

	hBackChara_ = Image::Load(path + "sheepImage.png");
	assert(hBackChara_ >= 0);

	//hExplanation_ = Image::Load(path + "spinlamb_exp.jpg");
	//assert(hExplanation_ >= 0);

	hBattle_ = Image::Load(path + "BattleButton.png");
	assert(hBattle_ >= 0);

	hPractice_ = Image::Load(path + "FreeplayButton.png");
	assert(hPractice_ >= 0);

	hHowtoPlay_ = Image::Load(path + "HowToPlayButton.png");
	assert(hHowtoPlay_ >= 0);

	hBackTitle_ = Image::Load(path + "TitleButton.png");
	assert(hBackTitle_ >= 0);

	hFrameLine_ = Image::Load(path + "LineFrame.png");
	assert(hFrameLine_ >= 0);

	hModeSelect_ = Image::Load(path + "ModeSelect.png");
	assert(hModeSelect_>= 0);

	hBattleText_ = Image::Load(path + "BattleText.png");
	assert(hBattleText_ >= 0);

	hFreePlayText_ = Image::Load(path + "FreeplayText.png"); 
	assert(hFreePlayText_ >= 0);

	hHowtoPlayText_ = Image::Load(path + "HowToPlayText.png");
	assert(hHowtoPlayText_ >= 0);

	hTitleText_ = Image::Load(path + "TitleText.png");
	assert(hTitleText_ >= 0);

	hModeSound_ = Audio::Load("Sound\\maou_game_rock54.wav");
	assert(hModeSound_ >= 0);

	//各テキストハンドルを配列に入れる
	TextArray_ = {hBattleText_, hFreePlayText_, hHowtoPlayText_, hTitleText_};
	
	ModeList_ = { Battle,Practice,HowToPlay,Title };
	itr = ModeList_.begin();
}

void GameModeScene::Update()
{
	if (Input::IsKeyDown(DIK_UP) || Input::GetPadStickL().y >= sticktilt 
		|| Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP))
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
	if (Input::IsKeyDown(DIK_DOWN) || Input::GetPadStickL().y <= -sticktilt
		|| Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
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
		ModeSetTrans[0].position_.y = 0.6f;
		break;
	case GameModeScene::Practice:
		ModeSetTrans[0].position_.y = 0.2f;
		break;
	case GameModeScene::HowToPlay:
		ModeSetTrans[0].position_.y = -0.2f;
		break;
	case GameModeScene::Title:
		ModeSetTrans[0].position_.y = -0.6f;
		break;
	default:
		break;
	}


	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
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
		case GameModeScene::HowToPlay:
			break;
		case GameModeScene::Title:
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
			Audio::Stop(hModeSound_);
			break;
		default:
			break;
		}
		
	}

	BackChara_.rotate_.z -= sheepRotate;

	Audio::Play(hModeSound_);
}

void GameModeScene::Draw()
{
	Image::SetTransform(hBackScreen_, transform_);
	Image::Draw(hBackScreen_);

	Image::SetTransform(hBackChara_, BackChara_);
	Image::Draw(hBackChara_);

	Image::SetTransform(hModeSelect_, Trans_Select_);
	Image::Draw(hModeSelect_);

	Image::SetTransform(hBattle_, ModeSetTrans[1]);
	Image::Draw(hBattle_);

	Image::SetTransform(hPractice_, ModeSetTrans[2]);
	Image::Draw(hPractice_);

	Image::SetTransform(hHowtoPlay_, ModeSetTrans[3]);
	Image::Draw(hHowtoPlay_);

	Image::SetTransform(hBackTitle_, ModeSetTrans[4]);
	Image::Draw(hBackTitle_);

	Image::SetTransform(hFrameLine_, ModeSetTrans[0]);
	Image::Draw(hFrameLine_);

	switch (SelectMode_)
	{
	case GameModeScene::Battle:
	{
		Image::SetTransform(TextArray_[0], Trans_Text_);
		Image::Draw(TextArray_[0]);
	}
		break;
	case GameModeScene::Practice:
	{
		Image::SetTransform(TextArray_[1], Trans_Text_);
		Image::Draw(TextArray_[1]);
	}
		break;
	case GameModeScene::HowToPlay:
	{
		Image::SetTransform(TextArray_[2], Trans_Text_);
		Image::Draw(TextArray_[2]);
	}
		break;
	case GameModeScene::Title:
	{
		Image::SetTransform(TextArray_[3], Trans_Text_);
		Image::Draw(TextArray_[3]);
	}
		break;
	default:
		break;
	}

#ifdef _DEBUG
	ImGui::SliderFloat("text", &Trans_Text_.position_.x, -1.0, 1.0);
	//ImGui::SliderFloat("battle", &ModeSetTrans[1].position_.y, 1.0, -1.0);
	//ImGui::SliderFloat("free", &ModeSetTrans[2].position_.y, 1.0, -1.0);
	//ImGui::SliderFloat("how", &ModeSetTrans[3].position_.y, 1.0, -1.0);
	//ImGui::SliderFloat("title", &ModeSetTrans[4].position_.y, 1.0, -1.0);
#endif
}

void GameModeScene::Release()
{
}

void GameModeScene::SetSCV()
{
	CsvReader csv;
	csv.Load("CSVdata\\GameModeData.csv");

	//選択枠
	ModeSetTrans[0].position_ = { csv.GetValueFloat(1, 1), csv.GetValueFloat(1, 2), csv.GetValueFloat(1, 3) };
	ModeSetTrans[0].rotate_ = { csv.GetValueFloat(1, 4), csv.GetValueFloat(1, 5), csv.GetValueFloat(1, 6) };
	ModeSetTrans[0].scale_ = { csv.GetValueFloat(1, 7), csv.GetValueFloat(1, 8), csv.GetValueFloat(1, 9) };

	//プレイボタン
	ModeSetTrans[1].position_ = { csv.GetValueFloat(2, 1), csv.GetValueFloat(2, 2), csv.GetValueFloat(2, 3) };
	ModeSetTrans[1].rotate_ = { csv.GetValueFloat(2, 4), csv.GetValueFloat(2, 5), csv.GetValueFloat(2, 6) };
	ModeSetTrans[1].scale_ = { csv.GetValueFloat(2, 7), csv.GetValueFloat(2, 8), csv.GetValueFloat(2, 9) };

	//フリープレイボタン
	ModeSetTrans[2].position_ = { csv.GetValueFloat(3, 1), csv.GetValueFloat(3, 2), csv.GetValueFloat(3, 3) };
	ModeSetTrans[2].rotate_ = { csv.GetValueFloat(3, 4), csv.GetValueFloat(3, 5), csv.GetValueFloat(3, 6) };
	ModeSetTrans[2].scale_ = { csv.GetValueFloat(3, 7), csv.GetValueFloat(3, 8), csv.GetValueFloat(3, 9) };

	//遊び方ボタン
	ModeSetTrans[3].position_ = { csv.GetValueFloat(4, 1), csv.GetValueFloat(4, 2), csv.GetValueFloat(4, 3) };
	ModeSetTrans[3].rotate_ = { csv.GetValueFloat(4, 4), csv.GetValueFloat(4, 5), csv.GetValueFloat(4, 6) };
	ModeSetTrans[3].scale_ = { csv.GetValueFloat(4, 7), csv.GetValueFloat(4, 8), csv.GetValueFloat(4, 9) };

	//タイトルボタン
	ModeSetTrans[4].position_ = { csv.GetValueFloat(5, 1), csv.GetValueFloat(5, 2), csv.GetValueFloat(5, 3) };
	ModeSetTrans[4].rotate_ = { csv.GetValueFloat(5, 4), csv.GetValueFloat(5, 5), csv.GetValueFloat(5, 6) };
	ModeSetTrans[4].scale_ = { csv.GetValueFloat(5, 7), csv.GetValueFloat(5, 8), csv.GetValueFloat(5, 9) };

	//"モードセレクト"
	Trans_Select_.position_ = { csv.GetValueFloat(6, 1), csv.GetValueFloat(6, 2), csv.GetValueFloat(6, 3) };
	Trans_Select_.rotate_ = { csv.GetValueFloat(6, 4), csv.GetValueFloat(6, 5), csv.GetValueFloat(6, 6) };
	Trans_Select_.scale_ = { csv.GetValueFloat(6, 7), csv.GetValueFloat(6, 8), csv.GetValueFloat(6, 9) };

	//画面下部のテキスト
	Trans_Text_.position_ = { csv.GetValueFloat(7, 1), csv.GetValueFloat(7, 2), csv.GetValueFloat(7, 3) };
	Trans_Text_.rotate_ = { csv.GetValueFloat(7, 4), csv.GetValueFloat(7, 5), csv.GetValueFloat(7, 6) };
	Trans_Text_.scale_ = { csv.GetValueFloat(7, 7), csv.GetValueFloat(7, 8), csv.GetValueFloat(7, 9) };
}

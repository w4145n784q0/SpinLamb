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
	const int Mode = 4;//モードの数
	std::array<Transform, 5> ModeSetTrans;//各ボタンのトランスフォーム管理配列
}

GameModeScene::GameModeScene(GameObject* parent)
	:GameObject(parent, "GameModeScene"), 
	hBackScreen_(-1), hBackChara_(-1),
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
		ModeSetTrans[0].position_.y = ModeSetTrans[1].position_.y;
		break;
	case GameModeScene::Practice:
		ModeSetTrans[0].position_.y = ModeSetTrans[2].position_.y;
		break;
	case GameModeScene::HowToPlay:
		ModeSetTrans[0].position_.y = ModeSetTrans[3].position_.y;
		break;
	case GameModeScene::Title:
		ModeSetTrans[0].position_.y = ModeSetTrans[4].position_.y;
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
			pSceneManager->ChangeScene(SCENE_ID_HOWTOPLAY);
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

	//選択枠,プレイボタン,フリープレイボタン,遊び方ボタン,タイトルボタン
	std::string ParamArray[] = { "FrameLine","Battle", "Practice", "HowToPlay", "BackTitle"};

    for (int i = 0; i < sizeof(ParamArray) / sizeof(ParamArray[0]); i++)
    {
		if (csv.IsGetParamName(ParamArray[i]))
		{
			std::vector<float> v = csv.GetParam(ParamArray[i]);
			ModeSetTrans[i].position_ = { v[pos_x] , v[pos_y], v[pos_z] };
			ModeSetTrans[i].rotate_ = { v[rot_x], v[rot_y], v[rot_z] };
			ModeSetTrans[i].scale_ = { v[sca_x], v[sca_y], v[sca_z] };
		}
    }


	//"モードセレクト"
	std::string modeselect = "ModeSelect";
	if (csv.IsGetParamName(modeselect))
	{
		std::vector<float> v = csv.GetParam(modeselect);
		Trans_Select_.position_ = { v[pos_x] , v[pos_y], v[pos_z] };
		Trans_Select_.rotate_ = { v[rot_x], v[rot_y], v[rot_z] };
		Trans_Select_.scale_ = { v[sca_x], v[sca_y], v[sca_z] };
	}

	//画面下部のテキスト
	std::string text = "Text";
	if (csv.IsGetParamName(text))
	{
		std::vector<float> v = csv.GetParam(text);
		Trans_Text_.position_ = { v[pos_x], v[pos_y], v[pos_z] };
		Trans_Text_.rotate_ = { v[rot_x], v[rot_y], v[rot_z] };
		Trans_Text_.scale_ = { v[sca_x], v[sca_y], v[sca_z] };
	}
}

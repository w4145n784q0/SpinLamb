#include "GameModeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"Engine/CsvReader.h"

namespace
{
	//プレイボタン,フリープレイボタン,遊び方ボタン,タイトルボタン
	std::string ParamArray[] = { "Battle", "Practice", "HowToPlay", "BackTitle" };
}

GameModeScene::GameModeScene(GameObject* parent)
	:BaseScene(parent, "GameModeScene"), 
	hBackScreen_(-1), hBackChara_(-1),
	hBattle_(-1),hPractice_(-1), hHowtoPlay_(-1),hBackTitle_(-1), hFrameLine_(-1),
	hModeSelect_(-1), hBattleText_(-1), hFreePlayText_(-1), hHowtoPlayText_(-1),hTitleText_(-1),
	TextArray_({}),ButtonArray_({}),
	hSoundGameMode_(-1), hSoundSelect_(-1), hSoundDecide_(-1), SelectMode_(Battle)
{
}

GameModeScene::~GameModeScene()
{
}

void GameModeScene::Initialize()
{
	SetGameModeSCV();
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

	hSoundGameMode_ = Audio::Load("Sound\\BGM\\GameMode.wav",true);
	assert(hSoundGameMode_ >= 0);

	hSoundDecide_ = Audio::Load("Sound\\SE\\decide.wav");
	assert(hSoundDecide_ >= 0);

	hSoundSelect_ = Audio::Load("Sound\\SE\\Select.wav");
	assert(hSoundSelect_ >= 0);

	//各モードのハンドルを配列に入れる
	ButtonArray_ = { hBattle_, hPractice_, hHowtoPlay_, hBackTitle_ };

	//各テキストハンドルを配列に入れる
	TextArray_ = {hBattleText_, hFreePlayText_, hHowtoPlayText_, hTitleText_};
	
	//各モードをリストに入れる
	ModeList_ = { Battle,Practice,HowToPlay,Title };
	itr = ModeList_.begin();
}

void GameModeScene::Update()
{
	BaseScene::Update();
	Audio::Play(hSoundGameMode_);
}

void GameModeScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

	Image::SetTransform(hBackChara_, BackChara_);
	Image::Draw(hBackChara_);

	Image::SetTransform(hModeSelect_, Trans_Select_);
	Image::Draw(hModeSelect_);

	for(int i = 0; i < ModeArray_.size(); i++)
	{
		Image::SetTransform(ButtonArray_[i], ModeArray_[i]);
		Image::Draw(ButtonArray_[i]);
	}

	switch (SelectMode_)
	{
	case GameModeScene::Battle:
	case GameModeScene::Practice:
	case GameModeScene::HowToPlay:
	case GameModeScene::Title:
	{
		Image::SetTransform(TextArray_[SelectMode_], Trans_Text_);
		Image::Draw(TextArray_[SelectMode_]);
	}
		break;
	default:
		break;
	}

	Image::SetTransform(hFrameLine_, Trans_Frame_);
	Image::Draw(hFrameLine_);

#ifdef _DEBUG
#endif
}

void GameModeScene::Release()
{
}

void GameModeScene::SetGameModeSCV()
{
	CsvReader csv;
	csv.Load("CSVdata\\GameModeData.csv");

	//選択枠
	std::string frame = "FrameLine";
	if (csv.IsGetParamName(frame))
	{
		std::vector<float> v = csv.GetParam(frame);
		SetTransformPRS(Trans_Frame_, v);
	}


    for (int i = 0; i < sizeof(ParamArray) / sizeof(ParamArray[0]); i++)
    {
		if (csv.IsGetParamName(ParamArray[i]))
		{
			std::vector<float> v = csv.GetParam(ParamArray[i]);
			SetTransformPRS(ModeArray_[i], v);
		}
    }


	//"モードセレクト"
	std::string modeselect = "ModeSelect";
	if (csv.IsGetParamName(modeselect))
	{
		std::vector<float> v = csv.GetParam(modeselect);
		SetTransformPRS(Trans_Select_, v);
	}

	//画面下部のテキスト
	std::string text = "Text";
	if (csv.IsGetParamName(text))
	{
		std::vector<float> v = csv.GetParam(text);
		SetTransformPRS(Trans_Text_, v);
	}
}

void GameModeScene::UpdateSelect()
{
	if (Input::IsKeyDown(DIK_UP) /*|| Input::GetPadStickL().y >= Input::StickTilt*/
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
		Audio::Play(hSoundSelect_);
	}
	if (Input::IsKeyDown(DIK_DOWN) /*|| Input::GetPadStickL().y <= -Input::StickTilt*/
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
		Audio::Play(hSoundSelect_);
	}

	switch (SelectMode_)
	{
		case GameModeScene::Battle:
		case GameModeScene::Practice:
		case GameModeScene::HowToPlay:
		case GameModeScene::Title:
			Trans_Frame_.position_.y = ModeArray_[SelectMode_].position_.y;
			break;
		default:
			break;
	}

	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		Audio::Play(hSoundDecide_);
		ModeDecide_ = Decided;
	}
}

void GameModeScene::UpdateDecide()
{
	if(++SceneTransitionTimer_ > SceneTransition)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		switch (SelectMode_)
		{
		case GameModeScene::Battle:
			pSceneManager->ChangeScene(SCENE_ID_BATTLE);
			break;
		case GameModeScene::Practice:
			pSceneManager->ChangeScene(SCENE_ID_PRACTICE);
			break;
		case GameModeScene::HowToPlay:
			pSceneManager->ChangeScene(SCENE_ID_HOWTOPLAY);
			break;
		case GameModeScene::Title:
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
			break;
		default:
			break;
		}

		SceneTransitionTimer_ = 0;
		Audio::Stop(hSoundGameMode_);
		ModeDecide_ = Selected;
	}
}

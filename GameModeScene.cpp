#include "GameModeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"Engine/CsvReader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

GameModeScene::GameModeScene(GameObject* parent)
	:GameObject(parent, "GameModeScene"), 
	hBackScreen_(-1), hBackChara_(-1),
	hBattle_(-1),hPractice_(-1), hHowtoPlay_(-1),hBackTitle_(-1), hFrameLine_(-1),
	hModeSelect_(-1), hBattleText_(-1), hFreePlayText_(-1), hHowtoPlayText_(-1),hTitleText_(-1),
	TextArray_({}),ButtonArray_({}), hSoundGameMode_(-1), SelectMode_(Battle)
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

	hSoundGameMode_ = Audio::Load("Sound\\BGM\\GameMode.wav",true);
	assert(hSoundGameMode_ >= 0);

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
	int i = StickTilt; //スティックの傾き閾値
	if (Input::IsKeyDown(DIK_UP) || Input::GetPadStickL().y >= StickTilt 
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
	if (Input::IsKeyDown(DIK_DOWN) || Input::GetPadStickL().y <= -StickTilt
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
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		switch (SelectMode_)
		{
		case GameModeScene::Battle:
			pSceneManager->ChangeScene(SCENE_ID_BATTLE);
			Audio::Stop(hSoundGameMode_);
			break;
		case GameModeScene::Practice:
			pSceneManager->ChangeScene(SCENE_ID_PRACTICE);
			Audio::Stop(hSoundGameMode_);
			break;
		case GameModeScene::HowToPlay:
			pSceneManager->ChangeScene(SCENE_ID_HOWTOPLAY);
			Audio::Stop(hSoundGameMode_);
			break;
		case GameModeScene::Title:
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
			Audio::Stop(hSoundGameMode_);
			break;
		default:
			break;
		}
		
	}
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
	//ImGui::SliderFloat("text", &Trans_Text_.position_.x, -1.0, 1.0);
	//ImGui::SliderFloat("battle", &ModeArray_[1].position_.y, 1.0, -1.0);
	//ImGui::SliderFloat("free", &ModeArray_[2].position_.y, 1.0, -1.0);
	//ImGui::SliderFloat("how", &ModeArray_[3].position_.y, 1.0, -1.0);
	//ImGui::SliderFloat("title", &ModeArray_[4].position_.y, 1.0, -1.0);
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
	std::string frame = "FrameLine";
	if (csv.IsGetParamName(frame))
	{
		std::vector<float> v = csv.GetParam(frame);
		SetTransformPRS(Trans_Frame_, v);
	}

	//プレイボタン,フリープレイボタン,遊び方ボタン,タイトルボタン
	std::string ParamArray[] = {"Battle", "Practice", "HowToPlay", "BackTitle"};

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

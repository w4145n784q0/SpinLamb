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
	TextArray_({}),ButtonImageArray_({}), ModeTransArray_({}),
	hSoundGameMode_(-1), hSoundSelect_(-1), hSoundDecide_(-1), SelectMode_(S_Battle),
	pTransitionEffect_(nullptr)
{
}

GameModeScene::~GameModeScene()
{
}

void GameModeScene::Initialize()
{
	//各クラス生成
	Instantiate<TransitionEffect>(this);

	//csvからパラメータ読み込み
	SetGameModeSCV();

	//各画像・サウンドの読み込み

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
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

	//各モードの画像ハンドルを配列に入れる
	ButtonImageArray_ = { hBattle_, hPractice_, hHowtoPlay_, hBackTitle_ };

	//各テキスト画像ハンドルを配列に入れる
	TextArray_ = {hBattleText_, hFreePlayText_, hHowtoPlayText_, hTitleText_};
	
	//各モードをリストに入れる
	ModeList_ = { S_Battle,S_Practice,S_HowToPlay,S_Title };

	//インデックスの初期位置を指定
	itr = ModeList_.begin();

	//インスタンス生成
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

	//モード選択用サウンド再生
	Audio::Play(hSoundGameMode_);
}

void GameModeScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTranslationは継承先の関数が呼ばれる
	BaseScene::Update();
}

void GameModeScene::Draw()
{
	//背景描画
	Image::SetAndDraw(hBackScreen_, this->transform_);

	//背景のキャラクターの影描画
	Image::SetAndDraw(hBackChara_, BackChara_);

	//モードセレクトのテキスト画像描画
	Image::SetAndDraw(hModeSelect_, TransSelect_);

	//各モードのボタン画像描画
	for(int i = 0; i < ModeTransArray_.size(); i++)
	{
		Image::SetAndDraw(ButtonImageArray_[i], ModeTransArray_[i]);
		//Image::Draw(ButtonImageArray_[i]);
	}

	//選択中のテキストの描画(現在選択しているモードによって変化)
	switch (SelectMode_)
	{
	case GameModeScene::S_Battle:
	case GameModeScene::S_Practice:
	case GameModeScene::S_HowToPlay:
	case GameModeScene::S_Title:
	{
		Image::SetAndDraw(TextArray_[SelectMode_], TransText_);
		//Image::Draw(TextArray_[SelectMode_]);
	}
		break;
	default:
		break;
	}

	//選択枠の描画
	Image::SetAndDraw(hFrameLine_, TransFrame_);

#ifdef _DEBUG
	//各画像トランスフォームの位置変更

	ImGui::SliderFloat("BackCharaX", &BackChara_.position_.x, Image::LeftEdge, Image::RightEdge);
	ImGui::SliderFloat("BackCharaY", &BackChara_.position_.y, Image::UpEdge, Image::DownEdge);

	ImGui::SliderFloat("TransSelectX", &TransSelect_.position_.x, Image::LeftEdge, Image::RightEdge);
	ImGui::SliderFloat("TransSelectY", &TransSelect_.position_.y, Image::UpEdge, Image::DownEdge);

	ImGui::SliderFloat("TransTextX", &TransText_.position_.x, Image::LeftEdge, Image::RightEdge);
	ImGui::SliderFloat("TransTextY", &TransText_.position_.y, Image::UpEdge, Image::DownEdge);

	for (int i = 0; i < ModeTransArray_.size(); i++)
	{
		ImGui::SliderFloat((ParamArray[i] + "X").c_str(), &ModeTransArray_[i].position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat((ParamArray[i] + "Y").c_str(), &ModeTransArray_[i].position_.y, Image::UpEdge, Image::DownEdge);
	}

#endif
}

void GameModeScene::Release()
{
}

void GameModeScene::SetGameModeSCV()
{
	//各画像を表示する際のトランスフォーム初期化
	CsvReader csv;
	csv.Load("CSVdata\\GameModeData.csv");

	//選択枠
	InitCSVTransform(csv, "FrameLine", TransFrame_);

	//各ボタン
    for (int i = 0; i < sizeof(ParamArray) / sizeof(ParamArray[0]); i++)
    {
		InitCSVTransform(csv, ParamArray[i], ModeTransArray_[i]);
    }

	//"モードセレクト"
	InitCSVTransform(csv, "ModeSelect", TransSelect_);

	//画面下部のテキスト
	InitCSVTransform(csv, "Text", TransText_);
}

void GameModeScene::UpdateActive()
{

	//ボタンの選択枠の移動
	//インデックスが先頭/末尾なら末尾/先頭へ戻る
	//前置デクリメントで配列オーバー防ぐ

	if (Input::IsKeyDown(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP))
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
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
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

	//選択枠の位置を選択中のモードに合わせる
	switch (SelectMode_)
	{
	case GameModeScene::S_Battle:
	case GameModeScene::S_Practice:
	case GameModeScene::S_HowToPlay:
	case GameModeScene::S_Title:
		TransFrame_.position_.y = ModeTransArray_[SelectMode_].position_.y;
		break;
	default:
		break;
	}

	////決定ボタン(Pキー・B/Startボタン)を押したらシーン遷移状態へ
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//決定音を再生
		Audio::Play(hSoundDecide_);
		SceneState_ = S_Transition;

		//シーン遷移エフェクト(ズームイン)を設定
		pTransitionEffect_->ZoomInStart();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);
	}
}

void GameModeScene::UpdateTransition()
{
	//時間経過で次のシーンに遷移
	//カウント中はシーン遷移エフェクト行う

	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManagerインスタンスから選択しているシーンへ遷移する

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		switch (SelectMode_)
		{
		case GameModeScene::S_Battle:
			pSceneManager->ChangeScene(SCENE_ID_BATTLE);
			break;
		case GameModeScene::S_Practice:
			pSceneManager->ChangeScene(SCENE_ID_PRACTICE);
			break;
		case GameModeScene::S_HowToPlay:
			pSceneManager->ChangeScene(SCENE_ID_HOWTOPLAY);
			break;
		case GameModeScene::S_Title:
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
			break;
		default:
			break;
		}

		//シーン遷移用タイマーを戻す
		SceneTransitionTimer_ = 0;

		//モード選択用サウンド停止
		Audio::Stop(hSoundGameMode_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
	}
}

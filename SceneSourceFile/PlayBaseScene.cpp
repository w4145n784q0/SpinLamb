#include "PlayBaseScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"

namespace
{
	//csv読み込み時のインデックス(プレイベースシーン用の変数)
	enum EasingIndex
	{
		i_IconPosYContinue = 0,
		i_IconPosYExit,
	};
}

PlayBaseScene::PlayBaseScene(GameObject* parent)
	:BaseScene(parent, "PlayBaseScene"), hBackScreen_(-1), PauseSelect_(S_Continue),
	TmpIconPosY_(0.0f), IconPosYArray({}),
	hSoundPause_(-1),hSoundSelect_(-1),hSoundExit_(-1)
{
}

PlayBaseScene::PlayBaseScene(GameObject* parent, const std::string& name)
	:BaseScene(parent, name), hBackScreen_(-1), PauseSelect_(S_Continue),
	TmpIconPosY_(0.0f), IconPosYArray({}),
	hSoundPause_(-1), hSoundSelect_(-1), hSoundExit_(-1)
{
}

PlayBaseScene::~PlayBaseScene()
{
}

void PlayBaseScene::Initialize()
{
	//csvからパラメータ読み込み
	SetPlayBaseSceneCSV();

	//各画像・サウンドの読み込み
	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string Play = "Image\\Play\\";
	std::string Sound = "Sound\\";
	std::string SE = "SE\\";

	hBackScreen_ = Image::Load(Play + "BackSky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundPause_ = Audio::Load(Sound + SE + "Pause.wav", false, Audio::SelectSoundNum_);
	assert(hSoundPause_ >= 0);

	hSoundSelect_ = Audio::Load(Sound + SE + "Select.wav", false, Audio::SelectSoundNum_);
	assert(hSoundSelect_ >= 0);

	hSoundExit_ = Audio::Load(Sound + SE + "Cancel.wav", false, Audio::SelectSoundNum_);
	assert(hSoundExit_ >= 0);

	//各モードをリストに入れる
	PauseSelectList_ = { S_Continue,S_Exit };

	//Pauseitrの初期位置を指定
	Pauseitr = PauseSelectList_.begin();

}

void PlayBaseScene::SetPlayBaseSceneCSV()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\PlayData.csv");

	//csvファイル(PlayData.csv)の0列目の文字列を取得
	std::string  PlayName= "Play";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> PlayData = GetCSVReadData(csv, PlayName);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	IconPosYArray = { PlayData[i_IconPosYContinue], PlayData[i_IconPosYExit] };
}

void PlayBaseScene::DrawBackScreen()
{
	//背景描画
	Image::SetAndDraw(hBackScreen_, TransBackScreen_);
}

void PlayBaseScene::UpdatePauseMenu()
{
	//ポーズ画面中の操作を行う
	if (Input::IsKeyDown(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::IsStickTiltLY_UP())
	{
		if (Pauseitr == PauseSelectList_.begin())
		{
			Pauseitr = --PauseSelectList_.end();
		}
		else
		{
			--Pauseitr;
		}
		PauseSelect_ = *Pauseitr;

		//選択SE再生
		Audio::Play(hSoundSelect_);

	}
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::IsStickTiltLY_DOWN())
	{
		if (Pauseitr == --PauseSelectList_.end())
		{
			Pauseitr = PauseSelectList_.begin();
		}
		else
		{
			++Pauseitr;
		}
		PauseSelect_ = *Pauseitr;

		//選択SE再生
		Audio::Play(hSoundSelect_);
	}

	//選択アイコンの位置を調整
	switch (PauseSelect_)
	{
	case PlayBaseScene::S_Continue:
		TmpIconPosY_ = IconPosYArray[S_Continue];
		break;
	case PlayBaseScene::S_Exit:
		TmpIconPosY_ = IconPosYArray[S_Exit];
		break;
	default:
		break;
	}

	//TmpIconPosY_の位置をHUDに渡す(処理は派生先で記述、ここではポインタは持たない)
	SetPauseIconY();

	//決定ボタン(Pキー・Bボタン)を押したら
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		
		if (PauseSelect_ == S_Continue)
		{
			//"ゲームをつづける"を選択中なら即座に戻る
			SceneState_ = S_Active;

			//ポーズ画面SE再生
			Audio::Play(hSoundPause_);

			//プレイ画面に行く際の処理(仮想関数なので派生先で処理を追加する)
			GotoPlay();
		}
		if (PauseSelect_ == S_Exit)
		{
			//"ゲームをやめる"を選択中なら遷移状態へ
			SceneState_ = S_Transition;

			//タイトルに戻るSE再生
			Audio::Play(hSoundExit_);

			//タイトルに戻る際の処理(仮想関数なので派生先で処理を追加する)
			GotoTitle();
		}
	}

	//S_Activeへの遷移を待つ(キーボードかボタン操作によって遷移)
	WaitGotoPlay();
}

void PlayBaseScene::WaitGotoPause()
{
	//escキーかstartボタンでポーズ画面へ
	if (Input::IsKeyUp(DIK_ESCAPE) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//シーンをポーズ状態にする(ここは共通)
		SceneState_ = S_InActive;

		//ポーズ画面SE再生
		Audio::Play(hSoundPause_);

		//ポーズ画面に行く際の処理(仮想関数なので派生先で処理を追加する)
		GotoPause();
	}
}

void PlayBaseScene::WaitGotoPlay()
{
	//ESCキー・Startボタンを押したらActiveに戻る
	if (Input::IsKeyUp(DIK_ESCAPE) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//シーンを通常状態にする(ここは共通)
		SceneState_ = S_Active;

		//ポーズ画面SE再生
		Audio::Play(hSoundPause_);

		//プレイ画面に行く際の処理(仮想関数なので派生先で処理を追加する)
		GotoPlay();
	}
}



#include "PlayScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"

namespace
{
	//csv読み込み時のインデックス(プレイシーン用の変数)
	enum EasingIndex
	{
		i_IconPosYContinue = 0,
		i_IconPosYExit,
	};
}

PlayScene::PlayScene(GameObject* parent)
	:BaseScene(parent, "PlayScene"), hBackScreen_(-1), PauseSelect_(S_Continue), IconPosYArray({})
{
}

PlayScene::PlayScene(GameObject* parent, const std::string& name)
	:BaseScene(parent, name), hBackScreen_(-1), PauseSelect_(S_Continue), IconPosYArray({})
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	//各画像・サウンドの読み込み
	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string Play = "Image\\Play\\";

	hBackScreen_ = Image::Load("Image\\Battle\\BackSky.jpg");
	assert(hBackScreen_ >= 0);

	//各モードをリストに入れる
	PauseSelectList_ = { S_Continue,S_Exit };

	//Pauseitrの初期位置を指定
	Pauseitr = PauseSelectList_.begin();

}

void PlayScene::SetPlaySceneCSV()
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

void PlayScene::DrawBackScreen()
{
	//背景描画
	Image::SetAndDraw(hBackScreen_, TransBackScreen_);
}

void PlayScene::UpdatePauseMenu()
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

	}

	//選択アイコンの位置を調整
	switch (PauseSelect_)
	{
	case PlayScene::S_Continue:
		TmpIconPos_.y = IconPosYArray[S_Continue];
		break;
	case PlayScene::S_Exit:
		TmpIconPos_.y = IconPosYArray[S_Exit];
		break;
	default:
		break;
	}

	//決定ボタン(Pキー・B/Startボタン)を押したら
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		
		if (PauseSelect_ == S_Continue)
		{
			//"ゲームをつづける"を選択中なら即座に戻る
			SceneState_ = S_Active;
		}
		if (PauseSelect_ == S_Exit)
		{
			//"ゲームをやめる"を選択中なら遷移状態へ
			SceneState_ = S_Transition;

			//継承先によって異なる遷移エフェクトを呼ぶ
			SetTransitionEffect();
		}
	}
}



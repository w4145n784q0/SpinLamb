#include "GameModeScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"
#include"../Engine/SceneManager.h"
#include"../Engine/CsvReader.h"
#include"../Engine/Global.h"
#include"../ViewSourceFile/GameView.h"

namespace
{
	//プレイボタン,フリープレイボタン,遊び方ボタン,タイトルボタン
	//クラス内で使いまわす文字列なのでnamespaceに宣言
	std::string ModeStringArray[] = { "Battle", "Practice", "HowToPlay", "BackTitle" };
}

GameModeScene::GameModeScene(GameObject* parent)
	:BaseScene(parent, "GameModeScene"), 
	//----------状態遷移----------
	SelectMode_(S_Battle), GameModeState_(S_Selecting), PlayerNum_(S_PvE),
	
	//----------背景----------
	hBackScreen_(-1), hBackChara_(-1),

	//----------ボタン----------
	hBattle_(-1),hPractice_(-1), hHowtoPlay_(-1),hBackTitle_(-1), hFrameLine_(-1),
	ModeTransArray_({}), ButtonImageArray_({}), 

	//----------ロゴ・テキスト----------
	hModeSelect_(-1), hBattleText_(-1), hFreePlayText_(-1), hHowtoPlayText_(-1),hTitleText_(-1),
	hPlayerNumSelect_(-1), hPlayerSelectIcon_(-1), 
	TextArray_({}), PlayerTransArray_({}), 

	//----------サウンド・ポインタ----------
	hSoundGameMode_(-1), hSoundSelect_(-1), hSoundDecide_(-1), hSoundCancel_(-1),
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
	SetGameModeCSV();

	//各画像・サウンドの読み込み

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string GameMode = "Image\\GameMode\\";
	std::string Sound = "Sound\\";
	std::string BGM = "BGM\\";
	std::string SE = "SE\\";

	hBackScreen_ = Image::Load(GameMode + "GameModeScreen.jpg");
	assert(hBackScreen_ >= 0);

	hBackChara_ = Image::Load(GameMode + "SheepImage.png");
	assert(hBackChara_ >= 0);

	hBattle_ = Image::Load(GameMode + "BattleButton.png");
	assert(hBattle_ >= 0);

	hPractice_ = Image::Load(GameMode + "FreeplayButton.png");
	assert(hPractice_ >= 0);

	hHowtoPlay_ = Image::Load(GameMode + "HowToPlayButton.png");
	assert(hHowtoPlay_ >= 0);

	hBackTitle_ = Image::Load(GameMode + "TitleButton.png");
	assert(hBackTitle_ >= 0);

	hFrameLine_ = Image::Load(GameMode + "LineFrame.png");
	assert(hFrameLine_ >= 0);

	hModeSelect_ = Image::Load(GameMode + "ModeSelect.png");
	assert(hModeSelect_>= 0);

	hBattleText_ = Image::Load(GameMode + "BattleText.png");
	assert(hBattleText_ >= 0);

	hFreePlayText_ = Image::Load(GameMode + "FreeplayText.png");
	assert(hFreePlayText_ >= 0);

	hHowtoPlayText_ = Image::Load(GameMode + "HowToPlayText.png");
	assert(hHowtoPlayText_ >= 0);

	hTitleText_ = Image::Load(GameMode + "TitleText.png");
	assert(hTitleText_ >= 0);

	hPlayerNumSelect_ = Image::Load(GameMode + "SelectPlayer.png");
	assert(hPlayerNumSelect_ >= 0);

	hPlayerSelectIcon_ = Image::Load(GameMode + "SelectIcon.png");
	assert(hPlayerSelectIcon_ >= 0);

	hSoundGameMode_ = Audio::Load(Sound + BGM + "GameMode.wav",true);
	assert(hSoundGameMode_ >= 0);

	hSoundDecide_ = Audio::Load(Sound + SE + "Decide.wav", false, Audio::DecideSoundNum_);
	assert(hSoundDecide_ >= 0);

	hSoundSelect_ = Audio::Load(Sound + SE + "Select.wav", false, Audio::SelectSoundNum_);
	assert(hSoundSelect_ >= 0);

	hSoundCancel_ = Audio::Load(Sound + SE + "Cancel.wav", false, Audio::CancelSoundNum_);
	assert(hSoundCancel_ >= 0);

	//各モードの画像ハンドルを配列に入れる
	ButtonImageArray_ = { hBattle_, hPractice_, hHowtoPlay_, hBackTitle_ };

	//各テキスト画像ハンドルを配列に入れる
	TextArray_ = {hBattleText_, hFreePlayText_, hHowtoPlayText_, hTitleText_};
	
	//各モードをリストに入れる
	ModeList_ = { S_Battle,S_Practice,S_HowToPlay,S_Title };

	//Modeitrの初期位置を指定
	Modeitr = ModeList_.begin();

	//PlayerNumの状態リストを初期化
	PlayerNumList_ = { S_PvE,S_PvP };

	//PlayerNumList_のインデックスの初期位置を指定
	PlayerNumitr = PlayerNumList_.begin();

	//ポインタを取得
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
	assert(pTransitionEffect_ != nullptr);

	//GameViewにポインタを渡す
	GameView::SetTransitionEffect(pTransitionEffect_);
	SetPointerGameView();

	//モード選択用サウンド再生
	Audio::Play(hSoundGameMode_);
}

void GameModeScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTransitionは継承先の関数が呼ばれる
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
	}

	//モード選択枠の描画
	Image::SetAndDraw(hFrameLine_, TransFrame_);

	//選択中のテキストの描画(現在選択しているモードによって変化)
	switch (SelectMode_)
	{
	case GameModeScene::S_Battle:
	case GameModeScene::S_Practice:
	case GameModeScene::S_HowToPlay:
	case GameModeScene::S_Title:
	{
		Image::SetAndDraw(TextArray_[SelectMode_], TransText_);
	}
		break;
	default:
		break;
	}

	//確認画面ではあそぶ人数を選択する画像描画
	if (GameModeState_ == GameModeScene::S_Confirmation)
	{
		switch (SelectMode_)
		{
		case GameModeScene::S_Battle:
		case GameModeScene::S_Practice:
		{
			Image::SetAndDraw(hPlayerNumSelect_, TransPlayer_);
			Image::SetAndDraw(hPlayerSelectIcon_, TransSelectPlayerIcon_);
			//選択している状態に合わせてアイコンつける
		}
			break;
		case GameModeScene::S_HowToPlay:
		case GameModeScene::S_Title:
		default:
			break;
		}
	}
}

void GameModeScene::Release()
{

}

void GameModeScene::SetGameModeCSV()
{
	//各画像を表示する際のトランスフォーム初期化
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\GameModeTransformData.csv");

	//選択枠のトランスフォーム初期化
	InitCSVTransform(csv, "FrameLine", TransFrame_);

	//各ボタンのトランスフォーム初期化
    for (int i = 0; i < MaxMode; i++)
    {
		InitCSVTransform(csv, ModeStringArray[i], ModeTransArray_[i]);
    }

	//"モードセレクト"のトランスフォーム初期化
	InitCSVTransform(csv, "ModeSelect", TransSelect_);

	//画面下部のテキストのトランスフォーム初期化
	InitCSVTransform(csv, "Text", TransText_);

	//プレイ人数選択画面のトランスフォーム初期化
	InitCSVTransform(csv, "PlayerNum", TransPlayer_);

	//PvE(一人プレイ),PvP(二人プレイ)
	std::string PlayerStringArray[] = { "PvE","PvP" };

	//プレイ人数選択画面のアイコン位置を初期化
	for (int i = 0; i < MaxPlayerNum; i++)
	{
		InitCSVTransform(csv, PlayerStringArray[i], PlayerTransArray_[i]);
	}
}

void GameModeScene::UpdateSelecting()
{
	//ボタンの選択枠の移動
	//インデックスが先頭/末尾の場合、末尾/先頭へ戻る
	//前置デクリメントで配列オーバー防ぐ

	if (Input::IsKeyDown(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::IsStickTiltLY_UP())
	{

		if (Modeitr == ModeList_.begin())
		{
			Modeitr = --ModeList_.end();
		}
		else
		{
			--Modeitr;
		}
		SelectMode_ = *Modeitr;

		//選択SE再生
		Audio::Play(hSoundSelect_);
	}
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::IsStickTiltLY_DOWN())
	{
		if (Modeitr == --ModeList_.end())
		{
			Modeitr = ModeList_.begin();
		}
		else
		{
			++Modeitr;
		}
		SelectMode_ = *Modeitr;

		//選択SE再生
		Audio::Play(hSoundSelect_);
	}

	//選択枠の位置を、選択中のモードの位置に合わせる
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

	//決定ボタン(Pキー・B/Startボタン)を押したら確認画面へ
	//あそびかたシーン選択ならシーン遷移状態へ
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B)
		|| Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//決定音を再生
		Audio::Play(hSoundDecide_);


		if (SelectMode_ == S_HowToPlay || SelectMode_ == S_Title)
		{
			//UpdateTransitionへ遷移
			SceneState_ = S_Transition;

			//シーン遷移エフェクト(ズームイン)を設定
			pTransitionEffect_->ZoomInStart();
			pTransitionEffect_->SetTransitionTime(SceneShortTransition);
		}
		else
		{
			//確認画面へ遷移
			GameModeState_ = S_Confirmation;
		}
	}
}

void GameModeScene::UpdateConfirmation()
{
	//ボタンの選択枠の移動
	//インデックスが先頭/末尾の場合、末尾/先頭へ戻る
	//前置デクリメントで配列オーバー防ぐ

	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::IsStickTiltLX_RIGHT())
	{
		if (PlayerNumitr == PlayerNumList_.begin())
		{
			PlayerNumitr = --PlayerNumList_.end();
		}
		else
		{
			--PlayerNumitr;
		}
		PlayerNum_ = *PlayerNumitr;

		//選択SE再生
		Audio::Play(hSoundSelect_);
	}
	if (Input::IsKeyDown(DIK_LEFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::IsStickTiltLX_LEFT())
	{
		if (PlayerNumitr == --PlayerNumList_.end())
		{
			PlayerNumitr = PlayerNumList_.begin();
		}
		else
		{
			++PlayerNumitr;
		}
		PlayerNum_ = *PlayerNumitr;

		//選択SE再生
		Audio::Play(hSoundSelect_);
	}

	//プレイ人数選択アイコンの位置を調整
	switch (PlayerNum_)
	{
	case GameModeScene::S_PvE:
		TransSelectPlayerIcon_.position_.x = PlayerTransArray_[S_PvE].position_.x;
		break;
	case GameModeScene::S_PvP:
		TransSelectPlayerIcon_.position_.x = PlayerTransArray_[S_PvP].position_.x;
		break;
	default:
		break;
	}


	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//決定音を再生
		Audio::Play(hSoundDecide_);

		//UpdateTransitionへ遷移
		SceneState_ = S_Transition;

		//シーン遷移エフェクト(ズームイン)を設定
		pTransitionEffect_->ZoomInStart();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);

		//SceneManagerのポインタを取得し
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		assert(pSceneManager != nullptr);

		//SceneManagerに遊ぶ人数を渡す
		switch (PlayerNum_)
		{
		case GameModeScene::S_PvE:
		{
			pSceneManager->SetPvEMode();
		}
			break;
		case GameModeScene::S_PvP:
		{
			pSceneManager->SetPvPMode();	
		}
			break;
		default:
			break;
		}

	}

	if (Input::IsKeyUp(DIK_ESCAPE) || Input::IsPadButtonUp(XINPUT_GAMEPAD_A))
	{
		//キャンセル音再生
		Audio::Play(hSoundCancel_);

		//選択画面へ遷移
		GameModeState_ = S_Selecting;
	}
}

void GameModeScene::UpdateActive()
{
	switch (GameModeState_)
	{
	case GameModeScene::S_Selecting:
		UpdateSelecting();
		break;
	case GameModeScene::S_Confirmation:
		UpdateConfirmation();
		break;
	default:
		break;
	}
}

void GameModeScene::UpdateTransition()
{
	//時間経過で次のシーンに遷移
	//カウント中はシーン遷移エフェクト行う

	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManagerポインタから選択しているシーンへ遷移する

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		assert(pSceneManager != nullptr);

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

		//シーン遷移用タイマーをリセット
		SceneTransitionTimer_ = 0;

		//モード選択用サウンド停止
		Audio::Stop(hSoundGameMode_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
	}
}

void GameModeScene::DrawImGuiMyScene()
{
#ifdef _DEBUG
	//各画像のトランスフォームの位置変更
	if (ImGui::TreeNode("GameModeScene"))
	{
		//背景のキャラクター
		if (ImGui::TreeNode("BackChara"))
		{
			if (ImGui::TreeNode("BackCharaPosition"))
			{
				ImGui::SliderFloat("BackCharaPositionX", &BackChara_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("BackCharaPositionY", &BackChara_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("BackCharaRotate"))
			{
				ImGui::SliderFloat("BackCharaRotateX", &BackChara_.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("BackCharaRotateY", &BackChara_.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("BackCharaRotateZ", &BackChara_.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("BackCharaScale"))
			{
				ImGui::InputFloat("BackCharaScaleX", &BackChara_.scale_.x, ZERO_POINT_ONE);
				ImGui::InputFloat("BackCharaScaleY", &BackChara_.scale_.y, ZERO_POINT_ONE);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//"モードセレクト"
		if (ImGui::TreeNode("TransSelect"))
		{
			if (ImGui::TreeNode("TransSelectPosition"))
			{
				ImGui::SliderFloat("TransSelectPositionX", &TransSelect_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("TransSelectPositionY", &TransSelect_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransSelectRotate"))
			{
				ImGui::SliderFloat("TransSelectRotateX", &TransSelect_.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("TransSelectRotateY", &TransSelect_.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("TransSelectRotateZ", &TransSelect_.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransSelectScale"))
			{
				ImGui::InputFloat("TransSelectScaleX", &TransSelect_.scale_.x, ZERO_POINT_ONE);
				ImGui::InputFloat("TransSelectScaleY", &TransSelect_.scale_.y, ZERO_POINT_ONE);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//画面下部の説明テキスト
		if (ImGui::TreeNode("TransText"))
		{
			if (ImGui::TreeNode("TransTextPosition"))
			{
				ImGui::SliderFloat("TransTextPositionX", &TransText_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("TransTextPositionY", &TransText_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransTextRotate"))
			{
				ImGui::SliderFloat("TransTextRotateX", &TransText_.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("TransTextRotateY", &TransText_.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("TransTextRotateZ", &TransText_.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransTextScale"))
			{
				ImGui::InputFloat("TransTextScaleX", &TransText_.scale_.x, ZERO_POINT_ONE);
				ImGui::InputFloat("TransTextScaleY", &TransText_.scale_.y, ZERO_POINT_ONE);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//各モードのボタン
		if (ImGui::TreeNode("ModeTransArray"))
		{
			if (ImGui::TreeNode("ModeTransArrayPosition"))
			{
				for (int i = 0; i < ModeTransArray_.size(); i++)
				{
					ImGui::SliderFloat((ModeStringArray[i] + "X").c_str(), &ModeTransArray_[i].position_.x, Image::LeftEdge, Image::RightEdge);
					ImGui::SliderFloat((ModeStringArray[i] + "Y").c_str(), &ModeTransArray_[i].position_.y, Image::UpEdge, Image::DownEdge);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("ModeTransArrayRotate"))
			{
				for (int i = 0; i < ModeTransArray_.size(); i++)
				{
					ImGui::SliderFloat((ModeStringArray[i] + "X").c_str(), &ModeTransArray_[i].rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
					ImGui::SliderFloat((ModeStringArray[i] + "Y").c_str(), &ModeTransArray_[i].rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
					ImGui::SliderFloat((ModeStringArray[i] + "Z").c_str(), &ModeTransArray_[i].rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("ModeTransArrayScale"))
			{
				for (int i = 0; i < ModeTransArray_.size(); i++)
				{
					ImGui::InputFloat((ModeStringArray[i] + "X").c_str(), &ModeTransArray_[i].scale_.x, ZERO_POINT_ONE);
					ImGui::InputFloat((ModeStringArray[i] + "Y").c_str(), &ModeTransArray_[i].scale_.y, ZERO_POINT_ONE);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	//遊ぶ人数確認画面
	if (ImGui::TreeNode("Confirmation"))
	{
		if (ImGui::TreeNode("TransPlayer"))
		{
			if (ImGui::TreeNode("TransPlayerPosition"))
			{
				ImGui::SliderFloat("TransPlayerPositionX", &TransPlayer_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("TransPlayerPositionY", &TransPlayer_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransPlayerRotate"))
			{
				ImGui::SliderFloat("TransPlayerRotateX", &TransPlayer_.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("TransPlayerRotateY", &TransPlayer_.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("TransPlayerRotateZ", &TransPlayer_.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransPlayerScale"))
			{
				ImGui::InputFloat("TransPlayerScaleX", &TransPlayer_.scale_.x, ZERO_POINT_ONE);
				ImGui::InputFloat("TransPlayerScaleY", &TransPlayer_.scale_.y, ZERO_POINT_ONE);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//プレイ人数選択選択中のアイコン
		if (ImGui::TreeNode("TransSelectPlayerIcon"))
		{
			if (ImGui::TreeNode("TransSelectPlayerPosition"))
			{
				ImGui::SliderFloat("TransSelectPlayerPositionX", &TransSelectPlayerIcon_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("TransSelectPlayerPositionY", &TransSelectPlayerIcon_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransSelectPlayerRotate"))
			{
				ImGui::SliderFloat("TransSelectPlayerRotateX", &TransSelectPlayerIcon_.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("TransSelectPlayerRotateY", &TransSelectPlayerIcon_.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("TransSelectPlayerRotateZ", &TransSelectPlayerIcon_.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransSelectPlayerScale"))
			{
				ImGui::InputFloat("TransSelectPlayerScaleX", &TransSelectPlayerIcon_.scale_.x, ZERO_POINT_ONE);
				ImGui::InputFloat("TransSelectPlayerScaleY", &TransSelectPlayerIcon_.scale_.y, ZERO_POINT_ONE);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif

}

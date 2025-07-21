#include "PracticeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"
#include"Engine/CsvReader.h"

#include"Player.h"
#include"Enemy.h"
#include"StageManager.h"

#include"Engine/Camera.h"

#include"GameView.h"

PracticeScene::PracticeScene(GameObject* parent)
	:BaseScene(parent,"PracticeScene"), hBackScreen_(-1),hSoundPractice_(-1),
	pPlayer1_(nullptr), pPlayer2_(nullptr), pEnemy_(nullptr), pHUD_(nullptr),pMiniMap_(nullptr),
	pTransitionEffect_(nullptr),Press_(0)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	//StageManagerクラス生成
	Instantiate<StageManager>(this);

	//StageManagerから各移動制限の値を取得
	StageManager* pStageManager = (StageManager*)FindObject("StageManager");
	float North = pStageManager->GetNorthEnd();
	float South = pStageManager->GetSouthEnd();
	float West = pStageManager->GetWestEnd();
	float East = pStageManager->GetEastEnd();

	//プレイヤークラス(Player1)を生成
	pPlayer1_ = Instantiate<Player>(this);
	assert(pPlayer1_ != nullptr);

	//Player1の名前を設定
	pPlayer1_->SetObjectName("Player1");

	//Player1に移動制限(各ステージの端)を渡す
	pPlayer1_->SetEnd(North, South, West, East);

	//Player1にIDを割り振る
	pPlayer1_->SetID(1);

	//使うコントローラーのID設定
	pPlayer1_->SetControllerID(0);

	//Player1の初期化
	pPlayer1_->PlayerInit("CSVdata\\CharacterData\\PlayerData1.csv", "Model\\chara.fbx");

	//プレイヤー１のポインタを設定
	GameView::SetPlayer1(pPlayer1_);

	//実際に動くプレイヤー(CPUではない)を登録
	ActivePlayers_.push_back(pPlayer1_);

	pPlayer1_->PlayerStart();

	//現在のモード(PvE or PvP)に合わせたキャラクターを生成
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager->IsPlayerVSEnemy())
	{
		//CPU(Enemyクラス)を生成
		pEnemy_ = Instantiate<Enemy>(this);
		assert(pEnemy_ != nullptr);

		//Enemyに移動制限(各ステージの端)を渡す
		pEnemy_->SetEnd(North, South, West, East);

		//EnemyにIDを割り振る
		pEnemy_->SetID(2);

		//画面状態のセット(一人プレイ用)
		GameView::SetGameViewMode(GameView::S_Single);
	}
	else if (pSceneManager->IsPlayerVSPlayer())
	{
		//Player2を生成
		pPlayer2_ = Instantiate<Player>(this);
		assert(pPlayer2_ != nullptr);

		//Player2の名前を設定
		pPlayer2_->SetObjectName("Player2");

		//Player2に移動制限(各ステージの端)を渡す
		pPlayer2_->SetEnd(North, South, West, East);

		//Player2にIDを割り振る
		pPlayer2_->SetID(2);

		//使うコントローラーのID設定
		pPlayer2_->SetControllerID(1);

		//Player2の初期化
		pPlayer2_->PlayerInit("CSVdata\\CharacterData\\PlayerData2.csv", "Model\\chara_black.fbx");

		Camera::HalfScreen();

		//画面状態のセット(二人プレイ用、左右分割する)
		GameView::SetGameViewMode(GameView::S_Dual);

		//プレイヤー２のポインタを設定
		GameView::SetPlayer2(pPlayer2_);

		//実際に動くプレイヤー(CPUではない)を登録
		ActivePlayers_.push_back(pPlayer2_);

		pPlayer2_->PlayerStart();
	}

	//各クラス生成
	Instantiate<MiniMap>(this);
	Instantiate<HUD>(this);
	Instantiate<TransitionEffect>(this);

	//インスタンスを初期化
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

	//HUDにポインタを渡す
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//各画像・サウンドの読み込み
	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundPractice_ = Audio::Load("Sound\\BGM\\practice.wav", true);
	assert(hSoundPractice_ >= 0);

	//フリープレイ用サウンド再生
	Audio::Play(hSoundPractice_);
}

void PracticeScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTranslationは継承先の関数が呼ばれる
	BaseScene::Update();

	//登録されたプレイヤーを更新
	//プレイヤーが複数存在する場合を想定して
	//Battle,Practiceシーンから動かす
	for (auto player : ActivePlayers_)
	{
		player->CharacterRun();
	}

	//ミニマップの位置を更新
	pMiniMap_->SetOriginalFirstPos(pPlayer1_->GetPosition());
	if (pPlayer2_ != nullptr)
	{
		pMiniMap_->SetOriginalSecondPos(pPlayer2_->GetPosition());
	}
	else if (pEnemy_ != nullptr)
	{
		pMiniMap_->SetOriginalSecondPos(pEnemy_->GetPosition());
	}
}

void PracticeScene::Draw()
{
	//背景描画
	Image::SetAndDraw(hBackScreen_, this->transform_);

	//HUDクラスに練習モード中であることを描画指示
	pHUD_->SetDrawMode(S_Practice);

}

void PracticeScene::Release()
{
}

void PracticeScene::UpdateActive()
{
	//Pキー・SL・SRボタン長押しでタイトルに戻る
	if (Input::IsKey(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		Press_++;
	}
	else
	{
		Press_ = 0;
	}

	
	if (Press_ >= SceneTransition)
	{
		//シーン遷移状態へ
		SceneState_ = S_Transition;

		//シーン遷移エフェクト(フェードアウト)を設定
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);
	}
}

void PracticeScene::UpdateTransition()
{
	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManagerのインスタンスからタイトルシーンへ
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);

		//シーン遷移用タイマーを戻す
		SceneTransitionTimer_ = 0;

		//練習用サウンド停止
		Audio::Stop(hSoundPractice_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
		
		//1画面に戻す
		GameView::SetGameViewMode(GameView::S_NormalScreen);
	}
}

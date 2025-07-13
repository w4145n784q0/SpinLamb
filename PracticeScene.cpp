#include "PracticeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"
#include"Engine/CsvReader.h"

#include"Player.h"
#include"Enemy.h"
#include"StageManager.h"


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
	Instantiate<Player>(this);

	//Player1に移動制限(各ステージの端)を渡す
	pPlayer1_ = (Player*)FindObject("Player");
	assert(pPlayer1_ != nullptr);
	pPlayer1_->SetEnd(North, South, West, East);

	//Player1にIDを割り振る
	pPlayer1_->SetID(1);

	//Player1に移動許可を出す
	pPlayer1_->PlayerStart();

	//現在のモード(PvE or PvP)に合わせたキャラクターを生成
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager->IsPlayerVSEnemy())
	{
		//CPU(Enemyクラス)を生成
		Instantiate<Enemy>(this);

		//Enemyに移動制限(各ステージの端)を渡す
		pEnemy_ = (Enemy*)FindObject("Enemy");
		assert(pEnemy_ != nullptr);
		pEnemy_->SetEnd(North, South, West, East);

		//EnemyにIDを割り振る
		pEnemy_->SetID(2);

		//Enemyは始め停止させる
		pEnemy_->EnemyStop();
	}
	else if (pSceneManager->IsPlayerVSPlayer())
	{
		//Player2を生成
		Instantiate<Player>(this);

		//Player2に移動制限(各ステージの端)を渡す
		pPlayer2_ = (Player*)FindObject("Player");
		assert(pPlayer2_ != nullptr);
		pPlayer2_->SetEnd(North, South, West, East);

		//Player2にIDを割り振る
		pPlayer2_->SetID(2);

		//Player2に移動許可を出す
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
	//決定ボタン(Pキー・B/Startボタン)を長押しでシーン遷移状態へ
	if (Input::IsKey(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))//ボタン長押しでタイトルに戻る
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
		
	}
}

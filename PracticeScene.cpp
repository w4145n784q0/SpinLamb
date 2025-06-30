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
	pPlayer_(nullptr),pEnemy_(nullptr), pHUD_(nullptr),pMiniMap_(nullptr),
	pTransitionEffect_(nullptr),Press_(0)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	//各クラス生成
	Instantiate<StageManager>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	Instantiate<MiniMap>(this);
	Instantiate<HUD>(this);
	Instantiate<TransitionEffect>(this);

	//各画像・サウンドの読み込み
	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundPractice_ = Audio::Load("Sound\\BGM\\practice.wav", true);
	assert(hSoundPractice_ >= 0);

	//StageManagerからPlayer,Enemyのインスタンスを生成し
	StageManager* pS = (StageManager*)FindObject("StageManager");
	float north = pS->GetNorthEnd();
	float south = pS->GetSouthEnd();
	float west = pS->GetWestEnd();
	float east = pS->GetEastEnd();

	// 移動制限(各ステージの端)を渡す
	pPlayer_ = (Player*)FindObject("Player");
	if(pPlayer_ != nullptr)
	{
		pPlayer_->PlayerStart();
		pPlayer_->SetEnd(north, south, west, east);
	}

	pEnemy_ = (Enemy*)FindObject("Enemy");
	if(pEnemy_ != nullptr)
	{
		pEnemy_->EnemyStop();
		pEnemy_->SetEnd(north, south, west, east);
	}

	//インスタンスを初期化
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

	//HUDにポインタを渡す
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//フリープレイ用サウンド再生
	Audio::Play(hSoundPractice_);
}

void PracticeScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTranslationは継承先の関数が呼ばれる
	BaseScene::Update();
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

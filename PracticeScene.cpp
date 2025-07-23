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

namespace
{
	//player初期化時の文字列配列
	std::string PlayerNames[] = { "Player1","Player2" };

	std::string csvPath[] = { "CSVdata\\CharacterData\\PlayerData1.csv" ,
		"CSVdata\\CharacterData\\PlayerData2.csv" };

	std::string modelPath[] = { "Model\\chara.fbx" ,"Model\\chara_black.fbx" };
}

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

	//各キャラクターの初期化用の配列
	std::vector<Player*> InitPlayers = {};
	std::vector<Enemy*> InitEnemys = {};
	std::vector<Character*> InitCharacters = {};

	//登場させるキャラクターは事前に生成だけ行う
	//値の初期化は後に行う
	pPlayer1_ = Instantiate<Player>(this);
	assert(pPlayer1_ != nullptr);

	//初期化配列に追加
	InitPlayers.push_back(pPlayer1_);
	InitCharacters.push_back(pPlayer1_);

	//「ひとりで」か「ふたりで」かで生成するキャラクターを変更
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager->IsPlayerVSEnemy())
	{
		//事前に生成
		pEnemy_ = Instantiate<Enemy>(this);
		assert(pEnemy_ != nullptr);

		//初期化配列に追加
		InitCharacters.push_back(pEnemy_);
		InitEnemys.push_back(pEnemy_);

		//画面状態のセット(一人プレイ用)
		GameView::SetGameViewMode(GameView::S_Single);

		//画面を一人プレイ用に設定
		Camera::FullScreen();
	}
	if (pSceneManager->IsPlayerVSPlayer())
	{
		//事前に生成
		pPlayer2_ = Instantiate<Player>(this);
		assert(pPlayer2_ != nullptr);

		//初期化配列に追加
		InitPlayers.push_back(pPlayer2_);
		InitCharacters.push_back(pPlayer2_);

		//画面状態のセット(二人プレイ用、左右分割する)
		GameView::SetGameViewMode(GameView::S_Dual);

		//二人プレイ用のカメラを設定
		Camera::HalfScreen();
	}

	//Player,Enemy共通の初期化処理
	for (int i = 0; i < InitCharacters.size(); i++)
	{
		//移動制限(各ステージの端)を渡す
		InitCharacters[i]->SetEnd(North, South, West, East);

		//IDを割り振る
		InitCharacters[i]->SetID(i + 1);
	}

	//Playerの初期化処理
	for (int i = 0; i < InitPlayers.size(); i++)
	{
		//プレイヤーの名前を設定
		InitPlayers[i]->SetObjectName(PlayerNames[i]);

		//使うコントローラーのID設定
		InitPlayers[i]->SetControllerID(i);

		//プレイヤーの初期化(csv、モデルのパス)
		InitPlayers[i]->PlayerInit(csvPath[i], modelPath[i]);

		//実際に動くプレイヤー(CPUではない)を登録
		ActivePlayers_.push_back(InitPlayers[i]);

		//プレイヤーに移動を許可
		InitPlayers[i]->PlayerStart();
	}
	
	//Enemyの初期化処理
	for (int i = 0; i < InitEnemys.size(); i++)
	{
		if (!InitPlayers.empty())
		{
			//プレイヤーの初期化配列からランダムなインスタンスをセット
			//(現状は敵と一対一なのでplayer1が選ばれる)
			//BattleSceneから設定することで値の相違・結合度の上昇を防ぐ
			InitEnemys[i]->SetPlayerPointer(InitPlayers[rand() % InitPlayers.size()]);

			//生成した敵を登録
			ActiveEnemys_.push_back(InitEnemys[i]);
		}
	}

	//プレイヤーのポインタを設定
	GameView::SetPlayers(InitPlayers);

	//敵のポインタを設定
	GameView::SetEnemy(pEnemy_);

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

	//GameViewにHUDのポインタを渡す
	GameView::SetHUD(pHUD_);


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

	//登録されたプレイヤー・CPUを更新
	//プレイヤーが複数存在する場合を想定して
	//Battle,Practiceシーンから動かす
	for (auto player : ActivePlayers_)
	{
		player->PlayerRun();
	}
	for (auto enemy : ActiveEnemys_)
	{
		enemy->EnemyRun();
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

		//画面分割しているならカメラを全体表示に戻す
		if (GameView::IsDual())
		{
			Camera::FullScreen();
		}
	}
}

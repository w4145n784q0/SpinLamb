#include "BattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"StageManager.h"

#include"Engine/Camera.h"

#include"GameView.h"

namespace
{
	//次のバトルの状態に遷移するまでの時間計測
	int StateCounter = 0;

	//バトルモードの制限時間
	int GameTimeLimit = 0;

	//エフェクト初期化時のインデックス
	enum ScorePosIndex
	{
		i_gametimelimit = 0,
	};
}

BattleScene::BattleScene(GameObject* parent)
	:BaseScene(parent,"BattleScene") ,BattleState_(S_BEFORE),
	 hBackScreen_(-1),hSoundBattle_(-1), hSoundWhistle_(-1),
	FirstScore_(0),SecondScore_(0),
	pPlayer1_(nullptr), pPlayer2_(nullptr), pEnemy_(nullptr),
	pHUD_(nullptr),pMiniMap_(nullptr),
	pGameTimer_(nullptr),pTransitionEffect_(nullptr)
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Initialize()
{
	//csvからパラメータ読み込み
	SetCSVBattle();

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

	//player1を監視対象に追加
	pPlayer1_->AddObserver(this);

	//プレイヤー１のポインタを設定
	GameView::SetPlayer1(pPlayer1_);

	//実際に動くプレイヤー(CPUではない)を登録
	ActivePlayers_.push_back(pPlayer1_);


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

		//Enemyを監視対象に追加
		pEnemy_->AddObserver(this);

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

		//player2を監視対象に追加
		pPlayer2_->AddObserver(this);

		Camera::HalfScreen();
		
		//画面状態のセット(二人プレイ用、左右分割する)
		GameView::SetGameViewMode(GameView::S_Dual);

		//プレイヤー２のポインタを設定
		GameView::SetPlayer2(pPlayer2_);

		//実際に動くプレイヤー(CPUではない)を登録
		ActivePlayers_.push_back(pPlayer2_);
	}

	//各クラス生成
	Instantiate<MiniMap>(this);
	Instantiate<GameTimer>(this);
	Instantiate<HUD>(this);
	Instantiate<TransitionEffect>(this);

	//インスタンスを初期化
	pGameTimer_ = (GameTimer*)FindObject("GameTimer");
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

	//GameTimer,MiniMapのポインタを渡す
	//HUDクラスと同じポインタを渡すことで値の相違を防ぐ
	pHUD_->SetTimerPointer(pGameTimer_);
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//ゲーム制限時間を渡す
	pGameTimer_->SetCurrentGameTime(GameTimeLimit);

	//各画像・サウンドの読み込み
	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundBattle_ = Audio::Load("Sound\\BGM\\battle.wav",true);
	assert(hSoundBattle_>= 0);

	hSoundWhistle_ = Audio::Load("Sound\\SE\\whistle.wav");
	assert(hSoundWhistle_ >= 0);

	//Player,Enemyのスコアを初期化
	pHUD_->SetFirstScore(FirstScore_);
	pHUD_->SetSecondScore(SecondScore_);


}

void BattleScene::Update()
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
	for (auto enemy : ActiveEnemys_)
	{
		//
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

void BattleScene::Draw()
{
	//背景描画
	Image::SetAndDraw(hBackScreen_, this->transform_);

	//今のBattleSceneの状態から、HUDクラスに描画するものを指示
	switch (BattleState_)
	{
	case BattleScene::S_BEFORE:
		pHUD_->SetDrawMode(S_BeforeStart);
		break;
	case BattleScene::S_READY:
		pHUD_->SetDrawMode(S_Ready);
		break;
	case BattleScene::S_NOW:
		pHUD_->SetDrawMode(S_Playing);
		break;
	case BattleScene::S_AFTER:
		pHUD_->SetDrawMode(S_Finish);
		break;
	default:
		break;
	}
}

void BattleScene::Release()
{
}

void BattleScene::UpdateActive()
{
	//BattleState_に応じて、各Update関数を呼ぶ
	switch (BattleState_)
	{
	case BattleScene::S_BEFORE:
		UpdateBattleBefore();
		break;
	case BattleScene::S_READY:
		UpdateBattleReady();
		break;
	case BattleScene::S_NOW:
		UpdateBattle();
		break;
	case BattleScene::S_AFTER:
		UpdateBattleAfter();
		break;
	default:
		break;
	}


}

void BattleScene::UpdateTransition()
{

	//時間経過で次のシーンに遷移
	//カウント中はシーン遷移エフェクト行う
	if (++SceneTransitionTimer_ > SceneShortTransition)
	{

		//SceneManagerのインスタンスからタイトルシーンへ
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT);

		//Player,Enemyのスコアを渡す
		pSceneManager->SetFirstCharaScore(FirstScore_);
		pSceneManager->SetSecondCharaScore(SecondScore_);

		//シーン遷移用タイマーを戻す
		SceneTransitionTimer_ = 0;

		//バトル用サウンド停止
		Audio::Stop(hSoundBattle_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;

		//1画面に戻す
		GameView::SetGameViewMode(GameView::S_NormalScreen);
	}
}

void BattleScene::UpdateBattleBefore()
{
	//説明文を出している状態
	//BボタンかPキーでスキップ
	if (++StateCounter > SceneTransition ||
		Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		//シーン遷移タイマーをリセット
		StateCounter = 0;

		//時間経過したら開始直前状態へ以降
		BattleState_ = S_READY;

		//Ready?を表示する時間を渡す
		pHUD_->SetReadyTimer(SceneTransition);
	}
}

void BattleScene::UpdateBattleReady()
{
	//Ready,GO!を表示している状態
	
	if (++StateCounter > SceneLongTransition)
	{
		//シーン遷移タイマーをリセット
		StateCounter = 0;

		//時間経過したらバトル中状態へ以降
		BattleState_ = S_NOW;

		//時間経過でPlayer,Enemyに移動許可を出す
		pPlayer1_->PlayerStart();
		//pEnemy_->EnemyStart();

		if (pPlayer2_ != nullptr)
		{
			pPlayer2_->PlayerStart();
		}
		else if (pEnemy_ != nullptr)
		{
			pEnemy_->EnemyStart();
		}

		//タイマーを起動
		pGameTimer_->StartTimer();

		//ホイッスルSE再生
		Audio::Play(hSoundWhistle_);
	}
}

void BattleScene::UpdateBattle()
{
	//ゲームプレイ中
	//GameTimerから時間切れになったことを受け取ったら終了状態へ
	if (pGameTimer_->GetCurrentGameTime() <= 0)
	{
		//終了状態へ以降
		BattleState_ = S_AFTER;

		//タイマーを止める
		pGameTimer_->StopTimer();

		//player,Enemyの動きを止める
		pPlayer1_->PlayerStop();

		if (pPlayer2_ != nullptr)
		{
			pPlayer2_->PlayerStop();
		}
		else if (pEnemy_ != nullptr)
		{
			pEnemy_->EnemyStop();
		}

		//ホイッスルSE再生
		Audio::Play(hSoundWhistle_);
	}

	//バトル中サウンド再生
	Audio::Play(hSoundBattle_);

	//スコアは毎フレーム渡し続ける
	pHUD_->SetFirstScore(FirstScore_);
	pHUD_->SetSecondScore(SecondScore_);
}

void BattleScene::UpdateBattleAfter()
{
	//Finish!を表示している状態
	if (++StateCounter > SceneTransition)
	{
		//時間経過でUpdateTransitionへ
		SceneState_ = S_Transition;

		//シーン遷移タイマーをリセット
		StateCounter = 0;

		//シーン遷移エフェクト(ズームイン)を設定
		pTransitionEffect_->FadeOutStartWhite();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);

	}
}

void BattleScene::SetCSVBattle()
{
	//csvファイル読み込む
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\BattleData.csv");

	//csvファイルの各0列目の文字列を取得
	std::string battleName = "Battle";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> battleData = GetCSVReadData(csv, battleName);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	GameTimeLimit = static_cast<int>(battleData[i_gametimelimit]);
}

void BattleScene::OnCharacterFenceHit(int HitCharaID)
{
	if (pPlayer1_->GetID() == HitCharaID)
	{
		PlusSecondScore();
	}
	else if (pEnemy_ != nullptr)
	{
		if (pEnemy_->GetID() == HitCharaID)
		{
			PlusFirstScore();
		}
	}
	else if (pPlayer2_ != nullptr)
	{
		if (pPlayer2_->GetID() == HitCharaID)
		{
			PlusFirstScore();
		}
	}
}

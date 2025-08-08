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

	//制限時間のうち、残りn秒で拡大イージング処理を行う際の開始時間
	//現状は残り10秒で制限時間を拡大
	int EasingTime = 0;

	//バトルシーン初期化時のインデックス
	enum ScorePosIndex
	{
		i_gametimelimit = 0,
		i_easingtime,
	};
}

BattleScene::BattleScene(GameObject* parent)
	:BaseScene(parent,"BattleScene") ,BattleState_(S_BEFORE),
	 hBackScreen_(-1), hSoundBattle_(-1), hSoundWhistle_(-1),
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

		//監視対象に追加
		InitCharacters[i]->AddObserver(this);
	}

	//Playerの初期化処理

	//player初期化時の文字列配列
	std::string PlayerNames[] = { "Player1","Player2" };

	//プレイヤー初期化時に読み込むcsvファイルのパス
	std::string csvPath[] = { "CSVdata\\CharacterData\\PlayerData1.csv" ,
		"CSVdata\\CharacterData\\PlayerData2.csv" };

	//プレイヤー初期化時に読み込むfbxモデルのパス
	std::string modelPath[] = { "Model\\Character.fbx" ,"Model\\Character_black.fbx" };

	//InitPlayers分だけ初期化
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

	//GameViewにポインタを渡す
	GameView::SetHUD(pHUD_);

	//ゲーム制限時間を渡す
	pGameTimer_->SetCurrentGameTime(GameTimeLimit);

	//イージング開始時間を渡す
	//GameTimerが受け取り保管、HUD側で制限時間がEasingTime以下になったことを検知し
	//イージング処理を行う
	pGameTimer_->SetEasingTime(EasingTime);

	//各画像・サウンドの読み込み
	
	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string Battle = "Image\\Battle\\";
	std::string Sound = "Sound\\";
	std::string BGM = "BGM\\";
	std::string SE = "SE\\";

	hBackScreen_ = Image::Load(Battle + "BackSky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundBattle_ = Audio::Load(Sound + BGM + "Battle.wav", true);
	assert(hSoundBattle_>= 0);

	hSoundWhistle_ = Audio::Load(Sound + SE + "Whistle.wav", false, Audio::GetWhistleNum());
	assert(hSoundWhistle_ >= 0);

	//Player,Enemyのスコアを初期化
	pHUD_->SetFirstScore(FirstScore_);
	pHUD_->SetSecondScore(SecondScore_);


}

void BattleScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTransitionは継承先の関数が呼ばれる
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
	//Enemy,Player2のnullチェックを行い,存在するなら位置データを渡す
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

		//シーン遷移用タイマーをリセット
		SceneTransitionTimer_ = 0;

		//バトル用サウンド停止
		Audio::Stop(hSoundBattle_);

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
		//Enemy,Player2のnullチェックを行い、存在するなら実行
		pPlayer1_->PlayerStart();

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
		//Enemy,Player2のnullチェックを行い、存在するなら実行

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
	EasingTime = static_cast<int>(battleData[i_easingtime]);
}

void BattleScene::OnCharacterFenceHit(int _HitCharaID)
{
	//_HitCharaIDには柵に接触したキャラクターIDが入る
	//それぞれ接触したキャラクターIDを確認し
	//一致するなら相手側のスコアを加算する
	//Enemy,Player2は必ずnullチェックする

	if (pPlayer1_->GetID() == _HitCharaID)
	{
		PlusSecondScore();
	}
	else if (pEnemy_ != nullptr)
	{
		if (pEnemy_->GetID() == _HitCharaID)
		{
			PlusFirstScore();
		}
	}
	else if (pPlayer2_ != nullptr)
	{
		if (pPlayer2_->GetID() == _HitCharaID)
		{
			PlusFirstScore();
		}
	}
}

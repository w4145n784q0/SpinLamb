#include "BattleScene.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Camera.h"
#include"../StageSourceFile/StageManager.h"
#include"../ViewSourceFile/GameView.h"

namespace
{
	//次のバトルの状態に遷移するまでの時間計測
	int StateCounter = 0;

	//キャラクターの初期化時、IDに加算する値(for文でiに加算する値)
	int CharacterAddID = 0;

	//バトルモードの制限時間
	int GameTimeLimit = 0;

	//制限時間のうち、残りn秒で拡大イージング処理を行う際の開始時間
	//現状は残り10秒で制限時間を拡大
	int EasingTime = 0;

	//csv読み込み時のインデックス(バトルシーンの各変数)
	enum BattleIndex
	{
		i_CharacterAddID = 0,
		i_GameTimeLimit,
		i_EasingTime,
	};

	//生成したキャラクター継承クラスのポインタを格納する配列
	std::vector<Character*> ActiveCharacters = {};
}

BattleScene::BattleScene(GameObject* parent)
	:PlayBaseScene(parent,"BattleScene"),
	hSoundBattle_(-1), hSoundWhistle_(-1),
	pPlayer1_(nullptr), pPlayer2_(nullptr), pEnemy_(nullptr),
	pHUD_(nullptr), pTransitionEffect_(nullptr),
	pGameTimer_(nullptr),pMiniMap_(nullptr),
	ActivePlayers_({}), ActiveEnemys_({}),
	BattleState_(S_Before),
	FirstScore_(0),SecondScore_(0),ScoreArray_({})
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Initialize()
{
	//プレイベースシーン(基底クラス)の初期化を行う
	PlayBaseScene::Initialize();

	//csvからパラメータ読み込み
	SetCSVBattle();

	//StageManagerクラス生成
	Instantiate<StageManager>(this);

	//StageManagerから各移動制限の値を取得
	StageManager* pStageManager = (StageManager*)FindObject("StageManager");
	assert(pStageManager != nullptr);

	//各ステージの端を保管
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

	//アクティブな配列に追加
	ActiveCharacters.push_back(pPlayer1_);

	//スコア配列に追加(1番目のキャラクターのスコア ActiveCharactersの追加とセットで行う)
	ScoreArray_.push_back(&FirstScore_);

	//「ひとりで」か「ふたりで」のどちらかによって生成するキャラクターを変更
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	assert(pSceneManager != nullptr);

	if (pSceneManager->IsPlayerVSEnemy())
	{
		//事前に生成
		pEnemy_ = Instantiate<Enemy>(this);
		assert(pEnemy_ != nullptr);

		//初期化配列に追加
		InitCharacters.push_back(pEnemy_);
		InitEnemys.push_back(pEnemy_);

		//アクティブな配列に追加
		ActiveCharacters.push_back(pEnemy_);

		//スコア配列に追加(2番目のキャラクターのスコア ActiveCharactersの追加とセットで行う)
		ScoreArray_.push_back(&SecondScore_);

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

		//アクティブな配列に追加
		ActiveCharacters.push_back(pPlayer2_);

		//スコア配列に追加(2番目のキャラクターのスコア ActiveCharactersの追加とセットで行う)
		ScoreArray_.push_back(&SecondScore_);

		//画面状態のセット(二人プレイ用、左右分割する)
		GameView::SetGameViewMode(GameView::S_Dual);

		//二人プレイ用のカメラを設定
		Camera::HalfScreen();
	}

	//Player,Enemy共通の初期化処理
	for (int i = 0; i < InitCharacters.size(); i++)
	{
		//移動制限(各ステージの端)を渡す
		InitCharacters[i]->GetParams()->SetEnd(North, South, West, East);

		//IDを割り振る
		InitCharacters[i]->GetParams()->SetID(i + CharacterAddID);

		//監視対象に追加
		InitCharacters[i]->OnAddObserver(this);
	}

	//player初期化時の文字列配列(追加する際はこの配列に文字列を追加)
	std::string PlayerNames[] = { "Player1","Player2" };

	//Enemy初期化時の文字列配列(今はCPUは一体のみ　追加する際はこの配列に文字列を追加)
	std::string EnemyNames[] = { "Enemy1","Enemy2"};

	//プレイヤー初期化時に読み込むcsvファイルのパス
	std::string csvPath[] = { "CSVdata\\CharacterData\\PlayerData1.csv",
		"CSVdata\\CharacterData\\PlayerData2.csv" };

	//プレイヤー初期化時に読み込むfbxモデルのパス
	std::string modelPath[] = { "Model\\Character.fbx" ,"Model\\Character_black.fbx" };

	//Playerの初期化処理
	//InitPlayers分だけ初期化
	for (int i = 0; i < InitPlayers.size(); i++)
	{
		//プレイヤーの名前を設定
		InitPlayers[i]->SetObjectName(PlayerNames[i]);

		//プレイヤーの名前を最後に接触したキャラクターとして初期化(名前が割り振られたタイミングで初期化)
		InitPlayers[i]->GetParams()->SetAttackedName(InitPlayers[i]->GetObjectName());

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
			//CPUの名前を登録
			InitEnemys[i]->SetObjectName(EnemyNames[i]);

			//CPUの名前を最後に接触したキャラクターとして初期化(名前が割り振られたタイミングで初期化)
			InitEnemys[i]->GetParams()->SetAttackedName(InitEnemys[i]->GetObjectName());

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
	assert(pGameTimer_ != nullptr);

	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	assert(pMiniMap_ != nullptr);

	pHUD_ = (HUD*)FindObject("HUD");
	assert(pHUD_ != nullptr);

	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
	assert(pTransitionEffect_ != nullptr);

	//GameTimer,MiniMapのポインタを渡す
	//HUDクラスと同じポインタを渡すことで値の相違を防ぐ
	pHUD_->HUDParam_->SetTimerPointer(pGameTimer_);
	pHUD_->HUDParam_->SetMiniMapPointer(pMiniMap_);

	//GameViewにHUD,TransitionEffect,Sceneのポインタを渡す(描画順番をずらすために使用)
	GameView::SetHUD(pHUD_);
	GameView::SetTransitionEffect(pTransitionEffect_);
	SetPointerGameView();

	//ゲーム制限時間を渡す
	pGameTimer_->SetCurrentGameTime(GameTimeLimit);

	//イージング開始時間を渡す
	//GameTimerが受け取り保管、HUD側で制限時間がEasingTime以下になったことを検知し
	//イージング処理を行う
	pGameTimer_->SetEasingTime(EasingTime);

	//各画像・サウンドの読み込み
	
	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string Sound = "Sound\\";
	std::string BGM = "BGM\\";
	std::string SE = "SE\\";

	hSoundBattle_ = Audio::Load(Sound + BGM + "Battle.wav", true);
	assert(hSoundBattle_>= 0);

	hSoundWhistle_ = Audio::Load(Sound + SE + "Whistle.wav", false, Audio::WhistleSoundNum_);
	assert(hSoundWhistle_ >= 0);

	//Player,Enemyのスコアを初期化
	pHUD_->HUDParam_->SetFirstScore(FirstScore_);
	pHUD_->HUDParam_->SetSecondScore(SecondScore_);


}

void BattleScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTransitionは継承先の関数が呼ばれる
	BaseScene::Update();
}

void BattleScene::Draw()
{	
	//背景描画
	PlayBaseScene::DrawBackScreen();

	//今のBattleStateの状態から、HUDクラスに描画するものを指示
	switch (BattleState_)
	{
	case BattleScene::S_Before:
		pHUD_->HUDParam_->SetDrawMode(DrawMode::Mode_BeforeStart);
		break;
	case BattleScene::S_Ready:
		pHUD_->HUDParam_->SetDrawMode(DrawMode::Mode_JustBefore);
		break;
	case BattleScene::S_Now:
		pHUD_->HUDParam_->SetDrawMode(DrawMode::Mode_Playing);
		break;
	case BattleScene::S_Pause:
		pHUD_->HUDParam_->SetDrawMode(DrawMode::Mode_PlayPause);
		break;
	case BattleScene::S_After:
		pHUD_->HUDParam_->SetDrawMode(DrawMode::Mode_Finish);
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
	//通常の動いている状態

	//BattleState_に応じて、各Update関数を呼ぶ
	//BattleScene::S_PauseはActiveでは呼ばれない
	switch (BattleState_)
	{
	case BattleScene::S_Before:
		UpdateBattleBefore();
		break;
	case BattleScene::S_Ready:
		UpdateBattleReady();
		break;
	case BattleScene::S_Now:
		UpdateBattle();
		break;
	case BattleScene::S_After:
		UpdateBattleAfter();
		break;
	default:
		break;
	}

	//以下はシーンがActiveのときに毎回通る処理

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

	//ミニマップの位置・回転を更新
	//Enemy,Player2のnullチェックを行い,存在するならデータを渡す
	//回転Z軸はキャラクターのY軸回転の逆向き(-にする)
	pMiniMap_->SetOriginalFirstPos(pPlayer1_->GetPosition());
	pMiniMap_->SetFirstRotZ(-pPlayer1_->GetRotate().y);

	if (pPlayer2_ != nullptr)
	{
		pMiniMap_->SetOriginalSecondPos(pPlayer2_->GetPosition());
		pMiniMap_->SetSecondRotZ(-pPlayer2_->GetRotate().y);
	}
	else if (pEnemy_ != nullptr)
	{
		pMiniMap_->SetOriginalSecondPos(pEnemy_->GetPosition());
		pMiniMap_->SetSecondRotZ(-pEnemy_->GetRotate().y);
	}

}

void BattleScene::UpdateInActive()
{
	//画面を止めている状態

	//Pause表示中の処理
	PlayBaseScene::UpdatePauseMenu();
}

void BattleScene::UpdateTransition()
{
	//シーン遷移状態

	//時間経過で次のシーンに遷移
	//カウント中はシーン遷移エフェクト行う
	if (++SceneTransitionTimer_ > SceneShortTransition)
	{

		//SceneManagerのインスタンスを取得
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		assert(pSceneManager != nullptr);

		if (BattleState_ == S_After)
		{
			//バトル終了していたらリザルトシーンへ
			pSceneManager->ChangeScene(SCENE_ID_RESULT);

			//Player,Enemyのスコアを渡す
			pSceneManager->SetFirstCharaScore(FirstScore_);
			pSceneManager->SetSecondCharaScore(SecondScore_);
		}
		else
		{
			//S_Afterではない (=ポーズから中断した場合)はタイトルシーンへ
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
		}

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

void BattleScene::GotoPause()
{
	//ポーズ画面に向かう処理 PlayBaseSceneから上書き

	//ポーズ画面状態へ移行
	BattleState_ = S_Pause;

	//タイマーを一旦止める
	pGameTimer_->StopTimer();
}

void BattleScene::GotoPlay()
{
	//ゲーム画面に向かう処理 PlayBaseSceneから上書き

	//バトル中状態へ移行
	BattleState_ = S_Now;

	//タイマーを再開
	pGameTimer_->StartTimer();
}

void BattleScene::GotoTitle()
{
	//タイトルに向かう処理 PlayBaseSceneから上書き

	//シーン遷移エフェクト(黒くフェードアウト)を設定
	if (pTransitionEffect_ != nullptr)
	{
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);
	}
}

void BattleScene::SetPauseIconY()
{
	if (pHUD_ != nullptr)
	{
		pHUD_->HUDParam_->SetPauseIcon(TmpIconPosY_);
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

		//時間経過したら開始直前状態へ移行
		BattleState_ = S_Ready;

		//Ready?を表示する時間を渡す
		pHUD_->HUDParam_->SetReadyTimer(SceneTransition);
	}
}

void BattleScene::UpdateBattleReady()
{
	//Ready,GO!を表示している状態
	
	if (++StateCounter > SceneLongTransition)
	{
		//シーン遷移タイマーをリセット
		StateCounter = 0;

		//時間経過したらバトル中状態へ移行
		BattleState_ = S_Now;

		//時間経過でPlayer,Enemyに移動許可を出す
		//Enemy,Player2のnullチェックを行い、存在するなら実行
		pPlayer1_->PlayerStartState();

		if (pPlayer2_ != nullptr)
		{
			pPlayer2_->PlayerStartState();
		}
		else if (pEnemy_ != nullptr)
		{
			pEnemy_->EnemyStartState();
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
		//終了状態へ移行
		BattleState_ = S_After;

		//タイマーを止める
		pGameTimer_->StopTimer();

		//player,Enemyの動きを止める
		//Enemy,Player2のnullチェックを行い、存在するなら実行

		pPlayer1_->PlayerStopState();

		if (pPlayer2_ != nullptr)
		{
			pPlayer2_->PlayerStopState();
		}
		else if (pEnemy_ != nullptr)
		{
			pEnemy_->EnemyStopState();
		}

		//ホイッスルSE再生
		Audio::Play(hSoundWhistle_);
	}

	//バトル中サウンド再生
	Audio::Play(hSoundBattle_);

	//スコアは毎フレーム渡し続ける
	pHUD_->HUDParam_->SetFirstScore(FirstScore_);
	pHUD_->HUDParam_->SetSecondScore(SecondScore_);

	//escキーかstartボタンでポーズ画面へ
	PlayBaseScene::WaitGotoPause();
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

		//シーン遷移エフェクト(白くフェードアウト)を設定
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
	std::string BattleName = "Battle";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> BattleData = GetCSVReadData(csv, BattleName);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	CharacterAddID = static_cast<int>(BattleData[i_CharacterAddID]);
	GameTimeLimit = static_cast<int>(BattleData[i_GameTimeLimit]);
	EasingTime = static_cast<int>(BattleData[i_EasingTime]);
}

void BattleScene::ScoreChange(int& _score, bool _IsSelfDestruction)
{
	//自爆判定ならスコアを減少
	//その際は相手ではなく自分のスコアが参照渡しされる
	if (_IsSelfDestruction)
	{
		if (_score > 0)
		{
			_score--;
		}
	}
	else
	{
		//通常ならスコアを加算
		_score++;
	}
}

void BattleScene::OnCharacterFenceHit(std::string _AttackedName, std::string _HitName)
{
	for (int i = 0; i < ActiveCharacters.size(); i++)
	{
		//最後の攻撃を当てたキャラクターの名前の存在確認
		if (ActiveCharacters[i]->GetObjectName() == _AttackedName)
		{
			//最後に攻撃したキャラクターの名前が、柵に接触した名前と同じ=自爆した場合は減点
			//(例:Player1が最後に攻撃した名前になっているのに、柵に接触したのもPlayer1のとき
			//柵に接触時、攻撃された相手の名前を自分のobjectName_にしているので、
			//なにも触れずに被弾した場合自爆判定となる )
			if (ActiveCharacters[i]->GetObjectName() == _HitName)
			{
				//念のため配列範囲チェック
				if (i >= 0 && i < ScoreArray_.size())
				{
					//自爆なので自分のスコアを減算
					//スコアの計算関数には自分のスコアを参照
					ScoreChange(*ScoreArray_[i],true);
				}
			}
			else
			{
				//念のため配列範囲チェック
				if (i >= 0 && i < ScoreArray_.size())
				{
					//スコアを加算
					//関数には攻撃した相手のスコアを参照
					ScoreChange(*ScoreArray_[i]);
				}
			}
			
		}
	}
}

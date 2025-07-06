#include "BattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"StageManager.h"

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
	PlayerScore_(0),EnemyScore_(0),
	pPlayer_(nullptr),pEnemy_(nullptr),pHUD_(nullptr),pMiniMap_(nullptr),
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

	//各クラス生成
	Instantiate<StageManager>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	Instantiate<MiniMap>(this);
	Instantiate<GameTimer>(this);
	Instantiate<HUD>(this);
	Instantiate<TransitionEffect>(this);

	//StageManagerからPlayer,Enemyのインスタンスを生成
	StageManager* pSceneManager = (StageManager*)FindObject("StageManager");
	float North = pSceneManager->GetNorthEnd();
	float South = pSceneManager->GetSouthEnd();
	float West = pSceneManager->GetWestEnd();
	float East = pSceneManager->GetEastEnd();

	// 移動制限(各ステージの端)を渡す
	pPlayer_ = (Player*)FindObject("Player");
	if(pPlayer_ != nullptr)
	{
		pPlayer_->SetEnd(North, South, West, East);
	}

	pEnemy_ = (Enemy*)FindObject("Enemy");
	if (pEnemy_ != nullptr)
	{
		pEnemy_->SetEnd(North, South, West, East);
	}

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
	pHUD_->SetPlayerScore(PlayerScore_);
	pHUD_->SetEnemyScore(EnemyScore_);

}

void BattleScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTranslationは継承先の関数が呼ばれる
	BaseScene::Update();
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
		pSceneManager->SetPlayerScore(PlayerScore_);
		pSceneManager->SetEnemyScore(EnemyScore_);

		//シーン遷移用タイマーを戻す
		SceneTransitionTimer_ = 0;

		//バトル用サウンド停止
		Audio::Stop(hSoundBattle_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
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
		pPlayer_->PlayerStart();
		pEnemy_->EnemyStart();

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
		pPlayer_->PlayerStop();
		pEnemy_->EnemyStop();

		//ホイッスルSE再生
		Audio::Play(hSoundWhistle_);
	}

	//バトル中サウンド再生
	Audio::Play(hSoundBattle_);

	//スコアは毎フレーム渡し続ける
	pHUD_->SetPlayerScore(PlayerScore_);
	pHUD_->SetEnemyScore(EnemyScore_);
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
	std::string pos = "Position";

	//指定した文字列がいずれかの0列目に存在したら
	if (csv.IsGetParamName(pos))
	{
		//その行を配列として全取得
		std::vector<float> v = csv.GetParam(pos);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		GameTimeLimit = static_cast<int>(v[i_gametimelimit]);
	}
}

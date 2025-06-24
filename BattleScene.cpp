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
	pPlayer_(nullptr),pEnemy_(nullptr),pHUD_(nullptr),pMiniMap_(nullptr),pGameTimer_(nullptr)
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Initialize()
{
	SetCSVBattle();

	Instantiate<StageManager>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	Instantiate<MiniMap>(this);
	Instantiate<GameTimer>(this);
	Instantiate<HUD>(this);

	StageManager* pSceneManager = (StageManager*)FindObject("StageManager");
	float north = pSceneManager->GetNorthEnd();
	float south = pSceneManager->GetSouthEnd();
	float west = pSceneManager->GetWestEnd();
	float east = pSceneManager->GetEastEnd();

	pPlayer_ = (Player*)FindObject("Player");
	if(pPlayer_ != nullptr)
	{
		pPlayer_->SetEnd(north, south, west, east);
	}

	pEnemy_ = (Enemy*)FindObject("Enemy");
	if (pEnemy_ != nullptr)
	{
		pEnemy_->SetEnd(north, south, west, east);
	}

	//インスタンスを初期化し、HUDに渡す
	pGameTimer_ = (GameTimer*)FindObject("GameTimer");
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");


	pHUD_->SetTimerPointer(pGameTimer_);
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//ゲーム制限時間を渡す
	pGameTimer_->SetCurrentGameTime(GameTimeLimit);

	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundBattle_ = Audio::Load("Sound\\BGM\\battle.wav",true);
	assert(hSoundBattle_>= 0);

	hSoundWhistle_ = Audio::Load("Sound\\SE\\Whistle.wav");
	assert(hSoundWhistle_ >= 0);

	pHUD_->SetPlayerScore(PlayerScore_);
	pHUD_->SetEnemyScore(EnemyScore_);

}

void BattleScene::Update()
{
	BaseScene::Update();
}

void BattleScene::Draw()
{
	//常に表示するもの
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

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
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	pSceneManager->ChangeScene(SCENE_ID_RESULT);
	pSceneManager->SetPlayerScore(PlayerScore_);
	pSceneManager->SetEnemyScore(EnemyScore_);
	Audio::Stop(hSoundBattle_);
	SceneState_ = S_Active;
}

void BattleScene::UpdateBattleBefore()
{
	if (++StateCounter > SceneTransition ||
		Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		StateCounter = 0;
		BattleState_ = S_READY;
		pHUD_->SetReadyTimer(SceneTransition);
	}
}

void BattleScene::UpdateBattleReady()
{
	if (++StateCounter > SceneLongTransition)
	{
		StateCounter = 0;
		BattleState_ = S_NOW;
		pPlayer_->PlayerStart();
		pEnemy_->EnemyStart();
		pGameTimer_->StartTimer();

		Audio::Play(hSoundWhistle_);
	}
}

void BattleScene::UpdateBattle()
{
	//GameTimerから時間切れになったことを受け取ったら終了状態へ
	if (pGameTimer_->GetCurrentGameTime() <= 0)
	{
		BattleState_ = S_AFTER;

		pGameTimer_->StopTimer();
		pPlayer_->PlayerStop();
		pEnemy_->EnemyStop();

		Audio::Play(hSoundWhistle_);
	}

	Audio::Play(hSoundBattle_);
	pHUD_->SetPlayerScore(PlayerScore_);
	pHUD_->SetEnemyScore(EnemyScore_);
}

void BattleScene::UpdateBattleAfter()
{
	if (++StateCounter > SceneTransition)
	{
		SceneState_ = S_Transition;
		StateCounter = 0;
	}
}

void BattleScene::SetCSVBattle()
{

	CsvReader csv;
	csv.Load("CSVdata\\BattleData.csv");

	std::string pos = "Position";
	if (csv.IsGetParamName(pos))
	{
		std::vector<float> v = csv.GetParam(pos);
		GameTimeLimit = static_cast<int>(v[i_gametimelimit]);
	}
}

#include "BattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

#include"MiniMap.h"
#include"StageManager.h"

namespace
{
	//時間計測
	int Timecounter = 0;
	int PlayerScorePosX = 0;
	int PlayerScorePosY = 0;
	int EnemyScorePosX = 0;
	int EnemyScorePosY = 0;

	enum ScorePosIndex
	{
		i_PlayerScorePosX = 0,
		i_PlayerScorePosY,
		i_EnemyScorePosX,
		i_EnemyScorePosY,
	};
}

BattleScene::BattleScene(GameObject* parent)
	:GameObject(parent,"BattleScene") ,BattleState_(BEFORE),
	 hBackScreen_(-1),hSoundBattle_(-1), hSoundWhistle_(-1),
	PlayerScore_(0),EnemyScore_(0),GameTime_(GameTimeLimit),pPlayerScore_(0),pEnemyScore_(0),
	pHUD_(nullptr),pGameTimer_(nullptr)
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Initialize()
{
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

	Player* pPlayer_ = (Player*)FindObject("Player");
	pPlayer_->SetEnd(north, south, west, east);

	Enemy* pEnemy = (Enemy*)FindObject("Enemy");
	pEnemy->SetEnd(north, south, west, east);


	pGameTimer_ = (GameTimer*)FindObject("GameTimer");
	pHUD_ = (HUD*)FindObject("HUD");
	pHUD_->SetTimerPointer(pGameTimer_);
	pGameTimer_->SetCurrentGameTime(GameTimeLimit);

	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundBattle_ = Audio::Load("Sound\\BGM\\battle.wav",true);
	assert(hSoundBattle_>= 0);

	hSoundWhistle_ = Audio::Load("Sound\\SE\\Whistle.wav");
	assert(hSoundWhistle_ >= 0);

	pPlayerScore_ = new Text;
	pPlayerScore_->Initialize();

	pEnemyScore_ = new Text;
	pEnemyScore_->Initialize();

	SetCSVBattle();

}

void BattleScene::Update()
{
	switch (BattleState_)
	{
	case BattleScene::BEFORE:
		UpdateBattleBefore();
		break;
	case BattleScene::NOW:
		UpdateBattle();
		break;
	case BattleScene::AFTER:
		UpdateBattleAfter();
		break;
	case BattleScene::MAX:
		break;
	default:
		break;
	}
}

void BattleScene::Draw()
{
	//常に表示するもの
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

	pPlayerScore_->Draw(PlayerScorePosX, PlayerScorePosY, PlayerScore_);
	pEnemyScore_->Draw(EnemyScorePosX, EnemyScorePosY, EnemyScore_);

	//pHUD_->SetDrawMode(S_MiniMap);

	//今のBattleSceneの状態から、HUDクラスに描画するものを指示
	switch (BattleState_)
	{
	case BattleScene::BEFORE:
		pHUD_->SetDrawMode(S_StartLogo);
		break;
	case BattleScene::NOW:
		pHUD_->SetDrawMode(S_Timer);
		break;
	case BattleScene::AFTER:
	{
		pHUD_->SetDrawMode(S_Timer);
		pHUD_->SetDrawMode(S_FinishLogo);
	}
		break;
	case BattleScene::MAX:
		break;
	default:
		break;
	}
}

void BattleScene::Release()
{
}

void BattleScene::UpdateBattleBefore()
{
	if (++Timecounter > SceneTransition)
	{
		Timecounter = 0;

		BattleState_ = NOW;
		Player* pPlayer_ = (Player*)FindObject("Player");
		pPlayer_->PlayerStart();

		Enemy* pEnemy = (Enemy*)FindObject("Enemy");
		pEnemy->EnemyStart();

		pGameTimer_->StartTimer();
	}
}

void BattleScene::UpdateBattle()
{
	//GameTimerから時間切れになったことを受け取ったら終了状態へ
	if (pGameTimer_->GetCurrentGameTime() <= 0)
	{
		BattleState_ = AFTER;
		pGameTimer_->StopTimer();

		Player* pPlayer_ = (Player*)FindObject("Player");
		pPlayer_->PlayerStop();

		Enemy* pEnemy = (Enemy*)FindObject("Enemy");
		pEnemy->EnemyStop();

		Audio::Play(hSoundWhistle_);
	}

	Audio::Play(hSoundBattle_);
}

void BattleScene::UpdateBattleAfter()
{

	if (++Timecounter > SceneTransition)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT);
		pSceneManager->SetPlayerScore(PlayerScore_);
		pSceneManager->SetEnemyScore(EnemyScore_);
		Timecounter = 0;
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
		PlayerScorePosX = static_cast<int>(v[i_PlayerScorePosX]);
		PlayerScorePosY = static_cast<int>(v[i_PlayerScorePosY]);
		EnemyScorePosX = static_cast<int>(v[i_EnemyScorePosX]);
		EnemyScorePosY = static_cast<int>(v[i_EnemyScorePosY]);
	}
}

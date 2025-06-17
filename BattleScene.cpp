#include "BattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"


#include"StageManager.h"

namespace
{
	//���Ԍv��
	int Timecounter = 0;

	//�o�g�����[�h�̐�������
	int GameTimeLimit = 0;

	enum ScorePosIndex
	{
		i_gametimelimit = 0,
	};
}

BattleScene::BattleScene(GameObject* parent)
	:GameObject(parent,"BattleScene") ,BattleState_(BEFORE),
	 hBackScreen_(-1),hSoundBattle_(-1), hSoundWhistle_(-1),
	PlayerScore_(0),EnemyScore_(0),
	pHUD_(nullptr),pMiniMap_(nullptr),pGameTimer_(nullptr)
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

	Player* pPlayer_ = (Player*)FindObject("Player");
	pPlayer_->SetEnd(north, south, west, east);

	Enemy* pEnemy = (Enemy*)FindObject("Enemy");
	pEnemy->SetEnd(north, south, west, east);

	//�C���X�^���X�����������AHUD�ɓn��
	pGameTimer_ = (GameTimer*)FindObject("GameTimer");
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");


	pHUD_->SetTimerPointer(pGameTimer_);
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//�Q�[���������Ԃ�n��
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
	//��ɕ\���������
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

	//����BattleScene�̏�Ԃ���AHUD�N���X�ɕ`�悷����̂��w��
	switch (BattleState_)
	{
	case BattleScene::BEFORE:
		pHUD_->SetDrawMode(S_StartLogo);
		break;
	case BattleScene::NOW:
		pHUD_->SetDrawMode(S_Timer);
		break;
	case BattleScene::AFTER:
		pHUD_->SetDrawMode(S_FinishLogo);
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
	//GameTimer���玞�Ԑ؂�ɂȂ������Ƃ��󂯎������I����Ԃ�
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
	pHUD_->SetPlayerScore(PlayerScore_);
	pHUD_->SetEnemyScore(EnemyScore_);
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
		GameTimeLimit = static_cast<int>(v[i_gametimelimit]);
	}
}

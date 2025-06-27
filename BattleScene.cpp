#include "BattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"StageManager.h"

namespace
{
	//���̃o�g���̏�ԂɑJ�ڂ���܂ł̎��Ԍv��
	int StateCounter = 0;

	//�o�g�����[�h�̐�������
	int GameTimeLimit = 0;

	//�G�t�F�N�g���������̃C���f�b�N�X
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
	//csv����p�����[�^�ǂݍ���
	SetCSVBattle();

	//�e�N���X����
	Instantiate<StageManager>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	Instantiate<MiniMap>(this);
	Instantiate<GameTimer>(this);
	Instantiate<HUD>(this);

	//StageManager����Player,Enemy�̃C���X�^���X��
	// �ړ�����(�e�X�e�[�W�̒[)��n��
	StageManager* pSceneManager = (StageManager*)FindObject("StageManager");
	float North = pSceneManager->GetNorthEnd();
	float South = pSceneManager->GetSouthEnd();
	float West = pSceneManager->GetWestEnd();
	float East = pSceneManager->GetEastEnd();

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

	//�C���X�^���X��������
	pGameTimer_ = (GameTimer*)FindObject("GameTimer");
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");

	//GameTimer,MiniMap�̃|�C���^��n��
	//HUD�N���X�Ɠ����|�C���^��n�����ƂŒl�̑����h��
	pHUD_->SetTimerPointer(pGameTimer_);
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//�Q�[���������Ԃ�n��
	pGameTimer_->SetCurrentGameTime(GameTimeLimit);

	//�e�摜�E�T�E���h�̓ǂݍ���
	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundBattle_ = Audio::Load("Sound\\BGM\\battle.wav",true);
	assert(hSoundBattle_>= 0);

	hSoundWhistle_ = Audio::Load("Sound\\SE\\Whistle.wav");
	assert(hSoundWhistle_ >= 0);

	//Player,Enemy�̃X�R�A��������
	pHUD_->SetPlayerScore(PlayerScore_);
	pHUD_->SetEnemyScore(EnemyScore_);

}

void BattleScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTranslation�͌p����̊֐����Ă΂��
	BaseScene::Update();
}

void BattleScene::Draw()
{
	//�w�i�͏�ɕ\��
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

	//����BattleScene�̏�Ԃ���AHUD�N���X�ɕ`�悷����̂��w��
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
	//BattleState_�ɉ����āA�eUpdate�֐����Ă�
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
	//UpdateBattleAfter���I�������炷���ɃV�[���J��

	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	pSceneManager->ChangeScene(SCENE_ID_RESULT);
	pSceneManager->SetPlayerScore(PlayerScore_);//Player,Enemy�̃X�R�A��n��
	pSceneManager->SetEnemyScore(EnemyScore_);
	Audio::Stop(hSoundBattle_);
	SceneState_ = S_Active;
}

void BattleScene::UpdateBattleBefore()
{
	//���������o���Ă�����
	//B�{�^����P�L�[�ŃX�L�b�v

	if (++StateCounter > SceneTransition ||
		Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		StateCounter = 0;
		BattleState_ = S_READY;

		//Ready?��\�����鎞�Ԃ�n��
		pHUD_->SetReadyTimer(SceneTransition);
	}
}

void BattleScene::UpdateBattleReady()
{
	//Ready,GO!��\�����Ă�����
	
	if (++StateCounter > SceneLongTransition)
	{
		StateCounter = 0;
		BattleState_ = S_NOW;
		pPlayer_->PlayerStart();//���Ԍo�߂�Player,Enemy�Ɉړ������o��
		pEnemy_->EnemyStart();
		pGameTimer_->StartTimer();//�^�C�}�[���N��

		Audio::Play(hSoundWhistle_);
	}
}

void BattleScene::UpdateBattle()
{
	//�Q�[���v���C��
	//GameTimer���玞�Ԑ؂�ɂȂ������Ƃ��󂯎������I����Ԃ�
	if (pGameTimer_->GetCurrentGameTime() <= 0)
	{
		BattleState_ = S_AFTER;

		pGameTimer_->StopTimer();
		pPlayer_->PlayerStop();
		pEnemy_->EnemyStop();

		Audio::Play(hSoundWhistle_);
	}

	Audio::Play(hSoundBattle_);

	//�X�R�A�͖��t���[���n��������
	pHUD_->SetPlayerScore(PlayerScore_);
	pHUD_->SetEnemyScore(EnemyScore_);
}

void BattleScene::UpdateBattleAfter()
{
	//Finish!��\�����Ă�����
	//���Ԍo�߂�UpdateTransition��
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

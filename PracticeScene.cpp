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
	pPlayer1_(nullptr), pPlayer2_(nullptr), pEnemy_(nullptr), pHUD_(nullptr),pMiniMap_(nullptr),
	pTransitionEffect_(nullptr),Press_(0)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	//StageManager�N���X����
	Instantiate<StageManager>(this);

	//StageManager����e�ړ������̒l���擾
	StageManager* pStageManager = (StageManager*)FindObject("StageManager");
	float North = pStageManager->GetNorthEnd();
	float South = pStageManager->GetSouthEnd();
	float West = pStageManager->GetWestEnd();
	float East = pStageManager->GetEastEnd();

	//�v���C���[�N���X(Player1)�𐶐�
	Instantiate<Player>(this);

	//Player1�Ɉړ�����(�e�X�e�[�W�̒[)��n��
	pPlayer1_ = (Player*)FindObject("Player");
	assert(pPlayer1_ != nullptr);
	pPlayer1_->SetEnd(North, South, West, East);

	//Player1��ID������U��
	pPlayer1_->SetID(1);

	//Player1�Ɉړ������o��
	pPlayer1_->PlayerStart();

	//���݂̃��[�h(PvE or PvP)�ɍ��킹���L�����N�^�[�𐶐�
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager->IsPlayerVSEnemy())
	{
		//CPU(Enemy�N���X)�𐶐�
		Instantiate<Enemy>(this);

		//Enemy�Ɉړ�����(�e�X�e�[�W�̒[)��n��
		pEnemy_ = (Enemy*)FindObject("Enemy");
		assert(pEnemy_ != nullptr);
		pEnemy_->SetEnd(North, South, West, East);

		//Enemy��ID������U��
		pEnemy_->SetID(2);

		//Enemy�͎n�ߒ�~������
		pEnemy_->EnemyStop();
	}
	else if (pSceneManager->IsPlayerVSPlayer())
	{
		//Player2�𐶐�
		Instantiate<Player>(this);

		//Player2�Ɉړ�����(�e�X�e�[�W�̒[)��n��
		pPlayer2_ = (Player*)FindObject("Player");
		assert(pPlayer2_ != nullptr);
		pPlayer2_->SetEnd(North, South, West, East);

		//Player2��ID������U��
		pPlayer2_->SetID(2);

		//Player2�Ɉړ������o��
		pPlayer2_->PlayerStart();

	}

	//�e�N���X����
	Instantiate<MiniMap>(this);
	Instantiate<HUD>(this);
	Instantiate<TransitionEffect>(this);

	//�C���X�^���X��������
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

	//HUD�Ƀ|�C���^��n��
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//�e�摜�E�T�E���h�̓ǂݍ���
	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundPractice_ = Audio::Load("Sound\\BGM\\practice.wav", true);
	assert(hSoundPractice_ >= 0);

	//�t���[�v���C�p�T�E���h�Đ�
	Audio::Play(hSoundPractice_);
}

void PracticeScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTranslation�͌p����̊֐����Ă΂��
	BaseScene::Update();

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
	//�w�i�`��
	Image::SetAndDraw(hBackScreen_, this->transform_);

	//HUD�N���X�ɗ��K���[�h���ł��邱�Ƃ�`��w��
	pHUD_->SetDrawMode(S_Practice);

}

void PracticeScene::Release()
{
}

void PracticeScene::UpdateActive()
{
	//����{�^��(P�L�[�EB/Start�{�^��)�𒷉����ŃV�[���J�ڏ�Ԃ�
	if (Input::IsKey(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))//�{�^���������Ń^�C�g���ɖ߂�
	{
		Press_++;
	}
	else
	{
		Press_ = 0;
	}

	
	if (Press_ >= SceneTransition)
	{
		//�V�[���J�ڏ�Ԃ�
		SceneState_ = S_Transition;

		//�V�[���J�ڃG�t�F�N�g(�t�F�[�h�A�E�g)��ݒ�
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);
	}
}

void PracticeScene::UpdateTransition()
{
	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManager�̃C���X�^���X����^�C�g���V�[����
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);

		//�V�[���J�ڗp�^�C�}�[��߂�
		SceneTransitionTimer_ = 0;

		//���K�p�T�E���h��~
		Audio::Stop(hSoundPractice_);

		//�Q�[���V�[����Ԃ�ʏ�ɖ߂��Ă���
		SceneState_ = S_Active;
		
	}
}

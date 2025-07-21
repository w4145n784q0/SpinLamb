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
	pPlayer1_ = Instantiate<Player>(this);
	assert(pPlayer1_ != nullptr);

	//Player1�̖��O��ݒ�
	pPlayer1_->SetObjectName("Player1");

	//Player1�Ɉړ�����(�e�X�e�[�W�̒[)��n��
	pPlayer1_->SetEnd(North, South, West, East);

	//Player1��ID������U��
	pPlayer1_->SetID(1);

	//�g���R���g���[���[��ID�ݒ�
	pPlayer1_->SetControllerID(0);

	//Player1�̏�����
	pPlayer1_->PlayerInit("CSVdata\\CharacterData\\PlayerData1.csv", "Model\\chara.fbx");

	//�v���C���[�P�̃|�C���^��ݒ�
	GameView::SetPlayer1(pPlayer1_);

	//���ۂɓ����v���C���[(CPU�ł͂Ȃ�)��o�^
	ActivePlayers_.push_back(pPlayer1_);

	pPlayer1_->PlayerStart();

	//���݂̃��[�h(PvE or PvP)�ɍ��킹���L�����N�^�[�𐶐�
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager->IsPlayerVSEnemy())
	{
		//CPU(Enemy�N���X)�𐶐�
		pEnemy_ = Instantiate<Enemy>(this);
		assert(pEnemy_ != nullptr);

		//Enemy�Ɉړ�����(�e�X�e�[�W�̒[)��n��
		pEnemy_->SetEnd(North, South, West, East);

		//Enemy��ID������U��
		pEnemy_->SetID(2);

		//��ʏ�Ԃ̃Z�b�g(��l�v���C�p)
		GameView::SetGameViewMode(GameView::S_Single);
	}
	else if (pSceneManager->IsPlayerVSPlayer())
	{
		//Player2�𐶐�
		pPlayer2_ = Instantiate<Player>(this);
		assert(pPlayer2_ != nullptr);

		//Player2�̖��O��ݒ�
		pPlayer2_->SetObjectName("Player2");

		//Player2�Ɉړ�����(�e�X�e�[�W�̒[)��n��
		pPlayer2_->SetEnd(North, South, West, East);

		//Player2��ID������U��
		pPlayer2_->SetID(2);

		//�g���R���g���[���[��ID�ݒ�
		pPlayer2_->SetControllerID(1);

		//Player2�̏�����
		pPlayer2_->PlayerInit("CSVdata\\CharacterData\\PlayerData2.csv", "Model\\chara_black.fbx");

		Camera::HalfScreen();

		//��ʏ�Ԃ̃Z�b�g(��l�v���C�p�A���E��������)
		GameView::SetGameViewMode(GameView::S_Dual);

		//�v���C���[�Q�̃|�C���^��ݒ�
		GameView::SetPlayer2(pPlayer2_);

		//���ۂɓ����v���C���[(CPU�ł͂Ȃ�)��o�^
		ActivePlayers_.push_back(pPlayer2_);

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

	//�o�^���ꂽ�v���C���[���X�V
	//�v���C���[���������݂���ꍇ��z�肵��
	//Battle,Practice�V�[�����瓮����
	for (auto player : ActivePlayers_)
	{
		player->CharacterRun();
	}

	//�~�j�}�b�v�̈ʒu���X�V
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
	//P�L�[�ESL�ESR�{�^���������Ń^�C�g���ɖ߂�
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
		
		//1��ʂɖ߂�
		GameView::SetGameViewMode(GameView::S_NormalScreen);
	}
}

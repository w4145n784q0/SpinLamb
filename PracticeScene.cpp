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
	pPlayer_(nullptr),pEnemy_(nullptr), pHUD_(nullptr),pMiniMap_(nullptr),
	pTransitionEffect_(nullptr),Press_(0)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	//�e�N���X����
	Instantiate<StageManager>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	Instantiate<MiniMap>(this);
	Instantiate<HUD>(this);
	Instantiate<TransitionEffect>(this);

	//�e�摜�E�T�E���h�̓ǂݍ���
	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundPractice_ = Audio::Load("Sound\\BGM\\practice.wav", true);
	assert(hSoundPractice_ >= 0);

	//StageManager����Player,Enemy�̃C���X�^���X�𐶐���
	StageManager* pS = (StageManager*)FindObject("StageManager");
	float north = pS->GetNorthEnd();
	float south = pS->GetSouthEnd();
	float west = pS->GetWestEnd();
	float east = pS->GetEastEnd();

	// �ړ�����(�e�X�e�[�W�̒[)��n��
	pPlayer_ = (Player*)FindObject("Player");
	if(pPlayer_ != nullptr)
	{
		pPlayer_->PlayerStart();
		pPlayer_->SetEnd(north, south, west, east);
	}

	pEnemy_ = (Enemy*)FindObject("Enemy");
	if(pEnemy_ != nullptr)
	{
		pEnemy_->EnemyStop();
		pEnemy_->SetEnd(north, south, west, east);
	}

	//�C���X�^���X��������
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

	//HUD�Ƀ|�C���^��n��
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//�t���[�v���C�p�T�E���h�Đ�
	Audio::Play(hSoundPractice_);
}

void PracticeScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTranslation�͌p����̊֐����Ă΂��
	BaseScene::Update();
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

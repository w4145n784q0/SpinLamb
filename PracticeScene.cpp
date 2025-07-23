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

namespace
{
	//player���������̕�����z��
	std::string PlayerNames[] = { "Player1","Player2" };

	std::string csvPath[] = { "CSVdata\\CharacterData\\PlayerData1.csv" ,
		"CSVdata\\CharacterData\\PlayerData2.csv" };

	std::string modelPath[] = { "Model\\chara.fbx" ,"Model\\chara_black.fbx" };
}

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

	//�e�L�����N�^�[�̏������p�̔z��
	std::vector<Player*> InitPlayers = {};
	std::vector<Enemy*> InitEnemys = {};
	std::vector<Character*> InitCharacters = {};

	//�o�ꂳ����L�����N�^�[�͎��O�ɐ��������s��
	//�l�̏������͌�ɍs��
	pPlayer1_ = Instantiate<Player>(this);
	assert(pPlayer1_ != nullptr);

	//�������z��ɒǉ�
	InitPlayers.push_back(pPlayer1_);
	InitCharacters.push_back(pPlayer1_);

	//�u�ЂƂ�Łv���u�ӂ���Łv���Ő�������L�����N�^�[��ύX
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager->IsPlayerVSEnemy())
	{
		//���O�ɐ���
		pEnemy_ = Instantiate<Enemy>(this);
		assert(pEnemy_ != nullptr);

		//�������z��ɒǉ�
		InitCharacters.push_back(pEnemy_);
		InitEnemys.push_back(pEnemy_);

		//��ʏ�Ԃ̃Z�b�g(��l�v���C�p)
		GameView::SetGameViewMode(GameView::S_Single);

		//��ʂ���l�v���C�p�ɐݒ�
		Camera::FullScreen();
	}
	if (pSceneManager->IsPlayerVSPlayer())
	{
		//���O�ɐ���
		pPlayer2_ = Instantiate<Player>(this);
		assert(pPlayer2_ != nullptr);

		//�������z��ɒǉ�
		InitPlayers.push_back(pPlayer2_);
		InitCharacters.push_back(pPlayer2_);

		//��ʏ�Ԃ̃Z�b�g(��l�v���C�p�A���E��������)
		GameView::SetGameViewMode(GameView::S_Dual);

		//��l�v���C�p�̃J������ݒ�
		Camera::HalfScreen();
	}

	//Player,Enemy���ʂ̏���������
	for (int i = 0; i < InitCharacters.size(); i++)
	{
		//�ړ�����(�e�X�e�[�W�̒[)��n��
		InitCharacters[i]->SetEnd(North, South, West, East);

		//ID������U��
		InitCharacters[i]->SetID(i + 1);
	}

	//Player�̏���������
	for (int i = 0; i < InitPlayers.size(); i++)
	{
		//�v���C���[�̖��O��ݒ�
		InitPlayers[i]->SetObjectName(PlayerNames[i]);

		//�g���R���g���[���[��ID�ݒ�
		InitPlayers[i]->SetControllerID(i);

		//�v���C���[�̏�����(csv�A���f���̃p�X)
		InitPlayers[i]->PlayerInit(csvPath[i], modelPath[i]);

		//���ۂɓ����v���C���[(CPU�ł͂Ȃ�)��o�^
		ActivePlayers_.push_back(InitPlayers[i]);

		//�v���C���[�Ɉړ�������
		InitPlayers[i]->PlayerStart();
	}
	
	//Enemy�̏���������
	for (int i = 0; i < InitEnemys.size(); i++)
	{
		if (!InitPlayers.empty())
		{
			//�v���C���[�̏������z�񂩂烉���_���ȃC���X�^���X���Z�b�g
			//(����͓G�ƈ�Έ�Ȃ̂�player1���I�΂��)
			//BattleScene����ݒ肷�邱�ƂŒl�̑���E�����x�̏㏸��h��
			InitEnemys[i]->SetPlayerPointer(InitPlayers[rand() % InitPlayers.size()]);

			//���������G��o�^
			ActiveEnemys_.push_back(InitEnemys[i]);
		}
	}

	//�v���C���[�̃|�C���^��ݒ�
	GameView::SetPlayers(InitPlayers);

	//�G�̃|�C���^��ݒ�
	GameView::SetEnemy(pEnemy_);

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

	//GameView��HUD�̃|�C���^��n��
	GameView::SetHUD(pHUD_);


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

	//�o�^���ꂽ�v���C���[�ECPU���X�V
	//�v���C���[���������݂���ꍇ��z�肵��
	//Battle,Practice�V�[�����瓮����
	for (auto player : ActivePlayers_)
	{
		player->PlayerRun();
	}
	for (auto enemy : ActiveEnemys_)
	{
		enemy->EnemyRun();
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

		//��ʕ������Ă���Ȃ�J������S�̕\���ɖ߂�
		if (GameView::IsDual())
		{
			Camera::FullScreen();
		}
	}
}

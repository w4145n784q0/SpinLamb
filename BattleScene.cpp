#include "BattleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"StageManager.h"

#include"Engine/Camera.h"

#include"GameView.h"

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
	FirstScore_(0),SecondScore_(0),
	pPlayer1_(nullptr), pPlayer2_(nullptr), pEnemy_(nullptr),
	pHUD_(nullptr),pMiniMap_(nullptr),
	pGameTimer_(nullptr),pTransitionEffect_(nullptr)
{
}

BattleScene::~BattleScene()
{
}

void BattleScene::Initialize()
{
	//csv����p�����[�^�ǂݍ���
	SetCSVBattle();

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

	//player1���Ď��Ώۂɒǉ�
	pPlayer1_->AddObserver(this);

	//�v���C���[�P�̃|�C���^��ݒ�
	GameView::SetPlayer1(pPlayer1_);

	//���ۂɓ����v���C���[(CPU�ł͂Ȃ�)��o�^
	ActivePlayers_.push_back(pPlayer1_);


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

		//Enemy���Ď��Ώۂɒǉ�
		pEnemy_->AddObserver(this);

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

		//player2���Ď��Ώۂɒǉ�
		pPlayer2_->AddObserver(this);

		Camera::HalfScreen();
		
		//��ʏ�Ԃ̃Z�b�g(��l�v���C�p�A���E��������)
		GameView::SetGameViewMode(GameView::S_Dual);

		//�v���C���[�Q�̃|�C���^��ݒ�
		GameView::SetPlayer2(pPlayer2_);

		//���ۂɓ����v���C���[(CPU�ł͂Ȃ�)��o�^
		ActivePlayers_.push_back(pPlayer2_);
	}

	//�e�N���X����
	Instantiate<MiniMap>(this);
	Instantiate<GameTimer>(this);
	Instantiate<HUD>(this);
	Instantiate<TransitionEffect>(this);

	//�C���X�^���X��������
	pGameTimer_ = (GameTimer*)FindObject("GameTimer");
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

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

	hSoundWhistle_ = Audio::Load("Sound\\SE\\whistle.wav");
	assert(hSoundWhistle_ >= 0);

	//Player,Enemy�̃X�R�A��������
	pHUD_->SetFirstScore(FirstScore_);
	pHUD_->SetSecondScore(SecondScore_);


}

void BattleScene::Update()
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
	for (auto enemy : ActiveEnemys_)
	{
		//
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

void BattleScene::Draw()
{
	//�w�i�`��
	Image::SetAndDraw(hBackScreen_, this->transform_);

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

	//���Ԍo�߂Ŏ��̃V�[���ɑJ��
	//�J�E���g���̓V�[���J�ڃG�t�F�N�g�s��
	if (++SceneTransitionTimer_ > SceneShortTransition)
	{

		//SceneManager�̃C���X�^���X����^�C�g���V�[����
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT);

		//Player,Enemy�̃X�R�A��n��
		pSceneManager->SetFirstCharaScore(FirstScore_);
		pSceneManager->SetSecondCharaScore(SecondScore_);

		//�V�[���J�ڗp�^�C�}�[��߂�
		SceneTransitionTimer_ = 0;

		//�o�g���p�T�E���h��~
		Audio::Stop(hSoundBattle_);

		//�Q�[���V�[����Ԃ�ʏ�ɖ߂��Ă���
		SceneState_ = S_Active;

		//1��ʂɖ߂�
		GameView::SetGameViewMode(GameView::S_NormalScreen);
	}
}

void BattleScene::UpdateBattleBefore()
{
	//���������o���Ă�����
	//B�{�^����P�L�[�ŃX�L�b�v
	if (++StateCounter > SceneTransition ||
		Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		//�V�[���J�ڃ^�C�}�[�����Z�b�g
		StateCounter = 0;

		//���Ԍo�߂�����J�n���O��Ԃֈȍ~
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
		//�V�[���J�ڃ^�C�}�[�����Z�b�g
		StateCounter = 0;

		//���Ԍo�߂�����o�g������Ԃֈȍ~
		BattleState_ = S_NOW;

		//���Ԍo�߂�Player,Enemy�Ɉړ������o��
		pPlayer1_->PlayerStart();
		//pEnemy_->EnemyStart();

		if (pPlayer2_ != nullptr)
		{
			pPlayer2_->PlayerStart();
		}
		else if (pEnemy_ != nullptr)
		{
			pEnemy_->EnemyStart();
		}

		//�^�C�}�[���N��
		pGameTimer_->StartTimer();

		//�z�C�b�X��SE�Đ�
		Audio::Play(hSoundWhistle_);
	}
}

void BattleScene::UpdateBattle()
{
	//�Q�[���v���C��
	//GameTimer���玞�Ԑ؂�ɂȂ������Ƃ��󂯎������I����Ԃ�
	if (pGameTimer_->GetCurrentGameTime() <= 0)
	{
		//�I����Ԃֈȍ~
		BattleState_ = S_AFTER;

		//�^�C�}�[���~�߂�
		pGameTimer_->StopTimer();

		//player,Enemy�̓������~�߂�
		pPlayer1_->PlayerStop();

		if (pPlayer2_ != nullptr)
		{
			pPlayer2_->PlayerStop();
		}
		else if (pEnemy_ != nullptr)
		{
			pEnemy_->EnemyStop();
		}

		//�z�C�b�X��SE�Đ�
		Audio::Play(hSoundWhistle_);
	}

	//�o�g�����T�E���h�Đ�
	Audio::Play(hSoundBattle_);

	//�X�R�A�͖��t���[���n��������
	pHUD_->SetFirstScore(FirstScore_);
	pHUD_->SetSecondScore(SecondScore_);
}

void BattleScene::UpdateBattleAfter()
{
	//Finish!��\�����Ă�����
	if (++StateCounter > SceneTransition)
	{
		//���Ԍo�߂�UpdateTransition��
		SceneState_ = S_Transition;

		//�V�[���J�ڃ^�C�}�[�����Z�b�g
		StateCounter = 0;

		//�V�[���J�ڃG�t�F�N�g(�Y�[���C��)��ݒ�
		pTransitionEffect_->FadeOutStartWhite();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);

	}
}

void BattleScene::SetCSVBattle()
{
	//csv�t�@�C���ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\BattleData.csv");

	//csv�t�@�C���̊e0��ڂ̕�������擾
	std::string battleName = "Battle";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> battleData = GetCSVReadData(csv, battleName);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	GameTimeLimit = static_cast<int>(battleData[i_gametimelimit]);
}

void BattleScene::OnCharacterFenceHit(int HitCharaID)
{
	if (pPlayer1_->GetID() == HitCharaID)
	{
		PlusSecondScore();
	}
	else if (pEnemy_ != nullptr)
	{
		if (pEnemy_->GetID() == HitCharaID)
		{
			PlusFirstScore();
		}
	}
	else if (pPlayer2_ != nullptr)
	{
		if (pPlayer2_->GetID() == HitCharaID)
		{
			PlusFirstScore();
		}
	}
}

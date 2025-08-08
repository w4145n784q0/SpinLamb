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

	//�������Ԃ̂����A�c��n�b�Ŋg��C�[�W���O�������s���ۂ̊J�n����
	//����͎c��10�b�Ő������Ԃ��g��
	int EasingTime = 0;

	//�o�g���V�[�����������̃C���f�b�N�X
	enum ScorePosIndex
	{
		i_gametimelimit = 0,
		i_easingtime,
	};
}

BattleScene::BattleScene(GameObject* parent)
	:BaseScene(parent,"BattleScene") ,BattleState_(S_BEFORE),
	 hBackScreen_(-1), hSoundBattle_(-1), hSoundWhistle_(-1),
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

		//�Ď��Ώۂɒǉ�
		InitCharacters[i]->AddObserver(this);
	}

	//Player�̏���������

	//player���������̕�����z��
	std::string PlayerNames[] = { "Player1","Player2" };

	//�v���C���[���������ɓǂݍ���csv�t�@�C���̃p�X
	std::string csvPath[] = { "CSVdata\\CharacterData\\PlayerData1.csv" ,
		"CSVdata\\CharacterData\\PlayerData2.csv" };

	//�v���C���[���������ɓǂݍ���fbx���f���̃p�X
	std::string modelPath[] = { "Model\\Character.fbx" ,"Model\\Character_black.fbx" };

	//InitPlayers������������
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

	//GameView�Ƀ|�C���^��n��
	GameView::SetHUD(pHUD_);

	//�Q�[���������Ԃ�n��
	pGameTimer_->SetCurrentGameTime(GameTimeLimit);

	//�C�[�W���O�J�n���Ԃ�n��
	//GameTimer���󂯎��ۊǁAHUD���Ő������Ԃ�EasingTime�ȉ��ɂȂ������Ƃ����m��
	//�C�[�W���O�������s��
	pGameTimer_->SetEasingTime(EasingTime);

	//�e�摜�E�T�E���h�̓ǂݍ���
	
	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string Battle = "Image\\Battle\\";
	std::string Sound = "Sound\\";
	std::string BGM = "BGM\\";
	std::string SE = "SE\\";

	hBackScreen_ = Image::Load(Battle + "BackSky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundBattle_ = Audio::Load(Sound + BGM + "Battle.wav", true);
	assert(hSoundBattle_>= 0);

	hSoundWhistle_ = Audio::Load(Sound + SE + "Whistle.wav", false, Audio::GetWhistleNum());
	assert(hSoundWhistle_ >= 0);

	//Player,Enemy�̃X�R�A��������
	pHUD_->SetFirstScore(FirstScore_);
	pHUD_->SetSecondScore(SecondScore_);


}

void BattleScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTransition�͌p����̊֐����Ă΂��
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
	//Enemy,Player2��null�`�F�b�N���s��,���݂���Ȃ�ʒu�f�[�^��n��
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

		//�V�[���J�ڗp�^�C�}�[�����Z�b�g
		SceneTransitionTimer_ = 0;

		//�o�g���p�T�E���h��~
		Audio::Stop(hSoundBattle_);

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
		//Enemy,Player2��null�`�F�b�N���s���A���݂���Ȃ���s
		pPlayer1_->PlayerStart();

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
		//Enemy,Player2��null�`�F�b�N���s���A���݂���Ȃ���s

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
	EasingTime = static_cast<int>(battleData[i_easingtime]);
}

void BattleScene::OnCharacterFenceHit(int _HitCharaID)
{
	//_HitCharaID�ɂ͍�ɐڐG�����L�����N�^�[ID������
	//���ꂼ��ڐG�����L�����N�^�[ID���m�F��
	//��v����Ȃ瑊�葤�̃X�R�A�����Z����
	//Enemy,Player2�͕K��null�`�F�b�N����

	if (pPlayer1_->GetID() == _HitCharaID)
	{
		PlusSecondScore();
	}
	else if (pEnemy_ != nullptr)
	{
		if (pEnemy_->GetID() == _HitCharaID)
		{
			PlusFirstScore();
		}
	}
	else if (pPlayer2_ != nullptr)
	{
		if (pPlayer2_->GetID() == _HitCharaID)
		{
			PlusFirstScore();
		}
	}
}

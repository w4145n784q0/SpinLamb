#include "PracticeScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Camera.h"
#include"../StageSourceFile/StageManager.h"
#include"../ViewSourceFile/GameView.h"

namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X(���K�V�[���̊e�ϐ�)
	enum PracticeIndex
	{
		i_CharacterAddID = 0,
	};

	//�L�����N�^�[�̏��������AID�ɉ��Z����l(for����i�ɉ��Z����l)
	int CharacterAddID = 0;
}

PracticeScene::PracticeScene(GameObject* parent)
	:PlayScene(parent,"PracticeScene"),
	hSoundPractice_(-1),
	pPlayer1_(nullptr), pPlayer2_(nullptr), pEnemy_(nullptr), pHUD_(nullptr),
	pTransitionEffect_(nullptr), pMiniMap_(nullptr),
	ActivePlayers_({}), ActiveEnemys_({}),PracticeState_(S_Now)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	//�v���C�V�[��(���N���X)�̏��������s��
	PlayScene::Initialize();

	//csv����p�����[�^�ǂݍ���
	SetCSVPractice();

	//StageManager�N���X����
	Instantiate<StageManager>(this);

	//StageManager����e�ړ������̒l���擾
	StageManager* pStageManager = (StageManager*)FindObject("StageManager");
	assert(pStageManager != nullptr);

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
	assert(pSceneManager != nullptr);

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
		InitCharacters[i]->SetID(i + CharacterAddID);
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
			//PracticeScene����ݒ肷�邱�ƂŒl�̑���E�����x�̏㏸��h��
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
	assert(pMiniMap_ != nullptr);

	pHUD_ = (HUD*)FindObject("HUD");
	assert(pHUD_ != nullptr);

	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
	assert(pTransitionEffect_ != nullptr);

	//MiniMap�̃|�C���^��n��
	//HUD�N���X�Ɠ����|�C���^��n�����ƂŒl�̑����h��
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//GameView��HUD�̃|�C���^��n��

	GameView::SetHUD(pHUD_);
	GameView::SetTransitionEffect(pTransitionEffect_);

	//�e�摜�E�T�E���h�̓ǂݍ���

	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string Sound = "Sound\\";
	std::string BGM = "BGM\\";

	hSoundPractice_ = Audio::Load(Sound + BGM + "Practice.wav", true);
	assert(hSoundPractice_ >= 0);

	//�t���[�v���C�p�T�E���h�Đ�
	Audio::Play(hSoundPractice_);
}

void PracticeScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTransition�͌p����̊֐����Ă΂��
	BaseScene::Update();
}

void PracticeScene::Draw()
{
	//�w�i�`��
	PlayScene::DrawBackScreen();

	//����PracticeState�̏�Ԃ���AHUD�N���X�ɕ`�悷����̂��w��
	switch (PracticeState_)
	{
	case PracticeScene::S_Now:
		pHUD_->SetDrawMode(Mode_Practice);
		break;
	case PracticeScene::S_Pause:
		pHUD_->SetDrawMode(Mode_PracticePause);
		break;
	default:
		break;
	}

}

void PracticeScene::Release()
{
	//���̂͏������A�A�h���X�̂ݖ���������
	if (pPlayer1_ != nullptr)
	{
		pPlayer1_ = nullptr;
	}
	if (pPlayer2_ != nullptr)
	{
		pPlayer2_ = nullptr;
	}
	if (pEnemy_ != nullptr)
	{
		pEnemy_ = nullptr;
	}
	if (pHUD_ != nullptr)
	{
		pHUD_ = nullptr;
	}
	if (pTransitionEffect_ != nullptr)
	{
		pTransitionEffect_ = nullptr;
	}

	if (pMiniMap_ != nullptr)
	{
		pMiniMap_ = nullptr;
	}
}

void PracticeScene::UpdateActive()
{
	//�ʏ�̓����Ă�����

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

	PlayScene::WaitGotoPause();
}

void PracticeScene::UpdateInActive()
{	
	//��ʂ��~�߂Ă�����

	//Pause�\�����̏���
	PlayScene::UpdatePauseMenu();
}

void PracticeScene::UpdateTransition()
{
	//�V�[���J�ڏ��

	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManager�̃C���X�^���X����^�C�g���V�[����
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		assert(pSceneManager != nullptr);

		pSceneManager->ChangeScene(SCENE_ID_TITLE);

		//�V�[���J�ڗp�^�C�}�[�����Z�b�g
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

void PracticeScene::GotoPause()
{
	//�|�[�Y��ʂɌ��������� PlayScene����㏑��

	//�|�[�Y��ʏ�Ԃֈڍs
	PracticeState_ = S_Pause;
}

void PracticeScene::GotoPlay()
{
	//�Q�[����ʂɌ��������� PlayScene����㏑��

	//���K����Ԃֈڍs
	PracticeState_ = S_Now;
}

void PracticeScene::GotoTitle()
{
	//�^�C�g���Ɍ��������� PlayScene����㏑��

	//�V�[���J�ڃG�t�F�N�g(�����t�F�[�h�A�E�g)��ݒ�
	if (pTransitionEffect_ != nullptr)
	{
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);
	}
}

void PracticeScene::SetPauseIconY()
{
	if (pHUD_ != nullptr)
	{
		pHUD_->SetPauseIcon(TmpIconPosY_);
	}
}

void PracticeScene::SetCSVPractice()
{
	//csv�t�@�C���ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\PracticeData.csv");

	//csv�t�@�C���̊e0��ڂ̕�������擾
	std::string PracticeName = "Practice";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> PracticeData = GetCSVReadData(csv, PracticeName);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	CharacterAddID = static_cast<int>(PracticeData[i_CharacterAddID]);
}

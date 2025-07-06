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
	pPlayer_(nullptr),pEnemy_(nullptr),pHUD_(nullptr),pMiniMap_(nullptr),
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

	//�e�N���X����
	Instantiate<StageManager>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	Instantiate<MiniMap>(this);
	Instantiate<GameTimer>(this);
	Instantiate<HUD>(this);
	Instantiate<TransitionEffect>(this);

	//StageManager����Player,Enemy�̃C���X�^���X�𐶐�
	StageManager* pSceneManager = (StageManager*)FindObject("StageManager");
	float North = pSceneManager->GetNorthEnd();
	float South = pSceneManager->GetSouthEnd();
	float West = pSceneManager->GetWestEnd();
	float East = pSceneManager->GetEastEnd();

	// �ړ�����(�e�X�e�[�W�̒[)��n��
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
		pSceneManager->SetPlayerScore(PlayerScore_);
		pSceneManager->SetEnemyScore(EnemyScore_);

		//�V�[���J�ڗp�^�C�}�[��߂�
		SceneTransitionTimer_ = 0;

		//�o�g���p�T�E���h��~
		Audio::Stop(hSoundBattle_);

		//�Q�[���V�[����Ԃ�ʏ�ɖ߂��Ă���
		SceneState_ = S_Active;
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
		pPlayer_->PlayerStart();
		pEnemy_->EnemyStart();

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
		pPlayer_->PlayerStop();
		pEnemy_->EnemyStop();

		//�z�C�b�X��SE�Đ�
		Audio::Play(hSoundWhistle_);
	}

	//�o�g�����T�E���h�Đ�
	Audio::Play(hSoundBattle_);

	//�X�R�A�͖��t���[���n��������
	pHUD_->SetPlayerScore(PlayerScore_);
	pHUD_->SetEnemyScore(EnemyScore_);
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
	std::string pos = "Position";

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (csv.IsGetParamName(pos))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv.GetParam(pos);

		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		GameTimeLimit = static_cast<int>(v[i_gametimelimit]);
	}
}

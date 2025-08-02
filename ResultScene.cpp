#include "ResultScene.h"
#include"Engine/CsvReader.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"



namespace
{
	//���ʂ̔���

	//�L�����N�^�[1�̃X�R�A
	int FirstCharaScore = 0;

	//�L�����N�^�[2(CPU)�̃X�R�A
	int SecondCharaScore = 0;
}

ResultScene::ResultScene(GameObject* parent)
	:BaseScene(parent, "ResultScene"), hBackScreen_(-1), hYouWin_(-1),hCpuWin_(-1),
	hPlayer1Win_(-1),hPlayer2Win_(-1),
	hDraw_(-1), hPushTitle_(-1),
	hSoundResult_(-1), hSoundBackTitle_(-1), winner_(RESULTMAX), ResultArray_({}),
	pTransitionEffect_(nullptr)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	//�e�N���X����
	Instantiate<TransitionEffect>(this);

	//csv����p�����[�^�ǂݍ���
	SetCSVResult();

	//�e�摜�E�T�E���h�̓ǂݍ���
	hBackScreen_ = Image::Load("Image\\GameMode\\GameModeScreen.jpg");
	assert(hBackScreen_ >= 0);

	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string path = "Image\\Result\\";

	hYouWin_ = Image::Load(path + "YouWin.png");
	assert(hYouWin_ >= 0);

	hCpuWin_ = Image::Load(path + "CPUWin.png");
	assert(hCpuWin_ >= 0);

	hPlayer1Win_ = Image::Load(path + "Player1Win.png");
	assert(hPlayer1Win_);

	hPlayer2Win_ = Image::Load(path + "Player2Win.png");
	assert(hPlayer2Win_);

	hDraw_ = Image::Load(path + "Draw.png");
	assert(hDraw_ >= 0);

	hPushTitle_ = Image::Load(path + "PushToTitle.png");
	assert(hPushTitle_ >= 0);

	hSoundResult_ = Audio::Load("Sound\\BGM\\Result.wav", true);
	assert(hSoundResult_ >= 0);

	hSoundBackTitle_ = Audio::Load("Sound\\SE\\Cancel.wav", false, Audio::GetCancelNum());
	assert(hSoundBackTitle_ >= 0);

	//SceneManager�C���X�^���X����BattleScene����e�X�R�A���擾
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	FirstCharaScore =  pSceneManager->GetFirstCharaScore();
	SecondCharaScore = pSceneManager->GetSecondCharaScore();

	//�X�R�A�ɂ���ď��s��Ԃ�ω�
	if (FirstCharaScore > SecondCharaScore)
	{
		winner_ = YOU_WIN;
	}
	else if (FirstCharaScore < SecondCharaScore)
	{
		winner_ = CPU_WIN;
	}
	else if (FirstCharaScore == SecondCharaScore)
	{
		winner_ = DRAW;
	}

	//�摜�n���h���z���������(�v���C�l���ɂ���ĕ�����)
	if (pSceneManager->IsPlayerVSEnemy())
	{
		ResultArray_ = { hYouWin_, hCpuWin_, hDraw_ };
	}
	else if (pSceneManager->IsPlayerVSPlayer())
	{
		ResultArray_ = { hPlayer1Win_, hPlayer2Win_, hDraw_ };
	}

	//�C���X�^���X����
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
}

void ResultScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTransition�͌p����̊֐����Ă΂��
	BaseScene::Update();
	Audio::Play(hSoundResult_);
}

void ResultScene::Draw()
{
	//�w�i�`��
	Image::SetAndDraw(hBackScreen_, this->transform_);

	//"Push Title"�`��
	Image::SetAndDraw(hPushTitle_, PushTitle_);

	//���ʂ̃e�L�X�g�摜��\��
	//ResultArray_�̓Y�����ɏ��s��Ԃ��g��
	switch (winner_)
	{
	case ResultScene::YOU_WIN:
	case ResultScene::CPU_WIN:
	case ResultScene::DRAW:
	{
		Image::SetAndDraw(ResultArray_[winner_], Result_);
	}
		break;
	default:
		break;
	}
	

#ifdef _DEBUG
	if (ImGui::TreeNode("GameModeSelect"))
	{
		if (ImGui::TreeNode("Result"))
		{
			ImGui::SliderFloat("ResultPositionX", &Result_.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("ResultPositionY", &Result_.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("PushTitle"))
		{
			ImGui::SliderFloat("PushTitlePositionX", &PushTitle_.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("PushTitlePositionY", &PushTitle_.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif
}

void ResultScene::Release()
{
}

void ResultScene::SetCSVResult()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\ResultData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾
	std::vector<std::string> ParamNames = {
		"Result","PushTitle"
	};

	//�摜�p�g�����X�t�H�[����z��ɓ����
	std::vector<std::reference_wrapper<Transform>> ImageArray = {
		Result_,PushTitle_
	};

	//�܂Ƃ߂ď�����
	InitCSVTransformArray(csv, ParamNames, ImageArray);
}

void ResultScene::UpdateActive()
{
	//����{�^��(P�L�[�EB/Start�{�^��)����������V�[���J�ڏ�Ԃ�
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;

		//�V�[���J�ڃG�t�F�N�g(�t�F�[�h�A�E�g)��ݒ�
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);

		//���艹���Đ�
		Audio::Play(hSoundBackTitle_);
	}
}

void ResultScene::UpdateTransition()
{
	//���Ԍo�߂Ŏ��̃V�[���ɑJ��
	//�J�E���g���̓V�[���J�ڃG�t�F�N�g�s��
	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManager�̃C���X�^���X����^�C�g���V�[����
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);

		//�V�[���J�ڗp�^�C�}�[�����Z�b�g
		SceneTransitionTimer_ = 0;

		//���U���g�p�T�E���h��~
		Audio::Stop(hSoundResult_);

		//�Q�[���V�[����Ԃ�ʏ�ɖ߂��Ă���
		SceneState_ = S_Active;
	}
}

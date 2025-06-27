#include "GameModeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"Engine/CsvReader.h"

namespace
{
	//�v���C�{�^��,�t���[�v���C�{�^��,�V�ѕ��{�^��,�^�C�g���{�^��
	std::string ParamArray[] = { "Battle", "Practice", "HowToPlay", "BackTitle" };
}

GameModeScene::GameModeScene(GameObject* parent)
	:BaseScene(parent, "GameModeScene"), 
	hBackScreen_(-1), hBackChara_(-1),
	hBattle_(-1),hPractice_(-1), hHowtoPlay_(-1),hBackTitle_(-1), hFrameLine_(-1),
	hModeSelect_(-1), hBattleText_(-1), hFreePlayText_(-1), hHowtoPlayText_(-1),hTitleText_(-1),
	TextArray_({}),ButtonImageArray_({}), ModeTransArray_({}),
	hSoundGameMode_(-1), hSoundSelect_(-1), hSoundDecide_(-1), SelectMode_(S_Battle),
	pTransitionEffect_(nullptr)
{
}

GameModeScene::~GameModeScene()
{
}

void GameModeScene::Initialize()
{
	//�e�N���X����
	Instantiate<TransitionEffect>(this);

	//csv����p�����[�^�ǂݍ���
	SetGameModeSCV();

	//�e�摜�E�T�E���h�̓ǂݍ���

	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string path = "Image\\GameMode\\";

	hBackScreen_ = Image::Load(path + "back_mode.jpg");
	assert(hBackScreen_ >= 0);

	hBackChara_ = Image::Load(path + "sheepImage.png");
	assert(hBackChara_ >= 0);

	hBattle_ = Image::Load(path + "BattleButton.png");
	assert(hBattle_ >= 0);

	hPractice_ = Image::Load(path + "FreeplayButton.png");
	assert(hPractice_ >= 0);

	hHowtoPlay_ = Image::Load(path + "HowToPlayButton.png");
	assert(hHowtoPlay_ >= 0);

	hBackTitle_ = Image::Load(path + "TitleButton.png");
	assert(hBackTitle_ >= 0);

	hFrameLine_ = Image::Load(path + "LineFrame.png");
	assert(hFrameLine_ >= 0);

	hModeSelect_ = Image::Load(path + "ModeSelect.png");
	assert(hModeSelect_>= 0);

	hBattleText_ = Image::Load(path + "BattleText.png");
	assert(hBattleText_ >= 0);

	hFreePlayText_ = Image::Load(path + "FreeplayText.png"); 
	assert(hFreePlayText_ >= 0);

	hHowtoPlayText_ = Image::Load(path + "HowToPlayText.png");
	assert(hHowtoPlayText_ >= 0);

	hTitleText_ = Image::Load(path + "TitleText.png");
	assert(hTitleText_ >= 0);

	hSoundGameMode_ = Audio::Load("Sound\\BGM\\GameMode.wav",true);
	assert(hSoundGameMode_ >= 0);

	hSoundDecide_ = Audio::Load("Sound\\SE\\decide.wav");
	assert(hSoundDecide_ >= 0);

	hSoundSelect_ = Audio::Load("Sound\\SE\\Select.wav");
	assert(hSoundSelect_ >= 0);

	//�e���[�h�̉摜�n���h����z��ɓ����
	ButtonImageArray_ = { hBattle_, hPractice_, hHowtoPlay_, hBackTitle_ };

	//�e�e�L�X�g�摜�n���h����z��ɓ����
	TextArray_ = {hBattleText_, hFreePlayText_, hHowtoPlayText_, hTitleText_};
	
	//�e���[�h�����X�g�ɓ����
	ModeList_ = { S_Battle,S_Practice,S_HowToPlay,S_Title };

	//�C���f�b�N�X�̏����ʒu���w��
	itr = ModeList_.begin();

	//�C���X�^���X����
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

	//���[�h�I��p�T�E���h�Đ�
	Audio::Play(hSoundGameMode_);
}

void GameModeScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTranslation�͌p����̊֐����Ă΂��
	BaseScene::Update();
}

void GameModeScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

	Image::SetTransform(hBackChara_, BackChara_);
	Image::Draw(hBackChara_);

	Image::SetTransform(hModeSelect_, TransSelect_);
	Image::Draw(hModeSelect_);

	for(int i = 0; i < ModeTransArray_.size(); i++)
	{
		Image::SetTransform(ButtonImageArray_[i], ModeTransArray_[i]);
		Image::Draw(ButtonImageArray_[i]);
	}

	switch (SelectMode_)
	{
	case GameModeScene::S_Battle:
	case GameModeScene::S_Practice:
	case GameModeScene::S_HowToPlay:
	case GameModeScene::S_Title:
	{
		Image::SetTransform(TextArray_[SelectMode_], TransText_);
		Image::Draw(TextArray_[SelectMode_]);
	}
		break;
	default:
		break;
	}

	Image::SetTransform(hFrameLine_, TransFrame_);
	Image::Draw(hFrameLine_);

#ifdef _DEBUG
#endif
}

void GameModeScene::Release()
{
}

void GameModeScene::SetGameModeSCV()
{
	//�e�摜��\������ۂ̃g�����X�t�H�[��������
	CsvReader csv;
	csv.Load("CSVdata\\GameModeData.csv");

	//�I��g
	InitCSVTransform(csv, "FrameLine", TransFrame_);

	//�e�{�^��
    for (int i = 0; i < sizeof(ParamArray) / sizeof(ParamArray[0]); i++)
    {
		InitCSVTransform(csv, ParamArray[i], ModeTransArray_[i]);
    }

	//"���[�h�Z���N�g"
	InitCSVTransform(csv, "ModeSelect", TransSelect_);

	//��ʉ����̃e�L�X�g
	InitCSVTransform(csv, "Text", TransText_);
}

void GameModeScene::UpdateActive()
{

	//�{�^���̑I��g�̈ړ�
	//�C���f�b�N�X���擪/�����Ȃ疖��/�擪�֖߂�
	//�O�u�f�N�������g�Ŕz��I�[�o�[�h��

	if (Input::IsKeyDown(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP))
	{

		if (itr == ModeList_.begin())
		{
			itr = --ModeList_.end();
		}
		else
		{
			--itr;
		}
		SelectMode_ = *itr;
		Audio::Play(hSoundSelect_);
	}
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
	{
		if (itr == --ModeList_.end())
		{
			itr = ModeList_.begin();
		}
		else
		{
			++itr;
		}
		SelectMode_ = *itr;
		Audio::Play(hSoundSelect_);
	}

	//�I��g�̈ʒu��I�𒆂̃��[�h�ɍ��킹��
	switch (SelectMode_)
	{
	case GameModeScene::S_Battle:
	case GameModeScene::S_Practice:
	case GameModeScene::S_HowToPlay:
	case GameModeScene::S_Title:
		TransFrame_.position_.y = ModeTransArray_[SelectMode_].position_.y;
		break;
	default:
		break;
	}

	////����{�^��(P�L�[�EB/Start�{�^��)����������V�[���J�ڏ�Ԃ�
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//���艹���Đ�
		Audio::Play(hSoundDecide_);
		SceneState_ = S_Transition;

		//�V�[���J�ڃG�t�F�N�g(�Y�[���C��)��ݒ�
		pTransitionEffect_->ZoomInStart();
		pTransitionEffect_->SetTransitionTime(SceneTransition);
	}
}

void GameModeScene::UpdateTransition()
{
	//���Ԍo�߂Ŏ��̃V�[���ɑJ��
	//�J�E���g���̓V�[���J�ڃG�t�F�N�g�s��

	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManager�C���X�^���X����I�����Ă���V�[���֑J�ڂ���

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		switch (SelectMode_)
		{
		case GameModeScene::S_Battle:
			pSceneManager->ChangeScene(SCENE_ID_BATTLE);
			break;
		case GameModeScene::S_Practice:
			pSceneManager->ChangeScene(SCENE_ID_PRACTICE);
			break;
		case GameModeScene::S_HowToPlay:
			pSceneManager->ChangeScene(SCENE_ID_HOWTOPLAY);
			break;
		case GameModeScene::S_Title:
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
			break;
		default:
			break;
		}

		//�V�[���J�ڗp�^�C�}�[��߂�
		SceneTransitionTimer_ = 0;

		//���[�h�I��p�T�E���h��~
		Audio::Stop(hSoundGameMode_);

		//�Q�[���V�[����Ԃ�ʏ�ɖ߂��Ă���
		SceneState_ = S_Active;
	}
}

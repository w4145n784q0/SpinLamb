#include "GameModeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"Engine/CsvReader.h"

#include"GameView.h"
namespace
{
	//�v���C�{�^��,�t���[�v���C�{�^��,�V�ѕ��{�^��,�^�C�g���{�^��
	//�N���X���Ŏg���܂킷������Ȃ̂�namespace�ɐ錾
	std::string ModeStringArray[] = { "Battle", "Practice", "HowToPlay", "BackTitle" };
}

GameModeScene::GameModeScene(GameObject* parent)
	:BaseScene(parent, "GameModeScene"), 
	hBackScreen_(-1), hBackChara_(-1),
	hBattle_(-1),hPractice_(-1), hHowtoPlay_(-1),hBackTitle_(-1), hFrameLine_(-1),
	hModeSelect_(-1), hBattleText_(-1), hFreePlayText_(-1), hHowtoPlayText_(-1),hTitleText_(-1),
	hPlayerNumSelect_(-1), hPlayerSelectIcon_(-1), 
	TextArray_({}), PlayerTransArray_({}), ButtonImageArray_({}), ModeTransArray_({}),
	hSoundGameMode_(-1), hSoundSelect_(-1), hSoundDecide_(-1), hSoundCancel_(-1),
	SelectMode_(S_Battle),GameModeState_(S_Selecting),PlayerNum_(S_PvE),
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

	hPlayerNumSelect_ = Image::Load(path + "selectplayer.png");
	assert(hPlayerNumSelect_ >= 0);

	hPlayerSelectIcon_ = Image::Load(path + "SelectIcon.png");
	assert(hPlayerSelectIcon_ >= 0);

	hSoundGameMode_ = Audio::Load("Sound\\BGM\\gameMode.wav",true);
	assert(hSoundGameMode_ >= 0);

	hSoundDecide_ = Audio::Load("Sound\\SE\\decide.wav", false, Audio::GetDecideNum());
	assert(hSoundDecide_ >= 0);

	hSoundSelect_ = Audio::Load("Sound\\SE\\select.wav", false, Audio::GetSelectNum());
	assert(hSoundSelect_ >= 0);

	hSoundCancel_ = Audio::Load("Sound\\SE\\cancel.wav", false, Audio::GetCancelNum());
	assert(hSoundCancel_ >= 0);

	//�e���[�h�̉摜�n���h����z��ɓ����
	ButtonImageArray_ = { hBattle_, hPractice_, hHowtoPlay_, hBackTitle_ };

	//�e�e�L�X�g�摜�n���h����z��ɓ����
	TextArray_ = {hBattleText_, hFreePlayText_, hHowtoPlayText_, hTitleText_};
	
	//�e���[�h�����X�g�ɓ����
	ModeList_ = { S_Battle,S_Practice,S_HowToPlay,S_Title };

	//Modeitr�̏����ʒu���w��
	Modeitr = ModeList_.begin();

	//PlayerNum�̏�ԃ��X�g��������
	PlayerNumList_ = { S_PvE,S_PvP };

	//PlayerNumList_�̃C���f�b�N�X�̏����ʒu���w��
	PlayerNumitr = PlayerNumList_.begin();

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
	//�w�i�`��
	Image::SetAndDraw(hBackScreen_, this->transform_);

	//�w�i�̃L�����N�^�[�̉e�`��
	Image::SetAndDraw(hBackChara_, BackChara_);

	//���[�h�Z���N�g�̃e�L�X�g�摜�`��
	Image::SetAndDraw(hModeSelect_, TransSelect_);

	//�e���[�h�̃{�^���摜�`��
	for(int i = 0; i < ModeTransArray_.size(); i++)
	{
		Image::SetAndDraw(ButtonImageArray_[i], ModeTransArray_[i]);
	}

	//���[�h�I��g�̕`��
	Image::SetAndDraw(hFrameLine_, TransFrame_);

	//�I�𒆂̃e�L�X�g�̕`��(���ݑI�����Ă��郂�[�h�ɂ���ĕω�)
	switch (SelectMode_)
	{
	case GameModeScene::S_Battle:
	case GameModeScene::S_Practice:
	case GameModeScene::S_HowToPlay:
	case GameModeScene::S_Title:
	{
		Image::SetAndDraw(TextArray_[SelectMode_], TransText_);
	}
		break;
	default:
		break;
	}

	//�m�F��ʂł͂����Ԑl����I������摜�`��
	if (GameModeState_ == GameModeScene::S_Confirmation)
	{
		switch (SelectMode_)
		{
		case GameModeScene::S_Battle:
		case GameModeScene::S_Practice:
		{
			Image::SetAndDraw(hPlayerNumSelect_, TransPlayer_);
			Image::SetAndDraw(hPlayerSelectIcon_, TransSelectPlayerNum_);
			//�I�����Ă����Ԃɍ��킹�ăA�C�R������
		}
			break;
		//case GameModeScene::S_HowToPlay:
		//	break;
		//case GameModeScene::S_Title:
		//	break;
		default:
			break;
		}
	}

#ifdef _DEBUG
	//�e�摜�̃g�����X�t�H�[���̈ʒu�ύX
	if (ImGui::TreeNode("GameModeSelect"))
	{
		ImGui::SliderFloat("BackCharaX", &BackChara_.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("BackCharaY", &BackChara_.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("TransSelectX", &TransSelect_.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("TransSelectY", &TransSelect_.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("TransTextX", &TransText_.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("TransTextY", &TransText_.position_.y, Image::UpEdge, Image::DownEdge);

		for (int i = 0; i < ModeTransArray_.size(); i++)
		{
			ImGui::SliderFloat((ModeStringArray[i] + "X").c_str(), &ModeTransArray_[i].position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat((ModeStringArray[i] + "Y").c_str(), &ModeTransArray_[i].position_.y, Image::UpEdge, Image::DownEdge);
		}

		ImGui::TreePop();
	}
#endif
}

void GameModeScene::Release()
{
}

void GameModeScene::SetGameModeSCV()
{
	//�e�摜��\������ۂ̃g�����X�t�H�[��������
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\GameModeData.csv");

	//�I��g�̃g�����X�t�H�[��������
	InitCSVTransform(csv, "FrameLine", TransFrame_);

	//�e�{�^���̃g�����X�t�H�[��������
    for (int i = 0; i < MaxMode; i++)
    {
		InitCSVTransform(csv, ModeStringArray[i], ModeTransArray_[i]);
    }

	//"���[�h�Z���N�g"�̃g�����X�t�H�[��������
	InitCSVTransform(csv, "ModeSelect", TransSelect_);

	//��ʉ����̃e�L�X�g�̃g�����X�t�H�[��������
	InitCSVTransform(csv, "Text", TransText_);

	//�v���C�l���I����ʂ̃g�����X�t�H�[��������
	InitCSVTransform(csv, "PlayerNum", TransPlayer_);

	//PvE(��l�v���C),PvP(��l�v���C)
	std::string PlayerStringArray[] = { "PvE","PvP" };

	//�v���C�l���I����ʂ̃A�C�R���ʒu��������
	for (int i = 0; i < MaxPlayerNum; i++)
	{
		InitCSVTransform(csv, PlayerStringArray[i], PlayerTransArray_[i]);
	}
}

void GameModeScene::UpdateSelecting()
{
	//�{�^���̑I��g�̈ړ�
	//�C���f�b�N�X���擪/�����̏ꍇ�A����/�擪�֖߂�
	//�O�u�f�N�������g�Ŕz��I�[�o�[�h��

	if (Input::IsKeyDown(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::IsStickTiltLY_UP())
	{

		if (Modeitr == ModeList_.begin())
		{
			Modeitr = --ModeList_.end();
		}
		else
		{
			--Modeitr;
		}
		SelectMode_ = *Modeitr;

		//�I��SE�Đ�
		Audio::Play(hSoundSelect_);
	}
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::IsStickTiltLY_DOWN())
	{
		if (Modeitr == --ModeList_.end())
		{
			Modeitr = ModeList_.begin();
		}
		else
		{
			++Modeitr;
		}
		SelectMode_ = *Modeitr;

		//�I��SE�Đ�
		Audio::Play(hSoundSelect_);
	}

	//�I��g�̈ʒu���A�I�𒆂̃��[�h�̈ʒu�ɍ��킹��
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

	//����{�^��(P�L�[�EB/Start�{�^��)����������m�F��ʂ�
	//�����т����V�[���I���Ȃ�V�[���J�ڏ�Ԃ�
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B)
		|| Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//���艹���Đ�
		Audio::Play(hSoundDecide_);


		if (SelectMode_ == S_HowToPlay || SelectMode_ == S_Title)
		{
			//UpdateTransition�֑J��
			SceneState_ = S_Transition;

			//�V�[���J�ڃG�t�F�N�g(�Y�[���C��)��ݒ�
			pTransitionEffect_->ZoomInStart();
			pTransitionEffect_->SetTransitionTime(SceneShortTransition);
		}
		else
		{
			//�m�F��ʂ֑J��
			GameModeState_ = S_Confirmation;
		}
	}
}

void GameModeScene::UpdateConfirmation()
{
	//�{�^���̑I��g�̈ړ�
	//�C���f�b�N�X���擪/�����̏ꍇ�A����/�擪�֖߂�
	//�O�u�f�N�������g�Ŕz��I�[�o�[�h��

	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::IsStickTiltLX_RIGHT())
	{
		if (PlayerNumitr == PlayerNumList_.begin())
		{
			PlayerNumitr = --PlayerNumList_.end();
		}
		else
		{
			--PlayerNumitr;
		}
		PlayerNum_ = *PlayerNumitr;

		//�I��SE�Đ�
		Audio::Play(hSoundSelect_);
	}
	if (Input::IsKeyDown(DIK_LEFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::IsStickTiltLX_LEFT())
	{
		if (PlayerNumitr == --PlayerNumList_.end())
		{
			PlayerNumitr = PlayerNumList_.begin();
		}
		else
		{
			++PlayerNumitr;
		}
		PlayerNum_ = *PlayerNumitr;

		//�I��SE�Đ�
		Audio::Play(hSoundSelect_);
	}

	//�v���C�l���I���A�C�R���̈ʒu�𒲐�
	switch (PlayerNum_)
	{
	case GameModeScene::S_PvE:
		TransSelectPlayerNum_.position_.x = PlayerTransArray_[S_PvE].position_.x;
		break;
	case GameModeScene::S_PvP:
		TransSelectPlayerNum_.position_.x = PlayerTransArray_[S_PvP].position_.x;
		break;
	default:
		break;
	}


	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//���艹���Đ�
		Audio::Play(hSoundDecide_);

		//UpdateTransition�֑J��
		SceneState_ = S_Transition;

		//�V�[���J�ڃG�t�F�N�g(�Y�[���C��)��ݒ�
		pTransitionEffect_->ZoomInStart();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

		switch (PlayerNum_)
		{
		case GameModeScene::S_PvE:
		{
			pSceneManager->SetPvEMode();
		}
			break;
		case GameModeScene::S_PvP:
		{
			pSceneManager->SetPvPMode();	
		}
			break;
		default:
			break;
		}

	}

	if (Input::IsKeyUp(DIK_ESCAPE) || Input::IsPadButtonUp(XINPUT_GAMEPAD_A))
	{
		//�L�����Z�����Đ�
		Audio::Play(hSoundCancel_);

		//�I����ʂ֑J��
		GameModeState_ = S_Selecting;
	}
}

void GameModeScene::UpdateActive()
{
	switch (GameModeState_)
	{
	case GameModeScene::S_Selecting:
		UpdateSelecting();
		break;
	case GameModeScene::S_Confirmation:
		UpdateConfirmation();
		break;
	default:
		break;
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

		//�V�[���J�ڗp�^�C�}�[�����Z�b�g
		SceneTransitionTimer_ = 0;

		//���[�h�I��p�T�E���h��~
		Audio::Stop(hSoundGameMode_);

		//�Q�[���V�[����Ԃ�ʏ�ɖ߂��Ă���
		SceneState_ = S_Active;
	}
}

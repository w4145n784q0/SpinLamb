#include "GameModeScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"
#include"../Engine/SceneManager.h"
#include"../Engine/CsvReader.h"
#include"../ViewSourceFile/GameView.h"

namespace
{
	//�v���C�{�^��,�t���[�v���C�{�^��,�V�ѕ��{�^��,�^�C�g���{�^��
	//�N���X���Ŏg���܂킷������Ȃ̂�namespace�ɐ錾
	std::string ModeStringArray[] = { "Battle", "Practice", "HowToPlay", "BackTitle" };
}

GameModeScene::GameModeScene(GameObject* parent)
	:BaseScene(parent, "GameModeScene"), 
	//----------��ԑJ��----------
	SelectMode_(S_Battle), GameModeState_(S_Selecting), PlayerNum_(S_PvE),
	
	//----------�w�i----------
	hBackScreen_(-1), hBackChara_(-1),

	//----------�{�^��----------
	hBattle_(-1),hPractice_(-1), hHowtoPlay_(-1),hBackTitle_(-1), hFrameLine_(-1),
	ModeTransArray_({}), ButtonImageArray_({}), 

	//----------���S�E�e�L�X�g----------
	hModeSelect_(-1), hBattleText_(-1), hFreePlayText_(-1), hHowtoPlayText_(-1),hTitleText_(-1),
	hPlayerNumSelect_(-1), hPlayerSelectIcon_(-1), 
	TextArray_({}), PlayerTransArray_({}), 

	//----------�T�E���h�E�C���X�^���X----------
	hSoundGameMode_(-1), hSoundSelect_(-1), hSoundDecide_(-1), hSoundCancel_(-1),
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
	SetGameModeCSV();

	//�e�摜�E�T�E���h�̓ǂݍ���

	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string GameMode = "Image\\GameMode\\";
	std::string Sound = "Sound\\";
	std::string BGM = "BGM\\";
	std::string SE = "SE\\";

	hBackScreen_ = Image::Load(GameMode + "GameModeScreen.jpg");
	assert(hBackScreen_ >= 0);

	hBackChara_ = Image::Load(GameMode + "SheepImage.png");
	assert(hBackChara_ >= 0);

	hBattle_ = Image::Load(GameMode + "BattleButton.png");
	assert(hBattle_ >= 0);

	hPractice_ = Image::Load(GameMode + "FreeplayButton.png");
	assert(hPractice_ >= 0);

	hHowtoPlay_ = Image::Load(GameMode + "HowToPlayButton.png");
	assert(hHowtoPlay_ >= 0);

	hBackTitle_ = Image::Load(GameMode + "TitleButton.png");
	assert(hBackTitle_ >= 0);

	hFrameLine_ = Image::Load(GameMode + "LineFrame.png");
	assert(hFrameLine_ >= 0);

	hModeSelect_ = Image::Load(GameMode + "ModeSelect.png");
	assert(hModeSelect_>= 0);

	hBattleText_ = Image::Load(GameMode + "BattleText.png");
	assert(hBattleText_ >= 0);

	hFreePlayText_ = Image::Load(GameMode + "FreeplayText.png");
	assert(hFreePlayText_ >= 0);

	hHowtoPlayText_ = Image::Load(GameMode + "HowToPlayText.png");
	assert(hHowtoPlayText_ >= 0);

	hTitleText_ = Image::Load(GameMode + "TitleText.png");
	assert(hTitleText_ >= 0);

	hPlayerNumSelect_ = Image::Load(GameMode + "SelectPlayer.png");
	assert(hPlayerNumSelect_ >= 0);

	hPlayerSelectIcon_ = Image::Load(GameMode + "SelectIcon.png");
	assert(hPlayerSelectIcon_ >= 0);

	hSoundGameMode_ = Audio::Load(Sound + BGM + "GameMode.wav",true);
	assert(hSoundGameMode_ >= 0);

	hSoundDecide_ = Audio::Load(Sound + SE + "Decide.wav", false, Audio::GetDecideNum());
	assert(hSoundDecide_ >= 0);

	hSoundSelect_ = Audio::Load(Sound + SE + "Select.wav", false, Audio::GetSelectNum());
	assert(hSoundSelect_ >= 0);

	hSoundCancel_ = Audio::Load(Sound + SE + "Cancel.wav", false, Audio::GetCancelNum());
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
	assert(pTransitionEffect_ != nullptr);

	//GameView�Ƀ|�C���^��n��
	GameView::SetTransitionEffect(pTransitionEffect_);

	//���[�h�I��p�T�E���h�Đ�
	Audio::Play(hSoundGameMode_);
}

void GameModeScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTransition�͌p����̊֐����Ă΂��
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
			Image::SetAndDraw(hPlayerSelectIcon_, TransSelectPlayerIcon_);
			//�I�����Ă����Ԃɍ��킹�ăA�C�R������
		}
			break;
		case GameModeScene::S_HowToPlay:
		case GameModeScene::S_Title:
		default:
			break;
		}
	}

#ifdef _DEBUG
	//�e�摜�̃g�����X�t�H�[���̈ʒu�ύX
	if (ImGui::TreeNode("GameModeScene"))
	{
		//�w�i�̃L�����N�^�[
		if (ImGui::TreeNode("BackChara"))
		{
			if (ImGui::TreeNode("BackCharaPosition"))
			{
				ImGui::SliderFloat("BackCharaPositionX", &BackChara_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("BackCharaPositionY", &BackChara_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("BackCharaRotate"))
			{
				ImGui::InputFloat("BackCharaRotateX", &BackChara_.rotate_.x, ZeroPointOne);
				ImGui::InputFloat("BackCharaRotateY", &BackChara_.rotate_.y, ZeroPointOne);
				ImGui::InputFloat("BackCharaRotateZ", &BackChara_.rotate_.z, ZeroPointOne);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("BackCharaScale"))
			{
				ImGui::InputFloat("BackCharaScaleX", &BackChara_.scale_.x, ZeroPointOne);
				ImGui::InputFloat("BackCharaScaleY", &BackChara_.scale_.y, ZeroPointOne);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//"���[�h�Z���N�g"
		if (ImGui::TreeNode("TransSelect"))
		{
			if (ImGui::TreeNode("TransSelectPosition"))
			{
				ImGui::SliderFloat("TransSelectPositionX", &TransSelect_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("TransSelectPositionY", &TransSelect_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransSelectRotate"))
			{
				ImGui::InputFloat("TransSelectRotateX", &TransSelect_.rotate_.x, ZeroPointOne);
				ImGui::InputFloat("TransSelectRotateY", &TransSelect_.rotate_.y, ZeroPointOne);
				ImGui::InputFloat("TransSelectRotateZ", &TransSelect_.rotate_.z, ZeroPointOne);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransSelectScale"))
			{
				ImGui::InputFloat("TransSelectScaleX", &TransSelect_.scale_.x, ZeroPointOne);
				ImGui::InputFloat("TransSelectScaleY", &TransSelect_.scale_.y, ZeroPointOne);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//��ʉ����̐����e�L�X�g
		if (ImGui::TreeNode("TransText"))
		{
			if (ImGui::TreeNode("TransTextPosition"))
			{
				ImGui::SliderFloat("TransTextPositionX", &TransText_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("TransTextPositionY", &TransText_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransTextRotate"))
			{
				ImGui::InputFloat("TransTextRotateX", &TransText_.rotate_.x, ZeroPointOne);
				ImGui::InputFloat("TransTextRotateY", &TransText_.rotate_.y, ZeroPointOne);
				ImGui::InputFloat("TransTextRotateZ", &TransText_.rotate_.z, ZeroPointOne);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransTextScale"))
			{
				ImGui::InputFloat("TransTextScaleX", &TransText_.scale_.x, ZeroPointOne);
				ImGui::InputFloat("TransTextScaleY", &TransText_.scale_.y, ZeroPointOne);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//�e���[�h�̃{�^��
		if (ImGui::TreeNode("ModeTransArray"))
		{
			if (ImGui::TreeNode("ModeTransArrayPosition"))
			{
				for (int i = 0; i < ModeTransArray_.size(); i++)
				{
					ImGui::SliderFloat((ModeStringArray[i] + "X").c_str(), &ModeTransArray_[i].position_.x, Image::LeftEdge, Image::RightEdge);
					ImGui::SliderFloat((ModeStringArray[i] + "Y").c_str(), &ModeTransArray_[i].position_.y, Image::UpEdge, Image::DownEdge);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("ModeTransArrayRotate"))
			{
				for (int i = 0; i < ModeTransArray_.size(); i++)
				{
					ImGui::InputFloat((ModeStringArray[i] + "X").c_str(), &ModeTransArray_[i].rotate_.x, ZeroPointOne);
					ImGui::InputFloat((ModeStringArray[i] + "Y").c_str(), &ModeTransArray_[i].rotate_.y, ZeroPointOne);
					ImGui::InputFloat((ModeStringArray[i] + "Z").c_str(), &ModeTransArray_[i].rotate_.z, ZeroPointOne);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("ModeTransArrayScale"))
			{
				for (int i = 0; i < ModeTransArray_.size(); i++)
				{
					ImGui::InputFloat((ModeStringArray[i] + "X").c_str(), &ModeTransArray_[i].scale_.x, ZeroPointOne);
					ImGui::InputFloat((ModeStringArray[i] + "Y").c_str(), &ModeTransArray_[i].scale_.y, ZeroPointOne);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	
	//�V�Ԑl���m�F���
	if (ImGui::TreeNode("Confirmation"))
	{
		if (ImGui::TreeNode("TransPlayer"))
		{
			if (ImGui::TreeNode("TransPlayerPosition"))
			{
				ImGui::SliderFloat("TransPlayerPositionX", &TransPlayer_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("TransPlayerPositionY", &TransPlayer_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransPlayerRotate"))
			{
				ImGui::InputFloat("TransPlayerRotateX", &TransPlayer_.rotate_.x, ZeroPointOne);
				ImGui::InputFloat("TransPlayerRotateY", &TransPlayer_.rotate_.y, ZeroPointOne);
				ImGui::InputFloat("TransPlayerRotateZ", &TransPlayer_.rotate_.z, ZeroPointOne);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransPlayerScale"))
			{
				ImGui::InputFloat("TransPlayerScaleX", &TransPlayer_.scale_.x, ZeroPointOne);
				ImGui::InputFloat("TransPlayerScaleY", &TransPlayer_.scale_.y, ZeroPointOne);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//�v���C�l���I��I�𒆂̃A�C�R��
		if (ImGui::TreeNode("TransSelectPlayerIcon"))
		{
			if (ImGui::TreeNode("TransSelectPlayerPosition"))
			{
				ImGui::SliderFloat("TransSelectPlayerPositionX", &TransSelectPlayerIcon_.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("TransSelectPlayerPositionY", &TransSelectPlayerIcon_.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransSelectPlayerRotate"))
			{
				ImGui::InputFloat("TransSelectPlayerRotateX", &TransSelectPlayerIcon_.rotate_.x, ZeroPointOne);
				ImGui::InputFloat("TransSelectPlayerRotateY", &TransSelectPlayerIcon_.rotate_.y, ZeroPointOne);
				ImGui::InputFloat("TransSelectPlayerRotateZ", &TransSelectPlayerIcon_.rotate_.z, ZeroPointOne);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("TransSelectPlayerScale"))
			{
				ImGui::InputFloat("TransSelectPlayerScaleX", &TransSelectPlayerIcon_.scale_.x, ZeroPointOne);
				ImGui::InputFloat("TransSelectPlayerScaleY", &TransSelectPlayerIcon_.scale_.y, ZeroPointOne);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}

void GameModeScene::Release()
{
	//���̂͏������A�A�h���X�̂ݖ���������
	if (pTransitionEffect_ != nullptr)
	{
		pTransitionEffect_ = nullptr;
	}
}

void GameModeScene::SetGameModeCSV()
{
	//�e�摜��\������ۂ̃g�����X�t�H�[��������
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\GameModeTransformData.csv");

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
		TransSelectPlayerIcon_.position_.x = PlayerTransArray_[S_PvE].position_.x;
		break;
	case GameModeScene::S_PvP:
		TransSelectPlayerIcon_.position_.x = PlayerTransArray_[S_PvP].position_.x;
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

		//SceneManager�̃|�C���^���擾��
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		assert(pSceneManager != nullptr);

		//SceneManager�ɗV�Ԑl����n��
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
		assert(pSceneManager != nullptr);

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

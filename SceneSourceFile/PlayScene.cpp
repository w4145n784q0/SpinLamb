#include "PlayScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"

namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X(�v���C�V�[���p�̕ϐ�)
	enum EasingIndex
	{
		i_IconPosYContinue = 0,
		i_IconPosYExit,
	};
}

PlayScene::PlayScene(GameObject* parent)
	:BaseScene(parent, "PlayScene"), hBackScreen_(-1), PauseSelect_(S_Continue),
	TmpIconPosY_(0.0f), IconPosYArray({}),
	hSoundPause_(-1),hSoundSelect_(-1),hSoundExit_(-1)
{
}

PlayScene::PlayScene(GameObject* parent, const std::string& name)
	:BaseScene(parent, name), hBackScreen_(-1), PauseSelect_(S_Continue),
	TmpIconPosY_(0.0f), IconPosYArray({}),
	hSoundPause_(-1), hSoundSelect_(-1), hSoundExit_(-1)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	//csv����p�����[�^�ǂݍ���
	SetPlaySceneCSV();

	//�e�摜�E�T�E���h�̓ǂݍ���
	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string Play = "Image\\Play\\";
	std::string Sound = "Sound\\";
	std::string SE = "SE\\";

	hBackScreen_ = Image::Load(Play + "BackSky.jpg");
	assert(hBackScreen_ >= 0);

	hSoundPause_ = Audio::Load(Sound + SE + "Pause.wav", false, Audio::GetSelectNum());
	assert(hSoundPause_ >= 0);

	hSoundSelect_ = Audio::Load(Sound + SE + "Select.wav", false, Audio::GetSelectNum());
	assert(hSoundSelect_ >= 0);

	hSoundExit_ = Audio::Load(Sound + SE + "Cancel.wav", false, Audio::GetCancelNum());
	assert(hSoundExit_ >= 0);

	//�e���[�h�����X�g�ɓ����
	PauseSelectList_ = { S_Continue,S_Exit };

	//Pauseitr�̏����ʒu���w��
	Pauseitr = PauseSelectList_.begin();

}

void PlayScene::SetPlaySceneCSV()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\PlayData.csv");

	//csv�t�@�C��(PlayData.csv)��0��ڂ̕�������擾
	std::string  PlayName= "Play";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> PlayData = GetCSVReadData(csv, PlayName);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	IconPosYArray = { PlayData[i_IconPosYContinue], PlayData[i_IconPosYExit] };
}

void PlayScene::DrawBackScreen()
{
	//�w�i�`��
	Image::SetAndDraw(hBackScreen_, TransBackScreen_);
}

void PlayScene::UpdatePauseMenu()
{
	//�|�[�Y��ʒ��̑�����s��
	if (Input::IsKeyDown(DIK_UP) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)
		|| Input::IsStickTiltLY_UP())
	{
		if (Pauseitr == PauseSelectList_.begin())
		{
			Pauseitr = --PauseSelectList_.end();
		}
		else
		{
			--Pauseitr;
		}
		PauseSelect_ = *Pauseitr;

		//�I��SE�Đ�
		Audio::Play(hSoundSelect_);

	}
	if (Input::IsKeyDown(DIK_DOWN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)
		|| Input::IsStickTiltLY_DOWN())
	{
		if (Pauseitr == --PauseSelectList_.end())
		{
			Pauseitr = PauseSelectList_.begin();
		}
		else
		{
			++Pauseitr;
		}
		PauseSelect_ = *Pauseitr;

		//�I��SE�Đ�
		Audio::Play(hSoundSelect_);
	}

	//�I���A�C�R���̈ʒu�𒲐�
	switch (PauseSelect_)
	{
	case PlayScene::S_Continue:
		TmpIconPosY_ = IconPosYArray[S_Continue];
		break;
	case PlayScene::S_Exit:
		TmpIconPosY_ = IconPosYArray[S_Exit];
		break;
	default:
		break;
	}

	//TmpIconPosY_�̈ʒu��HUD�ɓn��(�����͔h����ŋL�q�A�����ł̓|�C���^�͎����Ȃ�)
	SetPauseIconY();

	//����{�^��(P�L�[�EB�{�^��)����������
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		
		if (PauseSelect_ == S_Continue)
		{
			//"�Q�[�����Â���"��I�𒆂Ȃ瑦���ɖ߂�
			SceneState_ = S_Active;

			//�|�[�Y���SE�Đ�
			Audio::Play(hSoundPause_);

			//�v���C��ʂɍs���ۂ̏���(���z�֐��Ȃ̂Ŕh����ŏ�����ǉ�����)
			GotoPlay();
		}
		if (PauseSelect_ == S_Exit)
		{
			//"�Q�[������߂�"��I�𒆂Ȃ�J�ڏ�Ԃ�
			SceneState_ = S_Transition;

			//�^�C�g���ɖ߂�SE�Đ�
			Audio::Play(hSoundExit_);

			//�^�C�g���ɖ߂�ۂ̏���(���z�֐��Ȃ̂Ŕh����ŏ�����ǉ�����)
			GotoTitle();
		}
	}

	//S_Active�ւ̑J�ڂ�҂�(�L�[�{�[�h���{�^������ɂ���đJ��)
	WaitGotoPlay();
}

void PlayScene::WaitGotoPause()
{
	//esc�L�[��start�{�^���Ń|�[�Y��ʂ�
	if (Input::IsKeyUp(DIK_ESCAPE) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//�V�[�����|�[�Y��Ԃɂ���(�����͋���)
		SceneState_ = S_InActive;

		//�|�[�Y���SE�Đ�
		Audio::Play(hSoundPause_);

		//�|�[�Y��ʂɍs���ۂ̏���(���z�֐��Ȃ̂Ŕh����ŏ�����ǉ�����)
		GotoPause();
	}
}

void PlayScene::WaitGotoPlay()
{
	//ESC�L�[�EStart�{�^������������Active�ɖ߂�
	if (Input::IsKeyUp(DIK_ESCAPE) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		//�V�[����ʏ��Ԃɂ���(�����͋���)
		SceneState_ = S_Active;

		//�|�[�Y���SE�Đ�
		Audio::Play(hSoundPause_);

		//�v���C��ʂɍs���ۂ̏���(���z�֐��Ȃ̂Ŕh����ŏ�����ǉ�����)
		GotoPlay();
	}
}



#include "PlayScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"

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
	:BaseScene(parent, "PlayScene"), hBackScreen_(-1), PauseSelect_(S_Continue), IconPosYArray({})
{
}

PlayScene::PlayScene(GameObject* parent, const std::string& name)
	:BaseScene(parent, name), hBackScreen_(-1), PauseSelect_(S_Continue), IconPosYArray({})
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	//�e�摜�E�T�E���h�̓ǂݍ���
	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string Play = "Image\\Play\\";

	hBackScreen_ = Image::Load("Image\\Battle\\BackSky.jpg");
	assert(hBackScreen_ >= 0);

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

	}

	//�I���A�C�R���̈ʒu�𒲐�
	switch (PauseSelect_)
	{
	case PlayScene::S_Continue:
		TmpIconPos_.y = IconPosYArray[S_Continue];
		break;
	case PlayScene::S_Exit:
		TmpIconPos_.y = IconPosYArray[S_Exit];
		break;
	default:
		break;
	}

	//����{�^��(P�L�[�EB/Start�{�^��)����������
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		
		if (PauseSelect_ == S_Continue)
		{
			//"�Q�[�����Â���"��I�𒆂Ȃ瑦���ɖ߂�
			SceneState_ = S_Active;
		}
		if (PauseSelect_ == S_Exit)
		{
			//"�Q�[������߂�"��I�𒆂Ȃ�J�ڏ�Ԃ�
			SceneState_ = S_Transition;

			//�p����ɂ���ĈقȂ�J�ڃG�t�F�N�g���Ă�
			SetTransitionEffect();
		}
	}
}



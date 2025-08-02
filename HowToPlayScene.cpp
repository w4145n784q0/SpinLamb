#include "HowToPlayScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"Engine/CsvReader.h"

HowToPlayScene::HowToPlayScene(GameObject* parent)
	:BaseScene(parent, "HowToPlayScene"),
	hExplanation_(-1), hOperateKeyboard_(-1), hOperateController_(-1),
	hSoundHowtoPlay_(-1), hSoundSelect_(-1),hSoundBackGameMode_(-1),
	ImageState_(Explanation),pTransitionEffect_(nullptr)
{
}

HowToPlayScene::~HowToPlayScene()
{
}

void HowToPlayScene::Initialize()
{
	//�e�N���X����
	Instantiate<TransitionEffect>(this);

	//�e�摜�E�T�E���h�̓ǂݍ���

	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string path = "Image\\HowToPlay\\";

	hExplanation_ = Image::Load(path + "spinlamb_explanation.jpg");
	assert(hExplanation_ >= 0);

	hOperateKeyboard_ = Image::Load(path + "Howto_keyboard.jpg");
	assert(hOperateKeyboard_ >= 0);

	hOperateController_ = Image::Load(path + "Howto_controller.jpg");
	assert(hOperateController_ >= 0);

	hSoundHowtoPlay_ = Audio::Load("Sound\\BGM\\howtoPlay.wav", true);
	assert(hSoundHowtoPlay_ >= 0);

	hSoundSelect_ = Audio::Load("Sound\\SE\\select.wav", false, Audio::GetSelectNum());
	assert(hSoundSelect_ >= 0);

	hSoundBackGameMode_ = Audio::Load("Sound\\SE\\cancel.wav", false, Audio::GetCancelNum());
	assert(hSoundBackGameMode_ >= 0);

	//���X�g�Ɋe��Ԃ�ǉ�
	ImageList_ = { Explanation ,OperateKeyBoard,OperateController};

	////�C���f�b�N�X�̏����ʒu���w��
	itr = ImageList_.begin();

	//�C���X�^���X����
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

	//�����т����p�T�E���h�Đ�
	Audio::Play(hSoundHowtoPlay_);
}

void HowToPlayScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTransition�͌p����̊֐����Ă΂��
	BaseScene::Update();
}

void HowToPlayScene::Draw()
{
	switch (ImageState_)
	{
	case HowToPlayScene::Explanation:
		//�Q�[�������`��
		Image::SetAndDraw(hExplanation_, this->transform_);
		break;
	case HowToPlayScene::OperateKeyBoard:
		//�L�[�{�[�h��������`��
		Image::SetAndDraw(hOperateKeyboard_, this->transform_);
		break;
	case HowToPlayScene::OperateController:
		//�R���g���[���[��������`��
		Image::SetAndDraw(hOperateController_, this->transform_);
		break;
	default:
		break;
	}
}

void HowToPlayScene::Release()
{
}

void HowToPlayScene::UpdateActive()
{
	//�\���摜�̈ړ�
	//�C���f�b�N�X���擪/�����̏ꍇ�A����/�擪�֖߂�
	//�O�u�f�N�������g�Ŕz��I�[�o�[�h��
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::IsStickTiltLX_RIGHT())
	{
		if (itr == ImageList_.begin())
		{
			itr = --ImageList_.end();
		}
		else
		{
			--itr;
		}
		ImageState_ = *itr;

		//�I��SE�Đ�
		Audio::Play(hSoundSelect_);
	}
	if (Input::IsKeyDown(DIK_LEFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::IsStickTiltLX_LEFT())
	{
		if (itr == --ImageList_.end())
		{
			itr = ImageList_.begin();
		}
		else
		{
			++itr;
		}
		ImageState_ = *itr;

		//�I��SE�Đ�
		Audio::Play(hSoundSelect_);
	}

	//����{�^��(A�L�[�EA/Start�{�^��)����������V�[���J�ڏ�Ԃ�
	if (Input::IsKeyUp(DIK_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;

		//�V�[���J�ڃG�t�F�N�g(�t�F�[�h�A�E�g)��ݒ�
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);

		//����(�Q�[�����[�h�V�[���ɖ߂�)SE�Đ�
		Audio::Play(hSoundBackGameMode_);
	}
}

void HowToPlayScene::UpdateTransition()
{
	//���Ԍo�߂Ŏ��̃V�[���ɑJ��
	//�J�E���g���̓V�[���J�ڃG�t�F�N�g�s��

	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManager�̃C���X�^���X����^�C�g���V�[����
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);

		//�V�[���J�ڗp�^�C�}�[�����Z�b�g
		SceneTransitionTimer_ = 0;

		//�����т����p�T�E���h��~
		Audio::Stop(hSoundHowtoPlay_);

		//�Q�[���V�[����Ԃ�ʏ�ɖ߂��Ă���
		SceneState_ = S_Active;
	}
}

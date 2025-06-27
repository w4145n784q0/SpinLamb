#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

TitleScene::TitleScene(GameObject* parent)
	:BaseScene(parent,"TitleScene"), hBackScreen_(-1),hSoundTitle_(-1),hSoundStart_(-1),
	pTransitionEffect_(nullptr)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	//�e�N���X����
	Instantiate<TransitionEffect>(this);

	//�e�摜�E�T�E���h�̓ǂݍ���
	hBackScreen_ = Image::Load("Image\\Title\\TitleScreen.jpg");
	assert(hBackScreen_ >= 0);
	hSoundTitle_ = Audio::Load("Sound\\BGM\\title.wav",true); 
	assert(hSoundTitle_ >= 0);
	hSoundStart_ = Audio::Load("Sound\\SE\\start.wav",false);
	assert(hSoundStart_ >= 0);

	//�^�C�g���p�T�E���h�Đ�
	Audio::Play(hSoundTitle_);

	//�C���X�^���X����
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
}

void TitleScene::Update()
{
	//BaseScene�̍X�V�������Ă�
	//UpdateActive,UpdateTranslation�͌p����̊֐����Ă΂��
	BaseScene::Update();
}

void TitleScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);
}

void TitleScene::Release()
{
}

void TitleScene::UpdateActive()
{
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;
		pTransitionEffect_->FadeOutStartBlack();
		//pTransitionEffect_->ZoomInStart();
		pTransitionEffect_->SetTransitionTime(SceneTransition);
		Audio::Play(hSoundStart_);
	}
}

void TitleScene::UpdateTransition()
{
	if (++SceneTransitionTimer_ > SceneTransition)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);
		SceneTransitionTimer_ = 0;
		Audio::Stop(hSoundTitle_);
		SceneState_ = S_Active;
		pTransitionEffect_->ResetTransitionAlpha();
	}
}

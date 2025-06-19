#include "HowToPlayScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"Engine/CsvReader.h"

HowToPlayScene::HowToPlayScene(GameObject* parent)
	:BaseScene(parent, "GameModeScene"),
	hExplanation_(-1), hOperateKeyboard_(-1), hOperateController_(-1),
	hSoundHowtoPlay_(-1), ImageState_(Explanation)
{
}

HowToPlayScene::~HowToPlayScene()
{
}

void HowToPlayScene::Initialize()
{
	std::string path = "Image\\HowToPlay\\";

	hExplanation_ = Image::Load(path + "spinlamb_explanation.jpg");
	assert(hExplanation_ >= 0);

	hOperateKeyboard_ = Image::Load(path + "Howto_keyboard.jpg");
	assert(hOperateKeyboard_ >= 0);

	hOperateController_ = Image::Load(path + "Howto_controller.jpg");
	assert(hOperateController_ >= 0);

	hSoundHowtoPlay_ = Audio::Load("Sound\\BGM\\HowToPlay.wav", true);
	assert(hSoundHowtoPlay_ >= 0);

	ImageList_ = { Explanation ,OperateKeyBoard,OperateController};
	itr = ImageList_.begin();
}

void HowToPlayScene::Update()
{
	BaseScene::Update();
	Audio::Play(hSoundHowtoPlay_);
}

void HowToPlayScene::Draw()
{
	switch (ImageState_)
	{
	case HowToPlayScene::Explanation:
	{
		Image::SetTransform(hExplanation_, transform_);
		Image::Draw(hExplanation_);
	}
		break;
	case HowToPlayScene::OperateKeyBoard:
	{
		Image::SetTransform(hOperateKeyboard_, transform_);
		Image::Draw(hOperateKeyboard_);
	}
		break;
	case HowToPlayScene::OperateController:
	{
		Image::SetTransform(hOperateController_, transform_);
		Image::Draw(hOperateController_);
	}
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
	if (Input::IsKeyDown(DIK_RIGHT) || Input::GetPadStickL().y >= Input::StickTilt
		|| Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT))
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
	}
	if (Input::IsKeyDown(DIK_LEFT) || Input::GetPadStickL().y <= -Input::StickTilt
		|| Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT))
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
	}

	if (Input::IsKeyUp(DIK_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;
	}
}

void HowToPlayScene::UpdateTransition()
{
	if (++SceneTransitionTimer_ > SceneTransition)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);
		SceneTransitionTimer_ = 0;
		Audio::Stop(hSoundHowtoPlay_);
		SceneState_ = S_Active;
	}
}

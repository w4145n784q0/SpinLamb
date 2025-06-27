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
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT))
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
	if (Input::IsKeyDown(DIK_LEFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT))
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

	//決定ボタン(Aキー・A/Startボタン)を押したらシーン遷移状態へ
	if (Input::IsKeyUp(DIK_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;
	}
}

void HowToPlayScene::UpdateTransition()
{
	//時間経過で次のシーンに遷移
	//カウント中はシーン遷移エフェクト行う

	if (++SceneTransitionTimer_ > SceneTransition)
	{
		//SceneManagerのインスタンスからタイトルシーンへ
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);

		//シーン遷移用タイマーを戻す
		SceneTransitionTimer_ = 0;

		//あそびかた用サウンド停止
		Audio::Stop(hSoundHowtoPlay_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
	}
}

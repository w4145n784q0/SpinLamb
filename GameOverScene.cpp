#include "GameOverScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	Transform LogoResult;
	Transform LogoTitle;
}

GameOverScene::GameOverScene(GameObject* parent)
	:SceneData(parent,"GameOverScene"), hBackScreen_(-1), hlogoResult_(-1), hlogoTitle_(-1), hSoundGameOver_(-1)
{
}

void GameOverScene::Initialize()
{
	SetCSVScene();

	hBackScreen_ = Image::Load("Image\\GameMode\\back_mode.jpg");
	assert(hBackScreen_ >= 0);

	hlogoResult_ = Image::Load("Image\\Result\\YouLose.png");
	assert(hlogoResult_ >= 0);

	hlogoTitle_ = Image::Load("Image\\Result\\PushToTitle.png");
	assert(hlogoTitle_ >= 0);

	hSoundGameOver_ = Audio::Load("Sound\\BGM\\end.wav",true);
	assert(hSoundGameOver_ >= 0);

	LogoResult.position_ = ResultPosition_;
	LogoTitle.position_ = UnderPosition_;
}

void GameOverScene::Update()
{
	Audio::Play(hSoundGameOver_);
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

	Image::SetTransform(hlogoResult_, LogoResult);
	Image::Draw(hlogoResult_);

	Image::SetTransform(hlogoTitle_, LogoTitle);
	Image::Draw(hlogoTitle_);

#ifdef _DEBUG
	ImGui::SliderFloat("x", &LogoTitle.position_.x, -1.0, 1.0);
	ImGui::SliderFloat("y", &LogoTitle.position_.y, -1.0, 1.0);
#endif
}

void GameOverScene::Release()
{
}

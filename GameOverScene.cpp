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
	Transform LogoTitle;
	XMFLOAT3 LogoTitlePosition = {0,-0.8,0};
}

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene"), hImage_(-1), hlogoResult_(-1), hlogoTitle_(-1), hGameOverSound_(-1)
{
}

void GameOverScene::Initialize()
{
	hImage_ = Image::Load("Image\\back_mode2.jpg");
	assert(hImage_ >= 0);

	hlogoResult_ = Image::Load("Image\\YouLose.png");
	assert(hlogoResult_ >= 0);

	hlogoTitle_ = Image::Load("Image\\PushToTitle.png");
	assert(hlogoTitle_ >= 0);

	hGameOverSound_ = Audio::Load("Sound\\maou_game_jingle10.wav");
	assert(hGameOverSound_ >= 0);

	LogoTitle.position_ = LogoTitlePosition;
}

void GameOverScene::Update()
{
	Audio::Play(hGameOverSound_);
	if (Input::IsKeyDown(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);

	Image::SetTransform(hlogoResult_, transform_);
	Image::Draw(hlogoResult_);

	Image::SetTransform(hlogoTitle_, LogoTitle);
	Image::Draw(hlogoTitle_);

	ImGui::SliderFloat("x", &LogoTitle.position_.x, -1.0, 1.0);
	ImGui::SliderFloat("y", &LogoTitle.position_.y, -1.0, 1.0);
}

void GameOverScene::Release()
{
}

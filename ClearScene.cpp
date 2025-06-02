#include "ClearScene.h"
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

ClearScene::ClearScene(GameObject* parent)
	:SceneData(parent,"ClearScene"), hBackScreen_(-1), hlogoResult_(-1),hlogoTitle_(-1), hSoundClear_(-1)
{
}

void ClearScene::Initialize()
{
	SetCSVScene();

	hBackScreen_ = Image::Load("Image\\GameMode\\back_mode.jpg");
	assert(hBackScreen_ >= 0);

	hlogoResult_ = Image::Load("Image\\Result\\YouWin.png");
	assert(hlogoResult_ >= 0);

	hlogoTitle_ = Image::Load("Image\\Result\\PushToTitle.png");
	assert(hlogoTitle_ >= 0);

	hSoundClear_ = Audio::Load("Sound\\BGM\\end.wav",true);
	assert(hSoundClear_ >= 0);

}

void ClearScene::Update()
{
	Audio::Play(hSoundClear_);
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void ClearScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

	Image::SetTransform(hlogoResult_, Result);
	Image::Draw(hlogoResult_);

	Image::SetTransform(hlogoTitle_, PushTitle);
	Image::Draw(hlogoTitle_);

#ifdef _DEBUG
	ImGui::SliderFloat("x", &Result.position_.x, -1.0, 1.0);
	ImGui::SliderFloat("y", &PushTitle.position_.y, -1.0, 1.0);
#endif

}

void ClearScene::Release()
{
}

#include "PracticeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"
#include"Engine/CsvReader.h"

#include"Player.h"
#include"Enemy.h"
#include"StageManager.h"

namespace
{
	//アイリスアウト表現用のトランスフォーム
	Transform IrisIn;
}

PracticeScene::PracticeScene(GameObject* parent)
	:BaseScene(parent,"PracticeScene"), hBackScreen_(-1),hBackIrisIn_(-1),hSoundPractice_(-1),
	pPlayer_(nullptr),pEnemy_(nullptr), pHUD_(nullptr),pMiniMap_(nullptr), Press_(0)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	SetSCVTitle();

	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

	hBackIrisIn_ = Image::Load("Image\\Battle\\IrisIn.png");
	assert(hBackIrisIn_ >= 0);

	hSoundPractice_ = Audio::Load("Sound\\BGM\\practice.wav",true);
	assert(hSoundPractice_ >= 0);

	Instantiate<StageManager>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy>(this);
	Instantiate<MiniMap>(this);
	Instantiate<HUD>(this);

	StageManager* pS = (StageManager*)FindObject("StageManager");
	float north = pS->GetNorthEnd();
	float south = pS->GetSouthEnd();
	float west = pS->GetWestEnd();
	float east = pS->GetEastEnd();

	pPlayer_ = (Player*)FindObject("Player");
	if(pPlayer_ != nullptr)
	{
		pPlayer_->PlayerStart();
		pPlayer_->SetEnd(north, south, west, east);
	}

	pEnemy_ = (Enemy*)FindObject("Enemy");
	if(pEnemy_ != nullptr)
	{
		pEnemy_->EnemyStop();
		pEnemy_->SetEnd(north, south, west, east);
	}

	//インスタンスを初期化し、HUDに渡す
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	pHUD_ = (HUD*)FindObject("HUD");
	pHUD_->SetMiniMapPointer(pMiniMap_);
}

void PracticeScene::Update()
{
	BaseScene::Update();
	Audio::Play(hSoundPractice_);
}

void PracticeScene::Draw()
{
	ImGui::SliderFloat("irisin", &IrisIn.scale_.x, 1.0, 20.0);
	ImGui::SliderFloat("irisin", &IrisIn.scale_.y, 1.0, 20.0);

	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);
	pHUD_->SetDrawMode(S_Practice);

	Image::SetTransform(hBackIrisIn_, IrisIn);
	Image::Draw(hBackIrisIn_);
}

void PracticeScene::Release()
{
}

void PracticeScene::UpdateActive()
{
	if (Input::IsKey(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))//ボタン長押しでタイトルに戻る
	{
		Press_++;
	}
	else
	{
		Press_ = 0;
	}

	if (Press_ >= SceneTransition)//長押しでタイトルに戻る
	{
		SceneState_ = S_Transition;
	}
}

void PracticeScene::UpdateTransition()
{
	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
		SceneTransitionTimer_ = 0;
		Audio::Stop(hSoundPractice_);
		SceneState_ = S_Active;
	}
}

void PracticeScene::SetSCVTitle()
{
	CsvReader csv;
	csv.Load("CSVdata\\PracticeData.csv");

	std::string irisin = "IrisIn";
	if (csv.IsGetParamName(irisin))
	{
		std::vector<float> v = csv.GetParam(irisin);
		SetTransformPRS(IrisIn, v);
	}
}

#include "PracticeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"

#include"Player.h"
#include"Enemy.h"
#include"StageManager.h"

PracticeScene::PracticeScene(GameObject* parent)
	:BaseScene(parent,"PracticeScene"), hBackScreen_(-1),hSoundPractice_(-1), Press_(0),
	pHUD_(nullptr),pMiniMap_(nullptr)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	hBackScreen_ = Image::Load("Image\\Battle\\back_sky.jpg");
	assert(hBackScreen_ >= 0);

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

	//Player* pPlayer_ = (Player*)FindObject("Player");
	pPlayer_ = (Player*)FindObject("Player");
	if(pPlayer_ != nullptr)
	{
		pPlayer_->PlayerStart();
		pPlayer_->SetEnd(north, south, west, east);
	}

	//Enemy* pEnemy = (Enemy*)FindObject("Enemy");
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
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);
	pHUD_->SetDrawMode(S_Practice);
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

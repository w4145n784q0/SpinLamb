#include "PracticeScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"

#include"Player.h"
#include"Enemy.h"
#include"MiniMap.h"
#include"HUD.h"
#include"StageManager.h"

PracticeScene::PracticeScene(GameObject* parent)
	:SceneData(parent,"PracticeScene"), hBackScreen_(-1), Press_(0)
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

	Player* pPlayer_ = (Player*)FindObject("Player");
	pPlayer_->PlayerStart();
	pPlayer_->SetEnd(north, south, west, east);

	Enemy* pEnemy = (Enemy*)FindObject("Enemy");
	pEnemy->EnemyStart();
	pEnemy->SetEnd(north, south, west, east);

	HUD* pHUD = (HUD*)FindObject("HUD");
	pHUD->SetStatePractice();

	SetCSVScene();
}

void PracticeScene::Update()
{
	Audio::Play(hSoundPractice_);

	if (Input::IsKey(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))//�{�^���������Ń^�C�g���ɖ߂�
	{
		Press_++;
	}
	else
	{
		Press_ = 0;
	}

	if (Press_ >= SceneTransition)//�������Ń^�C�g���ɖ߂�
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void PracticeScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);
}

void PracticeScene::Release()
{
}

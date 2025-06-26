#include "GameTimer.h"
#include"Engine/Image.h"
#include<array>

namespace
{
	//���Ԍv���p�̐��� ���t���[�����Z
	int Timecounter = 0;
}

GameTimer::GameTimer(GameObject* parent)
	: GameObject(parent,"GameTimer"),
	CurrentGameTime_(0), Timeten_(0), Timeone_(0),TimeState_(STOP)
{
}

GameTimer::~GameTimer()
{
}

void GameTimer::Initialize()
{
	TimeCalclation();
}

void GameTimer::Update()
{
	switch (TimeState_)
	{
	case GameTimer::STOP:
		UpdateTimeStop();
		break;
	case GameTimer::COUNTING:
		UpdateTimeCount();
		break;
	default:
		break;
	}
}

void GameTimer::Draw()
{
}

void GameTimer::Release()
{
}

void GameTimer::UpdateTimeStop()
{
	TimeCalclation();
}

void GameTimer::UpdateTimeCount()
{
	//���Ԃ̍X�V����
	if (++Timecounter > oneSecond)
	{
		if (CurrentGameTime_ > 0)
		{
			CurrentGameTime_--;
		}
		TimeCalclation();
		Timecounter = 0;
	}
}

void GameTimer::TimeCalclation()
{
	//���݂̎��Ԃ����ꂼ��v�Z
	//Timeten,Timeone��HUD�N���X����擾����`�悳���

	Timeten_ = CurrentGameTime_ / TenDivision;
	Timeone_ = CurrentGameTime_ % TenDivision;
}

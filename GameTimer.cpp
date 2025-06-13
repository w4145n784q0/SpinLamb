#include "GameTimer.h"
#include"Engine/Image.h"
#include<array>

namespace
{
	//時間計測用の整数 毎フレーム加算
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
	Timeten_ = CurrentGameTime_ / TimeDivision;
	Timeone_ = CurrentGameTime_ % TimeDivision;
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
	//時間の更新処理

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
	Timeten_ = CurrentGameTime_ / TimeDivision;
	Timeone_ = CurrentGameTime_ % TimeDivision;
}

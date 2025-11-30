#include "GameTimer.h"
#include"../Engine/Global.h"

namespace
{
	//時間計測用の整数 毎フレーム加算
	int TimeCounter = 0;
}

GameTimer::GameTimer(GameObject* parent)
	: GameObject(parent,"GameTimer"),
	CurrentGameTime_(0), Timeten_(0), Timeone_(0),
	IsSecondCount_(false),EasingTime_(0),
	TimeState_(S_Stop)
{
}

GameTimer::~GameTimer()
{
}

void GameTimer::Initialize()
{
	TimeCalculation();
}

void GameTimer::Update()
{
	//現在の時間の状態によって更新を分ける
	switch (TimeState_)
	{
	case GameTimer::S_Stop:
		UpdateTimeStop();
		break;
	case GameTimer::S_Counting:
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
	//停止中も計算は行う
	TimeCalculation();
}

void GameTimer::UpdateTimeCount()
{
	//時間の更新処理
	if (++TimeCounter > ONE_SECOND)
	{
		if (CurrentGameTime_ > 0)
		{
			CurrentGameTime_--;
		}

		//一秒経過の度に計算
		TimeCalculation();

		//時間計測用のカウンターをリセット
		TimeCounter = 0;

		//このタイミングでtrue
		IsSecondCount_ = true;
	}
	else
	{
		//通常はfalse
		IsSecondCount_ = false;
	}
}

void GameTimer::TimeCalculation()
{
	//現在の時間をそれぞれ計算
	//十の位:現在の時間を10で除算
	//一の位:現在の時間を10で除算した余り
	//Timeten,TimeoneはHUDクラスから取得され描画される

	Timeten_ = DIVISION_TEN(CurrentGameTime_);
	Timeone_ = MODULO_TEN(CurrentGameTime_);
}
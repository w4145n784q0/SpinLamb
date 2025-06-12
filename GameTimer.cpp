#include "GameTimer.h"
#include"Engine/Image.h"
#include<array>

namespace
{
	////時間の数字表記のインデックス
	//enum NumberIndex
	//{
	//	i_Number0 = 0,
	//	i_Number1,
	//	i_Number2,
	//	i_Number3,
	//	i_Number4,
	//	i_Number5,
	//	i_Number6,
	//	i_Number7,
	//	i_Number8,
	//	i_Number9,
	//	MaxNumberIndex,
	//};

	////ナンバーハンドルの配列
	//std::array<int, MaxNumberIndex> ArrayHandle;

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
	/*Image::SetTransform(ArrayHandle[Timeten_], TenTime_);
	Image::Draw(ArrayHandle[Timeten_]);
	Image::SetTransform(ArrayHandle[Timeone_], OneTime_);
	Image::Draw(ArrayHandle[Timeone_]);*/
}

void GameTimer::Release()
{
}

void GameTimer::UpdateTimeStop()
{
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
		Timeten_ = CurrentGameTime_ / TimeDivision;
		Timeone_ = CurrentGameTime_ % TimeDivision;
		Timecounter = 0;
	}
}

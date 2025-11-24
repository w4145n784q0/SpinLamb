#include "CharacterHitStop.h"

CharacterHitStop::CharacterHitStop(GameObject* parent)
	:GameObject(parent, "CharacterHitStop"), params_(nullptr)
{
}

void CharacterHitStop::HitStopTimerAdd()
{
	if (params_ == nullptr)
	{
		return;
	}

	//ヒットストップを行うタイマーを増加
	params_->HitParam_.HitStopTimer_++;
}

void CharacterHitStop::HitStopTimerReset()
{
	if (params_ == nullptr)
	{
		return;
	}

	//ヒットストップを行うタイマーをリセット
	params_->HitParam_.HitStopTimer_ = 0;
}

bool CharacterHitStop::IsTimeOverHitStopTime()
{
	if (params_ == nullptr)
	{
		//params_がnullならfalseを返す
		return false;
	}

	//HitStopTimer_がヒットストップする時間を経過したか
	//(ヒットストップを終了するかどうか)返す
	if (params_->HitParam_.HitStopTimer_ > params_->HitParam_.HitStopValue_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

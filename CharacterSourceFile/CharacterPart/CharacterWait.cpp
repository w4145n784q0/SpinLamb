#include "CharacterWait.h"

CharacterWait::CharacterWait(GameObject* parent)
	:GameObject(parent, "CharacterWait"), params_(nullptr)
{
}

void CharacterWait::WaitTimerAdd()
{
	if (params_ == nullptr)
	{
		return;
	}

	//待機時間を計測するタイマーを増加
	params_->WaitParams_.WaitTimer_++;
}

void CharacterWait::WaitTimeReset()
{
	if (params_ == nullptr)
	{
		return;
	}

	//待機時間を計測するタイマーをリセット
	params_->WaitParams_.WaitTimer_ = 0;
}

bool CharacterWait::IsTimeOverWaitTime()
{
	if (params_ == nullptr)
	{
		//params_がnullならfalseを返す
		return false;
	}

	//WaitTimer_が待機時間を経過したか返す
	if (params_->WaitParams_.WaitTimer_ >= params_->WaitParams_.WaitValue_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#include "CharacterHitStop.h"

CharacterHitStop::CharacterHitStop(GameObject* parent)
	:GameObject(parent, "CharacterHitStop"), params_(nullptr)
{
}

void CharacterHitStop::HitStopTimerAdd(int _add)
{
	//ヒットストップを行うタイマーを増加(引数あり、かつ0より大きいならそれを加算)
	if (_add > 0)
	{
		params_->HitParam_.HitStopTimer_ += _add;
	}
	else
	{
		//引数なし(デフォルトは0)ならインクリメント
		params_->HitParam_.HitStopTimer_++;
	}
}

void CharacterHitStop::HitStopTimerReset()
{
	//ヒットストップを行うタイマーをリセット
	params_->HitParam_.HitStopTimer_ = 0;
}

bool CharacterHitStop::IsTimeOverHitStopTime()
{
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

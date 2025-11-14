#include "CharacterWait.h"

CharacterWait::CharacterWait(GameObject* parent)
	:GameObject(parent, "CharacterCharge"), params_(nullptr)
{
}

void CharacterWait::WaitTimerAdd()
{
	params_->WaitParams_.WaitTimer_++;
}

void CharacterWait::WaitTimeReset()
{
	params_->WaitParams_.WaitTimer_ = 0;
}

bool CharacterWait::IsTimeOverWaitTime()
{
	//WaitTimer_‚ª‘Ò‹@ŠÔ‚ğŒo‰ß‚µ‚½‚©•Ô‚·
	if (params_->WaitParams_.WaitTimer_ >= params_->WaitParams_.WaitValue_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

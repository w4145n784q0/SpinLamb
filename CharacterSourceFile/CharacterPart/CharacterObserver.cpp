#include "CharacterObserver.h"

CharacterObserver::CharacterObserver(GameObject* parent)
	:GameObject(parent, "CharacterObserver"), params_(nullptr)
{
}

void CharacterObserver::AddObserver(IGameObserver* _observer)
{
	//�Ď��҂�z��ɒǉ�����
	params_->InitParam_.observers.push_back(_observer);
}

void CharacterObserver::RemoveObserver(IGameObserver* _observer)
{
	auto it = params_->InitParam_.observers.begin();

	while (it != params_->InitParam_.observers.end())
	{
		if (*it == _observer)
		{
			//�Ď������Ώۂ̔z�񃋁[�v���ŁA�Ď��Ώۂ���������폜
			//�C�e���[�^�폜��͎��̗v�f���Q�Ƃ���
			it = params_->InitParam_.observers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

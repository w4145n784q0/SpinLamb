#include "CharacterObserver.h"

void CharacterObserver::AddObserver(IGameObserver* _observer)
{
	//�Ď��҂�z��ɒǉ�����
	InitParam_.observers.push_back(_observer);
}

void CharacterObserver::RemoveObserver(IGameObserver* _observer)
{
	auto it = InitParam_.observers.begin();

	while (it != InitParam_.observers.end())
	{
		if (*it == _observer)
		{
			//�Ď������Ώۂ̔z�񃋁[�v���ŁA�Ď��Ώۂ���������폜
			//�C�e���[�^�폜��͎��̗v�f���Q�Ƃ���
			it = InitParam_.observers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

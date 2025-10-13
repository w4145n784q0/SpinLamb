#include "CharacterObserver.h"

void CharacterObserver::AddObserver(IGameObserver* _observer)
{
	//監視者を配列に追加する
	InitParam_.observers.push_back(_observer);
}

void CharacterObserver::RemoveObserver(IGameObserver* _observer)
{
	auto it = InitParam_.observers.begin();

	while (it != InitParam_.observers.end())
	{
		if (*it == _observer)
		{
			//監視される対象の配列ループ内で、監視対象を見つけたら削除
			//イテレータ削除後は次の要素を参照する
			it = InitParam_.observers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

#pragma once
#include "../Engine/GameObject.h"
#include"BaseWire.h"

//����̓S���̓����蔻����s���N���X
class LowerWire :
    public BaseWire
{
public:
	LowerWire(GameObject* parent);
	~LowerWire();
};


#pragma once
#include "../Engine/GameObject.h"
#include"BaseWire.h"

//���̓S���̓����蔻����s���N���X
class LeftWire :
    public BaseWire
{
public:
	LeftWire(GameObject* parent);
	~LeftWire();
};


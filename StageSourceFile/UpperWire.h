#pragma once
#include "../Engine/GameObject.h"
#include"BaseWire.h"

//�O���̓S���̓����蔻����s���N���X
class UpperWire :
    public BaseWire
{
public:
	UpperWire(GameObject* parent);
	~UpperWire();
};


#pragma once
#include "../Engine/GameObject.h"
#include"Wire.h"

//�O���̓S���̓����蔻����s���N���X
class UpperWire :
    public Wire
{
public:
	UpperWire(GameObject* parent);
	~UpperWire();
};


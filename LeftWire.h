#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//���̓S���̓����蔻����s���N���X
class LeftWire :
    public Wire
{
public:
	LeftWire(GameObject* parent);
	~LeftWire();
};


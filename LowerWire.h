#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//����̓S���̓����蔻����s���N���X
class LowerWire :
    public Wire
{
public:
	LowerWire(GameObject* parent);
	~LowerWire();
};


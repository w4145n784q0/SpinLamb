#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//�E�̓S���̓����蔻����s���N���X
class RightWire :
    public Wire
{
public:
	RightWire(GameObject* parent);
	~RightWire();
};


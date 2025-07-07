#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//右の鉄線の当たり判定を行うクラス
class RightWire :
    public Wire
{
public:
	RightWire(GameObject* parent);
	~RightWire();
};


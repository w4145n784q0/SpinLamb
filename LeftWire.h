#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//左の鉄線の当たり判定を行うクラス
class LeftWire :
    public Wire
{
public:
	LeftWire(GameObject* parent);
	~LeftWire();
};


#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//後方の鉄線の当たり判定を行うクラス
class LowerWire :
    public Wire
{
public:
	LowerWire(GameObject* parent);
	~LowerWire();
};


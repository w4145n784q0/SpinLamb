#pragma once
#include "../Engine/GameObject.h"
#include"Wire.h"

//前方の鉄線の当たり判定を行うクラス
class UpperWire :
    public Wire
{
public:
	UpperWire(GameObject* parent);
	~UpperWire();
};


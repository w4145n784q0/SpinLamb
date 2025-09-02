#pragma once
#include "../Engine/GameObject.h"
#include"BaseWire.h"

//前方の鉄線の当たり判定を行うクラス
class UpperWire :
    public BaseWire
{
public:
	UpperWire(GameObject* parent);
	~UpperWire();
};


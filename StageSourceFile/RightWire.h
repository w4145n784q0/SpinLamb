#pragma once
#include "../Engine/GameObject.h"
#include"BaseWire.h"

//右の鉄線の当たり判定を行うクラス
class RightWire :
    public BaseWire
{
public:
	RightWire(GameObject* parent);
	~RightWire();
};


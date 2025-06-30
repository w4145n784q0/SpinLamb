#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//右の柵の当たり判定を行うクラス
class RightWire :
    public Wire
{
public:
	RightWire(GameObject* parent);
	~RightWire();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};


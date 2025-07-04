#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//後方の柵の当たり判定を行うクラス
class LowerWire :
    public Wire
{
public:
	LowerWire(GameObject* parent);
	~LowerWire();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};


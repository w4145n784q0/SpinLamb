#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//左の柵の当たり判定を行うクラス
class LeftWire :
    public Wire
{
public:
	LeftWire(GameObject* parent);
	~LeftWire();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};


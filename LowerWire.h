#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//����̍�̓����蔻����s���N���X
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


#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//���̍�̓����蔻����s���N���X
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


#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//�E�̍�̓����蔻����s���N���X
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


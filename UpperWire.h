#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//�O���̍�̓����蔻����s���N���X
class UpperWire :
    public Wire
{
public:
	UpperWire(GameObject* parent);
	~UpperWire();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};


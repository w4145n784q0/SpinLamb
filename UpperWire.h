#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//�O���̍�̓����蔻����s���N���X
class UpperWire :
    public Wire
{
//private:
	//XMVECTOR Normal_;//��̖@��
	//XMFLOAT3 CollisionPos_;//�����蔻��̈ʒu
	//XMFLOAT3 CollisionSize_;//�����蔻��̃T�C�Y
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

	//�����蔻�菈��
	//void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// �����蔻�菉����
	/// </summary>
	/// <param name="pos">�{�b�N�X�R���C�_�[�̈ʒu</param>
	/// <param name="size">�{�b�N�X�R���C�_�[�̃T�C�Y</param>
	//void InitCollision(XMFLOAT3 pos, XMFLOAT3 size);

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------

	//void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }
	//XMVECTOR GetNormal() { return Normal_; }
};


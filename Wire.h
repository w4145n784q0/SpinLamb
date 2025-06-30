#pragma once
#include "Engine/GameObject.h"

//��̓����蔻��E�@�����������N���X
//�O�㍶�E�̍�N���X�͂��̃N���X���p�����A�e���œ����蔻��Ɩ@��������
class Wire :
    public GameObject
{
protected:
	//��̖@��
	XMVECTOR Normal_;

public:
	Wire(GameObject* parent);
	Wire(GameObject* parent, const std::string& name);
	virtual ~Wire();

	//�����蔻�菈��
	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// �����蔻�菉����
	/// </summary>
	/// <param name="pos">�{�b�N�X�R���C�_�[�̈ʒu</param>
	/// <param name="size">�{�b�N�X�R���C�_�[�̃T�C�Y</param>
	void InitCollision(XMFLOAT3 pos, XMFLOAT3 size);

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------

	//�@�����Z�b�g
	void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }

	//�@�����擾
	XMVECTOR GetNormal() { return Normal_; }
};


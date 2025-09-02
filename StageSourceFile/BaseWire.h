#pragma once
#include "../Engine/GameObject.h"

//��̓����蔻��E�@�����������N���X
//�O�㍶�E�̓S���N���X�͂��̃N���X���p�����A�e���œ����蔻��Ɩ@��������
class BaseWire :
    public GameObject
{
protected:
	//��̖@��
	XMVECTOR Normal_;

public:
	BaseWire(GameObject* parent);
	BaseWire(GameObject* parent, const std::string& name);
	virtual ~BaseWire();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//���
	void Release() override;

	/// <summary>
	/// �����蔻�菉����
	/// </summary>
	/// <param name="pos">�{�b�N�X�R���C�_�[�̈ʒu</param>
	/// <param name="size">�{�b�N�X�R���C�_�[�̃T�C�Y</param>
	void InitCollision(XMFLOAT3 _pos, XMFLOAT3 _size);

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------

	//�@�����Z�b�g
	void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }

	//�@�����擾
	XMVECTOR GetNormal() const { return Normal_; }
};


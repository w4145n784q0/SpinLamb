#include "Wire.h"

Wire::Wire(GameObject* parent)
	:GameObject(parent, "Wire"), Normal_({ 0,0,0 })
{
}

Wire::Wire(GameObject* parent, const std::string& name)
	:GameObject(parent,name), Normal_({ 0,0,0 })
{
}

Wire::~Wire()
{
}

void Wire::Initialize()
{
}

void Wire::Update()
{
}

void Wire::Draw()
{
}

void Wire::Release()
{
}

void Wire::InitCollision(XMFLOAT3 _pos, XMFLOAT3 _size)
{
	// �{�b�N�X�R���C�_�[�̈ʒu,�T�C�Y��ݒ肵�A�����蔻��̒ǉ�
	//�p����̃|�C���^����Ăяo��
	BoxCollider* collision_wire = new BoxCollider(_pos, _size);
	AddCollider(collision_wire);
}

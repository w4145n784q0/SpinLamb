#include "BaseWire.h"

BaseWire::BaseWire(GameObject* parent)
	:GameObject(parent, "BaseWire"), Normal_({ 0,0,0 })
{
}

BaseWire::BaseWire(GameObject* parent, const std::string& name)
	:GameObject(parent,name), Normal_({ 0,0,0 })
{
}

BaseWire::~BaseWire()
{
}

void BaseWire::Initialize()
{
}

void BaseWire::Update()
{
}

void BaseWire::Draw()
{
}

void BaseWire::Release()
{
}

void BaseWire::InitCollision(XMFLOAT3 _pos, XMFLOAT3 _size)
{
	// �{�b�N�X�R���C�_�[�̈ʒu,�T�C�Y��ݒ肵�A�����蔻��̒ǉ�
	//�p����̃|�C���^����Ăяo��
	BoxCollider* collision_wire = new BoxCollider(_pos, _size);
	AddCollider(collision_wire);
}

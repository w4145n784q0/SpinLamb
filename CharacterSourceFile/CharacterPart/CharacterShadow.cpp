#include "CharacterShadow.h"
#include"../Character.h"
#include"../../Engine/Model.h"

CharacterShadow::CharacterShadow(GameObject* parent)
	:GameObject(parent, "CharacterShadow"), params_(nullptr), character_(nullptr)
{
}

void CharacterShadow::InitShadow()
{
	//�������̎��_�ŃX�e�[�W�N���X�̃C���X�^���X���擾
	params_->ShadowParam_.pGround_ = (Ground*)FindObject("Ground");
	assert(params_->ShadowParam_.pGround_ != nullptr);

	//�e���f���ǂݍ���
	params_->ShadowParam_.hShadow_ = Model::Load("Model\\ShadowPoint.fbx");
	assert(params_->ShadowParam_.hShadow_ >= 0);

}

void CharacterShadow::ShadowSet()
{
	//���g�̈ʒu���擾
	XMFLOAT3 tmp = character_->GetPosition();

	//�X�e�[�W�̃��f���ԍ����擾
	int hGroundModel = params_->ShadowParam_.pGround_->GetModelHandle();

	//�X�e�[�W�Ɍ������ă��C���΂��ݒ������
	RayCastData data;

	//���C�̔��ˈʒu��ݒ�
	data.start = character_->GetPosition();

	//���C�̕�����ݒ�(0, -1, 0�Ȃ̂ŉ�����)
	data.dir = XMFLOAT3(0, -1, 0);

	//���C�𔭎�
	Model::RayCast(hGroundModel, &data);

	//���C������������
	if (data.hit)
	{
		//���݈ʒu-�Փ˓_�܂ł̋��� + �␳�l�ŉe�̍�����ݒ�
		params_->ShadowParam_.ShadowHeight_ = 
			(tmp.y - data.dist) + params_->ShadowParam_.ShadowCorrection_;
	}

	//y���W�ȊO�̓L�����N�^�[�Ɠ����ʒu�ɐݒ�
	params_->ShadowParam_.ShadowTrans_.position_.x = tmp.x;
	params_->ShadowParam_.ShadowTrans_.position_.z = tmp.z;

	//�e�̍������g�����X�t�H�[���ɐݒ肷��
	params_->ShadowParam_.ShadowTrans_.position_.y = params_->ShadowParam_.ShadowHeight_;

}

void CharacterShadow::ShadowDraw()
{
	//ShadowSet�ňʒu��ݒ肵���e��`��
	Model::SetAndDraw(params_->ShadowParam_.hShadow_, params_->ShadowParam_.ShadowTrans_);
}
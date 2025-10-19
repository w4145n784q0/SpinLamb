#include "CharacterDebugPanel.h"
#include"../Character.h"

CharacterDebugPanel::CharacterDebugPanel(GameObject* parent)
	:GameObject(parent, "CharacterDebugPanel"), params_(nullptr), character_(nullptr)
{
}

void CharacterDebugPanel::DrawCharacterImGui()
{
	//ImGui�̕`��
	//Slider:�X���C�_�[���g���Ēl�𒲐��\
	//Input:�l�𒼐ړ��� +-�ɂ�钲�����\
	//Text:�L�q�̂�

	//�e�N���X��position
	XMFLOAT3 tmp = character_->GetPosition();

//�L�����N�^�[�̈ʒu(position_.x,y,z)
	if (ImGui::TreeNode("Transform.Position"))
	{
		ImGui::SliderFloat("PositionX", &tmp.x, 
			params_->EndParam_.WestEnd_, params_->EndParam_.EastEnd_);
		ImGui::SliderFloat("PositionY", &tmp.y,
			params_->JumpParam_.HeightLowerLimit_, params_->JumpParam_.HeightUpperLimit_);
		ImGui::SliderFloat("PositionZ", &tmp.z,
			params_->EndParam_.SouthEnd_, params_->EndParam_.NorthEnd_);
		ImGui::TreePop();
	}

	//�L�����N�^�[�̉�]��(rotate_.x,y,z)
	if (ImGui::TreeNode("Transform.Rotate"))
	{
		ImGui::InputFloat("RotateX", &tmp.x, ZeroPointOne);
		ImGui::InputFloat("RotateY", &tmp.y, ZeroPointOne);
		ImGui::InputFloat("RotateZ", &tmp.z, ZeroPointOne);
		ImGui::TreePop();
	}

	//���[���h���ʃx�N�g��
	if (ImGui::TreeNode("Forward"))
	{
		XMFLOAT3 tmp;
		XMStoreFloat3(&tmp, params_->MoveParam_.ForwardVector_);
		ImGui::Text("ForwardX:%.3f", tmp.x);
		ImGui::Text("ForwardY:%.3f", tmp.y);
		ImGui::Text("ForwardZ:%.3f", tmp.z);
		ImGui::TreePop();
	}

	//�L�����N�^�[ID,�����ʒu
	if (ImGui::TreeNode("Initialize"))
	{
		ImGui::Text("CharacterID:%.1d", params_->InitParam_.CharacterID);
		ImGui::Text("ForwardX:%.3f", params_->InitParam_.StartPosition_.x);
		ImGui::Text("ForwardY:%.3f", params_->InitParam_.StartPosition_.y);
		ImGui::Text("ForwardZ:%.3f", params_->InitParam_.StartPosition_.z);
		ImGui::TreePop();
	}

	//�����x,������,�`���[�W�̍ۂɎg�����̉����x,1f���Ƃ̑�����,�ő����,���C�W��,
	//�ړ������A�ړ���̍X�V�ʒu
	if (ImGui::TreeNode("Move"))
	{
		ImGui::InputFloat("Velocity", &params_->MoveParam_.Velocity_, ZeroPointOne);
		ImGui::InputFloat("Acceleration", &params_->MoveParam_.Acceleration_, ZeroPointOne);
		ImGui::InputFloat("TmpAccele", &params_->MoveParam_.TmpAccele_, ZeroPointOne);
		ImGui::InputFloat("AcceleValue", &params_->MoveParam_.AcceleValue_, ZeroPointOne);
		ImGui::InputFloat("FullAccelerate", &params_->MoveParam_.FullAccelerate_, ZeroPointOne);
		ImGui::InputFloat("Friction", &params_->MoveParam_.Friction_, ZeroPointOne);

		XMFLOAT3 move;
		XMStoreFloat3(&move, params_->MoveParam_.MoveDirection_);
		ImGui::Text("MoveDirectionX:%.3f", move.x);
		ImGui::Text("MoveDirectionY:%.3f", move.y);
		ImGui::Text("MoveDirectionZ:%.3f", move.z);

		XMFLOAT3 NewPos;
		XMStoreFloat3(&NewPos, params_->MoveParam_.NewPosition_);
		ImGui::Text("NewPositionX:%.3f", NewPos.x);
		ImGui::Text("NewPositionY:%.3f", NewPos.y);
		ImGui::Text("NewPositionZ:%.3f", NewPos.z);

		ImGui::TreePop();
	}

	//�L�����N�^�[�ړ����̒ʏ�̉�]�ʁA�`���[�W���̉�]��
	if (ImGui::TreeNode("Rotate"))
	{
		ImGui::InputFloat("NormalRotate", &params_->RotateParam_.MoveRotateX, ZeroPointOne);
		ImGui::InputFloat("FastRotate", &params_->RotateParam_.FastRotateX, ZeroPointOne);
		ImGui::TreePop();
	}

	//�L�����N�^�[�ɂ�����d��,�L�����N�^�[�̏�����̗́A�W�����v�̍���
	// �����̏��,����
	if (ImGui::TreeNode("Jump"))
	{
		ImGui::InputFloat("Gravity", &params_->JumpParam_.Gravity_, ZeroPointOne);
		ImGui::InputFloat("JumpSpeed", &params_->JumpParam_.JumpSpeed_, ZeroPointOne);
		ImGui::InputFloat("JumpHeight", &params_->JumpParam_.JumpHeight, ZeroPointOne);
		ImGui::InputFloat("HeightLowerLimit", &params_->JumpParam_.HeightLowerLimit_, ZeroPointOne);
		ImGui::InputFloat("HeightUpperLimit", &params_->JumpParam_.HeightUpperLimit_, ZeroPointOne);
		ImGui::TreePop();
	}

	//�m�b�N�o�b�N�ʌv�Z���̕ϊ��O�͈̔�,�ϊ���͈̔�,������,�m�b�N�o�b�N���I������l
	//�m�b�N�o�b�N��������A�m�b�N�o�b�N�̑��x
	if (ImGui::TreeNode("Hit"))
	{
		ImGui::InputFloat("OriginalRangeMin", &params_->HitParam_.OriginalRangeMin_, ZeroPointOne);
		ImGui::InputFloat("OriginalRangeMax", &params_->HitParam_.OriginalRangeMax_, ZeroPointOne);
		ImGui::InputFloat("ConvertedRangeMin", &params_->HitParam_.ConvertedRangeMin_, ZeroPointOne);
		ImGui::InputFloat("ConvertedRangeMax", &params_->HitParam_.ConvertedRangeMax_, ZeroPointOne);
		ImGui::InputFloat("DecelerationRate", &params_->HitParam_.DecelerationRate_, ZeroPointOne);
		ImGui::InputFloat("KnockBackEnd", &params_->HitParam_.KnockBackEnd_, ZeroPointOne);

		ImGui::Text("KnockBack_DirectionX:%.3f", params_->HitParam_.KnockBack_Direction_.x);
		ImGui::Text("KnockBack_DirectionY:%.3f", params_->HitParam_.KnockBack_Direction_.y);
		ImGui::Text("KnockBack_DirectionZ:%.3f", params_->HitParam_.KnockBack_Direction_.z);

		ImGui::Text("KnockBack_VelocityX:%.3f", params_->HitParam_.KnockBack_Velocity_.x);
		ImGui::Text("KnockBack_VelocityY:%.3f", params_->HitParam_.KnockBack_Velocity_.y);
		ImGui::Text("KnockBack_VelocityZ:%.3f", params_->HitParam_.KnockBack_Velocity_.z);

		std::string attackedName = params_->HitParam_.AttackedName_;
		ImGui::Text("AttackedName_:%s", attackedName.c_str());

		ImGui::TreePop();
	}

	//��q�b�g���̃m�b�N�o�b�N��,���G����,���G���Ԃ̏I������l
	// ���G���Ԓ��̕`��Ԋu,�`�撆�ɓ_�ŕ\����������l
	if (ImGui::TreeNode("FenceHit"))
	{
		ImGui::InputFloat("KnockBackPower", &params_->FenceHitParam_.KnockBackPower_, ZeroPointOne);
		ImGui::InputInt("InvincibilityTime", &params_->FenceHitParam_.InvincibilityTime_);
		ImGui::InputInt("InvincibilityValue", &params_->FenceHitParam_.InvincibilityValue_);
		ImGui::InputInt("blinkTimer", &params_->FenceHitParam_.BlinkTimer_);
		ImGui::InputInt("blinkValue", &params_->FenceHitParam_.BlinkValue_);

		ImGui::TreePop();
	}

	//�e�̈ʒu�̕␳
	if (ImGui::TreeNode("Shadow"))
	{
		ImGui::InputFloat("ShadowHeight", &params_->ShadowParam_.ShadowHeight_, ZeroPointOne);
		ImGui::InputFloat("ShadowCorrection", &params_->ShadowParam_.ShadowCorrection_, ZeroPointOne);
		ImGui::TreePop();
	}

	//��󃂃f���̈ʒu�E��]�E�g�嗦�A�ʒu�̕␳
	if (ImGui::TreeNode("Arrow"))
	{
		ImGui::InputFloat("ArrowTransform.PositionX", &params_->MoveParam_.ArrowTransform_.position_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.PositionY", &params_->MoveParam_.ArrowTransform_.position_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.PositionZ", &params_->MoveParam_.ArrowTransform_.position_.z, ZeroPointOne);

		ImGui::InputFloat("ArrowTransform.RotateX", &params_->MoveParam_.ArrowTransform_.rotate_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.RotateY", &params_->MoveParam_.ArrowTransform_.rotate_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.RotateZ", &params_->MoveParam_.ArrowTransform_.rotate_.z, ZeroPointOne);

		ImGui::InputFloat("ArrowTransform.ScaleX", &params_->MoveParam_.ArrowTransform_.scale_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.ScaleY", &params_->MoveParam_.ArrowTransform_.scale_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.ScaleZ", &params_->MoveParam_.ArrowTransform_.scale_.z, ZeroPointOne);

		ImGui::InputFloat("AddArrowDepth", &params_->MoveParam_.AddArrowDepth_, ZeroPointOne);

		ImGui::TreePop();
	}

	ImGui::TreePop();
}
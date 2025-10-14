#include "CharacterDebugPanel.h"

CharacterDebugPanel::CharacterDebugPanel(GameObject* parent)
	:GameObject(parent, "CharacterDebugPanel")
{
}

void CharacterDebugPanel::DrawCharacterImGui()
{
	//ImGui�̕`��
	//Slider:�X���C�_�[���g���Ēl�𒲐��\
	//Input:�l�𒼐ړ��� +-�ɂ�钲�����\
	//Text:�L�q�̂�

//�L�����N�^�[�̈ʒu(position_.x,y,z)
	if (ImGui::TreeNode("Transform.Position"))
	{
		ImGui::SliderFloat("PositionX", &this->transform_.position_.x, EndParam_.WestEnd_, EndParam_.EastEnd_);
		ImGui::SliderFloat("PositionY", &this->transform_.position_.y, JumpParam_.HeightLowerLimit_,
			JumpParam_.HeightUpperLimit_);
		ImGui::SliderFloat("PositionZ", &this->transform_.position_.z, EndParam_.SouthEnd_, EndParam_.NorthEnd_);
		ImGui::TreePop();
	}

	//�L�����N�^�[�̉�]��(rotate_.x,y,z)
	if (ImGui::TreeNode("Transform.Rotate"))
	{
		ImGui::InputFloat("RotateX", &this->transform_.rotate_.x, ZeroPointOne);
		ImGui::InputFloat("RotateY", &this->transform_.rotate_.y, ZeroPointOne);
		ImGui::InputFloat("RotateZ", &this->transform_.rotate_.z, ZeroPointOne);
		ImGui::TreePop();
	}

	//���[���h���ʃx�N�g��
	if (ImGui::TreeNode("Forward"))
	{
		XMFLOAT3 tmp;
		XMStoreFloat3(&tmp, MoveParam_.ForwardVector_);
		ImGui::Text("ForwardX:%.3f", tmp.x);
		ImGui::Text("ForwardY:%.3f", tmp.y);
		ImGui::Text("ForwardZ:%.3f", tmp.z);
		ImGui::TreePop();
	}

	//�L�����N�^�[ID,�����ʒu
	if (ImGui::TreeNode("Initialize"))
	{
		ImGui::Text("CharacterID:%.1d", InitParam_.CharacterID);
		ImGui::Text("ForwardX:%.3f", InitParam_.StartPosition_.x);
		ImGui::Text("ForwardY:%.3f", InitParam_.StartPosition_.y);
		ImGui::Text("ForwardZ:%.3f", InitParam_.StartPosition_.z);
		ImGui::TreePop();
	}

	//�����x,������,�`���[�W�̍ۂɎg�����̉����x,1f���Ƃ̑�����,�ő����,���C�W��,
	//�ړ������A�ړ���̍X�V�ʒu
	if (ImGui::TreeNode("Move"))
	{
		ImGui::InputFloat("Velocity", &MoveParam_.Velocity_, ZeroPointOne);
		ImGui::InputFloat("Acceleration", &MoveParam_.Acceleration_, ZeroPointOne);
		ImGui::InputFloat("TmpAccele", &MoveParam_.TmpAccele_, ZeroPointOne);
		ImGui::InputFloat("AcceleValue", &MoveParam_.AcceleValue_, ZeroPointOne);
		ImGui::InputFloat("FullAccelerate", &MoveParam_.FullAccelerate_, ZeroPointOne);
		ImGui::InputFloat("Friction", &MoveParam_.Friction_, ZeroPointOne);

		XMFLOAT3 move;
		XMStoreFloat3(&move, MoveParam_.MoveDirection_);
		ImGui::Text("MoveDirectionX:%.3f", move.x);
		ImGui::Text("MoveDirectionY:%.3f", move.y);
		ImGui::Text("MoveDirectionZ:%.3f", move.z);

		XMFLOAT3 NewPos;
		XMStoreFloat3(&NewPos, MoveParam_.NewPosition_);
		ImGui::Text("NewPositionX:%.3f", NewPos.x);
		ImGui::Text("NewPositionY:%.3f", NewPos.y);
		ImGui::Text("NewPositionZ:%.3f", NewPos.z);

		ImGui::TreePop();
	}

	//�L�����N�^�[�ړ����̒ʏ�̉�]�ʁA�`���[�W���̉�]��
	if (ImGui::TreeNode("Rotate"))
	{
		ImGui::InputFloat("NormalRotate", &RotateParam_.MoveRotateX, ZeroPointOne);
		ImGui::InputFloat("FastRotate", &RotateParam_.FastRotateX, ZeroPointOne);
		ImGui::TreePop();
	}

	//�L�����N�^�[�ɂ�����d��,�L�����N�^�[�̏�����̗́A�W�����v�̍���
	// �����̏��,����
	if (ImGui::TreeNode("Jump"))
	{
		ImGui::InputFloat("Gravity", &JumpParam_.Gravity_, ZeroPointOne);
		ImGui::InputFloat("JumpSpeed", &JumpParam_.JumpSpeed_, ZeroPointOne);
		ImGui::InputFloat("JumpHeight", &JumpParam_.JumpHeight, ZeroPointOne);
		ImGui::InputFloat("HeightLowerLimit", &JumpParam_.HeightLowerLimit_, ZeroPointOne);
		ImGui::InputFloat("HeightUpperLimit", &JumpParam_.HeightUpperLimit_, ZeroPointOne);
		ImGui::TreePop();
	}

	//�m�b�N�o�b�N�ʌv�Z���̕ϊ��O�͈̔�,�ϊ���͈̔�,������,�m�b�N�o�b�N���I������l
	//�m�b�N�o�b�N��������A�m�b�N�o�b�N�̑��x
	if (ImGui::TreeNode("Hit"))
	{
		ImGui::InputFloat("OriginalRangeMin", &HitParam_.OriginalRangeMin_, ZeroPointOne);
		ImGui::InputFloat("OriginalRangeMax", &HitParam_.OriginalRangeMax_, ZeroPointOne);
		ImGui::InputFloat("ConvertedRangeMin", &HitParam_.ConvertedRangeMin_, ZeroPointOne);
		ImGui::InputFloat("ConvertedRangeMax", &HitParam_.ConvertedRangeMax_, ZeroPointOne);
		ImGui::InputFloat("DecelerationRate", &HitParam_.DecelerationRate_, ZeroPointOne);
		ImGui::InputFloat("KnockBackEnd", &HitParam_.KnockBackEnd_, ZeroPointOne);

		ImGui::Text("KnockBack_DirectionX:%.3f", HitParam_.KnockBack_Direction_.x);
		ImGui::Text("KnockBack_DirectionY:%.3f", HitParam_.KnockBack_Direction_.y);
		ImGui::Text("KnockBack_DirectionZ:%.3f", HitParam_.KnockBack_Direction_.z);

		ImGui::Text("KnockBack_VelocityX:%.3f", HitParam_.KnockBack_Velocity_.x);
		ImGui::Text("KnockBack_VelocityY:%.3f", HitParam_.KnockBack_Velocity_.y);
		ImGui::Text("KnockBack_VelocityZ:%.3f", HitParam_.KnockBack_Velocity_.z);

		std::string attackedName = HitParam_.AttackedName_;
		ImGui::Text("AttackedName_:%s", attackedName.c_str());

		ImGui::TreePop();
	}

	//��q�b�g���̃m�b�N�o�b�N��,���G����,���G���Ԃ̏I������l
	// ���G���Ԓ��̕`��Ԋu,�`�撆�ɓ_�ŕ\����������l
	if (ImGui::TreeNode("FenceHit"))
	{
		ImGui::InputFloat("KnockBackPower", &FenceHitParam_.KnockBackPower_, ZeroPointOne);
		ImGui::InputInt("InvincibilityTime", &FenceHitParam_.InvincibilityTime_);
		ImGui::InputInt("InvincibilityValue", &FenceHitParam_.InvincibilityValue_);
		ImGui::InputInt("blinkTimer", &FenceHitParam_.BlinkTimer_);
		ImGui::InputInt("blinkValue", &FenceHitParam_.BlinkValue_);

		ImGui::TreePop();
	}

	//�e�̈ʒu�̕␳
	if (ImGui::TreeNode("Shadow"))
	{
		ImGui::InputFloat("ShadowHeight", &ShadowParam_.ShadowHeight_, ZeroPointOne);
		ImGui::InputFloat("ShadowCorrection", &ShadowParam_.ShadowCorrection_, ZeroPointOne);
		ImGui::TreePop();
	}

	//��󃂃f���̈ʒu�E��]�E�g�嗦�A�ʒu�̕␳
	if (ImGui::TreeNode("Arrow"))
	{
		ImGui::InputFloat("ArrowTransform.PositionX", &MoveParam_.ArrowTransform_.position_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.PositionY", &MoveParam_.ArrowTransform_.position_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.PositionZ", &MoveParam_.ArrowTransform_.position_.z, ZeroPointOne);

		ImGui::InputFloat("ArrowTransform.RotateX", &MoveParam_.ArrowTransform_.rotate_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.RotateY", &MoveParam_.ArrowTransform_.rotate_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.RotateZ", &MoveParam_.ArrowTransform_.rotate_.z, ZeroPointOne);

		ImGui::InputFloat("ArrowTransform.ScaleX", &MoveParam_.ArrowTransform_.scale_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.ScaleY", &MoveParam_.ArrowTransform_.scale_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.ScaleZ", &MoveParam_.ArrowTransform_.scale_.z, ZeroPointOne);

		ImGui::InputFloat("AddArrowDepth", &MoveParam_.AddArrowDepth_, ZeroPointOne);

		ImGui::TreePop();
	}

	ImGui::TreePop();
}
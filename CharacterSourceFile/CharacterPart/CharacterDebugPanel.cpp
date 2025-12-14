#include "CharacterDebugPanel.h"
#include"../Character.h"
#include"../../Engine/Global.h"

CharacterDebugPanel::CharacterDebugPanel(GameObject* parent)
	:GameObject(parent, "CharacterDebugPanel"), params_(nullptr), character_(nullptr)
{
}

void CharacterDebugPanel::DrawCharacterImGui()
{
	//ImGuiの描画
	//Slider:スライダーを使って値を調整可能
	//Input:値を直接入力 +-による調整も可能
	//Text:記述のみ

	if (params_ == nullptr)
	{
		return;
	}

	//キャラクターの位置(position_.x,y,z)
	if (ImGui::TreeNode("Transform.Position"))
	{
		if (character_ != nullptr)
		{
			//親クラスの位置
			XMFLOAT3 pos = character_->GetPosition();

			ImGui::SliderFloat("PositionX", &pos.x,
				params_->EndParam_.WestEnd_, params_->EndParam_.EastEnd_);
			ImGui::SliderFloat("PositionY", &pos.y,
				params_->JumpParam_.HeightLowerLimit_, params_->JumpParam_.HeightUpperLimit_);
			ImGui::SliderFloat("PositionZ", &pos.z,
				params_->EndParam_.SouthEnd_, params_->EndParam_.NorthEnd_);
			character_->SetPosition(pos);
		}
		ImGui::TreePop();
	}

	//キャラクターの回転量(rotate_.x,y,z)
	if (ImGui::TreeNode("Transform.Rotate"))
	{
		if (character_ != nullptr)
		{
			//親クラスの回転量
			XMFLOAT3 rot = character_->GetRotate();

			ImGui::SliderFloat("RotateX", &rot.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("RotateY", &rot.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("RotateZ", &rot.z, ANGLE_0_DEG, ANGLE_360_DEG);
			character_->SetRotate(rot);
		}
		ImGui::TreePop();
	}

	//キャラクターの拡大率(scale_.x,y,z)
	if (ImGui::TreeNode("Transform.Scale"))
	{
		if (character_ != nullptr)
		{
			//親クラスの拡大率
			XMFLOAT3 sca = character_->GetScale();

			ImGui::InputFloat("ScaleX", &sca.x, ZERO_POINT_ONE);
			ImGui::InputFloat("ScaleY", &sca.y, ZERO_POINT_ONE);
			ImGui::InputFloat("ScaleZ", &sca.z, ZERO_POINT_ONE);
			character_->SetScale(sca);
		}
		ImGui::TreePop();
	}

	//ワールド正面ベクトル
	if (ImGui::TreeNode("Forward"))
	{
		XMFLOAT3 tmp;
		XMStoreFloat3(&tmp, params_->MoveParam_.ForwardVector_);
		ImGui::Text("ForwardX:%.3f", tmp.x);
		ImGui::Text("ForwardY:%.3f", tmp.y);
		ImGui::Text("ForwardZ:%.3f", tmp.z);
		ImGui::TreePop();
	}

	//キャラクターID,初期位置
	if (ImGui::TreeNode("Initialize"))
	{
		ImGui::Text("CharacterID:%d", params_->InitParam_.CharacterID);
		ImGui::Text("InitializePos:%.3f", params_->InitParam_.StartPosition_.x);
		ImGui::Text("InitializePos:%.3f", params_->InitParam_.StartPosition_.y);
		ImGui::Text("InitializePos:%.3f", params_->InitParam_.StartPosition_.z);
		ImGui::TreePop();
	}

	//初速度,加速量,チャージの際に使う仮の加速度,1fごとの増加量,最大加速,摩擦係数,
	//移動方向、移動後の更新位置
	if (ImGui::TreeNode("Move"))
	{
		ImGui::InputFloat("CommonAcceleration", &params_->MoveParam_.CommonAcceleration_, ZERO_POINT_ONE);
		ImGui::InputFloat("NormalAcceleValue", &params_->MoveParam_.NormalAcceleValue_, ZERO_POINT_ONE);
		ImGui::InputFloat("NormalFullAccelerate", &params_->MoveParam_.NormalFullAccelerate_, ZERO_POINT_ONE);
		ImGui::InputFloat("NormalFriction", &params_->MoveParam_.NormalFriction_, ZERO_POINT_ONE);

		ImGui::InputFloat("TmpAccele", &params_->MoveParam_.TmpAccele_, ZERO_POINT_ONE);
		ImGui::InputFloat("AttackAcceleValue", &params_->MoveParam_.AttackAcceleValue_, ZERO_POINT_ONE);
		ImGui::InputFloat("AttackFullAccelerate", &params_->MoveParam_.AttackFullAccelerate_, ZERO_POINT_ONE);
		ImGui::InputFloat("AttackFriction", &params_->MoveParam_.AttackFriction_, ZERO_POINT_ONE);

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

	//キャラクター移動時の通常の回転量、チャージ中の回転量
	if (ImGui::TreeNode("Rotate"))
	{
		ImGui::InputFloat("NormalRotate", &params_->RotateParam_.MoveRotateX_, ZERO_POINT_ONE);
		ImGui::InputFloat("FastRotate", &params_->RotateParam_.FastRotateX_, ZERO_POINT_ONE);
		ImGui::TreePop();
	}

	//キャラクターにかかる重力,キャラクターの上方向の力、ジャンプの高さ
	// 高さの上限,下限
	if (ImGui::TreeNode("Jump"))
	{
		ImGui::InputFloat("Gravity", &params_->JumpParam_.Gravity_, ZERO_POINT_ONE);
		ImGui::InputFloat("JumpSpeed", &params_->JumpParam_.JumpSpeed_, ZERO_POINT_ONE);
		ImGui::InputFloat("JumpHeight", &params_->JumpParam_.JumpHeight, ZERO_POINT_ONE);
		ImGui::InputFloat("HeightLowerLimit", &params_->JumpParam_.HeightLowerLimit_, ZERO_POINT_ONE);
		ImGui::InputFloat("HeightUpperLimit", &params_->JumpParam_.HeightUpperLimit_, ZERO_POINT_ONE);
		ImGui::TreePop();
	}

	//ノックバック量計算時の変換前の範囲,変換後の範囲,減速率,ノックバックを終了する値
	//ノックバックする方向、ノックバックの速度
	if (ImGui::TreeNode("Hit"))
	{
		ImGui::InputInt("HitStopValue", &params_->HitParam_.HitStopValue_);
		ImGui::InputFloat("OriginalRangeMin", &params_->HitParam_.OriginalRangeMin_, ZERO_POINT_ONE);
		ImGui::InputFloat("OriginalRangeMax", &params_->HitParam_.OriginalRangeMax_, ZERO_POINT_ONE);
		ImGui::InputFloat("ConvertedRangeMin", &params_->HitParam_.ConvertedRangeMin_, ZERO_POINT_ONE);
		ImGui::InputFloat("ConvertedRangeMax", &params_->HitParam_.ConvertedRangeMax_, ZERO_POINT_ONE);
		ImGui::InputFloat("DecelerationRate", &params_->HitParam_.DecelerationRate_, ZERO_POINT_ONE);
		ImGui::InputFloat("KnockBackEnd", &params_->HitParam_.KnockBackEnd_, ZERO_POINT_ONE);

		ImGui::Text("KnockBack_DirectionX:%.3f", params_->HitParam_.KnockBack_Direction_.x);
		ImGui::Text("KnockBack_DirectionY:%.3f", params_->HitParam_.KnockBack_Direction_.y);
		ImGui::Text("KnockBack_DirectionZ:%.3f", params_->HitParam_.KnockBack_Direction_.z);

		ImGui::Text("KnockBack_VelocityX:%.3f", params_->HitParam_.KnockBack_Velocity_.x);
		ImGui::Text("KnockBack_VelocityY:%.3f", params_->HitParam_.KnockBack_Velocity_.y);
		ImGui::Text("KnockBack_VelocityZ:%.3f", params_->HitParam_.KnockBack_Velocity_.z);

		std::string attackedName = params_->HitParam_.AttackedName_;
		ImGui::Text("AttackedName_:%s", attackedName.c_str());

		std::string NextStateName = params_->HitParam_.NextStateName_;
		ImGui::Text("NextStateName_:%s", NextStateName.c_str());

		ImGui::TreePop();
	}

	//柵ヒット時のノックバック量,無敵時間,無敵時間の終了する値
	//無敵時間中の描画間隔,描画中に点滅表現をさせる値
	if (ImGui::TreeNode("FenceHit"))
	{
		ImGui::InputFloat("KnockBackPower", &params_->FenceHitParam_.KnockBackPower_, ZERO_POINT_ONE);
		ImGui::InputInt("InvincibilityTime", &params_->FenceHitParam_.InvincibilityTime_);
		ImGui::InputInt("InvincibilityValue", &params_->FenceHitParam_.InvincibilityValue_);
		ImGui::InputInt("blinkTimer", &params_->FenceHitParam_.BlinkTimer_);
		ImGui::InputInt("blinkValue", &params_->FenceHitParam_.BlinkValue_);

		ImGui::TreePop();
	}

	//待機時間の補正
	if (ImGui::TreeNode("Wait"))
	{
		ImGui::InputInt("WaitTimer", &params_->WaitParams_.WaitTimer_);
		ImGui::InputInt("WaitValue", &params_->WaitParams_.WaitValue_);
		ImGui::TreePop();
	}

	//影の位置の補正
	if (ImGui::TreeNode("Shadow"))
	{
		ImGui::InputFloat("ShadowHeight", &params_->ShadowParam_.ShadowHeight_, ZERO_POINT_ONE);
		ImGui::InputFloat("ShadowCorrection", &params_->ShadowParam_.ShadowCorrection_, ZERO_POINT_ONE);
		ImGui::TreePop();
	}

	//矢印モデルの位置・回転・拡大率、位置の補正
	if (ImGui::TreeNode("Arrow"))
	{
		ImGui::InputFloat("ArrowTransform.PositionX", &params_->MoveParam_.ArrowTransform_.position_.x, ZERO_POINT_ONE);
		ImGui::InputFloat("ArrowTransform.PositionY", &params_->MoveParam_.ArrowTransform_.position_.y, ZERO_POINT_ONE);
		ImGui::InputFloat("ArrowTransform.PositionZ", &params_->MoveParam_.ArrowTransform_.position_.z, ZERO_POINT_ONE);

		ImGui::SliderFloat("ArrowTransform.RotateX", &params_->MoveParam_.ArrowTransform_.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
		ImGui::SliderFloat("ArrowTransform.RotateY", &params_->MoveParam_.ArrowTransform_.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
		ImGui::SliderFloat("ArrowTransform.RotateZ", &params_->MoveParam_.ArrowTransform_.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);

		ImGui::InputFloat("ArrowTransform.ScaleX", &params_->MoveParam_.ArrowTransform_.scale_.x, ZERO_POINT_ONE);
		ImGui::InputFloat("ArrowTransform.ScaleY", &params_->MoveParam_.ArrowTransform_.scale_.y, ZERO_POINT_ONE);
		ImGui::InputFloat("ArrowTransform.ScaleZ", &params_->MoveParam_.ArrowTransform_.scale_.z, ZERO_POINT_ONE);

		ImGui::InputFloat("AddArrowDepth", &params_->MoveParam_.AddArrowDepth_, ZERO_POINT_ONE);

		ImGui::TreePop();
	}

	ImGui::TreePop();
}
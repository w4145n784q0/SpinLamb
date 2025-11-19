#include "CharacterCollision.h"
#include"../Character.h"

CharacterCollision::CharacterCollision(GameObject* parent)
	:GameObject(parent, "CharacterCollision"), params_(nullptr),character_(nullptr)
{
}

void CharacterCollision::CommonCollision(GameObject* target)
{
	//接触したオブジェクト名をとる
	std::string name = target->GetObjectName();

	//キャラクター同士の衝突
	if (IsHitCharacter(name))
	{
		if (IsInDamageState())
		{
			return;
		}

		//反射処理
		CharacterReflect(target);

		return; // キャラ衝突処理終了
	}

	//柵に衝突判定
	if (IsHitFence(name))
	{
		if (!character_->GetParams()->FenceHitParam_.IsInvincibility_ &&
			!character_->IsCharacterStateFenceHit()) {

			SomeFenceReflect(name);

		}
	}


}

bool CharacterCollision::IsHitCharacter(std::string _name)
{
	//接触相手の名前に"Player","Enemy"が含まれるかを返す
	if (_name.find("Player") != std::string::npos ||
		_name.find("Enemy") != std::string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CharacterCollision::IsHitFence(std::string _name)
{
	std::vector<std::string> Wires = {
		"UpperWire", "LowerWire", "RightWire", "LeftWire"
	};

	//文字列がWiresの中から見つかったかどうか返す
	//見つからなかったらWires.end()を返すので
	//文字列が見つかればWires.end()と一致しない = isFindはtrue
	bool isFind = std::find(Wires.begin(), Wires.end(), _name) != Wires.end();

	return isFind;
}

bool CharacterCollision::IsInDamageState()
{
	if (character_->IsCharacterStateHitStop() ||
		character_->IsCharacterStateHit() ||
		character_->IsCharacterStateFenceHit())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CharacterCollision::CharacterReflect(GameObject* target)
{
	//自身の位置・加速を先に保管する
	XMFLOAT3 mypos = character_->GetPosition();
	XMVECTOR MyVector = XMLoadFloat3(&mypos);
	float MySpeed = character_->GetParams()->MoveParam_.CommonAcceleration_;

	//接触相手のXMVECTORを用意
	XMFLOAT3 tarpos = character_->GetPosition();
	XMVECTOR TargetVector = XMLoadFloat3(&tarpos);

	//接触相手の加速量を保管する変数を用意
	float TargetSpeed = 0.0f;

	//接触した相手の名前を取得する文字列を用意
	std::string TargetName = target->GetObjectName();

	//PlayerかEnemyに接触したか(接触相手の名前を使用)
	if (IsHitCharacter(target->GetObjectName()))
	{
		auto* charTarget = dynamic_cast<Character*>(target);
		if (charTarget)
		{
			TargetSpeed = charTarget->GetParams()->GetAcceleration();
		}
	}

	character_->GetModuleHit()->Reflect(MyVector, TargetVector, MySpeed, TargetSpeed, TargetName);
}

void CharacterCollision::SomeFenceReflect(std::string wire)
{
	//接触している柵の法線(反射される方向)を取得
	XMVECTOR normal = character_->GetModuleHit()->HitNormal(wire);

	//反射開始
	character_->GetModuleFence()->FenceReflect(normal);

	//自身のノックバック時のY軸回転角を固定させる
	character_->GetModuleHit()->KnockBackAngleY(
		character_->GetParams()->HitParam_.KnockBack_Direction_,
		character_->GetParams()->FenceHitParam_.KnockBackPower_);

}

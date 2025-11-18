#include "CharacterCollision.h"

CharacterCollision::CharacterCollision(GameObject* parent)
	:GameObject(parent, "CharacterCollision"), params_(nullptr)
{
}

void CharacterCollision::HitCollision(GameObject* target)
{
	//接触したオブジェクト名をとる
	std::string name = target->GetObjectName();

	//キャラクター同士の衝突
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

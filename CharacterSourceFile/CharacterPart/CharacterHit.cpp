#include "CharacterHit.h"
#include "../Character.h"

CharacterHit::CharacterHit(GameObject* parent)
	:GameObject(parent, "CharacterHit"), params_(nullptr), character_(nullptr)
{
}

void CharacterHit::CommonCollision(GameObject* target)
{
	//接触したオブジェクト名をとる
	std::string name = target->GetObjectName();

	//キャラクター同士の衝突
	if (IsHitCharacter(name))
	{
		//ヒットストップ・被弾・柵に接触状態なら処理しない
		if (IsInDamageState())
		{
			return;
		}

		//キャラクターとの当たり判定
		CollisionCharacter(target);

		//継承先で定義する独自の当たり判定処理(キャラクター同士)
		character_->OwnCharacterCollision();

		//キャラ衝突処理終了
		return;
	}

	//柵に衝突判定
	if (IsHitFence(name))
	{
		//無敵時間中か柵に接触状態でなければ続ける
		if (!character_->GetParams()->FenceHitParam_.IsInvincibility_ &&
			!character_->IsCharacterStateFenceHit()) {

			//柵との当たり判定
			CollisionSomeFence(name);

			//継承先で定義する独自の当たり判定処理(キャラクター同士)
			character_->OwnFenceCollision();
		}
	}
}

bool CharacterHit::IsHitCharacter(std::string _name)
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

bool CharacterHit::IsHitFence(std::string _name)
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

bool CharacterHit::IsInDamageState()
{
	//親クラスからヒットストップ・被弾・柵に接触のいずれかに該当するか取得し返す
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

void CharacterHit::CollisionCharacter(GameObject* target)
{
	//自身の位置(XMVECTOR型)・加速量を用意する
	XMFLOAT3 mypos = character_->GetPosition();
	XMVECTOR MyVector = XMLoadFloat3(&mypos);
	float MySpeed = character_->GetParams()->MoveParam_.CommonAcceleration_;

	//接触相手の位置をXMVECTOR型で用意
	XMFLOAT3 tarpos = target->GetPosition();
	XMVECTOR TargetVector = XMLoadFloat3(&tarpos);

	//接触相手の加速量を保管する変数を用意
	float TargetSpeed = 0.0f;

	//接触した相手の名前を取得する文字列を用意
	std::string TargetName = target->GetObjectName();

	//PlayerかEnemyに接触したか(接触相手の名前を使用)
	//現在の呼び出し元でも行われているが、念のためチェック
	if (IsHitCharacter(target->GetObjectName()))
	{
		//targetがCharacter型か動的に判定
		auto* charTarget = dynamic_cast<Character*>(target);

		//正しくキャストできたなら速度を取得
		if (charTarget)
		{
			TargetSpeed = charTarget->GetParams()->GetAcceleration();
		}
	}

	//反射開始
	Reflect(MyVector, TargetVector, MySpeed, TargetSpeed, TargetName);
}

void CharacterHit::CollisionSomeFence(std::string wire)
{
	//接触している柵の法線(反射される方向)を取得
	XMVECTOR normal = HitNormal(wire);

	//反射開始
	character_->OnFenceReflect(normal);

	//自身のノックバック時のY軸回転角を固定させる
	KnockBackAngleY(character_->GetParams()->HitParam_.KnockBack_Direction_,
		character_->GetParams()->FenceHitParam_.KnockBackPower_);
}

void CharacterHit::Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity,
    std::string _attackName)
{
	//無敵状態なら処理しない
	if (params_->FenceHitParam_.IsInvincibility_)
	{
		return;
	}

	//接触相手のベクトルから自身のベクトルを引く
	XMVECTOR subVector = XMVectorSubtract(_targetVector, _myVector);

	//引いたベクトルが0ならお互いの位置が同じなので処理しない(0ベクトルを正規化はできない)
	if (XMVector3Equal(subVector, XMVectorZero()))
	{
		return;
	}

	//単位ベクトルに変換
	subVector = XMVector3Normalize(subVector);

	//逆ベクトルにして反射方向を決定
	subVector = XMVectorNegate(subVector);
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, subVector);

	//反射方向を設定
	params_->HitParam_.KnockBack_Direction_ = tmp;

	//自身の速度と相手の速度の差分をとる
	//速度の差分に応じてノックバック量を変化させる
	float subVelocity = _myVelocity - _targetVelocity;

	//ノックバック量の初期化
	//毎回値が変化するのでローカル変数
	float KnockBackValue = 0.0f;

	//速度差の判定は線形補完元の最大値を適用
	//速度差が自身の方が一定以上なら、自身のノックバック量は0
	if (subVelocity >= params_->HitParam_.OriginalRangeMax_)
	{
		KnockBackValue = 0.0f;
	}
	else
	{
		//値がマイナスならプラスに変更
		if (signbit(subVelocity))
		{
			subVelocity = -subVelocity;
		}

		//ノックバック量の線形補完を行う
		KnockBackValue = LinearInterpolation(subVelocity,
			params_->HitParam_.OriginalRangeMin_, 
			params_->HitParam_.OriginalRangeMax_,
			params_->HitParam_.ConvertedRangeMin_,
			params_->HitParam_.ConvertedRangeMax_);
	}

	//変換したノックバック量をノックバック時の速度x,zに代入
	params_->HitParam_.KnockBack_Velocity_.x = KnockBackValue;
	params_->HitParam_.KnockBack_Velocity_.z = KnockBackValue;

	//溜めている速度をリセット
	character_->OnChargeReset();

	//ノックバック時のY軸回転角の固定
	KnockBackAngleY(params_->HitParam_.KnockBack_Direction_, KnockBackValue);

	//攻撃相手の名前を取得
	params_->HitParam_.AttackedName_ = _attackName;
}

void CharacterHit::KnockBackAngleY(XMFLOAT3 _KnockBackVector, float _KnockBackValue)
{
	if (_KnockBackValue <= 0.0f)
	{
		//ノックバック量が0以下なら処理しない(方向そのまま)
		return;
	}

	//ノックバック中のY軸の回転角を求める
	float angleY = static_cast<float>(atan2(_KnockBackVector.x, _KnockBackVector.z));

	//angleYはラジアン角なのでディグリー角に変換し、Y軸回転にセット
	character_->SetRotateY(XMConvertToDegrees(angleY));
}

void CharacterHit::KnockBack()
{
	//x軸の+回転を行う
	character_->OnMoveRotateX();

	//毎フレームノックバック速度を減少
	params_->HitParam_.KnockBack_Velocity_.x *= params_->HitParam_.DecelerationRate_;
	params_->HitParam_.KnockBack_Velocity_.z *= params_->HitParam_.DecelerationRate_;

	//ノックバック後の位置を計算
	//位置 = 位置 + 方向 * 速度
	XMFLOAT3 TmpPos = character_->GetPosition();
	TmpPos.x += params_->HitParam_.KnockBack_Direction_.x * params_->HitParam_.KnockBack_Velocity_.x;
	TmpPos.z += params_->HitParam_.KnockBack_Direction_.z * params_->HitParam_.KnockBack_Velocity_.z;

	//この時点では変更せず、移動後の仮の位置に保管
	params_->MoveParam_.NewPosition_ = XMLoadFloat3(&TmpPos);

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, params_->MoveParam_.NewPosition_);

	bool IsOutSide = character_->OnIsOutsideStage(tmp);
	if (!IsOutSide)
	{
		character_->OnMoveConfirm();
	}
}

XMVECTOR CharacterHit::HitNormal(std::string _normal)
{
	//指定した名前の鉄線がWireArrayから見つかったら対応した法線を返す
	//返した方向が柵に接触した際のノックバック方向となる
	//見つからない場合は0を返す
	for (int i = 0; i < params_->FenceHitParam_.WireArray_.size(); i++)
	{
		if (_normal == params_->FenceHitParam_.WireArray_[i])
		{
			return params_->FenceHitParam_.NormalArray_[i];
		}
	}

	return { 0,0,0 };
}

bool CharacterHit::IsKnockBackEnd()
{
	//ノックバック速度が終了値に到達したか判定し、真偽を返す

	if (params_->HitParam_.KnockBack_Velocity_.x <= params_->HitParam_.KnockBackEnd_ ||
		params_->HitParam_.KnockBack_Velocity_.z <= params_->HitParam_.KnockBackEnd_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CharacterHit::KnockBackVelocityReset()
{
	params_->HitParam_.KnockBack_Velocity_ = { 0,0,0 };
}
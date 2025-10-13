#include "CharacterHit.h"

void CharacterHit::Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity,
    std::string _attackName)
{
	//無敵状態なら処理しない
	if (FenceHitParam_.IsInvincibility_)
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
	HitParam_.KnockBack_Direction_ = tmp;

	//自身の速度と相手の速度の差分をとる
	//速度の差分に応じてノックバック量を変化させる
	float subVelocity = _myVelocity - _targetVelocity;

	//ノックバック量の初期化
	//毎回値が変化するのでローカル変数
	float KnockBackValue = 0.0f;

	//速度差の判定は線形補完元の最大値を適用
	//速度差が自身の方が一定以上なら、自身のノックバック量は0
	if (subVelocity >= HitParam_.OriginalRangeMax_)
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
			HitParam_.OriginalRangeMin_, HitParam_.OriginalRangeMax_,
			HitParam_.ConvertedRangeMin_, HitParam_.ConvertedRangeMax_);
	}

	//変換したノックバック量をノックバック時の速度x,zに代入
	HitParam_.KnockBack_Velocity_.x = KnockBackValue;
	HitParam_.KnockBack_Velocity_.z = KnockBackValue;

	//溜めている速度をリセット
	charge_->ChargeReset();

	//ノックバック時のY軸回転角の固定
	KnockBackAngleY(HitParam_.KnockBack_Direction_, KnockBackValue);

	//攻撃相手の名前を取得
	HitParam_.AttackedName_ = _attackName;
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
	this->transform_.rotate_.y = XMConvertToDegrees(angleY);
}

void CharacterHit::KnockBack()
{
	//x軸の+回転を行う
	rotate_->MoveRotateX();

	//毎フレームノックバック速度を減少
	HitParam_.KnockBack_Velocity_.x *= HitParam_.DecelerationRate_;
	HitParam_.KnockBack_Velocity_.z *= HitParam_.DecelerationRate_;

	//ノックバック後の位置を計算
	//位置 = 位置 + 方向 * 速度
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += HitParam_.KnockBack_Direction_.x * HitParam_.KnockBack_Velocity_.x;
	TmpPos.z += HitParam_.KnockBack_Direction_.z * HitParam_.KnockBack_Velocity_.z;

	//この時点では変更せず、移動後の仮の位置に保管
	MoveParam_.NewPosition_ = XMLoadFloat3(&TmpPos);

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPosition_);
	if (!movement_->IsOutsideStage(tmp))
	{
		movement_->MoveConfirm();
	}
}

XMVECTOR CharacterHit::HitNormal(std::string _normal)
{
	//指定した名前の鉄線がWireArrayから見つかったら対応した法線を返す
	//返した方向が柵に接触した際のノックバック方向となる
	//見つからない場合は0を返す
	for (int i = 0; i < FenceHitParam_.WireArray_.size(); i++)
	{
		if (_normal == FenceHitParam_.WireArray_[i])
		{
			return FenceHitParam_.NormalArray_[i];
		}
	}

	return { 0,0,0 };
}

bool CharacterHit::IsKnockBackEnd()
{
	//ノックバック速度が終了値に到達したか判定し、真偽を返す

	if (HitParam_.KnockBack_Velocity_.x <= HitParam_.KnockBackEnd_ ||
		HitParam_.KnockBack_Velocity_.z <= HitParam_.KnockBackEnd_)
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
	HitParam_.KnockBack_Velocity_ = { 0,0,0 };
}
#include "CharacterMovement.h"
#include"../Character.h"
#include"../../Engine/Image.h"

CharacterMovement::CharacterMovement(GameObject* parent)
	:GameObject(parent, "CharacterMovement"), params_(nullptr),character_(nullptr)
{
}

void CharacterMovement::InitStartPosition()
{
	// 初期位置を親（Character）のtransformに設定する
	if (character_ != nullptr)
	{
		character_->SetPosition(params_->InitParam_.StartPosition_);
	}
	else
	{
		//親がなければ自分のtransformにセット(安全策)
		this->transform_.position_ = params_->InitParam_.StartPosition_;
	}
}

void CharacterMovement::MoveUpdate(XMVECTOR _input)
{
	//入力があるなら加速
	if (!XMVector3Equal(_input, XMVectorZero()))
	{
		AddAcceleration();
	}
	//入力がないなら減速
	else
	{
		//通常摩擦力による減速
		FrictionNormalDeceleration();
	}

	//加速度が0以下なら停止
	if (IsAcceleStop())
	{
		AccelerationStop();
	}
	else
	{
		//加速度が0以上ならX回転する
		character_->OnMoveRotateX();
	}

	//受け取った方向が0ベクトルでないなら移動方向を確定
	//0ベクトルの場合(入力をしていない)は現在の移動方向を維持し
	//その方向のまま移動～停止することで慣性移動を再現する
	if (!XMVector3Equal(_input, XMVectorZero()))
	{
		//単位ベクトル化し方向として使えるようにする
		params_->MoveParam_.MoveDirection_ = XMVector3Normalize(_input);
	}

	//移動ベクトルを作成
	CreateMoveVector();

	//場外でなければ位置更新 
	NewPositionConfirm();

}

void CharacterMovement::AddAcceleration()
{
	//一定の加速量を加算し続ける
	if (params_->MoveParam_.CommonAcceleration_ < params_->MoveParam_.NormalFullAccelerate_)
	{
		params_->MoveParam_.CommonAcceleration_ += params_->MoveParam_.NormalAcceleValue_;
	}
	else
	{
		//最大加速度を超えないようにする
		params_->MoveParam_.CommonAcceleration_ = params_->MoveParam_.NormalFullAccelerate_;
	}
}

void CharacterMovement::CharacterAttackMove(XMVECTOR _direction)
{
	//受け取った方向が0ベクトルなら処理しない(0ベクトルを正規化はできない)
	if (XMVector3Equal(_direction, XMVectorZero()))
	{
		return;
	}

	//単位ベクトル化し、移動方向を確定
	params_->MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	//移動ベクトルを作成
	CreateMoveVector();

	//場外でなければ位置更新 
	NewPositionConfirm();
}

void CharacterMovement::CreateMoveVector()
{
	//移動ベクトル = 移動方向 * ( 加速度 * 1fの移動量のスケーリング )
	//移動ベクトル化する
	XMVECTOR MoveVector = XMVectorScale(
		params_->MoveParam_.MoveDirection_, 
			 params_->MoveParam_.CommonAcceleration_ * DeltaTime);

	//現在位置と移動ベクトルを加算し
	//移動後のベクトルを作成(この時点では移動確定していない)
	XMFLOAT3 tmp = character_->GetPosition();
	XMVECTOR PrevPos = XMLoadFloat3(&tmp);
	params_->MoveParam_.NewPosition_ = PrevPos + MoveVector;
}

bool CharacterMovement::IsOutsideStage(XMFLOAT3 _position)
{
	//指定位置が一つでもステージ端を超えるかどうか判定し、真偽を返す
	//ステージ外判定をする際に使用

	if (_position.x > params_->EndParam_.EastEnd_ || _position.x <  params_->EndParam_.WestEnd_ ||
		_position.z > params_->EndParam_.NorthEnd_ || _position.z < params_->EndParam_.SouthEnd_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CharacterMovement::MoveConfirm()
{
	//移動後のベクトル(NewPosition_)をtransform_.positionに代入し、移動を確定する
	XMFLOAT3 tmp = character_->GetPosition();
	XMStoreFloat3(&tmp, params_->MoveParam_.NewPosition_);
	character_->SetPosition(tmp);
}

void CharacterMovement::NewPositionConfirm()
{
	//移動後のベクトルをXMFLOAT3型に変換
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, params_->MoveParam_.NewPosition_);

	//移動後の位置が場外でなければ移動を確定する
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void CharacterMovement::Deceleration()
{
	params_->MoveParam_.CommonAcceleration_ -= params_->MoveParam_.NormalAcceleValue_;
}

void CharacterMovement::FrictionNormalDeceleration()
{
	params_->MoveParam_.CommonAcceleration_ -= params_->MoveParam_.NormalFriction_;
}

void CharacterMovement::FrictionAttackDeceleration()
{
	params_->MoveParam_.CommonAcceleration_ -= params_->MoveParam_.AttackFriction_;
}

void CharacterMovement::AccelerationStop()
{
	params_->MoveParam_.CommonAcceleration_ = 0.0f;
}

bool CharacterMovement::IsAcceleStop()
{
	if (params_->MoveParam_.CommonAcceleration_ <= 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CharacterMovement::InitImage()
{
	//ダッシュ攻撃画像を読み込み
	params_->AnimeParam_.hDashImage_ = Image::Load("Image\\Play\\AttackEffect.png");
	assert(params_->AnimeParam_.hDashImage_ >= 0);
}

void CharacterMovement::UpdateDashImage()
{
	//スプライトによるアニメーション更新
	Image::SpriteUpdate(params_->AnimeParam_.DashFrameCount_,
		params_->AnimeParam_.DashAnimeFrame_,
		params_->AnimeParam_.DashOneFrameNum_,
		params_->AnimeParam_.DashFrameCountMax_);

}

void CharacterMovement::DrawDashImage()
{
	//スプライトによるアニメーション描画
	//UpdateDashImageで更新したAnimeFrame_を使用する
	Image::DrawSprite(params_->AnimeParam_.hDashImage_,
		params_->AnimeParam_.DashAnimeFrame_,
		params_->AnimeParam_.DashOneAnimeSize_,
		params_->AnimeParam_.DashOneAnimeSize_,
		params_->AnimeParam_.DashSheetSize_);

}
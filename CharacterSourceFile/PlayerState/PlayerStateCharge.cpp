#include "PlayerStateCharge.h"
#include "../Player.h"
#include "../../Engine/Input.h"

void PlayerStateCharge::Enter(Player* _player)
{
	_player->charge_->ChargeReset();
}

void PlayerStateCharge::Update(Player* _player)
{
	//チャージ中(TmpAcceleを溜めている状態) その場で左右回転できるが動けない

	//加速度を溜める
	_player->charge_->Charging();

	//矢印モデルをセット
	_player->charge_->SetArrow();

	//矢印モデルの位置を自身の回転と合わせる
	_player->GetParams().MoveParam_.ArrowTransform_.rotate_.y = _player->GetRotate().y;

	//チャージ中のエフェクトを出す
	_player->vfx_->SetChargingEffect("ParticleAssets\\circle_B.png");

	//SPACEキー/Aボタンが押され,地上にいるなら
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, 
		_player->GetControllerID()))
	{
		if (_player->GetParams().JumpParam_.IsOnGround_)
		{
			//溜めたチャージを0にする
			_player->charge_->ChargeReset();

			//ジャンプ開始
			_player->air_->SetJump();

			//通常状態へ戻る
			//PlayerState_ = S_Idle;
			//_player->ChangeState(Player::S_Idle);

			//状態遷移の際は一度x回転をストップ
			_player->rotate_->RotateXStop();
		}
	}

	//左右キー/スティックが倒されたら回転
	if (Input::IsKey(DIK_LEFT) || Input::GetPadStickL(_player->GetControllerID()).x < -Input::StickTilt)
	{
		float tmp = _player->GetRotate().y;
		tmp -= _player->GetChargeRotateY();
		_player->SetRotateY(tmp);
	}
	if (Input::IsKey(DIK_RIGHT) || Input::GetPadStickL(_player->GetControllerID()).x > Input::StickTilt)
	{
		float tmp = _player->GetRotate().y;
		tmp += _player->GetChargeRotateY();
		_player->SetRotateY(tmp);
	}

	//SHIFTキー/Bボタンを離したら
	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT)
		|| Input::IsPadButtonUp(XINPUT_GAMEPAD_B, _player->GetControllerID()))
	{
		//チャージ解放
		_player->charge_->ChargeRelease();

		//攻撃状態へ移行
		//PlayerState_ = S_Attack;
		//_player->ChangeState(Player::S_Attack);

		//状態遷移の際は一度x回転をストップ
		_player->rotate_->RotateXStop();
	}

	//高速X回転
	_player->rotate_->FastRotateX();

	//カメラ操作
	_player->CameraControl();
}

void PlayerStateCharge::Exit(Player* _player)
{
	_player->rotate_->RotateXStop();
}

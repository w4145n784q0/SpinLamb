#include "PlayerStateCharge.h"
#include "../Player.h"
#include "../../Engine/Input.h"

namespace
{
	std::string PlayerChargeEffectPath = "ParticleAssets\\circle_B.png";
}

void PlayerStateCharge::Enter(Player* _player)
{
	//溜めている速度をリセット
	//チャージ量は毎回0からスタートさせる
	_player->OnChargeReset();

	//加速度をリセット
	_player->OnAccelerationStop();
}

void PlayerStateCharge::Update(Player* _player)
{
	//チャージ中(TmpAcceleを溜めている状態) その場で左右回転できるが動けない

	//加速度を溜める
	_player->OnCharging();

	//矢印モデルをセット
	_player->OnSetArrow();

	//矢印モデルの位置を自身の回転と合わせる
	_player->GetParams()->MoveParam_.ArrowTransform_.rotate_.y = _player->GetRotate().y;

	//チャージ中のエフェクトを出す
	_player->OnChargingEffect(PlayerChargeEffectPath, _player->GetPosition());

	//SPACEキー/Aボタンが押され,地上にいるなら
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, 
		_player->GetControllerID()))
	{
		if (_player->GetParams()->JumpParam_.IsOnGround_)
		{
			//ジャンプ開始
			_player->OnSetJump();

			//通常状態へ戻る
			_player->ChangeState(Player::S_Idle);

			//シーン遷移のため早期リターン
			return;
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
		_player->OnChargeRelease();

		//攻撃状態へ移行
		_player->ChangeState(Player::S_Attack);

		//シーン遷移のため早期リターン
		return;
	}

	//高速X回転
	_player->OnFastRotateX();

	//カメラ操作
	_player->CameraControl();
}

void PlayerStateCharge::Exit(Player* _player)
{
	//状態遷移の際は一度x回転をストップ
	_player->OnRotateXStop();

	//溜めたチャージを0にする
	_player->OnChargeReset();
}

void PlayerStateCharge::Draw(Player* player)
{
	//矢印モデルの描画
	player->OnDrawArrow();
}
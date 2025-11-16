#include "PlayerStateCharge.h"
#include "../Player.h"
#include "../../Engine/Input.h"

namespace
{
	std::string PlayerChargeEffectPath = "ParticleAssets\\circle_B.png";
}

void PlayerStateCharge::Enter(Player* _player)
{
	_player->GetModuleCharge()->ChargeReset();
}

void PlayerStateCharge::Update(Player* _player)
{
	//チャージ中(TmpAcceleを溜めている状態) その場で左右回転できるが動けない

	//加速度を溜める
	_player->GetModuleCharge()->Charging();

	//矢印モデルをセット
	_player->GetModuleCharge()->SetArrow();

	//矢印モデルの位置を自身の回転と合わせる
	_player->GetParams()->MoveParam_.ArrowTransform_.rotate_.y = _player->GetRotate().y;

	//チャージ中のエフェクトを出す
	_player->GetModuleVFX()->SetChargingEffect(PlayerChargeEffectPath, _player->GetPosition());

	//SPACEキー/Aボタンが押され,地上にいるなら
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, 
		_player->GetControllerID()))
	{
		if (_player->GetParams()->JumpParam_.IsOnGround_)
		{
			//溜めたチャージを0にする
			_player->GetModuleCharge()->ChargeReset();

			//ジャンプ開始
			_player->GetModuleAir()->SetJump();

			//通常状態へ戻る
			_player->ChangeState(Player::S_Idle);

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
		_player->GetModuleCharge()->ChargeRelease();

		//攻撃状態へ移行
		_player->ChangeState(Player::S_Attack);

	}

	//高速X回転
	_player->GetModuleRotate()->FastRotateX();

	//カメラ操作
	_player->CameraControl();
}

void PlayerStateCharge::Exit(Player* _player)
{
	//状態遷移の際は一度x回転をストップ
	_player->GetModuleRotate()->RotateXStop();
}

void PlayerStateCharge::Draw(Player* player)
{
	//矢印モデルの描画
	player->GetModuleCharge()->DrawArrow();
}
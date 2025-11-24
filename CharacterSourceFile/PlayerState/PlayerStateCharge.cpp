#include "PlayerStateCharge.h"
#include "../Player.h"
#include "../../Engine/Input.h"

namespace
{
	std::string PlayerChargeEffectPath = "ParticleAssets\\circle_B.png";
}

void PlayerStateCharge::Enter(Player* _player)
{
	if (_player == nullptr)
	{
		return;
	}

	//溜めている速度をリセット
	//チャージ量は毎回0からスタートさせる
	_player->OnChargeReset();
}

void PlayerStateCharge::Update(Player* _player)
{
	//チャージ中(TmpAcceleを溜めている状態) その場で左右回転できるが動けない

	if (_player == nullptr)
	{
		return;
	}

	//チャージ中は加速度が低下する
	//キャラクター同士の反射にはお互いの加速度を考慮している
	//チャージを長く続けると解放したときに相手をふっとばしやすくなるが
	//自身の加速度が下がり、接触されると自身がふっとびやすくなる
	_player->OnFrictionNormalDeceleration();

	//加速度が0以下になったら0で固定する
	if (_player->OnIsAcceleStop())
	{
		_player->OnAccelerationStop();
	}

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

			//加速度をリセット(Idle状態の加速度が残っているため)
			_player->OnAccelerationStop();
			
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
	if (_player == nullptr)
	{
		return;
	}

	//状態遷移の際は一度x回転をストップ
	_player->OnRotateXStop();

	//溜めたチャージを0にする
	_player->OnChargeReset();
}

void PlayerStateCharge::Draw(Player* _player)
{
	if (_player == nullptr)
	{
		return;
	}

	//矢印モデルの描画
	_player->OnDrawArrow();
}
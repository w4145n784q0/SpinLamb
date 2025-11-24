#include "PlayerStateIdle.h"
#include "../Player.h"
#include "../../Engine/Input.h"

void PlayerStateIdle::Enter(Player* _player)
{
	if (_player == nullptr)
	{
		return;
	}

	//加速度をリセット
	_player->OnAccelerationStop();
}

void PlayerStateIdle::Update(Player* _player)
{
	//通常状態 移動・ジャンプなどをしている状態

	if (_player == nullptr)
	{
		return;
	}

	//キーボードの入力した分を実際に移動
	_player->InputKeyBoard();

	//コントローラー操作による移動
	_player->InputCotroller(_player->GetControllerID());

	//------------------チャージ状態へ移行------------------//

	//SHIFTキー/Bボタンが押されたら
	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT)
		|| Input::IsPadButtonDown(XINPUT_GAMEPAD_B, _player->GetControllerID()))
	{
		if (_player->GetParams()->JumpParam_.IsOnGround_)
		{
			//地上にいるならチャージ状態へ移行
			_player->ChangeState(Player::S_Charge);

			//シーン遷移のため早期リターン
			return;
		}
	}

	//------------------ジャンプ------------------//

	//SPACEキー/Aボタンが押されたら
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, _player->GetControllerID()))
	{
		if (_player->GetParams()->JumpParam_.IsOnGround_)
		{
			//地上にいるならジャンプ開始
			_player->OnSetJump();
		}
	}

	//プレイヤーの入力方向に即した回転
	_player->PlayerRotate(_player->GetPlayerInput());

	//実際の移動にはcameraTransform_.rotate_.yを適用したベクトルを使う
	XMVECTOR moveForCamera = _player->ConvertCameraDirection(_player->GetPlayerInput());

	//慣性処理のための移動処理（カメラ回転を反映したベクトルを渡す）
	_player->OnMoveUpdate(moveForCamera);

	//カメラ操作
	_player->CameraControl();
}

void PlayerStateIdle::Exit(Player* _player)
{
	if (_player == nullptr)
	{
		return;
	}

	//状態遷移の際は一度x回転をストップ
	_player->OnRotateXStop();
}

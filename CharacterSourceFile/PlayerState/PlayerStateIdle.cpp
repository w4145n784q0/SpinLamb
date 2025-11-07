#include "PlayerStateIdle.h"
#include "../Player.h"
#include "../../Engine/Input.h"

void PlayerStateIdle::Enter(Player* _player)
{
	_player->movement_->AccelerationStop();
}

void PlayerStateIdle::Update(Player* _player)
{
	//通常状態 移動・ジャンプなどをしている状態

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

			//加速度をリセット
			_player->movement_->AccelerationStop();
		}
	}

	//------------------ジャンプ------------------//

	//SPACEキー/Aボタンが押されたら
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, _player->GetControllerID()))
	{
		if (_player->GetParams()->JumpParam_.IsOnGround_)
		{
			//地上にいるならジャンプ開始
			_player->air_->SetJump();
			//PlayerState_ = S_Jump;
		}
	}

	//プレイヤーの入力方向に即した回転
	_player->PlayerRotate(_player->GetPlayerInput());

	//実際の移動にはcameraTransform_.rotate_.yを適用したベクトルを使う
	XMVECTOR moveForCamera = _player->ConvertCameraDirection(_player->GetPlayerInput());

	//慣性処理のための移動処理（カメラ回転を反映したベクトルを渡す）
	_player->movement_->MoveUpdate(moveForCamera);

	//カメラ操作
	_player->CameraControl();
}

void PlayerStateIdle::Exit(Player* _player)
{
	_player->rotate_->RotateXStop();
}

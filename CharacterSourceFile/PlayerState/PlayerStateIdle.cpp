#include "PlayerStateIdle.h"
#include "../Player.h"
#include "../../Engine/Input.h"

void PlayerStateIdle::Enter(Player* player)
{
	player->movement_->AccelerationStop();
}

void PlayerStateIdle::Update(Player* player)
{
	//通常状態 移動・ジャンプなどをしている状態

	//キーボードの入力した分を実際に移動
	player->InputKeyBoard();

	//コントローラー操作による移動
	player->InputCotroller(player->GetControllerID());

	//------------------チャージ状態へ移行------------------//

	//SHIFTキー/Bボタンが押されたら
	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT)
		|| Input::IsPadButtonDown(XINPUT_GAMEPAD_B, player->GetControllerID()))
	{
		if (player->GetParams().JumpParam_.IsOnGround_)
		{
			//地上にいるならチャージ状態へ移行
			//PlayerState_ = S_Charge;

			//加速度をリセット
			player->movement_->AccelerationStop();

			//状態遷移の際は一度x回転をストップ
			player->rotate_->RotateXStop();
		}
	}

	//------------------ジャンプ------------------//

	//SPACEキー/Aボタンが押されたら
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, player->GetControllerID()))
	{
		if (player->GetParams().JumpParam_.IsOnGround_)
		{
			//地上にいるならジャンプ開始
			player->air_->SetJump();
			//PlayerState_ = S_Jump;
		}
	}

	//プレイヤーの入力方向に即した回転
	player->PlayerRotate(player->GetPlayerInput());

	//実際の移動にはcameraTransform_.rotate_.yを適用したベクトルを使う
	XMVECTOR moveForCamera = player->ConvertCameraDirection(player->GetPlayerInput());

	//慣性処理のための移動処理（カメラ回転を反映したベクトルを渡す）
	player->movement_->MoveUpdate(moveForCamera);

	//カメラ操作
	player->CameraControl();
}

void PlayerStateIdle::Exit(Player* player)
{
	player->rotate_->RotateXStop();
}

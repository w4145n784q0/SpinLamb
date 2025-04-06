#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include"Enemy.h"
#include"BossBattleScene.h"

#include <algorithm>
#include<list>

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08f; //0.16333f
	const float DeltaTime = 0.016f;
	const float FullAccelerate = 60.0f;
	XMVECTOR BackCameraPos = { 0,2,-10,0 };//BackCameraの値は変わるが毎フレームこの値にする（値が変わり続けるのを防ぐ）
	
	const float TreeCollision = 4.0f;
	float PrevHeight = 0.0f;

	XMVECTOR PlayerFrontDirection = { 0,0,1 };//正面ベクトル ここからどれだけ回転したか
	int deadTimerValue = 60;//復活時間
	
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hPlayer_(-1), hLandingPoint_(-1), IsOnGround_(true), IsDash_(false),
	JumpSpeed_(0.0f),
	Direction_({ 0,0,0 }),  PlayerPosition_({ 0,0,0 }), Acceleration_(0.0f),BackCamera_(BackCameraPos),
	PlayerState_(S_IDLE), CanMove_(true),PlayerHeight_(0),
	deadTimer_(deadTimerValue)
{
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x ,this->transform_.position_.y + 1, this->transform_.position_.z - 8 };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	hPlayer_ = Model::Load("Player.fbx"); 
	assert(hPlayer_ >= 0);
	hLandingPoint_ = Model::Load("LandingPoint.fbx");
	assert(hLandingPoint_ >= 0);
	hCollisionSound_ = Audio::Load("maou_se_battle15.wav");
	assert(hCollisionSound_ >= 0);

	SetStartPosition();

	pGround_ = (Ground*)FindObject("Ground");
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),0.3f);
	this->AddCollider(collider);

	
}

void Player::Update()
{
	cameraTransform_.position_ = this->transform_.position_;
	PlayerPosition_ = XMLoadFloat3(&this->transform_.position_); 

	switch (PlayerState_)
	{
	case Player::S_IDLE:
		UpdateIdle();
		break;
	case Player::S_HIT:
		UpdateHit();
		break;
	case Player::S_CHARGE:
		UpdateCharge();
		break;
	case Player::S_ATTACK:
		UpdateAttack();
		break;
	case Player::S_OUT:
		UpdateOut();
		break;
	case Player::S_WALLHIT:
		UpdateWallHit();
		break;
	default:
		break;
	}

	//--------------カメラ追従--------------
	CameraTarget_ = { this->transform_.position_ };//カメラの焦点は自機の位置に固定
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform_.rotate_.y));//カメラの回転行列作成(Y軸)
	XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(cameraTransform_.rotate_.x));//カメラの回転行列作成(X軸)
	XMMATRIX rotCamera = XMMatrixMultiply(rotX, rotY);
	BackCamera_ = XMVector3TransformCoord(BackCamera_, rotCamera);//バックカメラのベクトルにかける
	XMStoreFloat3(&CameraPosition_, NewPos_ + BackCamera_);//移動ベクトルと加算

	//CameraPosition_.y += Camera::CameraShake();
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	Camera::SetPosition(CameraPosition_);//カメラの位置をセット 
	Camera::SetTarget(CameraTarget_);//カメラの焦点をセット
	BackCamera_ = { BackCameraPos };//バックカメラベクトルをリセット

}

void Player::Draw()
{
	Model::SetTransform(hPlayer_, transform_);
	Model::Draw(hPlayer_);

	Transform t;
	t.position_ = PlayerFront;

	ImGui::Text("PositionX:%.3f", this->transform_.position_.x);
	ImGui::Text("PositionY:%.3f", this->transform_.position_.y);
	ImGui::Text("PositionZ:%.3f", this->transform_.position_.z);

	//ImGui::Text("camera y :%.3f", CameraPosition_.y);
	//ImGui::Text("camera x :%.3f", CameraPosition_.x);

	/*ImGui::Text("IsOnGround:%.1f", IsOnGround_);*/

}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
	{
		//敵のノックバック
		Enemy* pEnemy = (Enemy*)FindObject("Enemy");

		//敵の位置-自機の位置を計算
		XMFLOAT3 direction = pEnemy->GetPosition() - this->transform_.position_;

		//単位ベクトルにする
		XMVECTOR v =  XMLoadFloat3(&direction);
		XMVECTOR normalDirection = XMVector3Normalize(v);

		//敵のノックバック処理
		pEnemy->PlayerReflect(normalDirection, IsDash_);

		Audio::Play(hCollisionSound_);


		//プレイヤーの衝突時処理
		XMFLOAT3 f;
		XMStoreFloat3(&f, normalDirection);
		if (pEnemy->GetStateAttack())
		{
			f.x *= -4.0;
			f.z *= -4.0;
			//PlayerState_ = S_HIT;
		}

		//カメラ振動
		Camera::CameraShakeStart(0.15f);

		Acceleration_ = 0;
		IsDash_ = false;
		IsDashStart_ = false;
	}
}

void Player::Dash()
{
	/*if (IsDash_)
	{
		Acceleration_ += 2.0;
		if (Acceleration_ > FullAccelerate) {
			Acceleration_ = FullAccelerate;
		}
	}
	else
	{
		Acceleration_ = 0;
	}*/
	if(IsDash_)
	{
		if (!IsDashStart_)
		{
			Acceleration_ += FullAccelerate;
			IsDashStart_ = true;
		}
		else
		{
			Acceleration_ -= 2;
			Direction_.z = 1.0;
			if (Acceleration_ <= 0)
			{
				IsDash_ = false;
				IsDashStart_ = false;
			}
		}
	}
}

void Player::UpdateIdle()
{
	//------------------キーボード入力の移動------------------//
	if (Input::IsKey(DIK_UP))
	{
		Direction_.z = 1.0;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z = -1.0;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
	}

	//------------------ゲームパッドスティックの移動------------------//

	//前方だけに移動
	if (Input::GetPadStickL().y >= 0.5 /*&& Input::GetPadStickL().x <= 0.5 && Input::GetPadStickL().x >= -0.5*/)
	{
		Direction_.z = 1.0;
	}

	//後方だけに移動
	if (Input::GetPadStickL().y <= -0.5 /*&& Input::GetPadStickL().x >= 0.5 && Input::GetPadStickL().x <= -0.5*/)
	{
		Direction_.z = -1.0;
	}

	//前進&左回転
	if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x <= -0.25)
	{
		Direction_.z = 1.0;
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
	}

	//前進&右回転
	if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x >= 0.25)
	{
		Direction_.z = 1.0;
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
	}

	//左回転だけ
	if (Input::GetPadStickL().x <= -0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
	}
	//右回転だけ
	if (Input::GetPadStickL().x >= 0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
	}

	//プレイヤーの正面ベクトルを更新
	//自分の前方ベクトル(回転した分も含む)
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, PlayerFrontDirection);

	XMFLOAT3 rot = { 0,0,0 };
	XMStoreFloat3(&rot, ForwardVector_);
	PlayerFront = { transform_.position_ + rot };

	//--------------ダッシュ関係--------------
	/*if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_RSHIFT) 
		|| Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		IsDash_ = true;
	}
	else
	{
		IsDash_ = false;
	}*/
	if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if(IsOnGround_)
		{
			IsDash_ = true;
		}
	}

	Dash();

	//プレイヤーのy回転をラジアン化して行列に
	XMMATRIX playerRot = XMMatrixRotationY(XMConvertToRadians(this->transform_.rotate_.y));

	//プレイヤーの進行方向をベクトル化
	XMVECTOR PrevDir = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);

	//方向ベクトルを回転行列で変換
	PrevDir = XMVector3TransformCoord(PrevDir, playerRot);

	//単位ベクトル化する
	XMVECTOR norm = XMVector3Normalize(PrevDir);

	//移動ベクトル化する
	XMVECTOR MoveVector = XMVectorScale(norm, (speed + Acceleration_) * DeltaTime);

	//現在位置と移動ベクトルを加算
	XMVECTOR PrevPos = PlayerPosition_;
	NewPos_ = PrevPos + MoveVector;

	XMStoreFloat3(&this->transform_.position_, NewPos_);

	//ジャンプ
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (IsOnGround_)
		{
			IsOnGround_ = false;
			PrevHeight = transform_.position_.y;
			JumpSpeed_ = 2.2f;//一時的にy方向にマイナスされている値を大きくする
		}
	}

	if (transform_.position_.x > 60.0f || transform_.position_.x < -60.0f ||
		transform_.position_.z > 60.0f || transform_.position_.z < -60.0f)
	{
		PlayerState_ = S_WALLHIT;
	}
	

	JumpSpeed_ -= Player_Gravity;//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	this->transform_.position_.y += JumpSpeed_;//フィールドに乗っているかは関係なく重力はかかり続ける

	if (this->transform_.position_.y <= 0.5f && IsOnGround_)//プレイヤーめりこみ防止に一定以下のy座標で値を固定
	{
		this->transform_.position_.y = 0.5f;
	}
	if (this->transform_.position_.y < -500) 
	{
		this->transform_.position_.y = -500;//高さの最低値
		PlayerState_ = S_OUT;
	}

	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}


	CameraControl();


	Direction_ = { 0,0,0 };//最後に進行方向のリセット毎フレーム行う
}

void Player::UpdateHit()
{
	//
}

void Player::UpdateCharge()
{
	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
	}
}

void Player::UpdateAttack()
{

}

void Player::UpdateOut()
{
	if (--deadTimer_ < 0)
	{
		//BossBattleScene* pBossBattleScene = (BossBattleScene*)FindObject("BossBattleScene");
		//pBossBattleScene->DeadCountPlus();

		deadTimer_ = deadTimerValue;
		PlayerState_ = S_IDLE;
		SetStartPosition();
	}
}

void Player::UpdateWallHit()
{
	if (--deadTimer_ < 0)
	{
		//BossBattleScene* pBossBattleScene = (BossBattleScene*)FindObject("BossBattleScene");
		//pBossBattleScene->DeadCountPlus();

		deadTimer_ = deadTimerValue;
		PlayerState_ = S_IDLE;
		SetStartPosition();
	}
}

void Player::UpdateDead()
{

}

void Player::CameraControl()
{
	if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -0.7)
	{
		cameraTransform_.rotate_.y -= 2.5;
	}
	if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= 0.7)
	{
		cameraTransform_.rotate_.y += 2.5;
	}

	if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -0.7)
	{
		if(cameraTransform_.rotate_.x >= 60.0f)
		{
			cameraTransform_.rotate_.x = 60.0f;
		}
		else
		{
			cameraTransform_.rotate_.x += 2.5;
		}
	}
	if (Input::IsKey(DIK_S) || Input::GetPadStickR().y >= 0.7)
	{
		if (cameraTransform_.rotate_.x <= -10.0f)
		{
			cameraTransform_.rotate_.x = -10.0f;
		}
		else
		{
			cameraTransform_.rotate_.x -= 2.5;
		}
	}

	if (Input::IsKey(DIK_Z) || Input::IsPadButton(XINPUT_GAMEPAD_Y))//カメラを正面に戻す（方向に変化なし）
	{
		cameraTransform_.rotate_.y = 0;
		cameraTransform_.rotate_.x = 0;
		this->transform_.rotate_.y = 0;
	}
}

void Player::CameraShake()
{

}
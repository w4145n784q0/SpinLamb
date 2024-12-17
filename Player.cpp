#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

#include"Enemy.h"

namespace {
	const float speed = 9.0f;
	const float Player_Gravity = 0.08; //0.16333f
	const float DeltaTime = 0.016f;
	const XMVECTOR front = { 0,0,1 };
	XMVECTOR BackCameraPos = { 0,2,-10,0 };//BackCameraの値は変わるが毎回この値にする（値が変わり続けるのを防ぐ）
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_Player(-1),IsOnGround_(true),JumpSpeed_(0.0f),
	 PlayerDirection({ 0,0,0 }),PlayerPosition({0,0,0}), Acceleration_(0.0f),BackCamera(BackCameraPos)
{
	cameraTransform = this->transform_;
	CameraPosition = { this->transform_.position_.x ,this->transform_.position_.y + 1, this->transform_.position_.z - 8 };
	CameraTarget = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

	//hcamera = - 1;
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	hModel_Player = Model::Load("ensui.fbx"); 
	//hcamera = Model::Load("box.fbx");
	this->transform_.position_ = { 0,1,0};

	
	SphereCollider* col = new SphereCollider(XMFLOAT3(0,0,0),0.1f);
	this->AddCollider(col);
}

void Player::Update()
{
	cameraTransform.position_ = this->transform_.position_;
	

	PlayerPosition = XMLoadFloat3(&this->transform_.position_); 
	XMFLOAT3 x;
	XMStoreFloat3(&x, PlayerPosition);

	if (Input::IsKey(DIK_UP))
	{
		Direction.z = 1.0;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction.z = -1.0;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform.rotate_.y -= 1;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += 1;
		cameraTransform.rotate_.y += 1;
	}
	
	//--------------ダッシュ関係--------------
	if (Input::IsKey(DIK_LSHIFT) || Input::IsKey(DIK_RSHIFT)) 
	{
		IsDash_ = true;
	}
	else
	{
		IsDash_ = false;
	}
	Dash();
	
	XMMATRIX playerRot = XMMatrixRotationY(XMConvertToRadians(this->transform_.rotate_.y));
	XMVECTOR PrevDir = XMVectorSet(Direction.x,Direction.y, Direction.z, 0.0f);
	PrevDir = XMVector3TransformCoord(PrevDir, playerRot);
	XMVECTOR norm = XMVector3Normalize(PrevDir);// 単位ベクトルに正規化
	XMVECTOR MoveVector = XMVectorScale(norm,(speed + Acceleration_) * DeltaTime);//移動ベクトル化する

	XMVECTOR PrevPos = PlayerPosition;

	XMVECTOR NewPos = PrevPos + MoveVector;
	XMStoreFloat3(&this->transform_.position_, NewPos);

	Direction = { 0,0,0 };

	//--------------ジャンプ(挙動のみ)--------------
	if (Input::IsKeyDown(DIK_SPACE))
	{
		if (IsOnGround_) {
			IsOnGround_ = false;
			JumpSpeed_ = 1.5;//一時的にy方向にマイナスされている値を大きくする
		}
	}

	JumpSpeed_ -= Player_Gravity;//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	this->transform_.position_.y += JumpSpeed_;
	if (JumpSpeed_ < -100) {
		JumpSpeed_ = -100;
	}

	if (this->transform_.position_.y <= 1.0f) {//プレイヤーめりこみ防止に一定以下のy座標で値を固定
		this->transform_.position_.y = 1.0f;
		IsOnGround_ = true;
	}

	//--------------カメラ追従--------------
	
	if (Input::IsKey(DIK_A))
	{
		cameraTransform.rotate_.y -= 5;
	}
	if (Input::IsKey(DIK_D))
	{
		cameraTransform.rotate_.y += 5;
	}
	if (Input::IsKey(DIK_Z))//カメラを正面に戻す（方向に変化なし）
	{
		cameraTransform.rotate_.y = 0;
		this->transform_.rotate_.y = 0;
	}

	CameraTarget = { this->transform_.position_ };//カメラの位置は自機の位置に固定
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform.rotate_.y));//カメラの回転行列をつくり
	BackCamera = XMVector3TransformCoord(BackCamera, rotY);//バックカメラのベクトルにかける
	XMStoreFloat3(&CameraPosition, NewPos + BackCamera);//移動ベクトルと加算

	Camera::SetPosition(CameraPosition);//カメラの位置をセット 
	Camera::SetTarget(CameraTarget);//カメラの焦点をセット
	BackCamera = { BackCameraPos };//バックカメラベクトルをリセット


}

void Player::Draw()
{
	Model::SetTransform(hModel_Player, transform_);
	Model::Draw(hModel_Player);

	/*Transform t;
	t.position_ = { 0,0,0 };
	Model::SetTransform(hcamera, t);
	Model::Draw(hcamera);*/
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{

}

void Player::Dash()
{
	if (IsDash_)
	{
		Acceleration_ += 2.0;
		if (Acceleration_ > 50.0) {
			Acceleration_ = 50.0;
		}
	}
	else
	{
		Acceleration_ = 0;
	}
}

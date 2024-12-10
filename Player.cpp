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
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_Player(-1),IsOnGround_(true),JumpSpeed_(0.0f),
	PlayerDirection({0,0,0}),Acceleration_(0.0f)
{
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	hModel_Player = Model::Load("ensui.fbx"); 
	transform_.position_ = { -20,0,-20};
	
	SphereCollider* col = new SphereCollider(XMFLOAT3(0,0,0),0.1f);
	this->AddCollider(col);
}

void Player::Update()
{
	PlayerPosition = XMLoadFloat3(&this->transform_.position_);

	if (Input::IsKey(DIK_UP))
	{
		Direction.z = 1.0;
		moveDir = Front;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction.z = -1.0;
		moveDir = Back;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		Direction.x = -1.0;
		moveDir = Left;
		
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		Direction.x = 1.0;
		moveDir = Right;
	}
	transform_.rotate_.y = MoveDirArray[moveDir];//キャラの回転　４方向のみ

	
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
	

	XMVECTOR PrevDir = XMVectorSet(Direction.x,Direction.y, Direction.z, 0.0f);
	PlayerDirection = XMVector3Normalize(PrevDir);// 単位ベクトルに正規化
	XMVECTOR MoveVector = XMVectorScale(PlayerDirection,(speed + Acceleration_) * DeltaTime);//移動ベクトル化する

	XMVECTOR PrevPos = PlayerPosition;

	XMVECTOR NewPos = PrevPos + MoveVector;
	XMStoreFloat3(&transform_.position_, NewPos);

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
	transform_.position_.y += JumpSpeed_;
	if (JumpSpeed_ < -100) {
		JumpSpeed_ = -100;
	}

	if (transform_.position_.y <= 0.0f) {//プレイヤーめりこみ防止に一定以下のy座標で値を固定
		transform_.position_.y = 0.0f;
		IsOnGround_ = true;
	}

	//--------------カメラ追従--------------
	XMFLOAT3 CameraPositionVec = { transform_.position_.x ,transform_.position_.y + 1, transform_.position_.z - 8 };
	XMFLOAT3 CameraTargetVec = { transform_.position_.x,transform_.position_.y, transform_.position_.z };

	Camera::SetPosition(CameraPositionVec);//カメラの位置をセット（今は追従するだけ） 
	Camera::SetTarget(CameraTargetVec);//カメラの焦点をセット（今は追従するだけ）


	/*
	Enemy* pEnemy = (Enemy*)FindObject("Enemy");    //ステージオブジェクトを探す
	int hEnemyModel = pEnemy->GetModelHandle();    //モデル番号を取得

	RayCastData data;
	data.start = transform_.position_;   //レイの発射位置
	data.dir = XMFLOAT3(0, 0, 1);       //レイの方向
	
	Model::RayCast(hEnemyModel, &data); //レイを発射

	if (data.hit) {
		transform_.position_.y = 3;
	}*/


}

void Player::Draw()
{
	Model::SetTransform(hModel_Player, transform_);
	Model::Draw(hModel_Player);
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

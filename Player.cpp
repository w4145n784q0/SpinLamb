#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"Engine/VFX.h"
#include"Engine/SceneManager.h"
#include"BattleScene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include"Enemy.h"

namespace {
	XMVECTOR BackCameraPos = { 0,3,-10,0 };//BackCameraの値は変わるが毎フレームこの値にする（値が変わり続けるのを防ぐ）

}

Player::Player(GameObject* parent) 
	: Character(parent,"Player"),
	hPlayer_(-1), /*hAttackArrow_(-1),*/ hCollisionSound_(-1),
	/*pGround_(nullptr),*/PlayerState_(S_MAX),CameraState_(S_NORMALCAMERA),
	

	Direction_({ 0,0,0 }),BackCamera_(BackCameraPos)
{
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x ,this->transform_.position_.y + 1, this->transform_.position_.z - 8 };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

	//srand((unsigned)time(NULL));
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	std::string path = "CSVdata\\PlayerData.csv";
	SetcsvStatus(path);

	hPlayer_ = Model::Load("Model\\chara2.fbx");
	assert(hPlayer_ >= 0);

	//hAttackArrow_ = Model::Load("Model\\AttackArrow.fbx");
	//assert(hAttackArrow_ >= 0);

	hCollisionSound_ = Audio::Load("Sound\\maou_se_battle15.wav");
	assert(hCollisionSound_ >= 0);

	SetStartPosition();
	//ArrowTransform_.rotate_.y = 180.0f;

	//pGround_ = (Ground*)FindObject("Ground");
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),ColliderSize_);
	this->AddCollider(collider);

	
}

void Player::Update()
{
	cameraTransform_.position_ = this->transform_.position_;

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
	case Player::S_WALLHIT:
		UpdateWallHit();
		break;
	default:
		break;
	}

	//カメラの更新
	CameraUpdate();

	//degug
	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}

}

void Player::Draw()
{
	Model::SetTransform(hPlayer_, transform_);
	Model::Draw(hPlayer_);

	/*if (PlayerState_ == S_CHARGE)
	{
		Model::SetTransform(hAttackArrow_, ArrowTransform_);
		Model::Draw(hAttackArrow_);
	}*/

#ifdef _DEBUG
	ImGui::Text("PositionX:%.3f", this->transform_.position_.x);
	ImGui::Text("PositionY:%.3f", this->transform_.position_.y);
	ImGui::Text("PositionZ:%.3f", this->transform_.position_.z);

	//ImGui::Text("camera y :%.3f", CameraPosition_.y);
	//ImGui::Text("camera x :%.3f", CameraPosition_.x);

	//ImGui::Text("dash:%.3f", Acceleration_);
	
	XMFLOAT3 tmp;
    XMStoreFloat3(&tmp, MoveDirection_);

	//ImGui::Text("front.x:%3f", (float)tmp.x);
	//ImGui::Text("front.y:%3f", (float)tmp.y);
	//ImGui::Text("front.z:%3f", (float)tmp.z);

	ImGui::Text("x:%3f", Input::GetPadStickL().x);
	ImGui::Text("y:%3f", Input::GetPadStickL().y);

#endif

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

		//敵の位置-自機の位置を計算（敵の反射）
		//単位ベクトルにする
		XMFLOAT3 Enemydirection = pEnemy->GetPosition() - this->transform_.position_;
		XMVECTOR EnemynormalDirection = XMVector3Normalize(XMLoadFloat3(&Enemydirection));

		//自機の位置-敵の位置を計算（自機の反射）
		//単位ベクトルにする
		XMFLOAT3 Playerdirection = this->transform_.position_ - pEnemy->GetPosition();
		XMVECTOR PlayernormalDirection = XMVector3Normalize(XMLoadFloat3(&Playerdirection));

		//プレイヤーの衝突時処理
		//プレイヤー:通常 敵:通常 双方がはじかれる(軽度)
		//プレイヤー:通常 敵:攻撃 プレイヤーを大きくはじく
		//プレイヤー:ダッシュ 敵:攻撃 敵をはじく プレイヤーは方向ベクトル(敵の位置-自機の位置)に対し垂直方向に移動（正面からぶつかったらプレイヤーは停止
		//プレイヤー:ダッシュ 敵:通常 敵を大きくはじく
		//プレイヤーは方向ベクトル(敵の位置-自機の位置)に対し垂直方向に移動（正面からぶつかったらプレイヤーは停止

		bool IsEnemyAttack = pEnemy->GetStateAttack();

		if (IsEnemyAttack)//敵:攻撃
		{
			if (IsAttackState())//プレイヤー:攻撃
			{
				//敵のノックバック処理
				pEnemy->PlayerReflect(EnemynormalDirection, true);
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
			else//プレイヤー:通常
			{
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
			//ヒットエフェクト
			SetHitEffect();
		}
		else//敵:通常
		{
			if (IsAttackState())//プレイヤー:攻撃
			{
				//敵のノックバック処理
				pEnemy->PlayerReflect(EnemynormalDirection,true);
				//ヒットエフェクト
				SetHitEffect();
			}
			else//プレイヤー:通常
			{
				pEnemy->PlayerReflect(EnemynormalDirection, false);
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
		}

		//カメラ振動
		Camera::CameraShakeStart(0.3f);

		//衝撃音
		Audio::Play(hCollisionSound_);

		Acceleration_ = 0;
	}

	if (pTarget->GetObjectName() == "Fence")
	{
		if(!IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			Acceleration_ = 0.0f;
			XMFLOAT3 inverse;
			XMStoreFloat3(&inverse, ForwardVector_);
			KnockBack_Direction_ = { inverse.x, inverse.y, inverse.z };
			KnockBack_Velocity_.x = KnockBackPower_;
			KnockBack_Velocity_.z = KnockBackPower_;
			PlayerState_ = S_WALLHIT;
		}
	}
}

void Player::UpdateIdle()
{
	//無敵時間の計算
	if (IsInvincibility_)
	{
		if (--InvincibilityTime_ < 0)
		{
			InvincibilityTime_ = InvincibilityValue;
			IsInvincibility_ = false;
		}
	}

	//------------------キーボード入力の移動------------------//
	if (Input::IsKey(DIK_UP))
	{
		Direction_.z = -1.0;
		this->transform_.rotate_.x -= MoveRotateX;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z = 1.0;
		this->transform_.rotate_.x += MoveRotateX;
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

	////前方だけに移動
	//if (Input::GetPadStickL().y >= 0.5 /*&& Input::GetPadStickL().x <= 0.5 && Input::GetPadStickL().x >= -0.5*/)
	//{
	//	Direction_.z = -1.0;
	//	this->transform_.rotate_.x -= MoveRotateX;
	//}

	////後方だけに移動
	//if (Input::GetPadStickL().y <= -0.5 /*&& Input::GetPadStickL().x >= 0.5 && Input::GetPadStickL().x <= -0.5*/)
	//{
	//	Direction_.z = 1.0;
	//	this->transform_.rotate_.x += MoveRotateX;
	//}

	////前進&左回転
	//if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x <= -0.1)
	//{
	//	//Direction_.z = -1.0;
	//	this->transform_.rotate_.y -= 1;
	//	this->transform_.rotate_.x -= MoveRotateX;
	//	cameraTransform_.rotate_.y -= 1;
	//}

	////前進&右回転
	//if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x >= 0.1)
	//{
	//	//Direction_.z = -1.0;
	//	this->transform_.rotate_.y += 1;
	//	this->transform_.rotate_.x += MoveRotateX;
	//	cameraTransform_.rotate_.y += 1;
	//}

	////左回転だけ
	//if (Input::GetPadStickL().x <= -0.25 && Input::GetPadStickL().y <= 0.25)
	//{
	//	this->transform_.rotate_.y -= 1;
	//	cameraTransform_.rotate_.y -= 1;
	//}
	////右回転だけ
	//if (Input::GetPadStickL().x >= 0.25 && Input::GetPadStickL().y <= 0.25)
	//{
	//	this->transform_.rotate_.y += 1;
	//	cameraTransform_.rotate_.y += 1;
	//}



	//--------------ダッシュ関係--------------

	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if (IsOnGround_)
		{
			//SetChargeEffect();
			PlayerState_ = S_CHARGE;
		}
	}
	//マウス用方向ベクトル
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
	CharacterMoveRotate(move,this->transform_.rotate_.y);

	//コントローラーを倒した方向・角度を取得
	/*XMVECTOR cont = XMVectorSet(Input::GetPadStickL().x, Input::GetPadStickL().y, Input::GetPadStickL().z, 0.0f);
	float length = XMVectorGetX(XMVector3Length(cont));
	
	if(length > 0.0001f)
	{
		cont = XMVector3Normalize(cont);
		XMVECTOR r = XMVector3AngleBetweenVectors(cont, FrontDirection_);
		float angle = XMVectorGetX(r);
		float angleDeg = XMConvertToDegrees(angle);
		this->transform_.rotate_.y = angleDeg;
		CharacterMoveRotate(cont,angleDeg);
	}
	else
	{

	}*/


	//自分の前方ベクトル(回転した分も含む)を更新
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, FrontDirection_);

	//プレイヤーの正面ベクトルを更新
	//自分の前方ベクトル(回転した分も含む)
	/*ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, PlayerFrontDirection);
	XMFLOAT3 rot = { 0,0,0 };
	XMStoreFloat3(&rot, ForwardVector_);
	PlayerFront = { transform_.position_ + rot };
	PlayerFront.z += 1.0;
	ArrowTransform_.position_ = PlayerFront;*/


	//ジャンプ
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (IsOnGround_)
		{
			IsOnGround_ = false;
			JumpSpeed_ = 1.8f;//一時的にy方向にマイナスされている値を大きくする
		}
	}

	CharacterGravity();

	CameraControl();


	Direction_ = { 0,0,0 };//最後に進行方向のリセット毎フレーム行う
}

void Player::UpdateHit()
{
	KnockBack();
	CharacterGravity();

	if (KnockBack_Velocity_.x <= KnockBackEnd_ || KnockBack_Velocity_.z <= KnockBackEnd_)
	{
		PlayerState_ = S_IDLE;
	}
}

void Player::UpdateCharge()
{
	SetChargingEffect("PaticleAssets\\circle_B.png");

	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (IsOnGround_)
		{
			Acceleration_ = 0.0f;
			PlayerState_ = S_IDLE;
			IsOnGround_ = false;
			JumpSpeed_ = 1.8f;//一時的にy方向にマイナスされている値を大きくする
		}
	}


	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
		//ArrowTransform_.rotate_.y -= 1;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
		//ArrowTransform_.rotate_.y += 1;
	}

	//左回転だけ
	if (Input::GetPadStickL().x <= -0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
		//ArrowTransform_.rotate_.y -= 1;
	}
	//右回転だけ
	if (Input::GetPadStickL().x >= 0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
		//ArrowTransform_.rotate_.y += 1;
	}

	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		//VFX::End(hChargeEmit_);
		//SetAttackLocusEffect();
		//SetAttackAuraEffect();
		PlayerState_ = S_ATTACK;
	}

	Charging();

	this->transform_.rotate_.x -= FastRotateX;
	CameraControl();
}

void Player::UpdateAttack()
{
	SetAttackLocusEffect();

	Acceleration_ -= AcceleValue_;
	Direction_.z = -1.0;
	this->transform_.rotate_.x -= FastRotateX;
	if (Acceleration_ <= 0)
	{
		Acceleration_ = 0.0f;
		PlayerState_ = S_IDLE;
	}

	//PlayerMove();

	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
	CharacterMoveRotate(move,this->transform_.rotate_.y);
}

void Player::UpdateWallHit()
{	
	KnockBack();
	CharacterGravity();

	if (KnockBack_Velocity_.x <= KnockBackEnd_ || KnockBack_Velocity_.z <= KnockBackEnd_)
	{
		PlayerState_ = S_IDLE;
		IsInvincibility_ = true;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			//pBattleScene->SetPlayerHp(CharacterLife_);
			pBattleScene->PlusEnemyScore();
		}
	}

	//if (--deadTimer_ < 0)
	//{
	//	//BossBattleScene* pBossBattleScene = (BossBattleScene*)FindObject("BossBattleScene");
	//	//pBossBattleScene->DeadCountPlus();

	//	CharacterLife_--;
	//	deadTimer_ = deadTimerValue;
	//	PlayerState_ = S_IDLE;
	//	IsInvincibility_ = true;

	//	SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
	//	if (pSM->IsBattleScene())
	//	{
	//		BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
	//		pBattleScene->SetPlayerHp(CharacterLife_);
	//	}
	//	else
	//	{

	//	}

	//	//SetStartPosition();
	//}
}

//void Player::PlayerMove()
//{
//	//プレイヤーのy回転をラジアン化して行列に
//	XMMATRIX playerRot = XMMatrixRotationY(XMConvertToRadians(this->transform_.rotate_.y));
//
//	//プレイヤーの進行方向ベクトルを作成
//	XMVECTOR PrevDir = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
//
//	//方向ベクトルを回転行列で変換
//	PrevDir = XMVector3TransformCoord(PrevDir, playerRot);
//
//	//単位ベクトル化し、移動方向を確定
//	XMVECTOR norm = XMVector3Normalize(PrevDir);
//
//	//移動ベクトル = 移動方向 * (初速度 + 加速度) * 移動量のスケーリング(60fpsのため0.016f)
//	//移動ベクトル化する
//	XMVECTOR MoveVector = XMVectorScale(norm, (Velocity_ + Acceleration_) * DeltaTime);
//
//	//現在位置と移動ベクトルを加算
//	XMVECTOR PrevPos = PlayerPosition_;
//	NewPos_ = PrevPos + MoveVector;
//
//	//場外でなければ位置更新 
//	XMFLOAT3 f;
//	XMStoreFloat3(&f, NewPos_);
//	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
//	{
//		XMStoreFloat3(&this->transform_.position_, NewPos_);
//	}
//}

void Player::Blown()
{
	this->transform_.rotate_.x += MoveRotateX;

	//毎フレーム速度を減少
	KnockBack_Velocity_.x *= DecelerationRate_;
	KnockBack_Velocity_.z *= DecelerationRate_;

	//位置 = 位置 + 方向 * 速度
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	TmpPos.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;

	NewPositon_ = XMLoadFloat3(&TmpPos);

	//場外でなければ位置更新 
	XMFLOAT3 f;
	XMStoreFloat3(&f, NewPositon_);
	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPositon_);
	}
}

void Player::CameraControl()
{
#ifdef _DEBUG
	//カメラを上部に移動
	if (Input::IsKeyDown(DIK_Q))
	{
		if (CameraState_ == S_NORMALCAMERA)
		{
			CameraState_ = S_DEBUGCAMERA;
		}
		else if (CameraState_ == S_DEBUGCAMERA)
		{
			CameraState_ = S_NORMALCAMERA;
			cameraTransform_.rotate_.x = 0.0f;
		}
	}
#endif 

	if(CameraState_ == S_NORMALCAMERA)
	{

		if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -0.7)	//カメラ左右移動
		{
			cameraTransform_.rotate_.y -= 2.5;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= 0.7)
		{
			cameraTransform_.rotate_.y += 2.5;
		}

		if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -0.7)	//カメラ上下移動
		{
			if (cameraTransform_.rotate_.x >= 60.0f)
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
			this->transform_.rotate_.y = 180;
		}
	}
	else if (CameraState_ == S_DEBUGCAMERA)
	{
		cameraTransform_.rotate_.x = 90.0f;
	}
	else if (CameraState_ == S_ROCKONCAMERA)
	{

	}

}

void Player::CameraUpdate()
{
	/*switch (CameraState_)
	{
	case Player::S_NORMALCAMERA:
		NormalCamera();
		break;
	case Player::S_DEBUGCAMERA:
		DebugCamera();
		break;
	case Player::S_ROCKONCAMERA:
		RockOnCamra();
		break;
	case Player::S_MAXCAMERA:
		break;
	default:
		break;
	}*/


	//--------------カメラ追従--------------
	
	//カメラが通常の時の処理

	//Enemy* pEnemy = (Enemy*)FindObject("Enemy");
	//XMFLOAT3 tmp = pEnemy->GetWorldPosition();

	CameraTarget_ = { this->transform_.position_ };//カメラの焦点は自機の位置
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform_.rotate_.y));//カメラの回転行列作成(Y軸)
	XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(cameraTransform_.rotate_.x));//カメラの回転行列作成(X軸)
	XMMATRIX rotCamera = XMMatrixMultiply(rotX, rotY);
	BackCamera_ = XMVector3TransformCoord(BackCamera_, rotCamera);//バックカメラのベクトルにかける
	XMStoreFloat3(&CameraPosition_, NewPositon_ + BackCamera_);//プレイヤーの移動ベクトルとバックカメラを加算

	//--------------カメラ振動--------------
	// 全ステート共有
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	Camera::SetPosition(CameraPosition_);//カメラの位置をセット 
	Camera::SetTarget(CameraTarget_);//カメラの焦点をセット
	BackCamera_ = { BackCameraPos };//バックカメラベクトルをリセット
}

void Player::NormalCamera()
{
}

void Player::DebugCamera()
{
	cameraTransform_.rotate_.x = 90.0f;
}

void Player::RockOnCamra()
{
}

void Player::EnemyReflect(XMVECTOR _vector, bool _IsAttack)
{
	
	if (!this->IsInvincibility_)
	{
	XMFLOAT3 f;
	XMStoreFloat3(&f, _vector);
	KnockBack_Direction_ = f;

	if (_IsAttack)
	{
		KnockBack_Velocity_.x = KnockBackPower_ * 1.5;
		KnockBack_Velocity_.z = KnockBackPower_ * 1.5;
	}
	else
	{
		KnockBack_Velocity_.x = KnockBackPower_;
		KnockBack_Velocity_.z = KnockBackPower_;
	}

	PlayerState_ = S_HIT;
	}
}

void Player::SetHitEffect()
{
	EmitterData hit;
	hit.textureFileName = "PaticleAssets\\flashB_W.png";
	hit.position = this->transform_.position_;
	hit.position.y = this->transform_.position_.y + 1.0f;
	hit.delay = 0;
	hit.direction = { 1,1,0 };
	hit.directionRnd = XMFLOAT3(90, 90, 90);
	hit.speed = 0.5;
	hit.speedRnd = 1.0;
	hit.accel = 1.0;
	hit.lifeTime = 10.0;
	hit.number = (DWORD)10;
	//hit.size = XMFLOAT2(0.1, 0.1);
	hit.sizeRnd = XMFLOAT2(0.4, 0.4);
	//hit.scale = XMFLOAT2(0.99, 0.99);
	//hit.color = XMFLOAT4(1, 1, 0.1, 1);
	//hit.deltaColor = XMFLOAT4(0, 0, 0, 0);
	//hit.gravity = 0.0f;
	VFX::Start(hit);


	/*Hitdata.textureFileName = "PaticleAssets\\flashB_W.png";
	Hitdata.position = this->transform_.position_;
	Hitdata.position.y = this->transform_.position_.y + 1.0f;
	Hitdata.direction = { 1,1,0 };
	Hitdata.directionRnd = { 360,360,0 };
	Hitdata.number = (DWORD)10;
	Hitdata.delay = 5;
	Hitdata.lifeTime = 10;
	hHitEmit_ = VFX::Start(Hitdata);
	HitEffectCount = 5;
	IsHitEffect = true;*/
}


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
	const float KeyBoardRotateY = 1.0f;
	int blinkTimer = 0;
	const int blink = 20;
	const float cameraShakeTime = 0.3f;
	const float jumpheight = 1.8f;

	const float cameraInitX = 0.0f;
	const float cameraInitY = 1.0f;
	const float cameraInitZ = -8.0f;

	const float cameraRotate = 2.5f;
	const float cameraUpperLimit = 60.0f;
	const float cameraLowerLimit = -10.0f;
	const float cameraDebugPos = 90.0f;
}

Player::Player(GameObject* parent) 
	: Character(parent,"Player"),
	hPlayer_(-1), /*hAttackArrow_(-1),*/ hShadow_(-1), hCollisionSound_(-1),
	PlayerState_(S_MAX),CameraState_(S_NORMALCAMERA),
	Direction_({ 0,0,0 }),BackCamera_(BackCameraPos)
{
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x + cameraInitX
		,this->transform_.position_.y + cameraInitY, this->transform_.position_.z + cameraInitZ };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };
}

Player::~Player()
{
	
}

void Player::Initialize()
{
	std::string path = "CSVdata\\PlayerData.csv";
	SetcsvStatus(path);

	hPlayer_ = Model::Load("Model\\chara2.fbx");
	assert(hPlayer_ >= 0);

	ShadowInit();

	hCollisionSound_ = Audio::Load("Sound\\maou_se_battle15.wav");
	assert(hCollisionSound_ >= 0);

	SetStartPosition();
	//ArrowTransform_.rotate_.y = 180.0f;
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),ColliderSize_);
	this->AddCollider(collider);
}

void Player::Update()
{
	cameraTransform_.position_ = this->transform_.position_;

	ShadowSet();

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

	if(!IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
	{
		if (IsOutsideStage(this->transform_.position_))
		{
			WallHit();
			PlayerState_ = S_WALLHIT;
		}
	}

	InvincibilityTimeCalclation();

	CharacterGravity();

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
	if (IsInvincibility_)
	{			
		if (++blinkTimer > blink) {

			blinkTimer = 0;	
			Model::SetTransform(hPlayer_, this->transform_);
			Model::Draw(hPlayer_);
		}
	}
	else
	{
		Model::SetTransform(hPlayer_, this->transform_);
		Model::Draw(hPlayer_);
	}

	ShadowDraw();

	/*if (PlayerState_ == S_CHARGE)
	{
		Model::SetTransform(hAttackArrow_, ArrowTransform_);
		Model::Draw(hAttackArrow_);
	}*/

#ifdef _DEBUG
	ImGui::Text("PositionX:%.3f", this->transform_.position_.x);
	ImGui::Text("PositionY:%.3f", this->transform_.position_.y);
	ImGui::Text("PositionZ:%.3f", this->transform_.position_.z);

	//ImGui::Text("shadow:%3f", shadow);

	//ImGui::Text("camera y :%.3f", CameraPosition_.y);
	//ImGui::Text("camera x :%.3f", CameraPosition_.x);
	

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

		//敵-自分のベクトル（相手の反射）,自分-敵のベクトル（自分の反射）をとる
		//相手のスピードと自分のスピードをとる
		XMFLOAT3 getposition =  pEnemy->GetPosition();
		XMVECTOR enemyvector = XMLoadFloat3(&getposition);
		XMVECTOR playervector = XMLoadFloat3(&this->transform_.position_);
		float enemyaccele = pEnemy->GetAcceleration();

		Reflect(playervector, enemyvector, this->Acceleration_, enemyaccele);

		PlayerState_ = S_HIT;
		SetHitEffect();

		//カメラ振動
		Camera::CameraShakeStart(cameraShakeTime);

		//衝撃音
		Audio::Play(hCollisionSound_);
	}

	if (pTarget->GetObjectName() == "Fence")
	{
		if(!IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallHit();
			PlayerState_ = S_WALLHIT;
		}
	}
}

void Player::UpdateIdle()
{
	//------------------キーボード入力の移動------------------//
	if (Input::IsKey(DIK_UP))
	{
		Direction_.z = 1.0;
		MoveRotate();
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z = -1.0;
		MoveRotateReverse();
	}
	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= KeyBoardRotateY;
		cameraTransform_.rotate_.y -= KeyBoardRotateY;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += KeyBoardRotateY;
		cameraTransform_.rotate_.y += KeyBoardRotateY;
	}

	KeyBoradMove();

	//------------------ゲームパッドスティックの移動------------------//

	//コントローラーを倒した方向・角度を取得
	XMVECTOR controller = XMVectorSet(Input::GetPadStickL().x, Input::GetPadStickL().y, Input::GetPadStickL().z, 0.0f);

	//コントローラで受けとったベクトルはXYなので
	//XZ方向のベクトルに直す
	XMFLOAT3 controllfloat = { XMVectorGetX(controller) , 0, XMVectorGetY(controller) };
	XMVECTOR SetController = { controllfloat.x, controllfloat.y , controllfloat.z };

	//ベクトルの長さを取得して、倒したかどうかを判別
	float length = XMVectorGetX(XMVector3Length(controller));

	if (length > stickMicrotilt)
	{
		//コントローラー方向と前向きベクトルの外積求める（）
		XMVECTOR cross = XMVector3Cross(SetController, FrontDirection_);

		//Y外積をとり+か-かで倒し回転方向を求める
		float crossY = XMVectorGetY(cross);

		//正面ベクトルとのラジアン角をとる
		XMVECTOR r = XMVector3AngleBetweenVectors(SetController, FrontDirection_);

		//ラジアン角度を取得
		float angle = XMVectorGetX(r);

		//ディグリー角に直す
		float angleDeg = XMConvertToDegrees(angle);

		// crossYの正負に応じて回転角度の符号を変える
		if (crossY > 0)
		{
			angleDeg = -angleDeg;
		}

		this->transform_.rotate_.y = angleDeg;
		cameraTransform_.rotate_.y = angleDeg;
		MoveRotate();
		CharacterMove(SetController);
	}

	//自分の前方ベクトル(回転した分も含む)を更新
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, FrontDirection_);


	//------------------攻撃状態へ移行------------------//

	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if (IsOnGround_)
		{
			PlayerState_ = S_CHARGE;
		}
	}

	//------------------ジャンプ------------------
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (IsOnGround_)
		{
			SetJump();
		}
	}

	CameraControl();

	Direction_ = { 0,0,0 };//最後に進行方向のリセット毎フレーム行う
}

void Player::UpdateHit()
{
	KnockBack();
	if (IsKnockBackEnd())
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
			AccelerationStop();
			SetJump();
			PlayerState_ = S_IDLE;
		}
	}

	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		PlayerState_ = S_ATTACK;
	}

	Charging();
	FastRotate();
	CameraControl();
}

void Player::UpdateAttack()
{
	SetAttackLocusEffect();

	Deceleration();
	Direction_.z = 1.0;
	FastRotate();
	if (IsDashStop())
	{
		AccelerationStop();
		PlayerState_ = S_IDLE;
	}

	KeyBoradMove();
}

void Player::UpdateWallHit()
{	
	KnockBack();

	if (IsKnockBackEnd())
	{
		PlayerState_ = S_IDLE;
		IsInvincibility_ = true;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			pBattleScene->PlusEnemyScore();
		}
	}
}

void Player::SetJump()
{
	IsOnGround_ = false;
	JumpSpeed_ = jumpheight;//一時的にy方向にマイナスされている値を大きくする
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

		if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -sticktilt)	//カメラ左右移動
		{
			cameraTransform_.rotate_.y -= cameraRotate;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= sticktilt)
		{
			cameraTransform_.rotate_.y += cameraRotate;
		}

		if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -sticktilt)	//カメラ上下移動
		{
			if (cameraTransform_.rotate_.x >= cameraUpperLimit)
			{
				cameraTransform_.rotate_.x = cameraUpperLimit;
			}
			else
			{
				cameraTransform_.rotate_.x += cameraRotate;
			}
		}
		if (Input::IsKey(DIK_S) || Input::GetPadStickR().y >= sticktilt)
		{
			if (cameraTransform_.rotate_.x <= cameraLowerLimit)
			{
				cameraTransform_.rotate_.x = cameraLowerLimit;
			}
			else
			{
				cameraTransform_.rotate_.x -= cameraRotate;
			}
		}

		if (Input::IsKey(DIK_Z) || Input::IsPadButton(XINPUT_GAMEPAD_Y))//カメラを正面に戻す（方向に変化なし）
		{
			cameraTransform_.rotate_.y = 0;
			cameraTransform_.rotate_.x = 0;
			this->transform_.rotate_.x = 0;
			this->transform_.rotate_.y = 0;
		}
	}
	else if (CameraState_ == S_DEBUGCAMERA)
	{
		cameraTransform_.rotate_.x = cameraDebugPos;
	}
}

void Player::CameraUpdate()
{
	//--------------カメラ追従--------------
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

void Player::KeyBoradMove()
{
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
	CharacterMoveRotate(move, this->transform_.rotate_.y);
}

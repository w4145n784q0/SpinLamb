#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"Engine/VFX.h"
#include"Engine/SceneManager.h"

#include"BattleScene.h"
#include"Enemy.h"

namespace {

	enum playeronlyIndex
	{
		i_backcameraX = 0,
		i_backcameraY,
		i_backcameraZ,
		i_keyboardrotateY,
		i_camerashaketime,
		i_jumpheight,
		i_camerainitx,
		i_camerainity,
		i_camerainitz,
		i_camerarotate,
		i_cameraupperlimit,
		i_cameralowerlimit,
		i_cameradebugPos,
	};

	XMVECTOR BackCameraPos = { 0.0f,0.0f,0.0f };//BackCameraの値は変わるが毎フレームこの値にする（値が変わり続けるのを防ぐ）
	float KeyBoardRotateY = 0.0f;
	
	float cameraShakeTime = 0.0f;
	float Jumpheight = 0.0f;

	float cameraInitX = 0.0f;
	float cameraInitY = 0.0f;
	float cameraInitZ = 0.0f;

	float cameraRotate = 0.0f;
	float cameraUpperLimit = 0.0f;
	float cameraLowerLimit = 0.0f;
	float cameraDebugPos = 0.0f;
}

Player::Player(GameObject* parent) 
	: Character(parent,"Player"),
	hPlayer_(-1),hShadow_(-1),
	PlayerState_(S_STOP),CameraState_(S_NORMALCAMERA),
	Direction_({ 0,0,0 }),BackCamera_({ 0,0,0 }), CameraPosition_({ 0,0,0 }), CameraTarget_({ 0,0,0 })
{
	
}

Player::~Player()
{

}

void Player::Initialize()
{
	std::string path = "CSVdata\\PlayerData.csv";
	SetcsvStatus(path);
	SetCSVPlayer();

	hPlayer_ = Model::Load("Model\\chara2.fbx");
	assert(hPlayer_ >= 0);

	ShadowInit();

	SetStartPosition();
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),HitParam_. ColliderSize_);
	this->AddCollider(collider);

	XMVECTOR cameraAdd = XMLoadFloat3(&this->transform_.position_);
	BackCamera_ = { BackCameraPos + cameraAdd };//バックカメラの初期値をセット
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x + cameraInitX
	,this->transform_.position_.y + cameraInitY, this->transform_.position_.z + cameraInitZ };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };
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
	case Player::S_CHARGE:
		UpdateCharge();
		break;
	case Player::S_ATTACK:
		UpdateAttack();
		break;
	case Player::S_HIT:
		UpdateHit();
		break;
	case Player::S_WALLHIT:
		UpdateWallHit();
		break;
	case Player::S_STOP:
		UpdateStop();
		break;
	default:
		break;
	}

	if(!(PlayerState_ == S_WALLHIT))
	{
		InvincibilityTimeCalclation();
	}

	CharacterGravity();

	//カメラの更新
	CameraUpdate();

#ifdef _DEBUG
	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}
#endif

}

void Player::Draw()
{
	DrawCharacterModel(hPlayer_, this->transform_);

	ShadowDraw();

#ifdef _DEBUG
	if (ImGui::TreeNode("PlayerStatus"))
	{
		if (ImGui::TreeNode("Position"))
		{
			ImGui::SliderFloat("PositionX:%.3f", &this->transform_.position_.x, WestEnd_, EastEnd_);
			ImGui::SliderFloat("PositionZ:%.3f", &this->transform_.position_.z, SouthEnd_, NorthEnd_);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Forward"))
		{
			XMFLOAT3 tmp;
			XMStoreFloat3(&tmp, MoveParam_.ForwardVector_);
			ImGui::Text("ForwardX:%.3f", tmp.x);
			ImGui::Text("ForwardY:%.3f", tmp.y);
			ImGui::Text("ForwardZ:%.3f", tmp.z);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Move"))
		{
			ImGui::InputFloat("velocity", &this->MoveParam_.Velocity_);
			ImGui::InputFloat("AcceleValue", &this->MoveParam_.AcceleValue_);
			ImGui::InputFloat("FullAccelerate", &this->MoveParam_.FullAccelerate_);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Rotate"))
		{
			ImGui::InputFloat("normalRotate", &this->RotateParam_.MoveRotateX);
			ImGui::InputFloat("fastRotate", &this->RotateParam_.FastRotateX);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Jump"))
		{
			ImGui::InputFloat("Gravity", &this->JumpParam_.Gravity_);
			ImGui::InputFloat("HeightLowerLimit", &this->JumpParam_.HeightLowerLimit_);
			ImGui::InputFloat("HeightUpperLimit", &this->JumpParam_.HeightUpperLimit_);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Hit"))
		{
			ImGui::InputFloat("Collider", &this->HitParam_.ColliderSize_);
			ImGui::InputFloat("OriginaRangeMin", &this->HitParam_.OriginaRangeMin_);
			ImGui::InputFloat("OriginaRangeMax", &this->HitParam_.OriginaRangeMax_);
			ImGui::InputFloat("ConvertedRangeMin", &this->HitParam_.ConvertedRangeMin_);
			ImGui::InputFloat("ConvertedRangeMax", &this->HitParam_.ConvertedRangeMax_);
			ImGui::InputFloat("DecelerationRate", &this->HitParam_.DecelerationRate_);
			ImGui::InputFloat("KnockBackEnd", &this->HitParam_.KnockBackEnd_);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("WallHit"))
		{
			ImGui::InputFloat("Collider", &this->WallHitParam_.KnockBackPower_);
			ImGui::InputInt("InvincibilityTime", &this->WallHitParam_.InvincibilityValue_);
			ImGui::InputInt("blinkValue", &this->WallHitParam_.blinkValue_);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Shadow"))
		{
			ImGui::InputFloat("Collider", &this->ShadowParam_.ShadowCorrection_);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	
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

		Reflect(playervector, enemyvector, this->MoveParam_.Acceleration_, enemyaccele);

		PlayerState_ = S_HIT;
		SetHitEffect();

		//カメラ振動
		Camera::CameraShakeStart(cameraShakeTime);

		//衝撃音
		Audio::Play(hSoundCollision_);
	}

	if (pTarget->GetObjectName() == "UpperWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.UpperNormal_);
			PlayerState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "LowerWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.LowerNormal_);
			PlayerState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "RightWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.RightNormal_);
			PlayerState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "LeftWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.LeftNormal_);
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
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += KeyBoardRotateY;
	}

	KeyBoradMove();

	//------------------ゲームパッドスティックの移動------------------//

	//コントローラーを倒した方向・角度を取得
	XMVECTOR controller = XMVectorSet(Input::GetPadStickL().x, Input::GetPadStickL().y, Input::GetPadStickL().z, 0.0f);

	//コントローラで受けとったベクトルはXYなので
	//XZ方向のベクトルに直す
	XMFLOAT3 controllfloat = { XMVectorGetX(controller) , 0.0f, XMVectorGetY(controller) };
	XMVECTOR SetController = { controllfloat.x, controllfloat.y , controllfloat.z };

	//ベクトルの長さを取得して、倒したかどうかを判別
	float length = XMVectorGetX(XMVector3Length(controller));

	if (length > StickMicroTilt)
	{
		//コントローラー方向と前向きベクトルの外積求める
		XMVECTOR cross = XMVector3Cross(SetController, InitParam_.FrontDirection_);

		//Y外積をとり+か-かで倒し回転方向を求める
		float crossY = XMVectorGetY(cross);

		//正面ベクトルとのラジアン角をとる
		XMVECTOR r = XMVector3AngleBetweenVectors(SetController, InitParam_.FrontDirection_);

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
		MoveRotate();
		CharacterMove(SetController);
	}

	//自分の前方ベクトル(回転した分も含む)を更新
	FrontVectorConfirm();


	//------------------攻撃状態へ移行------------------//

	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if (JumpParam_. IsOnGround_)
		{
			PlayerState_ = S_CHARGE;
		}
	}

	//------------------ジャンプ------------------
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (JumpParam_. IsOnGround_)
		{
			SetJump();
		}
	}

	CameraControl();
}

void Player::UpdateCharge()
{
	SetChargingEffect("PaticleAssets\\circle_B.png");

	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (JumpParam_. IsOnGround_)
		{
			AccelerationStop();
			SetJump();
			PlayerState_ = S_IDLE;
		}
	}

	if (Input::IsKey(DIK_LEFT) || Input::GetPadStickL().x < -StickTilt)
	{
		this->transform_.rotate_.y -= KeyBoardRotateY;
	}
	if (Input::IsKey(DIK_RIGHT) || Input::GetPadStickL().x > StickTilt)
	{
		this->transform_.rotate_.y += KeyBoardRotateY;
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
	Audio::Play(hSoundattack_);
	SetAttackLocusEffect();
	CharacterMove(MoveParam_.ForwardVector_);
	FrictionDeceleration();
	FastRotate();
	if (IsDashStop())
	{
		AccelerationStop();
		PlayerState_ = S_IDLE;
	}
}

void Player::UpdateHit()
{
	KnockBack();
	if (IsKnockBackEnd())
	{
		PlayerState_ = S_IDLE;
	}
}

void Player::UpdateWallHit()
{	
	KnockBack();

	if (IsKnockBackEnd())
	{
		PlayerState_ = S_IDLE;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			pBattleScene->PlusEnemyScore();
		}
	}
}

void Player::UpdateStop()
{
	//動かない
}

void Player::SetJump()
{
	JumpParam_. IsOnGround_ = false;
	JumpParam_. JumpSpeed_ = Jumpheight;//一時的にy方向にマイナスされている値を大きくする
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

		if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -StickTilt)	//カメラ左右移動
		{
			cameraTransform_.rotate_.y -= cameraRotate;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= StickTilt)
		{
			cameraTransform_.rotate_.y += cameraRotate;
		}

		if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -StickTilt)	//カメラ上下移動
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
		if (Input::IsKey(DIK_S) || Input::GetPadStickR().y >= StickTilt)
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
	XMStoreFloat3(&CameraPosition_, MoveParam_.NewPositon_ + BackCamera_);//プレイヤーの移動ベクトルとバックカメラを加算

	//--------------カメラ振動--------------
	// 全ステート共有
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	Camera::SetPosition(CameraPosition_);//カメラの位置をセット 
	Camera::SetTarget(CameraTarget_);//カメラの焦点をセット

	if (!(PlayerState_ == Player::S_STOP))
	{
		BackCamera_ = { BackCameraPos };//バックカメラベクトルをリセット
	}
	else
	{
		cameraTransform_.rotate_.y = 0;//開始前・終了後はカメラのY回転固定
	}
}

void Player::KeyBoradMove()
{
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
	CharacterMoveRotate(move, this->transform_.rotate_.y);
	Direction_ = { 0,0,0 };//最後に進行方向のリセット毎フレーム行う
}

void Player::SetCSVPlayer()
{
	CsvReader csv;
	csv.Load("CSVdata\\PlayerData.csv");

	std::string only = "PlayerOnlyParam";
	if (csv.IsGetParamName(only))
	{
		std::vector<float> v = csv.GetParam(only);
		BackCameraPos = { v[i_backcameraX], v[i_backcameraY], v[i_backcameraZ] };
		KeyBoardRotateY = v[i_keyboardrotateY];
		cameraShakeTime = v[i_camerashaketime];
		Jumpheight = v[i_jumpheight];
		cameraInitX = v[i_camerainitx];
		cameraInitY = v[i_camerainity];
		cameraInitZ = v[i_camerainitz];
		cameraRotate = v[i_camerarotate];
		cameraUpperLimit = v[i_cameraupperlimit];
		cameraLowerLimit = v[i_cameralowerlimit];
		cameraDebugPos = v[i_cameradebugPos];
	}
}

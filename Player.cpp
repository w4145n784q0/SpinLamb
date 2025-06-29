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

	//csv読み込み時のインデックス
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

	//カメラの固定位置
	//BackCameraの値を毎フレームこの値にする（値が変わり続けるのを防ぐ）
	XMVECTOR BackCameraPos = { 0.0f,0.0f,0.0f };

	//チャージ中の左右入力時の回転量
	float KeyBoardRotateY = 0.0f;
	
	//カメラの振動時間
	float cameraShakeTime = 0.0f;

	//ジャンプ時の一時的に代入する値
	float Jumpheight = 0.0f;

	//カメラの初期化位置
	XMFLOAT3 CameraInit = { 0,0,0 };

	//左右入力時のカメラの回転量
	float cameraRotate = 0.0f;

	//上下入力時のカメラの高さの最高値
	float cameraUpperLimit = 0.0f;

	//上下入力時のカメラの高さの最低値
	float cameraLowerLimit = 0.0f;

	//デバッグカメラ状態時のカメラの固定位置
	float cameraDebugPos = 0.0f;
}

Player::Player(GameObject* parent) 
	: Character(parent,"Player"),
	hPlayer_(-1), hArrow_(-1),
	PlayerState_(S_STOP),CameraState_(S_NORMALCAMERA),
	Direction_({ 0,0,0 }),BackCamera_({ 0,0,0 }), CameraPosition_({ 0,0,0 }), CameraTarget_({ 0,0,0 })
{
	
}

Player::~Player()
{

}

void Player::Initialize()
{
	//csvからパラメータ読み込み
	std::string path = "CSVdata\\PlayerData.csv";
	SetcsvStatus(path);

	//csvからパラメータ読み込み(Playerのみ使う情報)
	SetCSVPlayer();

	//各モデルの読み込み
	hPlayer_ = Model::Load("Model\\chara.fbx");
	assert(hPlayer_ >= 0);

	hArrow_ = Model::Load("Model\\AttackArrow2.fbx");
	assert(hArrow_ >= 0);

	InitArrow();
	SetStartPosition();
	
	//当たり判定付ける
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),HitParam_. ColliderSize_);
	this->AddCollider(collider);

	//バックカメラの初期値をセット 自分の位置に固定値を足す
	XMVECTOR cameraAdd = XMLoadFloat3(&this->transform_.position_);
	BackCamera_ = { BackCameraPos + cameraAdd };

	//カメラのトランスフォームを設定
	cameraTransform_ = this->transform_;

	//カメラの位置を設定 自分の位置に固定値を足す
	CameraPosition_ = { this->transform_.position_ + CameraInit };

	//カメラの焦点を設定
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };
}

void Player::Update()
{
	//カメラの位置を更新
	//cameraTransform_.position_ = this->transform_.position_;

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

	//柵に接触してないなら値を更新
	if(!(PlayerState_ == S_WALLHIT))
	{
		InvincibilityTimeCalclation();
	}

	//毎フレーム影の位置を更新
	ShadowSet();

	//毎フレーム重力をかけ続ける
	CharacterGravity();

	//毎フレームカメラの更新
	CameraUpdate();

#ifdef _DEBUG
	if (Input::IsKeyDown(DIK_RETURN))
	{
		SetStartPosition();
	}
#endif

}

void Player::Draw()
{
	DrawCharacterModel(hPlayer_, this->transform_);

	ShadowDraw();

	if (PlayerState_ == S_CHARGE)
	{
		Model::SetAndDraw(hArrow_, this->MoveParam_.ArrowTransform_);
	}

#ifdef _DEBUG
	if (ImGui::TreeNode("PlayerStatus"))
	{
		DrawCharacterImGui();
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

	if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
            for (const std::string& arr : WallHitParam_.WireArray_)
			{
				if (pTarget->GetObjectName() == arr)
				{
					XMVECTOR normal = HitNormal(arr);
					WallReflect(normal);
					PlayerState_ = S_WALLHIT;
				}
			}
		}
	}

}

void Player::UpdateIdle()
{
	//------------------キーボード入力の移動------------------//
	if (Input::IsKey(DIK_UP))
	{
		Direction_.z += 1.0f;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z -= 1.0f;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		Direction_.x -= 1.0f;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		Direction_.x += 1.0f;
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

	if (length > Input::StickMicroTilt)
	{
		this->transform_.rotate_.y = RotateDirectionVector(SetController);
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
	Charging();
	SetArrow();
	this->MoveParam_.ArrowTransform_.rotate_.y = this->transform_.rotate_.y;
	SetChargingEffect("PaticleAssets\\circle_B.png");

	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (JumpParam_. IsOnGround_)
		{
			ChargeReset();
			SetJump();
			PlayerState_ = S_IDLE;
		}
	}

	if (Input::IsKey(DIK_LEFT) || Input::GetPadStickL().x < -Input::StickTilt)
	{
		this->transform_.rotate_.y -= KeyBoardRotateY;
	}
	if (Input::IsKey(DIK_RIGHT) || Input::GetPadStickL().x > Input::StickTilt)
	{
		this->transform_.rotate_.y += KeyBoardRotateY;
	}

	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		ChargeRelease();
		PlayerState_ = S_ATTACK;
	}

	FrontVectorConfirm();
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
	JumpParam_.IsOnGround_ = false;
	JumpParam_.JumpSpeed_ = Jumpheight;//一時的にy方向にマイナスされている値を大きくする
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

		if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -Input::StickTilt)	//カメラ左右移動
		{
			cameraTransform_.rotate_.y -= cameraRotate;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= Input::StickTilt)
		{
			cameraTransform_.rotate_.y += cameraRotate;
		}

		if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -Input::StickTilt)	//カメラ上下移動
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
		if (Input::IsKey(DIK_S) || Input::GetPadStickR().y >= Input::StickTilt)
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
	XMVECTOR PlayerPosVec = XMLoadFloat3(&this->transform_.position_);//プレイヤーの位置を取得
	XMStoreFloat3(&CameraPosition_, PlayerPosVec + BackCamera_);//プレイヤーの移動ベクトルとバックカメラを加算

	//--------------カメラ振動--------------
	// 全ステート共有
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	Camera::SetPosition(CameraPosition_);//カメラの位置をセット 
	Camera::SetTarget(CameraTarget_);//カメラの焦点をセット

	//if (!(PlayerState_ == Player::S_STOP))
	//{
		BackCamera_ = { BackCameraPos };//バックカメラベクトルをリセット
	//}
	//else
	//{
	//	cameraTransform_.rotate_.y = 0;//開始前・終了後はカメラのY回転固定
	//}
}

void Player::KeyBoradMove()
{
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);

	if (!XMVector3Equal(move, XMVectorZero()))
	{
		move = XMVector3Normalize(move);
		this->transform_.rotate_.y = RotateDirectionVector(move);
		MoveRotate();
		CharacterMove(move);
	}

	//CharacterMoveRotate(move, this->transform_.rotate_.y);
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
		CameraInit = { v[i_camerainitx] ,v[i_camerainity] , v[i_camerainitz] };
		cameraRotate = v[i_camerarotate];
		cameraUpperLimit = v[i_cameraupperlimit];
		cameraLowerLimit = v[i_cameralowerlimit];
		cameraDebugPos = v[i_cameradebugPos];
	}
}

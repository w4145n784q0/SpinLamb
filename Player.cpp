#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"Enemy.h"

namespace {

	//csv読み込み時のインデックス
	enum playeronlyIndex
	{
		i_backcameraX = 0,
		i_backcameraY,
		i_backcameraZ,
		i_keyboardrotateY,
		i_movevalue,
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

	//キーボード入力時、Direction_に加算される値
	float MoveValue = 0.0f;

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
	hPlayer_(-1), ControllerID_(-1),
	PlayerState_(S_STOP),CameraState_(S_NORMALCAMERA),
	Direction_({ 0,0,0 }),BackCamera_({ 0,0,0 }), CameraPosition_({ 0,0,0 }), CameraTarget_({ 0,0,0 })
{
	
}

Player::~Player()
{

}

void Player::Initialize()
{
}

void Player::Update()
{
}

void Player::Draw()
{
	//Characterクラスの共通描画
	Character::Draw();

	//動かすキャラクターの描画
	DrawCharacterModel(hPlayer_, this->transform_);

	//チャージ中のみ矢印モデル描画
	if (PlayerState_ == S_CHARGE)
	{
		DrawArrow();
	}

}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	//敵クラス,プレイヤーと接触した時の処理
	if (pTarget->GetObjectName() == "Enemy" || pTarget->GetObjectName() == "Player1"
		|| pTarget->GetObjectName() == "Player2")
	{
		//接触したキャラクターの名前を取得
		std::string targetName = pTarget->GetObjectName();

		//当たった対象に応じた反射処理
		CollisionCharacter(targetName);

		//被弾状態になる
		PlayerState_ = S_HIT;

		//接触エフェクト
		SetHitEffect();

		//カメラ振動(短く)
		Camera::CameraShakeStart(Camera::GetShakeTimeShort());

		//衝撃音
		Audio::Play(hSoundCollision_);

		//状態遷移の際は一度x回転をストップ
		RotateXStop();
		RotateYStop();
	}

	//各柵に接触した時の処理
	if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		//自身が柵に接触状態ではない かつ無敵状態でないなら続ける
		if (!FenceHitParam_.IsInvincibility_ && !(PlayerState_ == S_FENCEHIT))
		{
			//柵の名前のいずれかに接触しているなら
            for (const std::string& arr : FenceHitParam_.WireArray_)
			{
				if (pTarget->GetObjectName() == arr)
				{
					//接触している柵の法線(反射される方向)を取得
					XMVECTOR normal = HitNormal(arr);

					//反射開始
					FenceReflect(normal);

					//プレイヤーの状態を柵に接触状態にする
					PlayerState_ = S_FENCEHIT;

					//カメラ振動(中くらいの長さ)
					Camera::CameraShakeStart(Camera::GetShakeTimeMiddle());

					//状態遷移の際は一度x回転をストップ
					RotateXStop();
					RotateYStop();
				}
			}
		}
	}

}

void Player::PlayerRun()
{
	//Characterクラスの共通処理
	Character::Update();

	//現在の状態によって更新を分ける
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
	case Player::S_FENCEHIT:
		UpdateFenceHit();
		break;
	case Player::S_STOP:
		UpdateStop();
		break;
	default:
		break;
	}

	//柵に接触状態でなければ無敵時間を更新
	if (!(PlayerState_ == S_FENCEHIT))
	{
		InvincibilityTimeCalculation();
	}

	//毎フレームカメラの更新
	CameraUpdate();

#ifdef _DEBUG

	//デバッグ中のみescキーで初期位置に戻る
	if (Input::IsKeyDown(DIK_RETURN))
	{
		SetStartPosition();
	}
#endif

}

void Player::UpdateIdle()
{
	//通常状態 移動・ジャンプなどをしている状態

	//------------------キーボード入力の移動------------------//

	//上下左右キーが押されたら各方向に移動量を加算
	if (Input::IsKey(DIK_UP))
	{
		Direction_.z += MoveValue;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z -= MoveValue;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		Direction_.x -= MoveValue;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		Direction_.x += MoveValue;
	}

	//キーボードの入力した分を実際に移動
	KeyBoardMove();

	//------------------ゲームパッドスティックの移動------------------//

	//コントローラー操作
	ControllerMove(ControllerID_);

	//------------------チャージ状態へ移行------------------//

	//SHIFTキー/Bボタンが押されたら
	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT)
		|| Input::IsPadButtonDown(XINPUT_GAMEPAD_B, ControllerID_))
	{
		if (JumpParam_.IsOnGround_)
		{
			//地上にいるならチャージ状態へ移行
			PlayerState_ = S_CHARGE;

			//状態遷移の際は一度x回転をストップ
			RotateXStop();
		}
	}

	//------------------ジャンプ------------------

	//SPACEキー/Aボタンが押されたら
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A,ControllerID_))
	{
		if (JumpParam_.IsOnGround_)
		{
			//地上にいるならジャンプ開始
			SetJump();
		}
	}

	//カメラ操作
	CameraControl();
}

void Player::UpdateCharge()
{
	//チャージ中(TmpAcceleを溜めている状態) その場で左右回転できるが動けない

	//加速度を溜める
	Charging();

	//矢印モデルをセット
	SetArrow();

	//矢印モデルの位置を自身の回転と合わせる
	this->MoveParam_.ArrowTransform_.rotate_.y = this->transform_.rotate_.y;
	
	//チャージ中のエフェクトを出す
	SetChargingEffect("ParticleAssets\\circle_B.png");

	//SPACEキー/Aボタンが押され,地上にいるなら
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, ControllerID_))
	{
		if (JumpParam_.IsOnGround_)
		{
			//溜めたチャージを0にする
			ChargeReset();

			//ジャンプ開始
			SetJump();

			//通常状態へ戻る
			PlayerState_ = S_IDLE;

			//状態遷移の際は一度x回転をストップ
			RotateXStop();
		}
	}

	//左右キー/スティックが倒されたら回転
	if (Input::IsKey(DIK_LEFT) || Input::GetPadStickL(ControllerID_).x < -Input::StickTilt)
	{
		this->transform_.rotate_.y -= KeyBoardRotateY;
	}
	if (Input::IsKey(DIK_RIGHT) || Input::GetPadStickL(ControllerID_).x > Input::StickTilt)
	{
		this->transform_.rotate_.y += KeyBoardRotateY;
	}

	//SHIFTキー/Bボタンを離したら
	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT)
		|| Input::IsPadButtonUp(XINPUT_GAMEPAD_B, ControllerID_))
	{
		//チャージ解放
		ChargeRelease();

		//攻撃状態へ移行
		PlayerState_ = S_ATTACK;

		//状態遷移の際は一度x回転をストップ
		RotateXStop();
	}

	//高速X回転
	FastRotateX();

	//カメラ操作
	CameraControl();
}

void Player::UpdateAttack()
{
	//攻撃状態 正面の方向に移動し操作不可

	//攻撃中のエフェクトを出す
	SetAttackLocusEffect();

	//正面ベクトルの方向に移動
	CharacterMove(MoveParam_.ForwardVector_);

	//摩擦量分速度を減少
	FrictionDeceleration();

	//高速X回転
	FastRotateX();

	//加速量が0になったら
	if (IsDashStop())
	{
		//明示的に加速量を0にする
		AccelerationStop();

		//通常状態へ戻る
		PlayerState_ = S_IDLE;

		//状態遷移の際は一度x回転をストップ
		RotateXStop();
	}

	//攻撃SE再生
	Audio::Play(hSoundattack_);
}

void Player::UpdateHit()
{
	//相手と接触した状態 操作不可

	//ノックバックする
	KnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (IsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		KnockBackVelocityReset();

		//通常状態へ戻る
		PlayerState_ = S_IDLE;

		//状態遷移の際は一度x回転をストップ
		//Y軸回転の停止はノックバックから復活する時のみ行う(攻撃やチャージへの干渉を防ぐため)
		RotateXStop();
		RotateYStop();

		//ダッシュ中の速度リセット(ノックバック終了時点でリセット)
		AccelerationStop();
	}
}

void Player::UpdateFenceHit()
{	
	//ダメージを受ける柵と接触した状態 操作不可

	//ノックバックする
	KnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (IsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		KnockBackVelocityReset();

		//通常状態へ戻る
		PlayerState_ = S_IDLE;

		//状態遷移の際は一度x回転をストップ
		//Y軸回転の停止はノックバックから復活する時のみ行う(攻撃やチャージへの干渉を防ぐため)
		RotateXStop();
		RotateYStop();

		//ダッシュ中の速度リセット(ノックバック終了時点でリセット)
		AccelerationStop();
	}
}

void Player::UpdateStop()
{
	//何も処理をしない
}

void Player::PlayerInit(std::string _CSVpath, std::string _Modelpath)
{
	//csvからパラメータ読み込み
	SetCSVStatus(_CSVpath);

	//csvからパラメータ読み込み(Playerのみ使う情報)
	SetCSVPlayer(_CSVpath);

	//矢印のトランスフォームの初期化
	InitArrow();

	//初期位置にキャラクターをセット
	SetStartPosition();

	//当たり判定付ける
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), HitParam_.ColliderSize_);
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

	//キャラクターモデルの読み込み
	hPlayer_ = Model::Load(_Modelpath);
	assert(hPlayer_ >= 0);
}

void Player::SetJump()
{
	//ジャンプを開始する処理

	//地上判定をfalseにする
	JumpParam_.IsOnGround_ = false;

	//一時的にy方向にマイナスされている値を大きくすることで、キャラクターが飛び上がる
	JumpParam_.JumpSpeed_ = Jumpheight;
}

void Player::CameraControl()
{
	//カメラを操作する

#ifdef _DEBUG
	//カメラを上部に移動

	//Qキーを押したらデバッグカメラに移行(Debugの時のみ)
	//再度押すと通常カメラに戻る
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

	//通常カメラ
	if(CameraState_ == S_NORMALCAMERA)
	{
		//A・Dキー/右スティックでカメラ回転
		if (Input::IsKey(DIK_A) || Input::GetPadStickR(ControllerID_).x <= -Input::StickTilt)
		{
			cameraTransform_.rotate_.y -= cameraRotate;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR(ControllerID_).x >= Input::StickTilt)
		{
			cameraTransform_.rotate_.y += cameraRotate;
		}

		//W・Sキーでカメラ上下移動
		if (Input::IsKey(DIK_W) || Input::GetPadStickR(ControllerID_).y <= -Input::StickTilt)
		{
			//カメラの上下移動の上限になったら位置固定
			if (cameraTransform_.rotate_.x >= cameraUpperLimit)
			{
				cameraTransform_.rotate_.x = cameraUpperLimit;
			}
			else
			{
				cameraTransform_.rotate_.x += cameraRotate;
			}
		}
		if (Input::IsKey(DIK_S) || Input::GetPadStickR(ControllerID_).y >= Input::StickTilt)
		{
			//カメラの上下移動の下限になったら位置固定
			if (cameraTransform_.rotate_.x <= cameraLowerLimit)
			{
				cameraTransform_.rotate_.x = cameraLowerLimit;
			}
			else
			{
				cameraTransform_.rotate_.x -= cameraRotate;
			}
		}

		//Zキー/X・Yボタンでカメラの回転リセット
		if (Input::IsKey(DIK_Z) ||
			Input::IsPadButton(XINPUT_GAMEPAD_Y, ControllerID_) || Input::IsPadButton(XINPUT_GAMEPAD_X, ControllerID_))
		{
			// カメラの回転をリセット
			cameraTransform_.rotate_.x = 0.0f;
			this->transform_.rotate_.x = 0.0f;
			
			// キャラクターのY回転をカメラのY回転に合わせる
			this->transform_.rotate_.y = cameraTransform_.rotate_.y;
		}
	}
	else if (CameraState_ == S_DEBUGCAMERA)
	{
		//デバッグカメラ中はカメラの回転位置を固定
		cameraTransform_.rotate_.x = cameraDebugPos;
	}
}

void Player::CameraUpdate()
{
	//カメラの位置・回転量の更新

	//--------------カメラ追従--------------

	//カメラの焦点は自機の位置
	CameraTarget_ = { this->transform_.position_ };

	//カメラの回転行列作成(X軸・Y軸)
	//CameraControl()で動かしたcameraTransform_をラジアン化し、回転行列にする
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform_.rotate_.y));
	XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(cameraTransform_.rotate_.x));
	
	//両方の回転行列を乗算
	XMMATRIX rotCamera = XMMatrixMultiply(rotX, rotY);

	//作成した両方の回転行列をバックカメラのベクトルに乗算
	BackCamera_ = XMVector3TransformCoord(BackCamera_, rotCamera);

	//プレイヤーの位置を取得
	XMVECTOR PlayerPosVec = XMLoadFloat3(&this->transform_.position_);	

	//プレイヤーの移動ベクトルとバックカメラを加算
	XMStoreFloat3(&CameraPosition_, PlayerPosVec + BackCamera_);

	//--------------カメラ振動--------------
	// 全ステート共有

	//カメラの振動量をカメラ位置に加算(振動量が0でも行う)
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	//バックカメラベクトルをリセット
	BackCamera_ = { BackCameraPos };
	
	//カメラの位置・焦点はGameViewから行う
}

void Player::KeyBoardMove()
{
	//キーボード入力時の移動・回転計算

	//キーボードを押した量を1つの仮の移動ベクトルに変換
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);

	//キーを押した(押してない状態は0ベクトルなので処理しない)
	if (!XMVector3Equal(move, XMVectorZero()))
	{
		//入力された分移動する
		PlayerMove(move);
	}

	//最後に進行方向のリセット毎フレーム行う
	Direction_ = { 0,0,0 };
}

void Player::ControllerMove(int _PadID)
{
	//コントローラーIDが初期化されていないなら処理しない
	if (_PadID < 0)
	{
		return;
	}

	//コントローラーを倒した方向・角度を取得
	XMVECTOR controller = XMVectorSet(Input::GetPadStickL(_PadID).x,
		Input::GetPadStickL(_PadID).y, Input::GetPadStickL(_PadID).z, 0.0f);

	//コントローラで受けとったベクトルはXYなので
	//XZ方向のベクトルに直す
	XMFLOAT3 controllfloat = { XMVectorGetX(controller) , 0.0f, XMVectorGetY(controller) };
	XMVECTOR SetController = { controllfloat.x, controllfloat.y , controllfloat.z };

	//ベクトルの長さを取得して、倒したかどうかを判別
	float length = XMVectorGetX(XMVector3Length(controller));

	//少しでもスティックを傾けたら
	if (length > Input::StickMicroTilt)
	{
		//入力された分移動する
		PlayerMove(SetController);
	}
}

void Player::PlayerMove(XMVECTOR _move)
{
	//キーボード・コントローラー入力時の移動・回転計算

	//移動方向ベクトルが0なら何もしない(0ベクトルを正規化はできない)
	if (XMVector3Equal(_move, XMVectorZero()))
	{
		return;
	}

	//移動方向ベクトルを正規化
	_move = XMVector3Normalize(_move);

	//カメラのY回転分だけ移動ベクトルを回転 
	//カメラのY軸回転量を行列にする
	float cameraY = cameraTransform_.rotate_.y;
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraY));

	//仮の移動ベクトルをカメラのY軸回転量で変形
	_move = XMVector3TransformCoord(_move, rotY);

	//コントローラー入力ベクトルからy軸回転量を計算
	this->transform_.rotate_.y = RotateDirectionVector(_move);

	//コントローラー入力ベクトルを渡し、実際に移動する
	CharacterMove(_move);

	//キャラクターをX回転
	MoveRotateX();
}

void Player::CollisionCharacter(std::string _name)
{
	//自身の位置をXMVECTOR型として先に保管する
	XMVECTOR PlayerVector = XMLoadFloat3(&this->transform_.position_);

	float TargetSpeed = 0.0f;
	XMFLOAT3 TargetPos = {};
	XMVECTOR TargetVector = {};

	if (_name == "Enemy")
	{
		//敵クラスのインスタンスを取得
		Enemy* pEnemy = (Enemy*)FindObject("Enemy");
		assert(pEnemy != nullptr);

		//敵の位置を取りXMVECTOR型にする
		TargetPos = pEnemy->GetPosition();
		TargetVector = XMLoadFloat3(&TargetPos);

		//相手のスピードを取得
		TargetSpeed = pEnemy->GetAcceleration();
	}
	else if (_name == "Player1" || _name == "Player2")
	{
		//プレイヤーのインスタンスを取得
		Player* pPlayer = (Player*)FindObject(_name);
		assert(pPlayer != nullptr);

		//プレイヤーの位置を取りXMVECTOR型にする
		TargetPos = pPlayer->GetPosition();
		TargetVector = XMLoadFloat3(&TargetPos);

		//相手のスピードを取得
		TargetSpeed = pPlayer->GetAcceleration();
	}
	else
	{
		return;
	}

	//反射処理を行う(自分の位置ベクトル,相手の位置ベクトル,自分の加速度,相手の加速度)
	Reflect(PlayerVector, TargetVector, this->MoveParam_.Acceleration_, TargetSpeed);
}

void Player::DrawImGui()
{
	//Debug中はImGuiを設定
#ifdef _DEBUG
	if (ImGui::TreeNode((objectName_ + " Status").c_str()))
	{
		DrawCharacterImGui();
	}

#endif
}

void Player::SetCSVPlayer(std::string _path)
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load(_path);

	//csvファイルの各0列目の文字列を取得
	std::string only = "PlayerOnlyParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> OnlyData = GetCSVReadData(csv, only);
		
	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	BackCameraPos = { OnlyData[i_backcameraX], OnlyData[i_backcameraY], OnlyData[i_backcameraZ] };
	KeyBoardRotateY = OnlyData[i_keyboardrotateY];
	MoveValue = OnlyData[i_movevalue];
	Jumpheight = OnlyData[i_jumpheight];
	CameraInit = { OnlyData[i_camerainitx] ,OnlyData[i_camerainity] , OnlyData[i_camerainitz] };
	cameraRotate = OnlyData[i_camerarotate];
	cameraUpperLimit = OnlyData[i_cameraupperlimit];
	cameraLowerLimit = OnlyData[i_cameralowerlimit];
	cameraDebugPos = OnlyData[i_cameradebugPos];
}

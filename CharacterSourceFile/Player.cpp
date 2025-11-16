#include "Player.h"
#include"../Engine/Model.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"
#include"../Engine/Camera.h"
#include"../Engine/SphereCollider.h"
#include"Enemy.h"
#include"../CharacterSourceFile/PlayerState/PlayerStateIdle.h"
#include"../CharacterSourceFile/PlayerState/PlayerStateCharge.h"
#include"../CharacterSourceFile/PlayerState/PlayerStateAttack.h"
#include"../CharacterSourceFile/PlayerState/PlayerStateHitStop.h"
#include"../CharacterSourceFile/PlayerState/PlayerStateHit.h"
#include"../CharacterSourceFile/PlayerState/PlayerStateFenceHit.h"
#include"../CharacterSourceFile/PlayerState/PlayerStateStop.h"

namespace {

	//csv読み込み時のインデックス
	enum PlayerOnlyIndex
	{
		i_BackCameraX = 0,
		i_BackCameraY,
		i_BackCameraZ,
		i_ChargeRotateY,
		i_MoveValue,
		i_CameraInitX,
		i_CameraInitY,
		i_CameraInitZ,
		i_CameraRotate,
		i_CameraUpperlimit,
		i_CameraLowerlimit,
		i_CameraDebugWidth,
		i_CameraDebugHeight,
		i_CameraDebugDepth,
	};

	//通常カメラの固定位置
	//BackCameraの値を毎フレームこの値にする（値が変わり続けるのを防ぐ）
	XMVECTOR BackCameraDefault = { 0.0f,0.0f,0.0f };

	//チャージ中の左右入力時の回転量
	float ChargeRotateY = 0.0f;

	//キーボード入力時、Direction_に加算される値
	float MoveValue = 0.0f;

	//カメラの初期化位置
	XMFLOAT3 CameraInit = { 0,0,0 };

	//左右入力時のカメラの回転量
	float CameraRotate = 0.0f;

	//上下入力時のカメラの高さの最高値
	float CameraUpperLimit = 0.0f;

	//上下入力時のカメラの高さの最低値
	float CameraLowerLimit = 0.0f;

	//デバッグカメラ状態時のカメラの位置
	XMFLOAT3 CameraDebugPos = { 0,0,0 };

	//着地硬直時間
	int JumpLandingTimer_ = 0;
}

Player::Player(GameObject* parent)
	: Character(parent, "Player"),
	hPlayer_(-1), PlayerInput_({ 0,0,0 }),
	CameraState_(S_NormalCamera),
	ControllerID_(-1), Direction_({ 0,0,0 }),
	CameraPosition_({ 0,0,0 }), CameraTarget_({ 0,0,0 }), BackCamera_({ 0,0,0 })
{

}

Player::~Player()
{

}

void Player::Initialize()
{
	//テーブルに各ステートを登録
	stateTable_[S_Idle]		= std::make_unique<PlayerStateIdle>();
	stateTable_[S_Charge]	= std::make_unique<PlayerStateCharge>();
	stateTable_[S_Attack]	= std::make_unique<PlayerStateAttack>();
	stateTable_[S_HitStop] = std::make_unique<PlayerStateHitStop>();
	stateTable_[S_Hit]		= std::make_unique<PlayerStateHit>();
	stateTable_[S_FenceHit] = std::make_unique<PlayerStateFenceHit>();
	stateTable_[S_Stop]		= std::make_unique<PlayerStateStop>();

	//最初のステートを登録
	ChangeState(S_Stop);
}

void Player::Update()
{
}

void Player::Draw()
{
	//Characterクラスの共通描画
	Character::Draw();

	//動かすキャラクターの描画
	modeldraw_->DrawCharacterModel(hPlayer_, this->transform_);

	//現在の状態によって描画を分ける(ステートパターン使用)
	//基底クラスのDrawは空なので、overrideしたステートのみDrawが呼ばれる
	if (currentState_) 
	{
		currentState_->Draw(this);
	}

}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	//敵クラス,プレイヤーと接触した時の処理
	if (pTarget->GetObjectName() == "Enemy1" || pTarget->GetObjectName() == "Enemy2"
		|| pTarget->GetObjectName() == "Player1" || pTarget->GetObjectName() == "Player2")
	{
		//ヒットストップ・被弾・柵に接触状態・無敵時間なら何もしない
		if (currentState_->IsHitStopState() || currentState_->isHitState() 
			|| currentState_->IsFenceHitState())
		{
			return;
		}

		//接触したキャラクターの名前を取得
		std::string targetName = pTarget->GetObjectName();

		//当たった対象に応じた反射処理
		CollisionCharacter(targetName);

		//被弾状態になる
		ChangeState(S_HitStop);

		//接触エフェクト
		vfx_->SetHitEffect(this->GetPosition());

		//カメラ振動(短く)
		Camera::CameraShakeStart(Camera::GetShakeTimeLong());

		//衝撃音
		Audio::Play(params_->SoundParam_.hSoundCollision_);

		//状態遷移の際は一度x回転をストップ
		rotate_->RotateXStop();

	}

	//各柵に接触した時の処理
	if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		//自身が柵に接触状態ではない かつ無敵状態でないなら続ける
		if (!params_->FenceHitParam_.IsInvincibility_ && !currentState_->IsFenceHitState())
		{
			//柵の名前のいずれかに接触しているなら
			for (const std::string& arr : params_->FenceHitParam_.WireArray_)
			{
				if (pTarget->GetObjectName() == arr)
				{
					//接触している柵の法線(反射される方向)を取得
					XMVECTOR normal = hit_->HitNormal(arr);

					//反射開始
					fence_->FenceReflect(normal);

					//自身のノックバック時のY軸回転角を固定させる
					hit_->KnockBackAngleY(params_->HitParam_.KnockBack_Direction_, params_->FenceHitParam_.KnockBackPower_);

					//プレイヤーの状態を柵に接触状態にする
					ChangeState(S_FenceHit);

					//カメラ振動(中くらいの長さ)
					Camera::CameraShakeStart(Camera::GetShakeTimeMiddle());

					//状態遷移の際は一度x回転をストップ
					rotate_->RotateXStop();
				}
			}
		}
	}

}

void Player::PlayerRun()
{
	//Characterクラスの共通処理
	Character::Update();

	//現在の状態によって更新を分ける(ステートパターン使用)
	if (currentState_) 
	{
		currentState_->Update(this);
	}

	//柵に接触状態でなければ無敵時間を更新
	if (currentState_ && currentState_->IsUpdateInvincibility())
	{
		fence_->InvincibilityTimeCalculation();
	}

	//毎フレームカメラの更新
	CameraUpdate();

#ifdef _DEBUG

	//デバッグ中のみ0キーで初期位置に戻る
	if (Input::IsKeyDown(DIK_0))
	{
		this->SetPosition(params_->InitParam_.StartPosition_);
	}
#endif

}

void Player::ChangeState(PlayerState newState)
{
	//現在のステートを終了させる
	if (currentState_) 
	{ 
		currentState_->Exit(this); 
	}

	//新しいステートに変更
	auto it = stateTable_.find(newState);
	if (it != stateTable_.end()) 
	{
		currentState_ = it->second.get();  //unique_ptrの中身の生ポインタを取得
	}
	else
	{
		//存在しないステート指定なら何もしない
		currentState_ = nullptr;
		return; 
	}

	//新しいステートを開始
	if (currentState_) 
	{
		currentState_->Enter(this);
	}
}

void Player::DrawImGui()
{
	//Debug中はImGuiを設定
#ifdef _DEBUG
	if (ImGui::TreeNode((objectName_ + " Status").c_str()))
	{
		//キャラクタークラス共通のImGui情報
		debugpanel_->DrawCharacterImGui();
	}

	if (ImGui::TreeNode((objectName_ + " OnlyStatus").c_str()))
	{
		if (ImGui::TreeNode("State"))
		{
			//現在のステート
			std::string state = currentState_->GetStateName();
			ImGui::Text("PlayerState:%s", state.c_str());
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("PlayerInit"))
		{
			//コントローラーID
			ImGui::Text("ControllerID:%1d", ControllerID_);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Camera"))
		{
			//カメラの最終的な位置
			ImGui::InputFloat("CameraPositionX", &CameraPosition_.x, ZeroPointOne);
			ImGui::InputFloat("CameraPositionY", &CameraPosition_.y, ZeroPointOne);
			ImGui::InputFloat("CameraPositionZ", &CameraPosition_.z, ZeroPointOne);

			//カメラの焦点
			ImGui::InputFloat("CameraTargetX", &CameraTarget_.x, ZeroPointOne);
			ImGui::InputFloat("CameraTargetY", &CameraTarget_.y, ZeroPointOne);
			ImGui::InputFloat("CameraTargetZ", &CameraTarget_.z, ZeroPointOne);

			//カメラの回転量
			ImGui::InputFloat("CameraTransform.rotateX", &cameraTransform_.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("CameraTransform.rotateY", &cameraTransform_.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("CameraTransform.rotateZ", &cameraTransform_.rotate_.z, ZeroPointOne);

			//カメラの後方位置
			XMFLOAT3 tmpBackCamera;
			XMStoreFloat3(&tmpBackCamera, BackCamera_);
			ImGui::InputFloat("BackCameraX", &tmpBackCamera.x, ZeroPointOne);
			ImGui::InputFloat("BackCameraY", &tmpBackCamera.y, ZeroPointOne);
			ImGui::InputFloat("BackCameraZ", &tmpBackCamera.z, ZeroPointOne);

			//カメラに毎フレーム代入する固定位置
			XMFLOAT3 tmpBackCameraDefault;
			XMStoreFloat3(&tmpBackCameraDefault, BackCameraDefault);
			ImGui::InputFloat("BackCameraDefaultX", &tmpBackCameraDefault.x, ZeroPointOne);
			ImGui::InputFloat("BackCameraDefaultY", &tmpBackCameraDefault.y, ZeroPointOne);
			ImGui::InputFloat("BackCameraDefaultZ", &tmpBackCameraDefault.z, ZeroPointOne);

			//デバッグカメラ時のカメラ位置
			ImGui::InputFloat("CameraDebugPosX", &CameraDebugPos.x, ZeroPointOne);
			ImGui::InputFloat("CameraDebugPosY", &CameraDebugPos.y, ZeroPointOne);
			ImGui::InputFloat("CameraDebugPosZ", &CameraDebugPos.z, ZeroPointOne);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Charge"))
		{
			//チャージ中にプレイヤーを回転させた際の1fごとの回転量
			ImGui::InputFloat("ChargeRotateY", &ChargeRotateY, ZeroPointOne);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

#endif
}

void Player::PlayerInit(std::string _CSVpath, std::string _Modelpath)
{
	//csvからパラメータ読み込み
	csvload_->SetCSVStatus(_CSVpath);

	//csvからパラメータ読み込み(Playerのみ使う情報)
	SetCSVPlayer(_CSVpath);

	//矢印のトランスフォームの初期化
	charge_->InitArrow();

	//初期位置にキャラクターをセット
	movement_->InitStartPosition();

	//当たり判定付ける
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), params_->HitParam_.ColliderSize_);
	this->AddCollider(collider);

	//バックカメラの初期値をセット 自分の位置に固定値を足す
	XMVECTOR cameraAdd = XMLoadFloat3(&this->transform_.position_);
	BackCamera_ = { BackCameraDefault + cameraAdd };

	//カメラのトランスフォームを設定
	cameraTransform_ = this->transform_;

	//カメラの位置を設定 自分の位置に固定値を足す
	CameraPosition_ = { this->transform_.position_ + CameraInit };

	//カメラの焦点を設定
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

	//キャラクターモデルの読み込み
	hPlayer_ = Model::Load(_Modelpath);
	assert(hPlayer_ >= 0);

	//ダッシュ用スプライト初期化
	movement_->InitImage();
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
		if (CameraState_ == S_NormalCamera)
		{
			CameraState_ = S_DebugCamera;
		}
		else if (CameraState_ == S_DebugCamera)
		{
			CameraState_ = S_NormalCamera;
			cameraTransform_.rotate_.x = 0.0f;
		}
	}
#endif 

	//通常カメラ
	if(CameraState_ == S_NormalCamera)
	{
		//A・Dキー/右スティックでカメラ回転
		if (Input::IsKey(DIK_A) || Input::GetPadStickR(ControllerID_).x <= -Input::StickTilt)
		{
			cameraTransform_.rotate_.y -= CameraRotate;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR(ControllerID_).x >= Input::StickTilt)
		{
			cameraTransform_.rotate_.y += CameraRotate;
		}

		//W・Sキーでカメラ上下移動
		if (Input::IsKey(DIK_W) || Input::GetPadStickR(ControllerID_).y <= -Input::StickTilt)
		{
			//カメラの上下移動の上限になったら位置固定
			if (cameraTransform_.rotate_.x >= CameraUpperLimit)
			{
				cameraTransform_.rotate_.x = CameraUpperLimit;
			}
			else
			{
				cameraTransform_.rotate_.x += CameraRotate;
			}
		}
		if (Input::IsKey(DIK_S) || Input::GetPadStickR(ControllerID_).y >= Input::StickTilt)
		{
			//カメラの上下移動の下限になったら位置固定
			if (cameraTransform_.rotate_.x <= CameraLowerLimit)
			{
				cameraTransform_.rotate_.x = CameraLowerLimit;
			}
			else
			{
				cameraTransform_.rotate_.x -= CameraRotate;
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
	else if (CameraState_ == S_DebugCamera)
	{
		//デバッグカメラ中は操作しない
	}
}

void Player::CameraUpdate()
{
	//カメラの位置・回転量の更新

	//--------------カメラ追従--------------

	//通常カメラの場合はプレイヤー後方にカメラをセット
	if (CameraState_ == S_NormalCamera)
	{
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
	}
	//デバッグカメラの場合は真上から見下ろすようにセット
	else if (CameraState_ == S_DebugCamera)
	{
		//カメラの位置をCameraUpdateの固定位置にセット
		CameraPosition_ = CameraDebugPos;

		//カメラの焦点をステージ中心(原点)にセット
		CameraTarget_ = { 0,0,0 };
	}

	//--------------カメラ振動--------------
	// 全ステート共有

	//カメラの振動量をカメラ位置に加算(振動量が0でも行う)
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	//バックカメラベクトルをリセット(DEBUGCAMERAでは使用しない)
	BackCamera_ = { BackCameraDefault };
	
	//カメラの位置・焦点のセットはGameViewから行う
}

void Player::InputKeyBoard()
{
	//キーボード入力時の移動・回転計算
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

	//キーボードを押した量を1つの仮の移動ベクトルに変換
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);

	//キーを押した(押してない状態は0ベクトルなので処理しない)
	if (!XMVector3Equal(move, XMVectorZero()))
	{
		//単位ベクトル化し、移動方向を確定
		params_->MoveParam_.MoveDirection_ = XMVector3Normalize(move);
	}

	//入力ベクトルを正規化して保存
	PlayerInput_ = XMVector3Normalize(move);

	//最後に進行方向のリセット毎フレーム行う
	Direction_ = { 0,0,0 };
}

void Player::InputCotroller(int _PadID)
{
	//コントローラーIDが0以下(シーン生成時に初期化されていない)なら処理しない
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

	//コントローラーを倒した(倒してない状態は0ベクトルなので処理しない)
	if (!XMVector3Equal(SetController, XMVectorZero()))
	{
		//単位ベクトル化し、移動方向を確定
		params_->MoveParam_.MoveDirection_ = XMVector3Normalize(SetController);

		//入力ベクトルを正規化して保存
		PlayerInput_ = XMVector3Normalize(SetController);
	}

}

void Player::PlayerRotate(XMVECTOR _move)
{
	//キーボード・コントローラー入力時の回転計算

	//移動方向ベクトルが0なら何もしない(0ベクトルを正規化はできない)
	if (!XMVector3Equal(_move, XMVectorZero()))
	{
		//移動方向ベクトルを正規化
		_move = XMVector3Normalize(_move);
	}

	//カメラのY回転分だけ移動ベクトルを回転
	_move = ConvertCameraDirection(_move);

	if (!XMVector3Equal(_move, XMVectorZero()))
	{
		//コントローラー入力ベクトルからy軸回転量を計算
		this->transform_.rotate_.y = rotate_->RotateDirectionVector(_move);
	}

}

XMVECTOR Player::ConvertCameraDirection(XMVECTOR _input)
{
	//入力がゼロベクトルなら変換せずそのまま返す
	if (XMVector3Equal(_input, XMVectorZero()))
	{
		return XMVectorZero();
	}

	//カメラのY軸回転を取得し、回転行列を作成
	float cameraY = cameraTransform_.rotate_.y;
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraY));

	//入力ベクトルをカメラのY軸回転で変換
	return XMVector3TransformCoord(_input, rotY);

}

void Player::CollisionCharacter(std::string _name)
{
	//自身の位置をXMVECTOR型として先に保管する
	XMVECTOR PlayerVector = XMLoadFloat3(&this->transform_.position_);

	//接触相手の加速量を保管する変数を用意
	float TargetSpeed = 0.0f;

	//接触相手の位置と変換用のXMVECTORを用意
	XMFLOAT3 TargetPos = {};
	XMVECTOR TargetVector = {};

	//接触した相手の名前を取得する文字列を用意
	std::string TargetName = "";

	if (_name == "Enemy1" || _name == "Enemy2")
	{
		//敵クラスのインスタンスを取得
		Enemy* pEnemy = (Enemy*)FindObject(_name);
		assert(pEnemy != nullptr);

		//敵の位置を取りXMVECTOR型にする
		TargetPos = pEnemy->GetPosition();
		TargetVector = XMLoadFloat3(&TargetPos);

		//相手のスピードを取得
		TargetSpeed = pEnemy->GetParams()->GetAcceleration();

		//相手の名前を取得
		TargetName = pEnemy->GetObjectName();
	
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
		TargetSpeed = pPlayer->GetParams()->GetAcceleration();

		//相手の名前を取得
		TargetName = _name;
	}
	else
	{
		return;
	}

	//反射処理を行う(自分の位置ベクトル,相手の位置ベクトル,自分の加速度,相手の加速度,接触相手の名前)
	hit_->Reflect(PlayerVector, TargetVector, params_->MoveParam_.CommonAcceleration_, TargetSpeed, TargetName);
}

bool Player::IsDamage()
{
	//ヒットストップ・被弾・柵に接触状態・無敵時間なら何もしない
	if (currentState_->IsHitStopState() || currentState_->isHitState()
		|| currentState_->IsFenceHitState() || params_->GetIsInvincibility())
	{
		return true;
	}
	else
	{
		return false;
	}
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
	BackCameraDefault = { OnlyData[i_BackCameraX], OnlyData[i_BackCameraY], OnlyData[i_BackCameraZ] };
	ChargeRotateY = OnlyData[i_ChargeRotateY];
	MoveValue = OnlyData[i_MoveValue];
	CameraInit = { OnlyData[i_CameraInitX] ,OnlyData[i_CameraInitY] , OnlyData[i_CameraInitZ] };
	CameraRotate = OnlyData[i_CameraRotate];
	CameraUpperLimit = OnlyData[i_CameraUpperlimit];
	CameraLowerLimit = OnlyData[i_CameraLowerlimit];
	CameraDebugPos = { OnlyData[i_CameraDebugWidth],OnlyData[i_CameraDebugHeight],OnlyData[i_CameraDebugDepth] };
}

void Player::SetChargeRotateY(float _rotate)
{
	ChargeRotateY = _rotate;
}

float Player::GetChargeRotateY() const
{
	return ChargeRotateY;
}

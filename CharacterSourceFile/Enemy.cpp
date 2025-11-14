#include "Enemy.h"

#include"../Engine/Model.h"
#include"../Engine/Camera.h"
#include"../Engine/SphereCollider.h"

#include"../CharacterSourceFile/EnemyState/EnemyStateRoot.h"
#include"../CharacterSourceFile/EnemyState/EnemyStateApproach.h"
#include"../CharacterSourceFile/EnemyState/EnemyStateAim.h"
#include"../CharacterSourceFile/EnemyState/EnemyStateAttack.h"
#include"../CharacterSourceFile/EnemyState/EnemyStateHitStop.h"
#include"../CharacterSourceFile/EnemyState/EnemyStateHit.h"
#include"../CharacterSourceFile/EnemyState/EnemyStateFenceHit.h"
#include"../CharacterSourceFile/EnemyState/EnemyStateWait.h"
#include"../CharacterSourceFile/EnemyState/EnemyStateLook.h"
#include"../CharacterSourceFile/EnemyState/EnemyStateStop.h"


namespace
{
	//csv読み込み時のインデックス(Enemyクラスの固有の変数)
	enum EnemyOnlyIndex
	{
		i_ChaseLength= 0,
		i_LookRotateAngle,
		i_LookRotateValue,
		i_EnemyAttackTime_1,
		i_EnemyAttackTime_2,
		i_EnemyAttackTime_3,
		i_EnemyAttackTime_4,
	};

	//追跡状態から攻撃準備に移る距離
	float ChaseLength = 0.0f;

	//敵がプレイヤーの方向を向く際のトリガー この値を超えたら回転
	float LookRotateAngle = 0;
	
	//プレイヤー方向を向く際の1fごとの回転量
	float LookRotateValue = 0.0f;

	//敵が攻撃するまでの時間の配列　ランダムに選ばれる
	std::vector<int> EnemyAttackTimeArray = {0,0,0,0};

	//敵が攻撃・移動する方向 LookPlayerで指定する
	XMVECTOR AutoAttackDirection = { 0,0,0 };
}

Enemy::Enemy(GameObject* parent)
	:Character(parent, "Enemy"), hEnemy_(-1), pPlayer_(nullptr),
	AimTimer_(0), RandomAim_(0), 
	TargetVec_({0,0,0}), TargetPosition_({0,0,0}), TargetAcceleration_(0.0f),TargetName_("")
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	//テーブルに各ステートを登録
	stateTable_[S_Root]		= std::make_unique<EnemyStateRoot>();
	stateTable_[S_Approach]	= std::make_unique<EnemyStateApproach>();
	stateTable_[S_Aim]		= std::make_unique<EnemyStateAim>();
	stateTable_[S_Attack]	= std::make_unique<EnemyStateAttack>();
	stateTable_[S_HitStop]  = std::make_unique<EnemyStateHitStop>();
	stateTable_[S_Hit]		= std::make_unique<EnemyStateHit>();
	stateTable_[S_FenceHit] = std::make_unique<EnemyStateFenceHit>();
	stateTable_[S_Wait]     = std::make_unique<EnemyStateWait>();
	stateTable_[S_Look]     = std::make_unique<EnemyStateLook>();
	stateTable_[S_Stop]		= std::make_unique<EnemyStateStop>();

	//最初のステートを登録
	ChangeState(S_Stop);

	//csvからパラメータ読み込み
	std::string path = "CSVdata\\CharacterData\\EnemyData.csv";
	csvload_->SetCSVStatus(path);

	//csvからパラメータ読み込み(Enemyのみ使う情報)
	SetCSVEnemy();

	//各モデルの読み込み
	hEnemy_ = Model::Load("Model\\Character_black.fbx");
	assert(hEnemy_ >= 0);

	//矢印のトランスフォームの初期化
	charge_->InitArrow();

	//初期位置にキャラクターをセット
	movement_->InitStartPosition();

	//当たり判定付ける
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), params_->HitParam_.ColliderSize_);
	this->AddCollider(collision);

	
	//敵の攻撃時間配列の添え字をランダムに設定
	RandomAimReLottery();
}

void Enemy::Update()
{

}

void Enemy::Draw()
{
	//Characterクラスの共通描画
	Character::Draw();

	//動かすキャラクターの描画
	modeldraw_->DrawCharacterModel(hEnemy_, this->transform_);

	//現在の状態によって描画を分ける(ステートパターン使用)
	//現状はチャージ中のみ使用(追加する場合は各ステートからDrawをoverrideする)
	if (currentState_)
	{
		currentState_->Draw(this);
	}
}

void Enemy::Release()
{
	//実体は消さず、アドレスのみ無効化する
	if (pPlayer_ != nullptr)
	{
		pPlayer_ = nullptr;
	}
}

void Enemy::OnCollision(GameObject* pTarget)
{
	//プレイヤーと接触した時の処理
	if (pTarget->GetObjectName() == "Player1" || pTarget->GetObjectName() == "Player2")
	{
		//ヒットストップ・被弾・柵に接触状態・無敵時間なら何もしない
		if (currentState_->IsHitStopState() || currentState_->isHitState()
			|| currentState_->IsFenceHitState()) 
		{
			return;
		}

		//自身の位置をXMVECTOR型にする
		XMVECTOR MyVector = XMLoadFloat3(&this->transform_.position_);

		//相手の位置を取得、XMVECTOR型にする
		XMFLOAT3 TargetPos = TargetPosition_;
		XMVECTOR TargetVector = XMLoadFloat3(&TargetPosition_);

		//反射処理を行う(自分の位置ベクトル,相手の位置ベクトル,自分の加速度,相手の加速度,接触相手の名前)
		hit_->Reflect(MyVector, TargetVector, params_->MoveParam_.CommonAcceleration_, TargetAcceleration_, TargetName_);
		
		//接触時点で攻撃までのタイマーをリセット
		AimTimer_ = 0;

		//被弾状態になる
		ChangeState(S_HitStop);

		//状態遷移の際は一度回転をストップ
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

					//接触時点で攻撃までのタイマーをリセット
					AimTimer_ = 0;

					//CPUの状態を柵に接触状態にする
					ChangeState(S_FenceHit);

					//カメラ振動
					Camera::CameraShakeStart(Camera::GetShakeTimeMiddle());

					//状態遷移の際は一度回転をストップ
					rotate_->RotateXStop();

				}
			}
		}
	}
}

void Enemy::EnemyRun()
{
	//プレイヤーの位置（ワールド座標）
	TargetPosition_ = pPlayer_->GetWorldPosition();

	//プレイヤーの位置をベクトル化し取り続ける
	TargetVec_ = XMLoadFloat3(&TargetPosition_);

	//プレイヤーの加速度を取り続ける
	TargetAcceleration_ = pPlayer_->GetParams()->GetAcceleration();

	//プレイヤーのオブジェクト名を取り続ける
	TargetName_ = pPlayer_->GetObjectName();

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
}

void Enemy::ChangeState(EnemyState newState)
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

void Enemy::DrawImGui()
{
	//Debug中はImGuiを設定
#ifdef _DEBUG
	if (ImGui::TreeNode("EnemyStatus"))
	{
		//キャラクタークラス共通のImGui情報
		debugpanel_->DrawCharacterImGui();
	}

	if (ImGui::TreeNode("EnemyOnlyStatus"))
	{
		if (ImGui::TreeNode("State"))
		{
			//現在のステート
			std::string state = currentState_->GetStateName();
			ImGui::Text("EnemyState:%s", state.c_str());
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Chase"))
		{
			//プレイヤーとの距離
			float dist = PlayerEnemyDistanceX();
			ImGui::Text("PlayerDistance:%.3f", dist);

			//攻撃準備状態(UpdateAim)に入るのに必要な距離
			ImGui::InputFloat("ChaseLength", &ChaseLength, ZeroPointOne);

			//プレイヤーを向く方向
			XMFLOAT3 AutoDir;
			XMStoreFloat3(&AutoDir, AutoAttackDirection);
			ImGui::Text("AutoAttackDirectionX:%.3f", AutoDir.x);
			ImGui::Text("AutoAttackDirectionY:%.3f", AutoDir.y);
			ImGui::Text("AutoAttackDirectionZ:%.3f", AutoDir.z);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Aim"))
		{
			//攻撃までの時間(定数)
			ImGui::Text("EnemyAttackTime:%3d", EnemyAttackTimeArray[RandomAim_]);

			//攻撃までの時間(残り何フレームか)
			ImGui::Text("AimTimer:%3d", AimTimer_);

			ImGui::TreePop();
		}

		//デバッグ用のEnemyState_切り替えボタン
		if (ImGui::Button("EnemyStop"))
		{
			ChangeState(S_Stop);
		}
		if (ImGui::Button("EnemyMove"))
		{
			ChangeState(S_Root);
		}

		ImGui::TreePop();
	}

#endif
}

void Enemy::LookPlayer()
{
	//Playerの方に回転する処理

	//------------敵と自機の回転処理------------

	//プレイヤーの位置(ベクトル)から敵の位置(ベクトル)を引く
	XMVECTOR enemyVector = XMLoadFloat3(&this->transform_.position_);
	XMVECTOR PlayerDist = XMVectorSubtract(TargetVec_ ,enemyVector);
	
	//回転する方向を設定(初期化)
	XMVECTOR RotateDirection = XMVectorZero();

	//プレイヤーの位置から自身の位置を引いた距離が0ベクトルなら何もしない(0ベクトルを正規化はできない)
	if(XMVector3Equal(PlayerDist, XMVectorZero()))
	{
		return;
	}
	
	//距離を単位ベクトル化し、移動方向として扱えるようにする
	RotateDirection = XMVector3Normalize(PlayerDist);

	//敵の移動方向を保管
	AutoAttackDirection = RotateDirection;

	//------------角度に応じて回転------------

	//二つのベクトル間のラジアン角を求める
	XMVECTOR angle = XMVector3AngleBetweenVectors(RotateDirection, params_->MoveParam_.ForwardVector_);

	//前向きベクトルとプレイヤーのいる方向のベクトルの外積を求める
	XMVECTOR cross = XMVector3Cross(params_->MoveParam_.ForwardVector_, RotateDirection);

	//前向きベクトルとプレイヤー方向ベクトルはXZに伸びるベクトルなので
	//外積のY軸（+か-で左右どちらにいるか判断）を求める
	float crossY = XMVectorGetY(cross);

	//二つのベクトル間のラジアン角を度数に戻し
	//一定以上開いているなら回転
	float Dig = XMConvertToDegrees(XMVectorGetX(angle));
	if (Dig > LookRotateAngle)
	{
		//外積Yが0以上なら左周り(反時計周り)
		if (crossY > 0.0)
		{
			transform_.rotate_.y -= LookRotateValue;
		}
		//外積Yが0以下なら右周り(時計周り)
		else if (crossY < 0.0)
		{
			transform_.rotate_.y += LookRotateValue;
		}
	}

	//計算を確定
	transform_.Calculation();
}

void Enemy::RotateFromDirection(XMVECTOR _direction)
{
	//移動方向が0ベクトルなら何もしない(0ベクトルを正規化はできない)
	if (XMVector3Equal(_direction, XMVectorZero()))
	{
		return;
	}

	//移動方向を正規化
	XMVECTOR moveDir = XMVector3Normalize(_direction);

	//XMFLOAT3に変換
	XMFLOAT3 dir = { 0,0,0 };
	XMStoreFloat3(&dir, moveDir);

	//XZ平面上のベクトルからY軸回転角を求める
	float angleY = static_cast<float>(atan2(dir.x, dir.z));

	//angleYはラジアン角なのでディグリー角に変換し、Y軸回転にセット
	transform_.rotate_.y = XMConvertToDegrees(angleY);
}

float Enemy::PlayerEnemyDistanceX()
{
	//Playerの位置ベクトル-自身の位置ベクトルの長さを計算する

	//自身の位置をベクトルにする
	XMVECTOR EnemyPosition = XMLoadFloat3(&this->transform_.position_);

	//Playerの位置ベクトル-自身の位置ベクトルを計算
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition, TargetVec_);

	//長さを取得し返す
	float tmp = XMVectorGetX(XMVector3Length(DistVec));
	return tmp;
}

bool Enemy::IsNearChaseLength(float _length)
{
	//追跡距離に近いか判定
	if (_length < ChaseLength) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Enemy::AimTimerAdd()
{
	//狙って攻撃までのタイマーを増加
	AimTimer_++;
}

void Enemy::AimTimerReset()
{
	//狙って攻撃までのタイマーをリセット
	AimTimer_ = 0;
}

bool Enemy::IsTimeOverAttackTime()
{
	//AimTimer_が攻撃までの時間を経過したか返す（配列中のランダムな時間）
	if (AimTimer_ > EnemyAttackTimeArray[RandomAim_])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Enemy::RandomAimReLottery()
{
	//攻撃までの時間を再抽選
	//攻撃終了後などに呼び出す
	RandomAim_ = rand() % EnemyAttackTimeArray.size();
}

bool Enemy::IsAttackDecision()
{
	//プレイヤーがダメージ状態(被弾・柵に衝突・無敵時間かどうか)を取得
	//いずれかに該当するならfalse(攻撃をしない)
	if (pPlayer_ != nullptr)
	{
		if (pPlayer_->IsDamage())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

void Enemy::SetCSVEnemy()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVdata\\CharacterData\\EnemyData.csv");

	//csvファイルの各0列目の文字列を取得
	std::string only = "EnemyOnlyParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> OnlyData = GetCSVReadData(csv, only);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	ChaseLength = OnlyData[i_ChaseLength];
	LookRotateAngle = OnlyData[i_LookRotateAngle];
	LookRotateValue = OnlyData[i_LookRotateValue];
	
	int arr[] = { static_cast<int>(OnlyData[i_EnemyAttackTime_1]), static_cast<int>(OnlyData[i_EnemyAttackTime_2]),
			static_cast<int>(OnlyData[i_EnemyAttackTime_3]), static_cast<int>(OnlyData[i_EnemyAttackTime_4]) };
	for (int i = 0; i < EnemyAttackTimeArray.size(); i++)
	{
		EnemyAttackTimeArray[i] = arr[i];
	}
}

void Enemy::SetChaseLength(float _chaselength)
{
	ChaseLength = _chaselength;
}

float Enemy::GetChaseLength() const
{
	return ChaseLength;
}

void Enemy::SetAutoAttackDirection(XMVECTOR _AutoAttackDirection)
{
	AutoAttackDirection = _AutoAttackDirection;
}

XMVECTOR Enemy::GetAutoAttackDirection() const
{
	return AutoAttackDirection;
}

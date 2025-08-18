#include "Enemy.h"

#include"../Engine/Model.h"
#include"../Engine/Camera.h"
#include"../Engine/SphereCollider.h"
#include"../Engine/Audio.h"



namespace
{
	//csv読み込み時のインデックス(Enemyクラスの固有の変数)
	enum EnemyOnlyIndex
	{
		i_HitStop = 0,
		i_ChaseLength,
		i_LookRotateAngle,
		i_LookRotateValue,
		i_EnemyAttackTime_1,
		i_EnemyAttackTime_2,
		i_EnemyAttackTime_3,
		i_EnemyAttackTime_4,
	};

	//ヒットストップする時間
	int HitStop = 0;

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
	:Character(parent,"Enemy"), hEnemy_(-1),pPlayer_(nullptr),
	EnemyState_(S_Stop),AimTimer_(0), 
	TargetVec_({0,0,0}), TargetPosition_({0,0,0}), TargetAcceleration_(0.0f),
	RandomAim_(0), HitStopTimer_(0)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	//csvからパラメータ読み込み
	std::string path = "CSVdata\\CharacterData\\EnemyData.csv";
	SetCSVStatus(path);

	//csvからパラメータ読み込み(Enemyのみ使う情報)
	SetCSVEnemy();

	//各モデルの読み込み
	hEnemy_ = Model::Load("Model\\Character_black.fbx");
	assert(hEnemy_ >= 0);


	//矢印のトランスフォームの初期化
	InitArrow();

	//初期位置にキャラクターをセット
	SetStartPosition();

	//当たり判定付ける
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), HitParam_.ColliderSize_);
	this->AddCollider(collision);

	
	//敵の攻撃時間配列の添え字をランダムに設定
	RandomAim_ = rand() % EnemyAttackTimeArray.size();
}

void Enemy::Update()
{

}

void Enemy::Draw()
{
	//Characterクラスの共通描画
	Character::Draw();

	//動かすキャラクターの描画
	DrawCharacterModel(hEnemy_, this->transform_);

	//チャージ中のみ矢印モデル描画
	if (EnemyState_ == S_Aim)
	{
		DrawArrow();
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
	if (pTarget->GetObjectName() == "Player1")
	{
		//自身の位置をXMVECTOR型にする
		XMVECTOR MyVector = XMLoadFloat3(&this->transform_.position_);

		//相手の位置を取得、XMVECTOR型にする
		XMFLOAT3 TargetPos = TargetPosition_;
		XMVECTOR TargetVector = XMLoadFloat3(&TargetPosition_);

		
		//反射処理を行う(自分の位置ベクトル,相手の位置ベクトル,自分の加速度,相手の加速度)
		Reflect(MyVector, TargetVector, this->MoveParam_.Acceleration_, TargetAcceleration_);
		
		//接触時点で攻撃までのタイマーをリセット
		AimTimer_ = 0;

		//被弾状態になる
		EnemyState_ = S_Hit;

		//状態遷移の際は一度回転をストップ
		RotateXStop();
	}

	//各柵に接触した時の処理
	if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		//自身が柵に接触状態ではない かつ無敵状態でないなら続ける
		if (!FenceHitParam_.IsInvincibility_ && !(EnemyState_ == S_FenceHit))
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

					//CPUの状態を柵に接触状態にする
					EnemyState_ = S_FenceHit;

					//カメラ振動
					Camera::CameraShakeStart(Camera::GetShakeTimeMiddle());

					//状態遷移の際は一度回転をストップ
					RotateXStop();

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
	TargetAcceleration_ = pPlayer_->GetAcceleration();

	//Characterクラスの共通処理
	Character::Update();

	//現在の状態によって更新を分ける
	switch (EnemyState_)
	{
	case Enemy::S_Root:
		UpdateRoot();
		break;
	case Enemy::S_Chase:
		UpdateChase();
		break;
	case Enemy::S_Aim:
		UpdateAim();
		break;
	case Enemy::S_Attack:
		UpdateAttack();
		break;
	case Enemy::S_HitStop:
		UpdateHitStop();
		break;
	case Enemy::S_Hit:
		UpdateHit();
		break;
	case Enemy::S_FenceHit:
		UpdateFenceHit();
		break;
	case Enemy::S_Stop:
		UpdateStop();
		break;
	default:
		break;
	}

	//柵に接触状態でなければ無敵時間を更新
	if (!(EnemyState_ == S_FenceHit))
	{
		InvincibilityTimeCalculation();
	}
}

void Enemy::UpdateRoot()
{
	//敵の状態遷移の最上位 攻撃や被弾状態が終わったらここに戻る
	//ここから次の状態へ遷移する

	//自身とPlayerの距離を測る
	float dist = PlayerEnemyDistanceX();

	//一定距離以上離れているなら追跡
	if (dist > ChaseLength)
	{
		EnemyState_ = S_Chase;

		//状態遷移の際は一度x回転をストップ
		RotateXStop();
	}
	else//接近しているなら攻撃準備
	{
		EnemyState_ = S_Aim;

		//状態遷移の際は一度x回転をストップ
		RotateXStop();
	}
}

void Enemy::UpdateChase()
{
	//Playerを追跡している状態

	//プレイヤーのいる方向へY回転する
	LookPlayer();
	
	//更新した方向へ移動
	CharacterMove(AutoAttackDirection);

	//進行方向に合わせてY軸を回転
	RotateFromDirection(AutoAttackDirection);

	//自身とPlayerの距離を測る 
	float dist = PlayerEnemyDistanceX();
	if (dist < ChaseLength)
	{
		//一定以下なら攻撃準備状態へ
		EnemyState_ = S_Aim;

		//状態遷移の際は一度x回転をストップ
		RotateXStop();
	}

	//通常X回転
	MoveRotateX();
}

void Enemy::UpdateAim()
{
	//チャージ(TmpAcceleを溜めている状態) しながらPlayerを狙う状態

	//プレイヤーのいる方向へY回転する
	LookPlayer();

	//進行方向に合わせてY軸を回転
	RotateFromDirection(AutoAttackDirection);

	//加速度を溜める
	Charging();

	//矢印モデルをセット
	SetArrow();

	//矢印モデルの位置を自身の回転と合わせる
	this->MoveParam_.ArrowTransform_.rotate_.y = this->transform_.rotate_.y;

	//チャージ中のエフェクトを出す
	SetChargingEffect("ParticleAssets\\circle_R.png");

	//高速X回転
	FastRotateX();

	//時間経過で攻撃状態へ（配列中のランダムな時間）
	if (++AimTimer_ > EnemyAttackTimeArray[RandomAim_])
	{
		//攻撃までのタイマーをリセット
		AimTimer_ = 0;

		//チャージ解放
		ChargeRelease();

		//攻撃状態へ移行
		EnemyState_ = S_Attack;

		//状態遷移の際は一度x回転をストップ
		RotateXStop();
	}

}

void Enemy::UpdateAttack()
{
	//攻撃状態 正面の方向に移動

	//攻撃中のエフェクトを出す
	SetAttackLocusEffect();

	//正面ベクトルの方向に移動
	CharacterMove(AutoAttackDirection);

	//進行方向に合わせてY軸を回転
	RotateFromDirection(AutoAttackDirection);

	//摩擦量分速度を減少
	FrictionDeceleration();

	//高速X回転
	FastRotateX();

	//加速量が0になったら
	if (IsDashStop())
	{
		//明示的に加速量を0にする
		AccelerationStop();

		//ルートへ戻る
		EnemyState_ = S_Root;

		//攻撃までの時間を再抽選
		RandomAim_ = rand() % EnemyAttackTimeArray.size();

		//状態遷移の際は一度x回転をストップ
		RotateXStop();
	}

	//攻撃SE再生
	Audio::Play(hSoundattack_);
}

void Enemy::UpdateHitStop()
{
	//ヒットストップ状態

	//ヒットストップする時間を超えたら被弾状態へ
	if (++HitStopTimer_ > HitStop)
	{
		HitStopTimer_ = 0;
		EnemyState_ = S_Hit;

		//状態遷移の際は一度x回転をストップ
		RotateXStop();
	}
}

void Enemy::UpdateHit()
{
	//相手と接触した状態

	//ノックバックする
	KnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (IsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		KnockBackVelocityReset();

		//ルートへ戻る
		EnemyState_ = S_Root;

		//状態遷移の際は一度x回転をストップ
		RotateXStop();

		//ダッシュ中の速度リセット(ノックバック終了時点でリセット)
		AccelerationStop();
	}
}

void Enemy::UpdateFenceHit()
{
	//ダメージを受ける柵と接触した状態 

	//ノックバックする
	KnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (IsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		KnockBackVelocityReset();

		//ルートへ戻る
		EnemyState_ = S_Root;

		//状態遷移の際は一度x回転をストップ
		RotateXStop();

		//ダッシュ中の速度リセット(ノックバック終了時点でリセット)
		AccelerationStop();
	}
}

void Enemy::UpdateStop()
{
	//何も処理をしない
}

void Enemy::DrawImGui()
{
	//Debug中はImGuiを設定
#ifdef _DEBUG
	if (ImGui::TreeNode("EnemyStatus"))
	{
		DrawCharacterImGui();

		//デバッグ用のEnemyState_切り替えボタン
		if (ImGui::Button("EnemyStop"))
		{
			if (EnemyState_ != S_Stop)
				EnemyState_ = S_Stop;
			else
				EnemyState_ = S_Root;
		}
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
	XMVECTOR angle = XMVector3AngleBetweenVectors(RotateDirection, MoveParam_.ForwardVector_);

	//前向きベクトルとプレイヤーのいる方向のベクトルの外積を求める
	XMVECTOR cross = XMVector3Cross(MoveParam_.ForwardVector_, RotateDirection);

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
	HitStop = static_cast<int>(OnlyData[i_HitStop]);
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

#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"

#include"BattleScene.h"



namespace
{
	enum enemyonlyIndex
	{
		i_hitstop = 0,
		i_chaseLength,
		i_lookRotateAngle,
		i_lookRotateValue,
		i_arrowrotatecorrection,
		i_EnemyAttackTime_1,
		i_EnemyAttackTime_2,
		i_EnemyAttackTime_3,
		i_EnemyAttackTime_4,
	};

	int HitStop = 0;//ヒットストップする時間
	float ChaseLength = 0.0f;//追跡状態から攻撃準備に移る距離
	float LookRotaeAngle = 0;//敵がプレイヤーの方向を向く際のトリガー　この値を超えたら回転
	float LookRotateValue = 0.0f;//プレイヤー方向を向く際の1fごとの回転量
	float ArrowRotateCorrection = 0.0f;//矢印モデルの回転補正
	
	std::vector<int> EnemyAttackTimeArray = {0,0,0,0};//敵が攻撃するまでの時間の配列　ランダムに選ばれる
}

Enemy::Enemy(GameObject* parent)
	:Character(parent,"Enemy"), hEnemy_(-1),hArrow_(-1), pPlayer_(nullptr),
	EnemyState_(S_STOP),AimTimer_(0), 
	pPositionVec_({0,0,0}),PlayerPosition_({0,0,0}),PlayerAcceleration_(0.0f),
	RandomAim_(0), HitStopTimer_(0)
{
	srand((unsigned)time(NULL));
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{

	std::string path = "CSVdata\\EnemyData.csv";
	SetcsvStatus(path);
	SetCSVEnemy();

	hEnemy_ = Model::Load("Model\\chara.fbx");
	assert(hEnemy_ >= 0);

	hArrow_ = Model::Load("Model\\AttackArrow2.fbx");
	assert(hArrow_ >= 0);

	InitArrow();
	SetStartPosition();

	pPlayer_ = (Player*)FindObject("Player");
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), HitParam_.ColliderSize_);
	this->AddCollider(collision);

	RandomAim_ = rand() % EnemyAttackTimeArray.size();
}

void Enemy::Update()
{
	PlayerPosition_ = pPlayer_->GetWorldPosition();//プレイヤーの位置（ワールド座標）
	pPositionVec_ = XMLoadFloat3(&PlayerPosition_);//プレイヤーの位置をベクトル化し取り続ける
	PlayerAcceleration_ = pPlayer_->GetAcceleration();
	
	//正面ベクトルからどれだけ回転したかを計算し、前向きベクトルを計算
	FrontVectorConfirm();

	ShadowSet();

	switch (EnemyState_)
	{
	case Enemy::S_ROOT:
		UpdateRoot();
		break;
	case Enemy::S_CHASE:
		UpdateChase();
		break;
	case Enemy::S_AIM:
		UpdateAim();
		break;
	case Enemy::S_ATTACK:
		UpdateAttack();
		break;
	case Enemy::S_HITSTOP:
		UpdateHitStop();
		break;
	case Enemy::S_HIT:
		UpdateHit();
		break;
	case Enemy::S_WALLHIT:
		UpdateWallHit();
		break;
	case Enemy::S_STOP:
		UpdateStop();
		break;
	default:
		break;
	}

	if (!(EnemyState_ == S_WALLHIT))//壁ダメージ判定
	{
		InvincibilityTimeCalclation();
	}

	CharacterGravity();
}

void Enemy::Draw()
{
	DrawCharacterModel(hEnemy_, this->transform_);

	ShadowDraw();

	if (EnemyState_ == S_AIM)
	{
		DrawModel(hArrow_, this->MoveParam_.ArrowTransform_);
	}

#ifdef _DEBUG
	if (ImGui::TreeNode("EnemyStatus"))
	{
		DrawCharacterImGui();
	}

	if (ImGui::Button("EnemyStop"))
	{
		if (EnemyState_ != S_STOP)
			EnemyState_ = S_STOP;
		else
			EnemyState_ = S_ROOT;
	}
#endif
}

void Enemy::Release()
{
}

void Enemy::UpdateRoot()
{
	float dist = PlayerEnemyDistanceX();

	if (dist > ChaseLength)//一定距離以上離れているなら追跡
	{
		EnemyState_ = S_CHASE;
	}
	else//接近しているなら攻撃準備
	{
		EnemyState_ = S_AIM;
	}
}

void Enemy::UpdateChase()
{
	MoveRotate();

	LookPlayer();
	CharacterMove(MoveParam_. MoveDirection_);

	float dist = PlayerEnemyDistanceX();
	if (dist < ChaseLength)
	{
		EnemyState_ = S_AIM;
		RotateStop();
	}
}

void Enemy::UpdateAim()
{
	LookPlayer();
	SetChargingEffect("PaticleAssets\\circle_R.png");
	FastRotate();
	Charging();
	SetArrow();

	this->MoveParam_.ArrowTransform_.rotate_.y = this->transform_.rotate_.y + ArrowRotateCorrection;



	//時間経過で攻撃状態へ（配列中のランダムな時間）
	if (++AimTimer_ > EnemyAttackTimeArray[RandomAim_])
	{
		AimTimer_ = 0;
		ChargeRelease();
		EnemyState_ = S_ATTACK;
	}

}

void Enemy::UpdateAttack()
{
	Audio::Play(hSoundattack_);
	SetAttackLocusEffect();
	CharacterMove(MoveParam_.MoveDirection_);
	FrictionDeceleration();
	FastRotate();

	if (IsDashStop())
	{
		RotateStop();
		EnemyState_ = S_ROOT;
		RandomAim_ = rand() % EnemyAttackTimeArray.size();//攻撃終了後に攻撃時間をリセット
	}
}

void Enemy::UpdateHitStop()
{
	//ヒットストップ活用時のみ使用
	if (++HitStopTimer_ > HitStop)
	{
		HitStopTimer_ = 0;
		EnemyState_ = S_HIT;
	}
}

void Enemy::UpdateHit()
{
	if (IsKnockBackEnd())
	{
		RotateStop();
		HitParam_. KnockBack_Velocity_ = { 0,0,0 };
		EnemyState_ = S_ROOT;
	}
	KnockBack();
}

void Enemy::UpdateWallHit()
{
	KnockBack();
	if (IsKnockBackEnd())
	{
		RotateStop();
		EnemyState_ = S_ROOT;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			pBattleScene->PlusPlayerScore();
		}
	}
}

void Enemy::UpdateStop()
{

}

void Enemy::OnCollision(GameObject* pTarget)
{

	if (pTarget->GetObjectName() == "Player")
	{	
		XMVECTOR enemyvector = XMLoadFloat3(&this->transform_.position_);
		XMFLOAT3 getpositon = PlayerPosition_;
		XMVECTOR playervector = XMLoadFloat3(&getpositon);
		float playeraccele = PlayerAcceleration_;

		Reflect(enemyvector, playervector, this->MoveParam_. Acceleration_, playeraccele);
		AimTimer_ = 0;
		EnemyState_ = S_HIT;
	}

	if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
			for (const std::string& arr : WallHitParam_.WireArray_)
			{
				if (pTarget->GetObjectName() == arr)
				{
					XMVECTOR normal = HitNormal(arr);
					WallReflect(normal);
					EnemyState_ = S_WALLHIT;
				}
			}
		}
	}
}

void Enemy::LookPlayer()
{
	//------------敵と自機の回転処理------------

	//XMMATRIX mvec = transform_.matRotate_;//現在の回転している方向（自分の回転行列）

	//プレイヤーの位置(ベクトル)から敵の位置(ベクトル)を引く
	XMVECTOR enemyVector = XMLoadFloat3(&this->transform_.position_);
	XMVECTOR PlayerDist = XMVectorSubtract(pPositionVec_ ,enemyVector);

	//回転する方向を設定(初期化)
	XMVECTOR RotateDirection = XMVectorZero();

	//距離を単位ベクトル化(PlayerDistが0,0,0でエラー防止)
	if(!XMVector3Equal(PlayerDist, XMVectorZero()))
	{
		RotateDirection = XMVector3Normalize(PlayerDist);
	}

	//敵への移動方向を保管
	MoveParam_.MoveDirection_ = RotateDirection;

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
	if (Dig > LookRotaeAngle)
	{
		//外積Yが0以上なら左周り(半時計周り)
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

	transform_.Calclation();
}

float Enemy::PlayerEnemyDistanceX()
{
	XMVECTOR EnemyPosition = XMLoadFloat3(&this->transform_.position_);
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition, pPositionVec_);
	float tmp = XMVectorGetX(XMVector3Length(DistVec));
	return tmp;
}

void Enemy::SetCSVEnemy()
{
	CsvReader csv;
	csv.Load("CSVdata\\EnemyData.csv");

	std::string only = "EnemyOnlyParam";
	if (csv.IsGetParamName(only))
	{
		std::vector<float> v = csv.GetParam(only);
		HitStop = static_cast<int>(v[i_hitstop]);
		ChaseLength = v[i_chaseLength];
		LookRotaeAngle = v[i_lookRotateAngle];
		LookRotateValue = v[i_lookRotateValue];
		ArrowRotateCorrection = v[i_arrowrotatecorrection];

		int arr[] = { static_cast<int>(v[i_EnemyAttackTime_1]), static_cast<int>(v[i_EnemyAttackTime_2]),
			static_cast<int>(v[i_EnemyAttackTime_3]), static_cast<int>(v[i_EnemyAttackTime_4]) };
		for (int i = 0; i < EnemyAttackTimeArray.size(); i++)
		{
			EnemyAttackTimeArray[i] = arr[i];
		}
	}
}

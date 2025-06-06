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
		i_EnemyAttackTime_1,
		i_EnemyAttackTime_2,
		i_EnemyAttackTime_3,
		i_EnemyAttackTime_4,
	};

	int HitStop = 0;//ヒットストップする時間
	float ChaseLength = 0.0f;//追跡状態から攻撃準備に移る距離
	float LookRotaeAngle = 0;//敵がプレイヤーの方向を向く際のトリガー　この値を超えたら回転
	float LookRotateValue = 0.0f;//プレイヤー方向を向く際の1fごとの回転量
	
	std::vector<int> EnemyAttackTimeArray = {0,0,0,0};//敵が攻撃するまでの時間の配列　ランダムに選ばれる
}

Enemy::Enemy(GameObject* parent)
	:Character(parent,"Enemy"), hEnemy_(-1), pPlayer_(nullptr),
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

	hEnemy_ = Model::Load("Model\\chara2.fbx");
	assert(hEnemy_ >= 0);

	ShadowInit();
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

	//if (!WallHitParam_.IsInvincibility_ && !(EnemyState_ == S_WALLHIT))//壁ダメージ判定
	//{
	//	if (IsOutsideStage(this->transform_.position_))
	//	{
	//		WallHit();
	//		EnemyState_ = S_WALLHIT;
	//	}
	//}

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

#ifdef _DEBUG
	if (ImGui::TreeNode("EnemyStatus"))
	{
		if (ImGui::TreeNode("Position"))
		{
			ImGui::InputFloat("PositionX:%.3f", &this->transform_.position_.x);
			ImGui::InputFloat("PositionY:%.3f", &this->transform_.position_.y);
			ImGui::InputFloat("PositionZ:%.3f", &this->transform_.position_.z);
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

	//時間経過で攻撃状態へ（配列中のランダムな時間）
	if (++AimTimer_ > EnemyAttackTimeArray[RandomAim_])
	{
		AimTimer_ = 0;
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

	if (pTarget->GetObjectName() == "UpperWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.UpperNormal_);
			EnemyState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "LowerWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.LowerNormal_);
			EnemyState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "RightWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.RightNormal_);
			EnemyState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "LeftWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.LeftNormal_);
			EnemyState_ = S_WALLHIT;
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

		int arr[] = { static_cast<int>(v[i_EnemyAttackTime_1]), static_cast<int>(v[i_EnemyAttackTime_2]),
			static_cast<int>(v[i_EnemyAttackTime_3]), static_cast<int>(v[i_EnemyAttackTime_4]) };
		for (int i = 0; i < EnemyAttackTimeArray.size(); i++)
		{
			EnemyAttackTimeArray[i] = arr[i];
		}
	}
}

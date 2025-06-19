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

	int HitStop = 0;//�q�b�g�X�g�b�v���鎞��
	float ChaseLength = 0.0f;//�ǐՏ�Ԃ���U�������Ɉڂ鋗��
	float LookRotaeAngle = 0;//�G���v���C���[�̕����������ۂ̃g���K�[�@���̒l�𒴂������]
	float LookRotateValue = 0.0f;//�v���C���[�����������ۂ�1f���Ƃ̉�]��
	float ArrowRotateCorrection = 0.0f;//��󃂃f���̉�]�␳
	
	std::vector<int> EnemyAttackTimeArray = {0,0,0,0};//�G���U������܂ł̎��Ԃ̔z��@�����_���ɑI�΂��
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
	PlayerPosition_ = pPlayer_->GetWorldPosition();//�v���C���[�̈ʒu�i���[���h���W�j
	pPositionVec_ = XMLoadFloat3(&PlayerPosition_);//�v���C���[�̈ʒu���x�N�g��������葱����
	PlayerAcceleration_ = pPlayer_->GetAcceleration();
	
	//���ʃx�N�g������ǂꂾ����]���������v�Z���A�O�����x�N�g�����v�Z
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

	if (!(EnemyState_ == S_WALLHIT))//�ǃ_���[�W����
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

	if (dist > ChaseLength)//��苗���ȏ㗣��Ă���Ȃ�ǐ�
	{
		EnemyState_ = S_CHASE;
	}
	else//�ڋ߂��Ă���Ȃ�U������
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



	//���Ԍo�߂ōU����Ԃցi�z�񒆂̃����_���Ȏ��ԁj
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
		RandomAim_ = rand() % EnemyAttackTimeArray.size();//�U���I����ɍU�����Ԃ����Z�b�g
	}
}

void Enemy::UpdateHitStop()
{
	//�q�b�g�X�g�b�v���p���̂ݎg�p
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
	//------------�G�Ǝ��@�̉�]����------------

	//XMMATRIX mvec = transform_.matRotate_;//���݂̉�]���Ă�������i�����̉�]�s��j

	//�v���C���[�̈ʒu(�x�N�g��)����G�̈ʒu(�x�N�g��)������
	XMVECTOR enemyVector = XMLoadFloat3(&this->transform_.position_);
	XMVECTOR PlayerDist = XMVectorSubtract(pPositionVec_ ,enemyVector);

	//��]���������ݒ�(������)
	XMVECTOR RotateDirection = XMVectorZero();

	//������P�ʃx�N�g����(PlayerDist��0,0,0�ŃG���[�h�~)
	if(!XMVector3Equal(PlayerDist, XMVectorZero()))
	{
		RotateDirection = XMVector3Normalize(PlayerDist);
	}

	//�G�ւ̈ړ�������ۊ�
	MoveParam_.MoveDirection_ = RotateDirection;

	//------------�p�x�ɉ����ĉ�]------------
	
	//��̃x�N�g���Ԃ̃��W�A���p�����߂�
	XMVECTOR angle = XMVector3AngleBetweenVectors(RotateDirection, MoveParam_.ForwardVector_);

	//�O�����x�N�g���ƃv���C���[�̂�������̃x�N�g���̊O�ς����߂�
	XMVECTOR cross = XMVector3Cross(MoveParam_.ForwardVector_, RotateDirection);

	//�O�����x�N�g���ƃv���C���[�����x�N�g����XZ�ɐL�т�x�N�g���Ȃ̂�
	//�O�ς�Y���i+��-�ō��E�ǂ���ɂ��邩���f�j�����߂�
	float crossY = XMVectorGetY(cross);

	//��̃x�N�g���Ԃ̃��W�A���p��x���ɖ߂�
	//���ȏ�J���Ă���Ȃ��]
	float Dig = XMConvertToDegrees(XMVectorGetX(angle));
	if (Dig > LookRotaeAngle)
	{
		//�O��Y��0�ȏ�Ȃ獶����(�����v����)
		if (crossY > 0.0)
		{
			transform_.rotate_.y -= LookRotateValue;
		}
		//�O��Y��0�ȉ��Ȃ�E����(���v����)
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

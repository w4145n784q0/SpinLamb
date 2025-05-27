#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"BattleScene.h"
#include"Engine/SceneManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	int blinkTimer = 0;
	const int blink = 20;

	const int HitStop = 2;//�q�b�g�X�g�b�v���鎞��
	const float ChaseLength = 10.0f;//�ǐՏ�Ԃ���U�������Ɉڂ鋗��
	const float LookRotaeAngle = 3;//�G���v���C���[�̕����������ۂ̃g���K�[�@���̒l�𒴂������]
	const float LookRotateValue = 1.5f;//�v���C���[�����������ۂ�1f���Ƃ̉�]��
	int RandomAim = 0;//EnemyAttackTimeArray�̓Y���� 
	const int EnemyAttackTimeArray[] = { 180,150,120,60 };//�G���U������܂ł̎��Ԃ̔z��@�����_���ɑI�΂��
}

Enemy::Enemy(GameObject* parent)
	:Character(parent,"Enemy"), hEnemy_(-1), pPlayer_(nullptr),
	EnemyState_(S_IDLE),HitStopTimer_(0)
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

	hEnemy_ = Model::Load("Model\\chara2.fbx");
	assert(hEnemy_ >= 0);

	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrix�ŕϊ�
	ShadowInit();
	SetStartPosition();

	pPlayer_ = (Player*)FindObject("Player");
	//pGround_ = (Ground*)FindObject("Ground");
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), ColliderSize_);
	this->AddCollider(collision);

	RandomAim = rand() % 4;
}

void Enemy::Update()
{
	PlayerPosition_ = pPlayer_->GetWorldPosition();//�v���C���[�̈ʒu�i���[���h���W�j
	pPositionVec_ = XMLoadFloat3(&PlayerPosition_);//�v���C���[�̈ʒu���x�N�g��������葱����
	PlayerAcceleration_ = pPlayer_->GetAcceleration();
	
	//���ʃx�N�g������ǂꂾ����]���������v�Z���A�O�����x�N�g�����v�Z
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, FrontDirection_);

	ShadowSet();

	switch (EnemyState_)
	{
	case Enemy::S_IDLE:
		UpdateIdle();
		break;
	case Enemy::S_ROOT:
		UpdateRoot();
		break;
	case Enemy::S_CHASE:
		UpdateChase();
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
	case Enemy::S_AIM:
		UpdateAim();
		break;
	case Enemy::S_ONALEAT:
		UpdateOnAlert();
		break;
	case Enemy::S_MAX:
		break;
	default:
		break;
	}

	if (!IsInvincibility_ && !(EnemyState_ == S_WALLHIT))//�ǃ_���[�W����
	{
		if (IsOutsideStage(this->transform_.position_))
		{
			WallHit();
			EnemyState_ = S_WALLHIT;
		}
	}

	InvincibilityTimeCalclation();
	
	CharacterGravity();
}

void Enemy::Draw()
{
	//Model::SetTransform(hEnemy_, transform_);
	//Model::Draw(hEnemy_);

	if (IsInvincibility_)
	{
		if (++blinkTimer > blink) {

			blinkTimer = 0;
			Model::SetTransform(hEnemy_, this->transform_);
			Model::Draw(hEnemy_);
		}
	}
	else
	{
		Model::SetTransform(hEnemy_, this->transform_);
		Model::Draw(hEnemy_);
	}

	ShadowDraw();

#ifdef _DEBUG
	if (ImGui::Button("EnemystateChange"))
	{
		if (!EnemyState_ == S_IDLE)
			EnemyState_ = S_IDLE;
		else
			EnemyState_ = S_ROOT;
	}

	//XMFLOAT3 tmp;
	//XMStoreFloat3(&tmp, MoveDirection_);

	//ImGui::Text("front.x:%3f", (float)tmp.x);
	//ImGui::Text("front.y:%3f", (float)tmp.y);
	//ImGui::Text("front.z:%3f", (float)tmp.z);

	ImGui::Text("knockback:%.3f",this->KnockBack_Velocity_.x );
#endif
}

void Enemy::Release()
{
}

void Enemy::UpdateIdle()
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
	MoveRotateReverse();

	LookPlayer();
	CharacterMove(MoveDirection_);

	float dist = PlayerEnemyDistanceX();
	if (dist < ChaseLength)
	{
		EnemyState_ = S_AIM;
		RotateStop();
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
		KnockBack_Velocity_ = { 0,0,0 };
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
		KnockBack_Velocity_ = { 0,0,0 };
		EnemyState_ = S_ROOT;
		IsInvincibility_ = true;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			//pBattleScene->SetPlayerHp(CharacterLife_);
			pBattleScene->PlusPlayerScore();
		}
	}
}

void Enemy::UpdateAim()
{
	LookPlayer();
	SetChargingEffect("PaticleAssets\\circle_R.png");
	FastRotate();

	//	EnemyAttackTime = 180

	if (++AimTimer_ > EnemyAttackTimeArray[RandomAim])
	{
		AimTimer_ = 0;
		Acceleration_ = FullAccelerate_;
		EnemyState_ = S_ATTACK;
	}

}

void Enemy::UpdateOnAlert()
{
	LookPlayer();
}

void Enemy::UpdateAttack()
{
	SetAttackLocusEffect();
	CharacterMove(MoveDirection_);

	//���x�𖈃t���[������
	Deceleration();

	//�L�������f����]
	FastRotateReverse();

	if (IsDashStop())
	{
		RotateStop();
		EnemyState_ = S_ROOT;
		RandomAim = rand() % 4;//�U���I����ɍU�����Ԃ����Z�b�g
	}
}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Fence")
	{
		if (!IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
			WallHit();

			EnemyState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "Player")
	{	
		XMVECTOR enemyvector = XMLoadFloat3(&this->transform_.position_);
		XMFLOAT3 getpositon = PlayerPosition_;
		XMVECTOR playervector = XMLoadFloat3(&getpositon);
		float playeraccele = PlayerAcceleration_;

		Reflect(enemyvector, playervector, this->Acceleration_, playeraccele);
		AimTimer_ = 0;
		EnemyState_ = S_HIT;
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
	MoveDirection_ = RotateDirection;

	//------------�p�x�ɉ����ĉ�]------------
	
	//��̃x�N�g���Ԃ̃��W�A���p�����߂�
	XMVECTOR angle = XMVector3AngleBetweenVectors(RotateDirection, ForwardVector_);

	//�O�����x�N�g���ƃv���C���[�̂�������̃x�N�g���̊O�ς����߂�
	XMVECTOR cross = XMVector3Cross(ForwardVector_, RotateDirection);

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

#include "Enemy.h"

#include"../Engine/Model.h"
#include"../Engine/Camera.h"
#include"../Engine/SphereCollider.h"
#include"../Engine/Audio.h"



namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X(Enemy�N���X�̌ŗL�̕ϐ�)
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

	//�q�b�g�X�g�b�v���鎞��
	int HitStop = 0;

	//�ǐՏ�Ԃ���U�������Ɉڂ鋗��
	float ChaseLength = 0.0f;

	//�G���v���C���[�̕����������ۂ̃g���K�[ ���̒l�𒴂������]
	float LookRotateAngle = 0;
	
	//�v���C���[�����������ۂ�1f���Ƃ̉�]��
	float LookRotateValue = 0.0f;

	//�G���U������܂ł̎��Ԃ̔z��@�����_���ɑI�΂��
	std::vector<int> EnemyAttackTimeArray = {0,0,0,0};

	//�G���U���E�ړ�������� LookPlayer�Ŏw�肷��
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
	//csv����p�����[�^�ǂݍ���
	std::string path = "CSVdata\\CharacterData\\EnemyData.csv";
	SetCSVStatus(path);

	//csv����p�����[�^�ǂݍ���(Enemy�̂ݎg�����)
	SetCSVEnemy();

	//�e���f���̓ǂݍ���
	hEnemy_ = Model::Load("Model\\Character_black.fbx");
	assert(hEnemy_ >= 0);


	//���̃g�����X�t�H�[���̏�����
	InitArrow();

	//�����ʒu�ɃL�����N�^�[���Z�b�g
	SetStartPosition();

	//�����蔻��t����
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), HitParam_.ColliderSize_);
	this->AddCollider(collision);

	
	//�G�̍U�����Ԕz��̓Y�����������_���ɐݒ�
	RandomAim_ = rand() % EnemyAttackTimeArray.size();
}

void Enemy::Update()
{

}

void Enemy::Draw()
{
	//Character�N���X�̋��ʕ`��
	Character::Draw();

	//�������L�����N�^�[�̕`��
	DrawCharacterModel(hEnemy_, this->transform_);

	//�`���[�W���̂ݖ�󃂃f���`��
	if (EnemyState_ == S_Aim)
	{
		DrawArrow();
	}
}

void Enemy::Release()
{
	//���̂͏������A�A�h���X�̂ݖ���������
	if (pPlayer_ != nullptr)
	{
		pPlayer_ = nullptr;
	}
}

void Enemy::OnCollision(GameObject* pTarget)
{
	//�v���C���[�ƐڐG�������̏���
	if (pTarget->GetObjectName() == "Player1")
	{
		//���g�̈ʒu��XMVECTOR�^�ɂ���
		XMVECTOR MyVector = XMLoadFloat3(&this->transform_.position_);

		//����̈ʒu���擾�AXMVECTOR�^�ɂ���
		XMFLOAT3 TargetPos = TargetPosition_;
		XMVECTOR TargetVector = XMLoadFloat3(&TargetPosition_);

		
		//���ˏ������s��(�����̈ʒu�x�N�g��,����̈ʒu�x�N�g��,�����̉����x,����̉����x)
		Reflect(MyVector, TargetVector, this->MoveParam_.Acceleration_, TargetAcceleration_);
		
		//�ڐG���_�ōU���܂ł̃^�C�}�[�����Z�b�g
		AimTimer_ = 0;

		//��e��ԂɂȂ�
		EnemyState_ = S_Hit;

		//��ԑJ�ڂ̍ۂ͈�x��]���X�g�b�v
		RotateXStop();
	}

	//�e��ɐڐG�������̏���
	if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		//���g����ɐڐG��Ԃł͂Ȃ� �����G��ԂłȂ��Ȃ瑱����
		if (!FenceHitParam_.IsInvincibility_ && !(EnemyState_ == S_FenceHit))
		{
			//��̖��O�̂����ꂩ�ɐڐG���Ă���Ȃ�
			for (const std::string& arr : FenceHitParam_.WireArray_)
			{
				if (pTarget->GetObjectName() == arr)
				{
					//�ڐG���Ă����̖@��(���˂�������)���擾
					XMVECTOR normal = HitNormal(arr);

					//���ˊJ�n
					FenceReflect(normal);

					//CPU�̏�Ԃ���ɐڐG��Ԃɂ���
					EnemyState_ = S_FenceHit;

					//�J�����U��
					Camera::CameraShakeStart(Camera::GetShakeTimeMiddle());

					//��ԑJ�ڂ̍ۂ͈�x��]���X�g�b�v
					RotateXStop();

				}
			}
		}
	}
}

void Enemy::EnemyRun()
{
	//�v���C���[�̈ʒu�i���[���h���W�j
	TargetPosition_ = pPlayer_->GetWorldPosition();

	//�v���C���[�̈ʒu���x�N�g��������葱����
	TargetVec_ = XMLoadFloat3(&TargetPosition_);

	//�v���C���[�̉����x����葱����
	TargetAcceleration_ = pPlayer_->GetAcceleration();

	//Character�N���X�̋��ʏ���
	Character::Update();

	//���݂̏�Ԃɂ���čX�V�𕪂���
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

	//��ɐڐG��ԂłȂ���Ζ��G���Ԃ��X�V
	if (!(EnemyState_ == S_FenceHit))
	{
		InvincibilityTimeCalculation();
	}
}

void Enemy::UpdateRoot()
{
	//�G�̏�ԑJ�ڂ̍ŏ�� �U�����e��Ԃ��I������炱���ɖ߂�
	//�������玟�̏�Ԃ֑J�ڂ���

	//���g��Player�̋����𑪂�
	float dist = PlayerEnemyDistanceX();

	//��苗���ȏ㗣��Ă���Ȃ�ǐ�
	if (dist > ChaseLength)
	{
		EnemyState_ = S_Chase;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateXStop();
	}
	else//�ڋ߂��Ă���Ȃ�U������
	{
		EnemyState_ = S_Aim;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateXStop();
	}
}

void Enemy::UpdateChase()
{
	//Player��ǐՂ��Ă�����

	//�v���C���[�̂��������Y��]����
	LookPlayer();
	
	//�X�V���������ֈړ�
	CharacterMove(AutoAttackDirection);

	//�i�s�����ɍ��킹��Y������]
	RotateFromDirection(AutoAttackDirection);

	//���g��Player�̋����𑪂� 
	float dist = PlayerEnemyDistanceX();
	if (dist < ChaseLength)
	{
		//���ȉ��Ȃ�U��������Ԃ�
		EnemyState_ = S_Aim;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateXStop();
	}

	//�ʏ�X��]
	MoveRotateX();
}

void Enemy::UpdateAim()
{
	//�`���[�W(TmpAccele�𗭂߂Ă�����) ���Ȃ���Player��_�����

	//�v���C���[�̂��������Y��]����
	LookPlayer();

	//�i�s�����ɍ��킹��Y������]
	RotateFromDirection(AutoAttackDirection);

	//�����x�𗭂߂�
	Charging();

	//��󃂃f�����Z�b�g
	SetArrow();

	//��󃂃f���̈ʒu�����g�̉�]�ƍ��킹��
	this->MoveParam_.ArrowTransform_.rotate_.y = this->transform_.rotate_.y;

	//�`���[�W���̃G�t�F�N�g���o��
	SetChargingEffect("ParticleAssets\\circle_R.png");

	//����X��]
	FastRotateX();

	//���Ԍo�߂ōU����Ԃցi�z�񒆂̃����_���Ȏ��ԁj
	if (++AimTimer_ > EnemyAttackTimeArray[RandomAim_])
	{
		//�U���܂ł̃^�C�}�[�����Z�b�g
		AimTimer_ = 0;

		//�`���[�W���
		ChargeRelease();

		//�U����Ԃֈڍs
		EnemyState_ = S_Attack;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateXStop();
	}

}

void Enemy::UpdateAttack()
{
	//�U����� ���ʂ̕����Ɉړ�

	//�U�����̃G�t�F�N�g���o��
	SetAttackLocusEffect();

	//���ʃx�N�g���̕����Ɉړ�
	CharacterMove(AutoAttackDirection);

	//�i�s�����ɍ��킹��Y������]
	RotateFromDirection(AutoAttackDirection);

	//���C�ʕ����x������
	FrictionDeceleration();

	//����X��]
	FastRotateX();

	//�����ʂ�0�ɂȂ�����
	if (IsDashStop())
	{
		//�����I�ɉ����ʂ�0�ɂ���
		AccelerationStop();

		//���[�g�֖߂�
		EnemyState_ = S_Root;

		//�U���܂ł̎��Ԃ��Ē��I
		RandomAim_ = rand() % EnemyAttackTimeArray.size();

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateXStop();
	}

	//�U��SE�Đ�
	Audio::Play(hSoundattack_);
}

void Enemy::UpdateHitStop()
{
	//�q�b�g�X�g�b�v���

	//�q�b�g�X�g�b�v���鎞�Ԃ𒴂������e��Ԃ�
	if (++HitStopTimer_ > HitStop)
	{
		HitStopTimer_ = 0;
		EnemyState_ = S_Hit;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateXStop();
	}
}

void Enemy::UpdateHit()
{
	//����ƐڐG�������

	//�m�b�N�o�b�N����
	KnockBack();

	//�m�b�N�o�b�N���鑬�x�����ȉ��Ȃ�ʏ��Ԃ֖߂�
	if (IsKnockBackEnd())
	{
		//�m�b�N�o�b�N���x��0�ɖ߂��Ă���
		KnockBackVelocityReset();

		//���[�g�֖߂�
		EnemyState_ = S_Root;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateXStop();

		//�_�b�V�����̑��x���Z�b�g(�m�b�N�o�b�N�I�����_�Ń��Z�b�g)
		AccelerationStop();
	}
}

void Enemy::UpdateFenceHit()
{
	//�_���[�W���󂯂��ƐڐG������� 

	//�m�b�N�o�b�N����
	KnockBack();

	//�m�b�N�o�b�N���鑬�x�����ȉ��Ȃ�ʏ��Ԃ֖߂�
	if (IsKnockBackEnd())
	{
		//�m�b�N�o�b�N���x��0�ɖ߂��Ă���
		KnockBackVelocityReset();

		//���[�g�֖߂�
		EnemyState_ = S_Root;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateXStop();

		//�_�b�V�����̑��x���Z�b�g(�m�b�N�o�b�N�I�����_�Ń��Z�b�g)
		AccelerationStop();
	}
}

void Enemy::UpdateStop()
{
	//�������������Ȃ�
}

void Enemy::DrawImGui()
{
	//Debug����ImGui��ݒ�
#ifdef _DEBUG
	if (ImGui::TreeNode("EnemyStatus"))
	{
		DrawCharacterImGui();

		//�f�o�b�O�p��EnemyState_�؂�ւ��{�^��
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
	//Player�̕��ɉ�]���鏈��

	//------------�G�Ǝ��@�̉�]����------------

	//�v���C���[�̈ʒu(�x�N�g��)����G�̈ʒu(�x�N�g��)������
	XMVECTOR enemyVector = XMLoadFloat3(&this->transform_.position_);
	XMVECTOR PlayerDist = XMVectorSubtract(TargetVec_ ,enemyVector);
	
	//��]���������ݒ�(������)
	XMVECTOR RotateDirection = XMVectorZero();

	//�v���C���[�̈ʒu���玩�g�̈ʒu��������������0�x�N�g���Ȃ牽�����Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if(XMVector3Equal(PlayerDist, XMVectorZero()))
	{
		return;
	}
	
	//������P�ʃx�N�g�������A�ړ������Ƃ��Ĉ�����悤�ɂ���
	RotateDirection = XMVector3Normalize(PlayerDist);

	//�G�̈ړ�������ۊ�
	AutoAttackDirection = RotateDirection;

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
	if (Dig > LookRotateAngle)
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

	//�v�Z���m��
	transform_.Calculation();
}

void Enemy::RotateFromDirection(XMVECTOR _direction)
{
	//�ړ�������0�x�N�g���Ȃ牽�����Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if (XMVector3Equal(_direction, XMVectorZero()))
	{
		return;
	}

	//�ړ������𐳋K��
	XMVECTOR moveDir = XMVector3Normalize(_direction);

	//XMFLOAT3�ɕϊ�
	XMFLOAT3 dir = { 0,0,0 };
	XMStoreFloat3(&dir, moveDir);

	//XZ���ʏ�̃x�N�g������Y����]�p�����߂�
	float angleY = static_cast<float>(atan2(dir.x, dir.z));

	//angleY�̓��W�A���p�Ȃ̂Ńf�B�O���[�p�ɕϊ����AY����]�ɃZ�b�g
	transform_.rotate_.y = XMConvertToDegrees(angleY);
}

float Enemy::PlayerEnemyDistanceX()
{
	//Player�̈ʒu�x�N�g��-���g�̈ʒu�x�N�g���̒������v�Z����

	//���g�̈ʒu���x�N�g���ɂ���
	XMVECTOR EnemyPosition = XMLoadFloat3(&this->transform_.position_);

	//Player�̈ʒu�x�N�g��-���g�̈ʒu�x�N�g�����v�Z
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition, TargetVec_);

	//�������擾���Ԃ�
	float tmp = XMVectorGetX(XMVector3Length(DistVec));
	return tmp;
}

void Enemy::SetCSVEnemy()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\CharacterData\\EnemyData.csv");

	//csv�t�@�C���̊e0��ڂ̕�������擾
	std::string only = "EnemyOnlyParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> OnlyData = GetCSVReadData(csv, only);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
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

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
	//csv�ǂݍ��ݎ��̃C���f�b�N�X
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

	//�q�b�g�X�g�b�v���鎞��
	int HitStop = 0;

	//�ǐՏ�Ԃ���U�������Ɉڂ鋗��
	float ChaseLength = 0.0f;

	//�G���v���C���[�̕����������ۂ̃g���K�[�@���̒l�𒴂������]
	float LookRotaeAngle = 0;
	
	//�v���C���[�����������ۂ�1f���Ƃ̉�]��
	float LookRotateValue = 0.0f;

	//��󃂃f���̉�]�␳
	float ArrowRotateCorrection = 0.0f;

	//�G���U������܂ł̎��Ԃ̔z��@�����_���ɑI�΂��
	std::vector<int> EnemyAttackTimeArray = {0,0,0,0};

	//�G���U���E�ړ�������� LookPlayer�Ŏw�肷��
	XMVECTOR AutoAttackDirection = { 0,0,0 };
}

Enemy::Enemy(GameObject* parent)
	:Character(parent,"Enemy"), hEnemy_(-1),pPlayer_(nullptr),
	EnemyState_(S_STOP),AimTimer_(0), 
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
	SetcsvStatus(path);

	//csv����p�����[�^�ǂݍ���(Enemy�̂ݎg�����)
	SetCSVEnemy();

	//�e���f���̓ǂݍ���
	hEnemy_ = Model::Load("Model\\chara_black.fbx");
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
	if (EnemyState_ == S_AIM)
	{
		DrawArrow();
	}
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* pTarget)
{
	//�v���C���[�ƐڐG�������̏���
	if (pTarget->GetObjectName() == "Player1")
	{
		//���g�̈ʒu��XMVECTOR�^�ɂ���
		XMVECTOR enemyvector = XMLoadFloat3(&this->transform_.position_);
		XMFLOAT3 getpositon = TargetPosition_;

		//�G�̈ʒu�����XMVECTOR�^�ɂ���
		XMVECTOR playervector = XMLoadFloat3(&getpositon);

		
		//���ˏ������s��(�����̈ʒu�x�N�g��,����̈ʒu�x�N�g��,�����̉����x,����̉����x)
		Reflect(enemyvector, playervector, this->MoveParam_.Acceleration_, TargetAcceleration_);
		
		//�ڐG���_�ōU���܂ł̃^�C�}�[�����Z�b�g
		AimTimer_ = 0;

		//��e��ԂɂȂ�
		EnemyState_ = S_HIT;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateStop();
	}

	//�e��ɐڐG�������̏���
	if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		//���g����ɐڐG��Ԃł͂Ȃ� and ���G��ԂłȂ��Ȃ瑱����
		if (!WallHitParam_.IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
			//��̖��O�̂����ꂩ�ɐڐG���Ă���Ȃ�
			for (const std::string& arr : WallHitParam_.WireArray_)
			{
				if (pTarget->GetObjectName() == arr)
				{
					//�ڐG���Ă����̖@��(���˂�������)���擾
					XMVECTOR normal = HitNormal(arr);

					//���ˊJ�n
					WallReflect(normal);

					//CPU�̏�Ԃ���ɐڐG��Ԃɂ���
					EnemyState_ = S_WALLHIT;

					//�J�����U��
					Camera::CameraShakeStart(Camera::GetShakeTimeMiddle());

					//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
					RotateStop();

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

	//��ɐڐG��ԂłȂ���Ζ��G���Ԃ��X�V
	if (!(EnemyState_ == S_WALLHIT))
	{
		InvincibilityTimeCalclation();
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
		EnemyState_ = S_CHASE;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateStop();
	}
	else//�ڋ߂��Ă���Ȃ�U������
	{
		EnemyState_ = S_AIM;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateStop();
	}
}

void Enemy::UpdateChase()
{
	//Player��ǐՂ��Ă�����

	//�v���C���[�̂��������Y��]����
	LookPlayer();

	//�X�V���������ֈړ�
	CharacterMove(AutoAttackDirection);

	//���g��Player�̋����𑪂� 
	float dist = PlayerEnemyDistanceX();
	if (dist < ChaseLength)
	{
		//���ȉ��Ȃ�U��������Ԃ�
		EnemyState_ = S_AIM;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateStop();
	}

	//�ʏ�X��]
	MoveRotate();
}

void Enemy::UpdateAim()
{
	//�`���[�W(TmpAccele�𗭂߂Ă�����) ���Ȃ���Player��_�����

	//�v���C���[�̂��������Y��]����
	LookPlayer();

	//�����x�𗭂߂�
	Charging();

	//��󃂃f�����Z�b�g
	SetArrow();

	//��󃂃f���̈ʒu�����g�̉�]�ƍ��킹��(Enemy�̏ꍇ�A���̉�]�ɕ␳�������Ă���)
	this->MoveParam_.ArrowTransform_.rotate_.y = this->transform_.rotate_.y + ArrowRotateCorrection;

	//�`���[�W���̃G�t�F�N�g���o��
	SetChargingEffect("PaticleAssets\\circle_R.png");

	//����X��]
	FastRotate();

	//���Ԍo�߂ōU����Ԃցi�z�񒆂̃����_���Ȏ��ԁj
	if (++AimTimer_ > EnemyAttackTimeArray[RandomAim_])
	{
		//�U���܂ł̃^�C�}�[�����Z�b�g
		AimTimer_ = 0;

		//�`���[�W���
		ChargeRelease();

		//�U����Ԃֈڍs
		EnemyState_ = S_ATTACK;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateStop();
	}

}

void Enemy::UpdateAttack()
{
	//�U����� ���ʂ̕����Ɉړ�

	//�U�����̃G�t�F�N�g���o��
	SetAttackLocusEffect();

	//���ʃx�N�g���̕����Ɉړ�
	CharacterMove(AutoAttackDirection);

	//���C�ʕ����x������
	FrictionDeceleration();

	//����X��]
	FastRotate();

	//�����ʂ�0�ɂȂ�����
	if (IsDashStop())
	{
		//�����I�ɉ����ʂ�0�ɂ���
		AccelerationStop();

		//���[�g�֖߂�
		EnemyState_ = S_ROOT;

		//�U���܂ł̎��Ԃ��Ē��I
		RandomAim_ = rand() % EnemyAttackTimeArray.size();

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateStop();
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
		EnemyState_ = S_HIT;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateStop();
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
		EnemyState_ = S_ROOT;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateStop();
	}
}

void Enemy::UpdateWallHit()
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
		EnemyState_ = S_ROOT;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		RotateStop();
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
			if (EnemyState_ != S_STOP)
				EnemyState_ = S_STOP;
			else
				EnemyState_ = S_ROOT;
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

	//������P�ʃx�N�g����(PlayerDist��0,0,0�ŃG���[�h�~)
	if(!XMVector3Equal(PlayerDist, XMVectorZero()))
	{
		RotateDirection = XMVector3Normalize(PlayerDist);
	}

	//�G�ւ̈ړ�������ۊ�
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
	HitStop = static_cast<int>(OnlyData[i_hitstop]);
	ChaseLength = OnlyData[i_chaseLength];
	LookRotaeAngle = OnlyData[i_lookRotateAngle];
	LookRotateValue = OnlyData[i_lookRotateValue];
	ArrowRotateCorrection = OnlyData[i_arrowrotatecorrection];

	int arr[] = { static_cast<int>(OnlyData[i_EnemyAttackTime_1]), static_cast<int>(OnlyData[i_EnemyAttackTime_2]),
			static_cast<int>(OnlyData[i_EnemyAttackTime_3]), static_cast<int>(OnlyData[i_EnemyAttackTime_4]) };
	for (int i = 0; i < EnemyAttackTimeArray.size(); i++)
	{
		EnemyAttackTimeArray[i] = arr[i];
	}
}

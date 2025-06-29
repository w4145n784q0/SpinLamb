#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"Engine/VFX.h"
#include"Engine/SceneManager.h"

#include"BattleScene.h"
#include"Enemy.h"

namespace {

	//csv�ǂݍ��ݎ��̃C���f�b�N�X
	enum playeronlyIndex
	{
		i_backcameraX = 0,
		i_backcameraY,
		i_backcameraZ,
		i_keyboardrotateY,
		i_camerashaketime,
		i_jumpheight,
		i_camerainitx,
		i_camerainity,
		i_camerainitz,
		i_camerarotate,
		i_cameraupperlimit,
		i_cameralowerlimit,
		i_cameradebugPos,
	};

	//�J�����̌Œ�ʒu
	//BackCamera�̒l�𖈃t���[�����̒l�ɂ���i�l���ς�葱����̂�h���j
	XMVECTOR BackCameraPos = { 0.0f,0.0f,0.0f };

	//�`���[�W���̍��E���͎��̉�]��
	float KeyBoardRotateY = 0.0f;
	
	//�J�����̐U������
	float cameraShakeTime = 0.0f;

	//�W�����v���̈ꎞ�I�ɑ������l
	float Jumpheight = 0.0f;

	//�J�����̏������ʒu
	XMFLOAT3 CameraInit = { 0,0,0 };

	//���E���͎��̃J�����̉�]��
	float cameraRotate = 0.0f;

	//�㉺���͎��̃J�����̍����̍ō��l
	float cameraUpperLimit = 0.0f;

	//�㉺���͎��̃J�����̍����̍Œ�l
	float cameraLowerLimit = 0.0f;

	//�f�o�b�O�J������Ԏ��̃J�����̌Œ�ʒu
	float cameraDebugPos = 0.0f;
}

Player::Player(GameObject* parent) 
	: Character(parent,"Player"),
	hPlayer_(-1), hArrow_(-1),
	PlayerState_(S_STOP),CameraState_(S_NORMALCAMERA),
	Direction_({ 0,0,0 }),BackCamera_({ 0,0,0 }), CameraPosition_({ 0,0,0 }), CameraTarget_({ 0,0,0 })
{
	
}

Player::~Player()
{

}

void Player::Initialize()
{
	//csv����p�����[�^�ǂݍ���
	std::string path = "CSVdata\\PlayerData.csv";
	SetcsvStatus(path);

	//csv����p�����[�^�ǂݍ���(Player�̂ݎg�����)
	SetCSVPlayer();

	//�e���f���̓ǂݍ���
	hPlayer_ = Model::Load("Model\\chara.fbx");
	assert(hPlayer_ >= 0);

	hArrow_ = Model::Load("Model\\AttackArrow2.fbx");
	assert(hArrow_ >= 0);

	InitArrow();
	SetStartPosition();
	
	//�����蔻��t����
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),HitParam_. ColliderSize_);
	this->AddCollider(collider);

	//�o�b�N�J�����̏����l���Z�b�g �����̈ʒu�ɌŒ�l�𑫂�
	XMVECTOR cameraAdd = XMLoadFloat3(&this->transform_.position_);
	BackCamera_ = { BackCameraPos + cameraAdd };

	//�J�����̃g�����X�t�H�[����ݒ�
	cameraTransform_ = this->transform_;

	//�J�����̈ʒu��ݒ� �����̈ʒu�ɌŒ�l�𑫂�
	CameraPosition_ = { this->transform_.position_ + CameraInit };

	//�J�����̏œ_��ݒ�
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };
}

void Player::Update()
{
	//�J�����̈ʒu���X�V
	//cameraTransform_.position_ = this->transform_.position_;

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
	case Player::S_WALLHIT:
		UpdateWallHit();
		break;
	case Player::S_STOP:
		UpdateStop();
		break;
	default:
		break;
	}

	//��ɐڐG���ĂȂ��Ȃ�l���X�V
	if(!(PlayerState_ == S_WALLHIT))
	{
		InvincibilityTimeCalclation();
	}

	//���t���[���e�̈ʒu���X�V
	ShadowSet();

	//���t���[���d�͂�����������
	CharacterGravity();

	//���t���[���J�����̍X�V
	CameraUpdate();

#ifdef _DEBUG
	if (Input::IsKeyDown(DIK_RETURN))
	{
		SetStartPosition();
	}
#endif

}

void Player::Draw()
{
	DrawCharacterModel(hPlayer_, this->transform_);

	ShadowDraw();

	if (PlayerState_ == S_CHARGE)
	{
		Model::SetAndDraw(hArrow_, this->MoveParam_.ArrowTransform_);
	}

#ifdef _DEBUG
	if (ImGui::TreeNode("PlayerStatus"))
	{
		DrawCharacterImGui();
	}
	
#endif

}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
	{
		//�G�̃m�b�N�o�b�N
		Enemy* pEnemy = (Enemy*)FindObject("Enemy");

		//�G-�����̃x�N�g���i����̔��ˁj,����-�G�̃x�N�g���i�����̔��ˁj���Ƃ�
		//����̃X�s�[�h�Ǝ����̃X�s�[�h���Ƃ�
		XMFLOAT3 getposition =  pEnemy->GetPosition();
		XMVECTOR enemyvector = XMLoadFloat3(&getposition);
		XMVECTOR playervector = XMLoadFloat3(&this->transform_.position_);
		float enemyaccele = pEnemy->GetAcceleration();

		Reflect(playervector, enemyvector, this->MoveParam_.Acceleration_, enemyaccele);

		PlayerState_ = S_HIT;
		SetHitEffect();

		//�J�����U��
		Camera::CameraShakeStart(cameraShakeTime);

		//�Ռ���
		Audio::Play(hSoundCollision_);
	}

	if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
            for (const std::string& arr : WallHitParam_.WireArray_)
			{
				if (pTarget->GetObjectName() == arr)
				{
					XMVECTOR normal = HitNormal(arr);
					WallReflect(normal);
					PlayerState_ = S_WALLHIT;
				}
			}
		}
	}

}

void Player::UpdateIdle()
{
	//------------------�L�[�{�[�h���͂̈ړ�------------------//
	if (Input::IsKey(DIK_UP))
	{
		Direction_.z += 1.0f;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z -= 1.0f;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		Direction_.x -= 1.0f;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		Direction_.x += 1.0f;
	}

	KeyBoradMove();

	//------------------�Q�[���p�b�h�X�e�B�b�N�̈ړ�------------------//

	//�R���g���[���[��|���������E�p�x���擾
	XMVECTOR controller = XMVectorSet(Input::GetPadStickL().x, Input::GetPadStickL().y, Input::GetPadStickL().z, 0.0f);

	//�R���g���[���Ŏ󂯂Ƃ����x�N�g����XY�Ȃ̂�
	//XZ�����̃x�N�g���ɒ���
	XMFLOAT3 controllfloat = { XMVectorGetX(controller) , 0.0f, XMVectorGetY(controller) };
	XMVECTOR SetController = { controllfloat.x, controllfloat.y , controllfloat.z };

	//�x�N�g���̒������擾���āA�|�������ǂ����𔻕�
	float length = XMVectorGetX(XMVector3Length(controller));

	if (length > Input::StickMicroTilt)
	{
		this->transform_.rotate_.y = RotateDirectionVector(SetController);
		MoveRotate();
		CharacterMove(SetController);
	}

	//�����̑O���x�N�g��(��]���������܂�)���X�V
	FrontVectorConfirm();


	//------------------�U����Ԃֈڍs------------------//

	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if (JumpParam_. IsOnGround_)
		{
			PlayerState_ = S_CHARGE;
		}
	}

	//------------------�W�����v------------------
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (JumpParam_. IsOnGround_)
		{
			SetJump();
		}
	}

	CameraControl();
}

void Player::UpdateCharge()
{
	Charging();
	SetArrow();
	this->MoveParam_.ArrowTransform_.rotate_.y = this->transform_.rotate_.y;
	SetChargingEffect("PaticleAssets\\circle_B.png");

	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (JumpParam_. IsOnGround_)
		{
			ChargeReset();
			SetJump();
			PlayerState_ = S_IDLE;
		}
	}

	if (Input::IsKey(DIK_LEFT) || Input::GetPadStickL().x < -Input::StickTilt)
	{
		this->transform_.rotate_.y -= KeyBoardRotateY;
	}
	if (Input::IsKey(DIK_RIGHT) || Input::GetPadStickL().x > Input::StickTilt)
	{
		this->transform_.rotate_.y += KeyBoardRotateY;
	}

	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		ChargeRelease();
		PlayerState_ = S_ATTACK;
	}

	FrontVectorConfirm();
	FastRotate();
	CameraControl();
}

void Player::UpdateAttack()
{
	Audio::Play(hSoundattack_);
	SetAttackLocusEffect();
	CharacterMove(MoveParam_.ForwardVector_);
	FrictionDeceleration();
	FastRotate();
	if (IsDashStop())
	{
		AccelerationStop();
		PlayerState_ = S_IDLE;
	}
}

void Player::UpdateHit()
{
	KnockBack();
	if (IsKnockBackEnd())
	{
		PlayerState_ = S_IDLE;
	}
}

void Player::UpdateWallHit()
{	
	KnockBack();

	if (IsKnockBackEnd())
	{
		PlayerState_ = S_IDLE;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			pBattleScene->PlusEnemyScore();
		}
	}
}

void Player::UpdateStop()
{
	//�����Ȃ�
}

void Player::SetJump()
{
	JumpParam_.IsOnGround_ = false;
	JumpParam_.JumpSpeed_ = Jumpheight;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
}

void Player::CameraControl()
{
#ifdef _DEBUG
	//�J�������㕔�Ɉړ�
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

	if(CameraState_ == S_NORMALCAMERA)
	{

		if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -Input::StickTilt)	//�J�������E�ړ�
		{
			cameraTransform_.rotate_.y -= cameraRotate;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= Input::StickTilt)
		{
			cameraTransform_.rotate_.y += cameraRotate;
		}

		if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -Input::StickTilt)	//�J�����㉺�ړ�
		{
			if (cameraTransform_.rotate_.x >= cameraUpperLimit)
			{
				cameraTransform_.rotate_.x = cameraUpperLimit;
			}
			else
			{
				cameraTransform_.rotate_.x += cameraRotate;
			}
		}
		if (Input::IsKey(DIK_S) || Input::GetPadStickR().y >= Input::StickTilt)
		{
			if (cameraTransform_.rotate_.x <= cameraLowerLimit)
			{
				cameraTransform_.rotate_.x = cameraLowerLimit;
			}
			else
			{
				cameraTransform_.rotate_.x -= cameraRotate;
			}
		}

		if (Input::IsKey(DIK_Z) || Input::IsPadButton(XINPUT_GAMEPAD_Y))//�J�����𐳖ʂɖ߂��i�����ɕω��Ȃ��j
		{
			cameraTransform_.rotate_.y = 0;
			cameraTransform_.rotate_.x = 0;
			this->transform_.rotate_.x = 0;
			this->transform_.rotate_.y = 0;
		}
	}
	else if (CameraState_ == S_DEBUGCAMERA)
	{

		cameraTransform_.rotate_.x = cameraDebugPos;
	}
}

void Player::CameraUpdate()
{
	//--------------�J�����Ǐ]--------------
	CameraTarget_ = { this->transform_.position_ };//�J�����̏œ_�͎��@�̈ʒu
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform_.rotate_.y));//�J�����̉�]�s��쐬(Y��)
	XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(cameraTransform_.rotate_.x));//�J�����̉�]�s��쐬(X��)
	XMMATRIX rotCamera = XMMatrixMultiply(rotX, rotY);
	BackCamera_ = XMVector3TransformCoord(BackCamera_, rotCamera);//�o�b�N�J�����̃x�N�g���ɂ�����
	XMVECTOR PlayerPosVec = XMLoadFloat3(&this->transform_.position_);//�v���C���[�̈ʒu���擾
	XMStoreFloat3(&CameraPosition_, PlayerPosVec + BackCamera_);//�v���C���[�̈ړ��x�N�g���ƃo�b�N�J���������Z

	//--------------�J�����U��--------------
	// �S�X�e�[�g���L
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	Camera::SetPosition(CameraPosition_);//�J�����̈ʒu���Z�b�g 
	Camera::SetTarget(CameraTarget_);//�J�����̏œ_���Z�b�g

	//if (!(PlayerState_ == Player::S_STOP))
	//{
		BackCamera_ = { BackCameraPos };//�o�b�N�J�����x�N�g�������Z�b�g
	//}
	//else
	//{
	//	cameraTransform_.rotate_.y = 0;//�J�n�O�E�I����̓J������Y��]�Œ�
	//}
}

void Player::KeyBoradMove()
{
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);

	if (!XMVector3Equal(move, XMVectorZero()))
	{
		move = XMVector3Normalize(move);
		this->transform_.rotate_.y = RotateDirectionVector(move);
		MoveRotate();
		CharacterMove(move);
	}

	//CharacterMoveRotate(move, this->transform_.rotate_.y);
	Direction_ = { 0,0,0 };//�Ō�ɐi�s�����̃��Z�b�g���t���[���s��
}

void Player::SetCSVPlayer()
{
	CsvReader csv;
	csv.Load("CSVdata\\PlayerData.csv");

	std::string only = "PlayerOnlyParam";
	if (csv.IsGetParamName(only))
	{
		std::vector<float> v = csv.GetParam(only);
		BackCameraPos = { v[i_backcameraX], v[i_backcameraY], v[i_backcameraZ] };
		KeyBoardRotateY = v[i_keyboardrotateY];
		cameraShakeTime = v[i_camerashaketime];
		Jumpheight = v[i_jumpheight];
		CameraInit = { v[i_camerainitx] ,v[i_camerainity] , v[i_camerainitz] };
		cameraRotate = v[i_camerarotate];
		cameraUpperLimit = v[i_cameraupperlimit];
		cameraLowerLimit = v[i_cameralowerlimit];
		cameraDebugPos = v[i_cameradebugPos];
	}
}

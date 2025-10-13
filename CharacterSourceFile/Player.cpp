#include "Player.h"
#include"../Engine/Model.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"
#include"../Engine/Camera.h"
#include"../Engine/SphereCollider.h"
#include"Enemy.h"

namespace {

	//csv�ǂݍ��ݎ��̃C���f�b�N�X
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

	//�ʏ�J�����̌Œ�ʒu
	//BackCamera�̒l�𖈃t���[�����̒l�ɂ���i�l���ς�葱����̂�h���j
	XMVECTOR BackCameraDefault = { 0.0f,0.0f,0.0f };

	//�`���[�W���̍��E���͎��̉�]��
	float ChargeRotateY = 0.0f;

	//�L�[�{�[�h���͎��ADirection_�ɉ��Z�����l
	float MoveValue = 0.0f;

	//�J�����̏������ʒu
	XMFLOAT3 CameraInit = { 0,0,0 };

	//���E���͎��̃J�����̉�]��
	float CameraRotate = 0.0f;

	//�㉺���͎��̃J�����̍����̍ō��l
	float CameraUpperLimit = 0.0f;

	//�㉺���͎��̃J�����̍����̍Œ�l
	float CameraLowerLimit = 0.0f;

	//�f�o�b�O�J������Ԏ��̃J�����̈ʒu
	XMFLOAT3 CameraDebugPos = { 0,0,0 };
}

Player::Player(GameObject* parent) 
	: Character(parent,"Player"),
	hPlayer_(-1), 
	PlayerState_(S_Stop),CameraState_(S_NormalCamera),
	ControllerID_(-1),Direction_({ 0,0,0 }),
	CameraPosition_({ 0,0,0 }), CameraTarget_({ 0,0,0 }),BackCamera_({ 0,0,0 })
{
	
}

Player::~Player()
{

}

void Player::Initialize()
{
}

void Player::Update()
{
}

void Player::Draw()
{
	//Character�N���X�̋��ʕ`��
	Character::Draw();

	//�������L�����N�^�[�̕`��
	blink_->DrawCharacterModel(hPlayer_, this->transform_);

	//�`���[�W���̂ݖ�󃂃f���`��
	if (PlayerState_ == S_Charge)
	{
		charge_->DrawArrow();
	}

}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	//�G�N���X,�v���C���[�ƐڐG�������̏���
	if (pTarget->GetObjectName() == "Enemy1" || pTarget->GetObjectName() == "Enemy2"
		|| pTarget->GetObjectName() == "Player1" || pTarget->GetObjectName() == "Player2")
	{
		//�ڐG�����L�����N�^�[�̖��O���擾
		std::string targetName = pTarget->GetObjectName();

		//���������Ώۂɉ��������ˏ���
		CollisionCharacter(targetName);

		//��e��ԂɂȂ�
		PlayerState_ = S_Hit;

		//�ڐG�G�t�F�N�g
		vfx_->SetHitEffect();

		//�J�����U��(�Z��)
		Camera::CameraShakeStart(Camera::GetShakeTimeShort());

		//�Ռ���
		Audio::Play(SoundParam_.hSoundCollision_);

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		rotate_->RotateXStop();

	}

	//�e��ɐڐG�������̏���
	if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		//���g����ɐڐG��Ԃł͂Ȃ� �����G��ԂłȂ��Ȃ瑱����
		if (!FenceHitParam_.IsInvincibility_ && !(PlayerState_ == S_FenceHit))
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

					//�v���C���[�̏�Ԃ���ɐڐG��Ԃɂ���
					PlayerState_ = S_FenceHit;

					//�J�����U��(�����炢�̒���)
					Camera::CameraShakeStart(Camera::GetShakeTimeMiddle());

					//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
					rotate_->RotateXStop();
				}
			}
		}
	}

}

void Player::PlayerRun()
{
	//Character�N���X�̋��ʏ���
	Character::Update();

	//���݂̏�Ԃɂ���čX�V�𕪂���
	switch (PlayerState_)
	{
	case Player::S_Idle:
		UpdateIdle();
		break;
	case Player::S_Charge:
		UpdateCharge();
		break;
	case Player::S_Attack:
		UpdateAttack();
		break;
	case Player::S_Hit:
		UpdateHit();
		break;
	case Player::S_FenceHit:
		UpdateFenceHit();
		break;
	case Player::S_Stop:
		UpdateStop();
		break;
	default:
		break;
	}

	//��ɐڐG��ԂłȂ���Ζ��G���Ԃ��X�V
	if (!(PlayerState_ == S_FenceHit))
	{
		InvincibilityTimeCalculation();
	}

	//���t���[���J�����̍X�V
	CameraUpdate();

#ifdef _DEBUG

	//�f�o�b�O���̂�Enter�L�[�ŏ����ʒu�ɖ߂�
	if (Input::IsKeyDown(DIK_RETURN))
	{
		SetStartPosition({0.0f, 0.0f, 0.0f});
	}
#endif

}

void Player::UpdateIdle()
{
	//�ʏ��� �ړ��E�W�����v�Ȃǂ����Ă�����

	//------------------�L�[�{�[�h���͂̈ړ�------------------//

	//�㉺���E�L�[�������ꂽ��e�����Ɉړ��ʂ����Z
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

	//�L�[�{�[�h�̓��͂����������ۂɈړ�
	KeyBoardMove();

	//------------------�Q�[���p�b�h�X�e�B�b�N�̈ړ�------------------//

	//�R���g���[���[����
	ControllerMove(ControllerID_);

	//------------------�`���[�W��Ԃֈڍs------------------//

	//SHIFT�L�[/B�{�^���������ꂽ��
	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT)
		|| Input::IsPadButtonDown(XINPUT_GAMEPAD_B, ControllerID_))
	{
		if (JumpParam_.IsOnGround_)
		{
			//�n��ɂ���Ȃ�`���[�W��Ԃֈڍs
			PlayerState_ = S_Charge;

			//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
			rotate_->RotateXStop();
		}
	}

	//------------------�W�����v------------------

	//SPACE�L�[/A�{�^���������ꂽ��
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A,ControllerID_))
	{
		if (JumpParam_.IsOnGround_)
		{
			//�n��ɂ���Ȃ�W�����v�J�n
			air_->SetJump();
		}
	}

	//�J��������
	CameraControl();
}

void Player::UpdateCharge()
{
	//�`���[�W��(TmpAccele�𗭂߂Ă�����) ���̏�ō��E��]�ł��邪�����Ȃ�

	//�����x�𗭂߂�
	charge_->Charging();

	//��󃂃f�����Z�b�g
	charge_->SetArrow();

	//��󃂃f���̈ʒu�����g�̉�]�ƍ��킹��
	MoveParam_.ArrowTransform_.rotate_.y = this->transform_.rotate_.y;
	
	//�`���[�W���̃G�t�F�N�g���o��
	vfx_->SetChargingEffect("ParticleAssets\\circle_B.png");

	//SPACE�L�[/A�{�^����������,�n��ɂ���Ȃ�
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, ControllerID_))
	{
		if (JumpParam_.IsOnGround_)
		{
			//���߂��`���[�W��0�ɂ���
			charge_->ChargeReset();

			//�W�����v�J�n
			air_->SetJump();

			//�ʏ��Ԃ֖߂�
			PlayerState_ = S_Idle;

			//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
			rotate_->RotateXStop();
		}
	}

	//���E�L�[/�X�e�B�b�N���|���ꂽ���]
	if (Input::IsKey(DIK_LEFT) || Input::GetPadStickL(ControllerID_).x < -Input::StickTilt)
	{
		this->transform_.rotate_.y -= ChargeRotateY;
	}
	if (Input::IsKey(DIK_RIGHT) || Input::GetPadStickL(ControllerID_).x > Input::StickTilt)
	{
		this->transform_.rotate_.y += ChargeRotateY;
	}

	//SHIFT�L�[/B�{�^���𗣂�����
	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT)
		|| Input::IsPadButtonUp(XINPUT_GAMEPAD_B, ControllerID_))
	{
		//�`���[�W���
		charge_->ChargeRelease();

		//�U����Ԃֈڍs
		PlayerState_ = S_Attack;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		rotate_->RotateXStop();
	}

	//����X��]
	rotate_->FastRotateX();

	//�J��������
	CameraControl();
}

void Player::UpdateAttack()
{
	//�U����� ���ʂ̕����Ɉړ�������s��

	//�U�����̃G�t�F�N�g���o��
	vfx_->SetAttackLocusEffect();

	//���ʃx�N�g���̕����Ɉړ�
	movement_->CharacterMove(MoveParam_.ForwardVector_);

	//���C�ʕ����x������
	movement_->FrictionDeceleration();

	//����X��]
	rotate_->FastRotateX();

	//�����ʂ�0�ɂȂ�����
	if (movement_->IsDashStop())
	{
		//�����I�ɉ����ʂ�0�ɂ���
		movement_->AccelerationStop();

		//�ʏ��Ԃ֖߂�
		PlayerState_ = S_Idle;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		rotate_->RotateXStop();
	}

	//�U��SE�Đ�
	Audio::Play(SoundParam_.hSoundattack_);
}

void Player::UpdateHit()
{
	//����ƐڐG������� ����s��

	//�m�b�N�o�b�N����
	KnockBack();

	//�m�b�N�o�b�N���鑬�x�����ȉ��Ȃ�ʏ��Ԃ֖߂�
	if (IsKnockBackEnd())
	{
		//�m�b�N�o�b�N���x��0�ɖ߂��Ă���
		KnockBackVelocityReset();

		//�ʏ��Ԃ֖߂�
		PlayerState_ = S_Idle;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		rotate_->RotateXStop();

		//�_�b�V�����̑��x���Z�b�g(�m�b�N�o�b�N�I�����_�Ń��Z�b�g)
		movement_->AccelerationStop();
	}
}

void Player::UpdateFenceHit()
{	
	//�_���[�W���󂯂��ƐڐG������� ����s��

	//�m�b�N�o�b�N����
	KnockBack();

	//�m�b�N�o�b�N���鑬�x�����ȉ��Ȃ�ʏ��Ԃ֖߂�
	if (IsKnockBackEnd())
	{
		//�m�b�N�o�b�N���x��0�ɖ߂��Ă���
		KnockBackVelocityReset();

		//�ʏ��Ԃ֖߂�
		PlayerState_ = S_Idle;

		//��ԑJ�ڂ̍ۂ͈�xx��]���X�g�b�v
		rotate_->RotateXStop();

		//�_�b�V�����̑��x���Z�b�g(�m�b�N�o�b�N�I�����_�Ń��Z�b�g)
		movement_->AccelerationStop();
	}
}

void Player::UpdateStop()
{
	//�������������Ȃ�
}

void Player::DrawImGui()
{
	//Debug����ImGui��ݒ�
#ifdef _DEBUG
	if (ImGui::TreeNode((objectName_ + " Status").c_str()))
	{
		//�L�����N�^�[�N���X���ʂ�ImGui���
		DrawCharacterImGui();
	}

	if (ImGui::TreeNode((objectName_ + " OnlyStatus").c_str()))
	{
		if (ImGui::TreeNode("PlayerInit"))
		{
			//�R���g���[���[ID
			ImGui::Text("ControllerID:%1d", ControllerID_);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Camera"))
		{
			//�J�����̍ŏI�I�Ȉʒu
			ImGui::InputFloat("CameraPositionX", &CameraPosition_.x, ZeroPointOne);
			ImGui::InputFloat("CameraPositionY", &CameraPosition_.y, ZeroPointOne);
			ImGui::InputFloat("CameraPositionZ", &CameraPosition_.z, ZeroPointOne);

			//�J�����̏œ_
			ImGui::InputFloat("CameraTargetX", &CameraTarget_.x, ZeroPointOne);
			ImGui::InputFloat("CameraTargetY", &CameraTarget_.y, ZeroPointOne);
			ImGui::InputFloat("CameraTargetZ", &CameraTarget_.z, ZeroPointOne);

			//�J�����̉�]��
			ImGui::InputFloat("CameraTransform.rotateX", &cameraTransform_.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("CameraTransform.rotateY", &cameraTransform_.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("CameraTransform.rotateZ", &cameraTransform_.rotate_.z, ZeroPointOne);

			//�J�����̌���ʒu
			XMFLOAT3 tmpBackCamera;
			XMStoreFloat3(&tmpBackCamera, BackCamera_);
			ImGui::InputFloat("BackCameraX", &tmpBackCamera.x, ZeroPointOne);
			ImGui::InputFloat("BackCameraY", &tmpBackCamera.y, ZeroPointOne);
			ImGui::InputFloat("BackCameraZ", &tmpBackCamera.z, ZeroPointOne);

			//�J�����ɖ��t���[���������Œ�ʒu
			XMFLOAT3 tmpBackCameraDefault;
			XMStoreFloat3(&tmpBackCameraDefault, BackCameraDefault);
			ImGui::InputFloat("BackCameraDefaultX", &tmpBackCameraDefault.x, ZeroPointOne);
			ImGui::InputFloat("BackCameraDefaultY", &tmpBackCameraDefault.y, ZeroPointOne);
			ImGui::InputFloat("BackCameraDefaultZ", &tmpBackCameraDefault.z, ZeroPointOne);

			//�f�o�b�O�J�������̃J�����ʒu
			ImGui::InputFloat("CameraDebugPosX", &CameraDebugPos.x, ZeroPointOne);
			ImGui::InputFloat("CameraDebugPosY", &CameraDebugPos.y, ZeroPointOne);
			ImGui::InputFloat("CameraDebugPosZ", &CameraDebugPos.z, ZeroPointOne);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Charge"))
		{
			//�`���[�W���Ƀv���C���[����]�������ۂ�1f���Ƃ̉�]��
			ImGui::InputFloat("ChargeRotateY", &ChargeRotateY, ZeroPointOne);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

#endif
}

void Player::PlayerInit(std::string _CSVpath, std::string _Modelpath)
{
	//csv����p�����[�^�ǂݍ���
	SetCSVStatus(_CSVpath);

	//csv����p�����[�^�ǂݍ���(Player�̂ݎg�����)
	SetCSVPlayer(_CSVpath);

	//���̃g�����X�t�H�[���̏�����
	InitArrow();

	//�����ʒu�ɃL�����N�^�[���Z�b�g
	movement_->InitStartPosition();

	//�����蔻��t����
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0, 0, 0), HitParam_.ColliderSize_);
	this->AddCollider(collider);

	//�o�b�N�J�����̏����l���Z�b�g �����̈ʒu�ɌŒ�l�𑫂�
	XMVECTOR cameraAdd = XMLoadFloat3(&this->transform_.position_);
	BackCamera_ = { BackCameraDefault + cameraAdd };

	//�J�����̃g�����X�t�H�[����ݒ�
	cameraTransform_ = this->transform_;

	//�J�����̈ʒu��ݒ� �����̈ʒu�ɌŒ�l�𑫂�
	CameraPosition_ = { this->transform_.position_ + CameraInit };

	//�J�����̏œ_��ݒ�
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

	//�L�����N�^�[���f���̓ǂݍ���
	hPlayer_ = Model::Load(_Modelpath);
	assert(hPlayer_ >= 0);
}

void Player::CameraControl()
{
	//�J�����𑀍삷��

#ifdef _DEBUG
	//�J�������㕔�Ɉړ�

	//Q�L�[����������f�o�b�O�J�����Ɉڍs(Debug�̎��̂�)
	//�ēx�����ƒʏ�J�����ɖ߂�
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

	//�ʏ�J����
	if(CameraState_ == S_NormalCamera)
	{
		//A�ED�L�[/�E�X�e�B�b�N�ŃJ������]
		if (Input::IsKey(DIK_A) || Input::GetPadStickR(ControllerID_).x <= -Input::StickTilt)
		{
			cameraTransform_.rotate_.y -= CameraRotate;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR(ControllerID_).x >= Input::StickTilt)
		{
			cameraTransform_.rotate_.y += CameraRotate;
		}

		//W�ES�L�[�ŃJ�����㉺�ړ�
		if (Input::IsKey(DIK_W) || Input::GetPadStickR(ControllerID_).y <= -Input::StickTilt)
		{
			//�J�����̏㉺�ړ��̏���ɂȂ�����ʒu�Œ�
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
			//�J�����̏㉺�ړ��̉����ɂȂ�����ʒu�Œ�
			if (cameraTransform_.rotate_.x <= CameraLowerLimit)
			{
				cameraTransform_.rotate_.x = CameraLowerLimit;
			}
			else
			{
				cameraTransform_.rotate_.x -= CameraRotate;
			}
		}

		//Z�L�[/X�EY�{�^���ŃJ�����̉�]���Z�b�g
		if (Input::IsKey(DIK_Z) ||
			Input::IsPadButton(XINPUT_GAMEPAD_Y, ControllerID_) || Input::IsPadButton(XINPUT_GAMEPAD_X, ControllerID_))
		{
			// �J�����̉�]�����Z�b�g
			cameraTransform_.rotate_.x = 0.0f;
			this->transform_.rotate_.x = 0.0f;
			
			// �L�����N�^�[��Y��]���J������Y��]�ɍ��킹��
			this->transform_.rotate_.y = cameraTransform_.rotate_.y;
		}
	}
	else if (CameraState_ == S_DebugCamera)
	{
		//�f�o�b�O�J�������͑��삵�Ȃ�
	}
}

void Player::CameraUpdate()
{
	//�J�����̈ʒu�E��]�ʂ̍X�V

	//--------------�J�����Ǐ]--------------

	//�ʏ�J�����̏ꍇ�̓v���C���[����ɃJ�������Z�b�g
	if (CameraState_ == S_NormalCamera)
	{
		//�J�����̏œ_�͎��@�̈ʒu
		CameraTarget_ = { this->transform_.position_ };

		//�J�����̉�]�s��쐬(X���EY��)
		//CameraControl()�œ�������cameraTransform_�����W�A�������A��]�s��ɂ���
		XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraTransform_.rotate_.y));
		XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(cameraTransform_.rotate_.x));

		//�����̉�]�s�����Z
		XMMATRIX rotCamera = XMMatrixMultiply(rotX, rotY);

		//�쐬���������̉�]�s����o�b�N�J�����̃x�N�g���ɏ�Z
		BackCamera_ = XMVector3TransformCoord(BackCamera_, rotCamera);

		//�v���C���[�̈ʒu���擾
		XMVECTOR PlayerPosVec = XMLoadFloat3(&this->transform_.position_);

		//�v���C���[�̈ړ��x�N�g���ƃo�b�N�J���������Z
		XMStoreFloat3(&CameraPosition_, PlayerPosVec + BackCamera_);
	}
	//�f�o�b�O�J�����̏ꍇ�͐^�ォ�猩���낷�悤�ɃZ�b�g
	else if (CameraState_ == S_DebugCamera)
	{
		//�J�����̈ʒu��CameraUpdate�̌Œ�ʒu�ɃZ�b�g
		CameraPosition_ = CameraDebugPos;

		//�J�����̏œ_���X�e�[�W���S(���_)�ɃZ�b�g
		CameraTarget_ = { 0,0,0 };
	}

	//--------------�J�����U��--------------
	// �S�X�e�[�g���L

	//�J�����̐U���ʂ��J�����ʒu�ɉ��Z(�U���ʂ�0�ł��s��)
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	//�o�b�N�J�����x�N�g�������Z�b�g(DEBUGCAMERA�ł͎g�p���Ȃ�)
	BackCamera_ = { BackCameraDefault };
	
	//�J�����̈ʒu�E�œ_�̃Z�b�g��GameView����s��
}

void Player::KeyBoardMove()
{
	//�L�[�{�[�h���͎��̈ړ��E��]�v�Z

	//�L�[�{�[�h���������ʂ�1�̉��̈ړ��x�N�g���ɕϊ�
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);

	//�L�[��������(�����ĂȂ���Ԃ�0�x�N�g���Ȃ̂ŏ������Ȃ�)
	if (!XMVector3Equal(move, XMVectorZero()))
	{
		//���͂��ꂽ���ړ�����
		PlayerMove(move);
	}

	//�Ō�ɐi�s�����̃��Z�b�g���t���[���s��
	Direction_ = { 0,0,0 };
}

void Player::ControllerMove(int _PadID)
{
	//�R���g���[���[ID���}�C�i�X(����������Ă��Ȃ�)�Ȃ珈�����Ȃ�
	if (_PadID < 0)
	{
		return;
	}

	//�R���g���[���[��|���������E�p�x���擾
	XMVECTOR controller = XMVectorSet(Input::GetPadStickL(_PadID).x,
		Input::GetPadStickL(_PadID).y, Input::GetPadStickL(_PadID).z, 0.0f);

	//�R���g���[���Ŏ󂯂Ƃ����x�N�g����XY�Ȃ̂�
	//XZ�����̃x�N�g���ɒ���
	XMFLOAT3 controllfloat = { XMVectorGetX(controller) , 0.0f, XMVectorGetY(controller) };
	XMVECTOR SetController = { controllfloat.x, controllfloat.y , controllfloat.z };

	//�x�N�g���̒������擾���āA�|�������ǂ����𔻕�
	float length = XMVectorGetX(XMVector3Length(controller));

	//�����ł��X�e�B�b�N���X������
	if (length > Input::StickMicroTilt)
	{
		//���͂��ꂽ���ړ�����
		PlayerMove(SetController);
	}
}

void Player::PlayerMove(XMVECTOR _move)
{
	//�L�[�{�[�h�E�R���g���[���[���͎��̈ړ��E��]�v�Z

	//�ړ������x�N�g����0�Ȃ牽�����Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if (XMVector3Equal(_move, XMVectorZero()))
	{
		return;
	}

	//�ړ������x�N�g���𐳋K��
	_move = XMVector3Normalize(_move);

	//�J������Y��]�������ړ��x�N�g������] 
	//�J������Y����]�ʂ��s��ɂ���
	float cameraY = cameraTransform_.rotate_.y;
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(cameraY));

	//���̈ړ��x�N�g�����J������Y����]�ʂŕό`
	_move = XMVector3TransformCoord(_move, rotY);

	//�R���g���[���[���̓x�N�g������y����]�ʂ��v�Z
	this->transform_.rotate_.y = rotate_->RotateDirectionVector(_move);

	//�R���g���[���[���̓x�N�g����n���A���ۂɈړ�����
	movement_->CharacterMove(_move);

	//�L�����N�^�[��X��]
	rotate_->MoveRotateX();
}

void Player::CollisionCharacter(std::string _name)
{
	//���g�̈ʒu��XMVECTOR�^�Ƃ��Đ�ɕۊǂ���
	XMVECTOR PlayerVector = XMLoadFloat3(&this->transform_.position_);

	//�ڐG����̉����ʂ�ۊǂ���ϐ���p��
	float TargetSpeed = 0.0f;

	//�ڐG����̈ʒu�ƕϊ��p��XMVECTOR��p��
	XMFLOAT3 TargetPos = {};
	XMVECTOR TargetVector = {};

	//�ڐG��������̖��O���擾���镶�����p��
	std::string TargetName = "";

	if (_name == "Enemy1" || _name == "Enemy2")
	{
		//�G�N���X�̃C���X�^���X���擾
		Enemy* pEnemy = (Enemy*)FindObject(_name);
		assert(pEnemy != nullptr);

		//�G�̈ʒu�����XMVECTOR�^�ɂ���
		TargetPos = pEnemy->GetPosition();
		TargetVector = XMLoadFloat3(&TargetPos);

		//����̃X�s�[�h���擾
		TargetSpeed = pEnemy->GetAcceleration();

		//����̖��O���擾
		TargetName = pEnemy->GetObjectName();
	
	}
	else if (_name == "Player1" || _name == "Player2")
	{
		//�v���C���[�̃C���X�^���X���擾
		Player* pPlayer = (Player*)FindObject(_name);
		assert(pPlayer != nullptr);

		//�v���C���[�̈ʒu�����XMVECTOR�^�ɂ���
		TargetPos = pPlayer->GetPosition();
		TargetVector = XMLoadFloat3(&TargetPos);

		//����̃X�s�[�h���擾
		TargetSpeed = pPlayer->GetAcceleration();

		//����̖��O���擾
		TargetName = _name;
	}
	else
	{
		return;
	}

	//���ˏ������s��(�����̈ʒu�x�N�g��,����̈ʒu�x�N�g��,�����̉����x,����̉����x,�ڐG����̖��O)
	Reflect(PlayerVector, TargetVector, MoveParam_.Acceleration_, TargetSpeed, TargetName);
}

void Player::SetCSVPlayer(std::string _path)
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load(_path);

	//csv�t�@�C���̊e0��ڂ̕�������擾
	std::string only = "PlayerOnlyParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> OnlyData = GetCSVReadData(csv, only);
		
	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	BackCameraDefault = { OnlyData[i_BackCameraX], OnlyData[i_BackCameraY], OnlyData[i_BackCameraZ] };
	ChargeRotateY = OnlyData[i_ChargeRotateY];
	MoveValue = OnlyData[i_MoveValue];
	CameraInit = { OnlyData[i_CameraInitX] ,OnlyData[i_CameraInitY] , OnlyData[i_CameraInitZ] };
	CameraRotate = OnlyData[i_CameraRotate];
	CameraUpperLimit = OnlyData[i_CameraUpperlimit];
	CameraLowerLimit = OnlyData[i_CameraLowerlimit];
	CameraDebugPos = { OnlyData[i_CameraDebugWidth],OnlyData[i_CameraDebugHeight],OnlyData[i_CameraDebugDepth] };
}

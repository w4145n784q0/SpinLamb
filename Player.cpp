#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"Engine/VFX.h"
#include"Engine/SceneManager.h"
#include"BattleScene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include"Enemy.h"

namespace {
	XMVECTOR BackCameraPos = { 0,3,-10,0 };//BackCamera�̒l�͕ς�邪���t���[�����̒l�ɂ���i�l���ς�葱����̂�h���j
	const float KeyBoardRotateY = 1.0f;
	int blinkTimer = 0;
	const int blink = 20;
	const float cameraShakeTime = 0.3f;
	const float jumpheight = 1.8f;

	const float cameraInitX = 0.0f;
	const float cameraInitY = 1.0f;
	const float cameraInitZ = -8.0f;

	const float cameraRotate = 2.5f;
	const float cameraUpperLimit = 60.0f;
	const float cameraLowerLimit = -10.0f;
	const float cameraDebugPos = 90.0f;
}

Player::Player(GameObject* parent) 
	: Character(parent,"Player"),
	hPlayer_(-1), /*hAttackArrow_(-1),*/ hShadow_(-1), hCollisionSound_(-1),
	PlayerState_(S_MAX),CameraState_(S_NORMALCAMERA),
	Direction_({ 0,0,0 }),BackCamera_(BackCameraPos)
{
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x + cameraInitX
		,this->transform_.position_.y + cameraInitY, this->transform_.position_.z + cameraInitZ };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };
}

Player::~Player()
{
	
}

void Player::Initialize()
{
	std::string path = "CSVdata\\PlayerData.csv";
	SetcsvStatus(path);

	hPlayer_ = Model::Load("Model\\chara2.fbx");
	assert(hPlayer_ >= 0);

	ShadowInit();

	hCollisionSound_ = Audio::Load("Sound\\maou_se_battle15.wav");
	assert(hCollisionSound_ >= 0);

	SetStartPosition();
	//ArrowTransform_.rotate_.y = 180.0f;
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),ColliderSize_);
	this->AddCollider(collider);
}

void Player::Update()
{
	cameraTransform_.position_ = this->transform_.position_;

	ShadowSet();

	switch (PlayerState_)
	{
	case Player::S_IDLE:
		UpdateIdle();
		break;
	case Player::S_HIT:
		UpdateHit();
		break;
	case Player::S_CHARGE:
		UpdateCharge();
		break;
	case Player::S_ATTACK:
		UpdateAttack();
		break;
	case Player::S_WALLHIT:
		UpdateWallHit();
		break;
	default:
		break;
	}

	if(!IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
	{
		if (IsOutsideStage(this->transform_.position_))
		{
			WallHit();
			PlayerState_ = S_WALLHIT;
		}
	}

	InvincibilityTimeCalclation();

	CharacterGravity();

	//�J�����̍X�V
	CameraUpdate();

	//degug
	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}

}

void Player::Draw()
{
	if (IsInvincibility_)
	{			
		if (++blinkTimer > blink) {

			blinkTimer = 0;	
			Model::SetTransform(hPlayer_, this->transform_);
			Model::Draw(hPlayer_);
		}
	}
	else
	{
		Model::SetTransform(hPlayer_, this->transform_);
		Model::Draw(hPlayer_);
	}

	ShadowDraw();

	/*if (PlayerState_ == S_CHARGE)
	{
		Model::SetTransform(hAttackArrow_, ArrowTransform_);
		Model::Draw(hAttackArrow_);
	}*/

#ifdef _DEBUG
	ImGui::Text("PositionX:%.3f", this->transform_.position_.x);
	ImGui::Text("PositionY:%.3f", this->transform_.position_.y);
	ImGui::Text("PositionZ:%.3f", this->transform_.position_.z);

	//ImGui::Text("shadow:%3f", shadow);

	//ImGui::Text("camera y :%.3f", CameraPosition_.y);
	//ImGui::Text("camera x :%.3f", CameraPosition_.x);
	

	ImGui::Text("x:%3f", Input::GetPadStickL().x);
	ImGui::Text("y:%3f", Input::GetPadStickL().y);
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

		Reflect(playervector, enemyvector, this->Acceleration_, enemyaccele);

		PlayerState_ = S_HIT;
		SetHitEffect();

		//�J�����U��
		Camera::CameraShakeStart(cameraShakeTime);

		//�Ռ���
		Audio::Play(hCollisionSound_);
	}

	if (pTarget->GetObjectName() == "Fence")
	{
		if(!IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallHit();
			PlayerState_ = S_WALLHIT;
		}
	}
}

void Player::UpdateIdle()
{
	//------------------�L�[�{�[�h���͂̈ړ�------------------//
	if (Input::IsKey(DIK_UP))
	{
		Direction_.z = 1.0;
		MoveRotate();
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z = -1.0;
		MoveRotateReverse();
	}
	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= KeyBoardRotateY;
		cameraTransform_.rotate_.y -= KeyBoardRotateY;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += KeyBoardRotateY;
		cameraTransform_.rotate_.y += KeyBoardRotateY;
	}

	KeyBoradMove();

	//------------------�Q�[���p�b�h�X�e�B�b�N�̈ړ�------------------//

	//�R���g���[���[��|���������E�p�x���擾
	XMVECTOR controller = XMVectorSet(Input::GetPadStickL().x, Input::GetPadStickL().y, Input::GetPadStickL().z, 0.0f);

	//�R���g���[���Ŏ󂯂Ƃ����x�N�g����XY�Ȃ̂�
	//XZ�����̃x�N�g���ɒ���
	XMFLOAT3 controllfloat = { XMVectorGetX(controller) , 0, XMVectorGetY(controller) };
	XMVECTOR SetController = { controllfloat.x, controllfloat.y , controllfloat.z };

	//�x�N�g���̒������擾���āA�|�������ǂ����𔻕�
	float length = XMVectorGetX(XMVector3Length(controller));

	if (length > stickMicrotilt)
	{
		//�R���g���[���[�����ƑO�����x�N�g���̊O�ϋ��߂�i�j
		XMVECTOR cross = XMVector3Cross(SetController, FrontDirection_);

		//Y�O�ς��Ƃ�+��-���œ|����]���������߂�
		float crossY = XMVectorGetY(cross);

		//���ʃx�N�g���Ƃ̃��W�A���p���Ƃ�
		XMVECTOR r = XMVector3AngleBetweenVectors(SetController, FrontDirection_);

		//���W�A���p�x���擾
		float angle = XMVectorGetX(r);

		//�f�B�O���[�p�ɒ���
		float angleDeg = XMConvertToDegrees(angle);

		// crossY�̐����ɉ����ĉ�]�p�x�̕�����ς���
		if (crossY > 0)
		{
			angleDeg = -angleDeg;
		}

		this->transform_.rotate_.y = angleDeg;
		cameraTransform_.rotate_.y = angleDeg;
		MoveRotate();
		CharacterMove(SetController);
	}

	//�����̑O���x�N�g��(��]���������܂�)���X�V
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, FrontDirection_);


	//------------------�U����Ԃֈڍs------------------//

	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if (IsOnGround_)
		{
			PlayerState_ = S_CHARGE;
		}
	}

	//------------------�W�����v------------------
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (IsOnGround_)
		{
			SetJump();
		}
	}

	CameraControl();

	Direction_ = { 0,0,0 };//�Ō�ɐi�s�����̃��Z�b�g���t���[���s��
}

void Player::UpdateHit()
{
	KnockBack();
	if (IsKnockBackEnd())
	{
		PlayerState_ = S_IDLE;
	}
}

void Player::UpdateCharge()
{
	SetChargingEffect("PaticleAssets\\circle_B.png");

	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (IsOnGround_)
		{
			AccelerationStop();
			SetJump();
			PlayerState_ = S_IDLE;
		}
	}

	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		PlayerState_ = S_ATTACK;
	}

	Charging();
	FastRotate();
	CameraControl();
}

void Player::UpdateAttack()
{
	SetAttackLocusEffect();

	Deceleration();
	Direction_.z = 1.0;
	FastRotate();
	if (IsDashStop())
	{
		AccelerationStop();
		PlayerState_ = S_IDLE;
	}

	KeyBoradMove();
}

void Player::UpdateWallHit()
{	
	KnockBack();

	if (IsKnockBackEnd())
	{
		PlayerState_ = S_IDLE;
		IsInvincibility_ = true;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			pBattleScene->PlusEnemyScore();
		}
	}
}

void Player::SetJump()
{
	IsOnGround_ = false;
	JumpSpeed_ = jumpheight;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
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

		if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -sticktilt)	//�J�������E�ړ�
		{
			cameraTransform_.rotate_.y -= cameraRotate;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= sticktilt)
		{
			cameraTransform_.rotate_.y += cameraRotate;
		}

		if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -sticktilt)	//�J�����㉺�ړ�
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
		if (Input::IsKey(DIK_S) || Input::GetPadStickR().y >= sticktilt)
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
	XMStoreFloat3(&CameraPosition_, NewPositon_ + BackCamera_);//�v���C���[�̈ړ��x�N�g���ƃo�b�N�J���������Z

	//--------------�J�����U��--------------
	// �S�X�e�[�g���L
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	Camera::SetPosition(CameraPosition_);//�J�����̈ʒu���Z�b�g 
	Camera::SetTarget(CameraTarget_);//�J�����̏œ_���Z�b�g
	BackCamera_ = { BackCameraPos };//�o�b�N�J�����x�N�g�������Z�b�g
}

void Player::KeyBoradMove()
{
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
	CharacterMoveRotate(move, this->transform_.rotate_.y);
}

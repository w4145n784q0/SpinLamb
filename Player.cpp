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
	int blinkTimer = 0;
	const int blink = 20;
}

Player::Player(GameObject* parent) 
	: Character(parent,"Player"),
	hPlayer_(-1), /*hAttackArrow_(-1),*/ hShadow_(-1), hCollisionSound_(-1),
	/*pGround_(nullptr),*/PlayerState_(S_MAX),CameraState_(S_NORMALCAMERA),
	

	Direction_({ 0,0,0 }),BackCamera_(BackCameraPos)
{
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x ,this->transform_.position_.y + 1, this->transform_.position_.z - 8 };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };

	//srand((unsigned)time(NULL));
}

Player::~Player()
{
	Release();
}

void Player::Initialize()
{
	std::string path = "CSVdata\\PlayerData.csv";
	SetcsvStatus(path);

	hPlayer_ = Model::Load("Model\\chara2.fbx");
	assert(hPlayer_ >= 0);

	//hAttackArrow_ = Model::Load("Model\\AttackArrow.fbx");
	//assert(hAttackArrow_ >= 0);

	ShadowInit();

	hCollisionSound_ = Audio::Load("Sound\\maou_se_battle15.wav");
	assert(hCollisionSound_ >= 0);

	SetStartPosition();
	//ArrowTransform_.rotate_.y = 180.0f;

	//pGround_ = (Ground*)FindObject("Ground");
	
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

	if(!IsInvincibility_ && !PlayerState_ == S_WALLHIT)
	{
		if (IsOutsideStage(this->transform_.position_))
		{
			WallHit();
			PlayerState_ = S_WALLHIT;
		}
	}

	InvincibilityTimeCalclation();

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
	//Model::SetTransform(hPlayer_, this->transform_);
	//Model::Draw(hPlayer_);

	ShadowDraw();

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
	
	XMFLOAT3 tmp;
    XMStoreFloat3(&tmp, MoveDirection_);

	ImGui::Text("front.x:%3f", (float)tmp.x);
	ImGui::Text("front.y:%3f", (float)tmp.y);
	ImGui::Text("front.z:%3f", (float)tmp.z);

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
		//


		//�G�̈ʒu-���@�̈ʒu���v�Z�i�G�̔��ˁj
		//�P�ʃx�N�g���ɂ���
		XMFLOAT3 Enemydirection = pEnemy->GetPosition() - this->transform_.position_;
		XMVECTOR EnemynormalDirection = XMVector3Normalize(XMLoadFloat3(&Enemydirection));

		//���@�̈ʒu-�G�̈ʒu���v�Z�i���@�̔��ˁj
		//�P�ʃx�N�g���ɂ���
		XMFLOAT3 Playerdirection = this->transform_.position_ - pEnemy->GetPosition();
		XMVECTOR PlayernormalDirection = XMVector3Normalize(XMLoadFloat3(&Playerdirection));

		//�v���C���[�̏Փˎ�����
		//�v���C���[:�ʏ� �G:�ʏ� �o�����͂������(�y�x)
		//�v���C���[:�ʏ� �G:�U�� �v���C���[��傫���͂���
		//�v���C���[:�_�b�V�� �G:�U�� �G���͂��� �v���C���[�͕����x�N�g��(�G�̈ʒu-���@�̈ʒu)�ɑ΂����������Ɉړ��i���ʂ���Ԃ�������v���C���[�͒�~
		//�v���C���[:�_�b�V�� �G:�ʏ� �G��傫���͂���
		//�v���C���[�͕����x�N�g��(�G�̈ʒu-���@�̈ʒu)�ɑ΂����������Ɉړ��i���ʂ���Ԃ�������v���C���[�͒�~

		bool IsEnemyAttack = pEnemy->GetStateAttack();

		if (IsEnemyAttack)//�G:�U��
		{
			if (IsAttackState())//�v���C���[:�U��
			{
				//�G�̃m�b�N�o�b�N����
				pEnemy->PlayerReflect(EnemynormalDirection, true);
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
			else//�v���C���[:�ʏ�
			{
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
			//�q�b�g�G�t�F�N�g
			SetHitEffect();
		}
		else//�G:�ʏ�
		{
			if (IsAttackState())//�v���C���[:�U��
			{
				//�G�̃m�b�N�o�b�N����
				pEnemy->PlayerReflect(EnemynormalDirection,true);
				//�q�b�g�G�t�F�N�g
				SetHitEffect();
			}
			else//�v���C���[:�ʏ�
			{
				pEnemy->PlayerReflect(EnemynormalDirection, false);
				EnemyReflect(PlayernormalDirection, IsEnemyAttack);
			}
		}

		//�J�����U��
		Camera::CameraShakeStart(0.3f);

		//�Ռ���
		Audio::Play(hCollisionSound_);

		Acceleration_ = 0;
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
		this->transform_.rotate_.x += MoveRotateX;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		Direction_.z = -1.0;
		this->transform_.rotate_.x -= MoveRotateX;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
	}

	//------------------�Q�[���p�b�h�X�e�B�b�N�̈ړ�------------------//

	////�O�������Ɉړ�
	//if (Input::GetPadStickL().y >= 0.5 /*&& Input::GetPadStickL().x <= 0.5 && Input::GetPadStickL().x >= -0.5*/)
	//{
	//	Direction_.z = -1.0;
	//	this->transform_.rotate_.x -= MoveRotateX;
	//}

	////��������Ɉړ�
	//if (Input::GetPadStickL().y <= -0.5 /*&& Input::GetPadStickL().x >= 0.5 && Input::GetPadStickL().x <= -0.5*/)
	//{
	//	Direction_.z = 1.0;
	//	this->transform_.rotate_.x += MoveRotateX;
	//}

	////�O�i&����]
	//if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x <= -0.1)
	//{
	//	//Direction_.z = -1.0;
	//	this->transform_.rotate_.y -= 1;
	//	this->transform_.rotate_.x -= MoveRotateX;
	//	cameraTransform_.rotate_.y -= 1;
	//}

	////�O�i&�E��]
	//if (Input::GetPadStickL().y >= 0.5 && Input::GetPadStickL().x >= 0.1)
	//{
	//	//Direction_.z = -1.0;
	//	this->transform_.rotate_.y += 1;
	//	this->transform_.rotate_.x += MoveRotateX;
	//	cameraTransform_.rotate_.y += 1;
	//}

	////����]����
	//if (Input::GetPadStickL().x <= -0.25 && Input::GetPadStickL().y <= 0.25)
	//{
	//	this->transform_.rotate_.y -= 1;
	//	cameraTransform_.rotate_.y -= 1;
	//}
	////�E��]����
	//if (Input::GetPadStickL().x >= 0.25 && Input::GetPadStickL().y <= 0.25)
	//{
	//	this->transform_.rotate_.y += 1;
	//	cameraTransform_.rotate_.y += 1;
	//}



	//--------------�_�b�V���֌W--------------

	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if (IsOnGround_)
		{
			//SetChargeEffect();
			PlayerState_ = S_CHARGE;
		}
	}
	//�}�E�X�p�����x�N�g��
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
	CharacterMoveRotate(move,this->transform_.rotate_.y);

	//�R���g���[���[��|���������E�p�x���擾
	XMVECTOR controller = XMVectorSet(Input::GetPadStickL().x, Input::GetPadStickL().y, Input::GetPadStickL().z, 0.0f);

	//�R���g���[���Ŏ󂯂Ƃ����x�N�g����XY�Ȃ̂�
	//XZ�����̃x�N�g���ɒ���
	XMFLOAT3 controllfloat = { XMVectorGetX(controller) , 0, XMVectorGetY(controller) };
	XMVECTOR SetController = { controllfloat.x, controllfloat.y , controllfloat.z };

	//�x�N�g���̒������擾���āA�|�������ǂ����𔻕�
	float length = XMVectorGetX(XMVector3Length(controller));

	//if(length > 0.01f)
	//{
	//	//��ɊO�ϋ��߂�
	//	XMVECTOR cross = XMVector3Cross(SetController,FrontDirection_);
	//	
	//	//Y�O�ς��Ƃ�+��-���œ|����]���������߂�
	//	float crossY = XMVectorGetY(cross);

	//	if (crossY > 0.0)
	//	{
	//		//���ʃx�N�g���Ƃ̃��W�A���p���Ƃ�
	//		XMVECTOR r = XMVector3AngleBetweenVectors(SetController, FrontDirection_);

	//		//���W�A���p�x���擾
	//		float angle = XMVectorGetX(r);

	//		//�f�B�O���[�p�ɒ���
	//		float angleDeg = XMConvertToDegrees(angle);

	//		//Y���W�ɐݒ�
	//		this->transform_.rotate_.y = angleDeg;
	//	}
	//	//�O��Y��0�ȉ��Ȃ�E����(���v����)
	//	else if (crossY < 0.0)
	//	{
	//		//���ʃx�N�g���Ƃ̃��W�A���p���Ƃ�
	//		XMVECTOR r = XMVector3AngleBetweenVectors(SetController, FrontDirection_);

	//		//���W�A���p�x���擾
	//		float angle = XMVectorGetX(r);

	//		//�f�B�O���[�p�ɒ���
	//		float angleDeg = XMConvertToDegrees(angle);

	//		//Y���W�ɐݒ�
	//		this->transform_.rotate_.y = angleDeg;
	//	}
	//	//CharacterMoveRotate(controller,angleDeg);
	//}
	//else
	//{
	//}


	//�����̑O���x�N�g��(��]���������܂�)���X�V
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, FrontDirection_);

	//�v���C���[�̐��ʃx�N�g�����X�V
	//�����̑O���x�N�g��(��]���������܂�)
	/*ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, PlayerFrontDirection);
	XMFLOAT3 rot = { 0,0,0 };
	XMStoreFloat3(&rot, ForwardVector_);
	PlayerFront = { transform_.position_ + rot };
	PlayerFront.z += 1.0;
	ArrowTransform_.position_ = PlayerFront;*/


	//�W�����v
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (IsOnGround_)
		{
			IsOnGround_ = false;
			JumpSpeed_ = 1.8f;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
		}
	}

	CharacterGravity();

	CameraControl();


	Direction_ = { 0,0,0 };//�Ō�ɐi�s�����̃��Z�b�g���t���[���s��
}

void Player::UpdateHit()
{
	KnockBack();
	CharacterGravity();

	if (KnockBack_Velocity_.x <= KnockBackEnd_ || KnockBack_Velocity_.z <= KnockBackEnd_)
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
			Acceleration_ = 0.0f;
			PlayerState_ = S_IDLE;
			IsOnGround_ = false;
			JumpSpeed_ = 1.8f;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
		}
	}


	if (Input::IsKey(DIK_LEFT))
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
		//ArrowTransform_.rotate_.y -= 1;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
		//ArrowTransform_.rotate_.y += 1;
	}

	//����]����
	if (Input::GetPadStickL().x <= -0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y -= 1;
		cameraTransform_.rotate_.y -= 1;
		//ArrowTransform_.rotate_.y -= 1;
	}
	//�E��]����
	if (Input::GetPadStickL().x >= 0.8 && Input::GetPadStickL().y <= 0.8)
	{
		this->transform_.rotate_.y += 1;
		cameraTransform_.rotate_.y += 1;
		//ArrowTransform_.rotate_.y += 1;
	}

	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		//VFX::End(hChargeEmit_);
		//SetAttackLocusEffect();
		//SetAttackAuraEffect();
		PlayerState_ = S_ATTACK;
	}

	Charging();

	this->transform_.rotate_.x += FastRotateX;
	CameraControl();
}

void Player::UpdateAttack()
{
	SetAttackLocusEffect();

	Acceleration_ -= AcceleValue_;
	Direction_.z = 1.0;
	this->transform_.rotate_.x -= FastRotateX;
	if (Acceleration_ <= 0)
	{
		Acceleration_ = 0.0f;
		PlayerState_ = S_IDLE;
	}

	//PlayerMove();

	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
	CharacterMoveRotate(move,this->transform_.rotate_.y);
}

void Player::UpdateWallHit()
{	
	KnockBack();
	CharacterGravity();

	if (KnockBack_Velocity_.x <= KnockBackEnd_ || KnockBack_Velocity_.z <= KnockBackEnd_)
	{
		PlayerState_ = S_IDLE;
		IsInvincibility_ = true;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			//pBattleScene->SetPlayerHp(CharacterLife_);
			pBattleScene->PlusEnemyScore();
		}
	}

	//if (--deadTimer_ < 0)
	//{
	//	//BossBattleScene* pBossBattleScene = (BossBattleScene*)FindObject("BossBattleScene");
	//	//pBossBattleScene->DeadCountPlus();

	//	CharacterLife_--;
	//	deadTimer_ = deadTimerValue;
	//	PlayerState_ = S_IDLE;
	//	IsInvincibility_ = true;

	//	SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
	//	if (pSM->IsBattleScene())
	//	{
	//		BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
	//		pBattleScene->SetPlayerHp(CharacterLife_);
	//	}
	//	else
	//	{

	//	}

	//	//SetStartPosition();
	//}
}

//void Player::PlayerMove()
//{
//	//�v���C���[��y��]�����W�A�������čs���
//	XMMATRIX playerRot = XMMatrixRotationY(XMConvertToRadians(this->transform_.rotate_.y));
//
//	//�v���C���[�̐i�s�����x�N�g�����쐬
//	XMVECTOR PrevDir = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
//
//	//�����x�N�g������]�s��ŕϊ�
//	PrevDir = XMVector3TransformCoord(PrevDir, playerRot);
//
//	//�P�ʃx�N�g�������A�ړ��������m��
//	XMVECTOR norm = XMVector3Normalize(PrevDir);
//
//	//�ړ��x�N�g�� = �ړ����� * (�����x + �����x) * �ړ��ʂ̃X�P�[�����O(60fps�̂���0.016f)
//	//�ړ��x�N�g��������
//	XMVECTOR MoveVector = XMVectorScale(norm, (Velocity_ + Acceleration_) * DeltaTime);
//
//	//���݈ʒu�ƈړ��x�N�g�������Z
//	XMVECTOR PrevPos = PlayerPosition_;
//	NewPos_ = PrevPos + MoveVector;
//
//	//��O�łȂ���Έʒu�X�V 
//	XMFLOAT3 f;
//	XMStoreFloat3(&f, NewPos_);
//	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
//	{
//		XMStoreFloat3(&this->transform_.position_, NewPos_);
//	}
//}

void Player::Blown()
{
	this->transform_.rotate_.x -= MoveRotateX;

	//���t���[�����x������
	KnockBack_Velocity_.x *= DecelerationRate_;
	KnockBack_Velocity_.z *= DecelerationRate_;

	//�ʒu = �ʒu + ���� * ���x
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	TmpPos.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;

	NewPositon_ = XMLoadFloat3(&TmpPos);

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 f;
	XMStoreFloat3(&f, NewPositon_);
	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPositon_);
	}
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

		if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -0.7)	//�J�������E�ړ�
		{
			cameraTransform_.rotate_.y -= 2.5;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= 0.7)
		{
			cameraTransform_.rotate_.y += 2.5;
		}

		if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -0.7)	//�J�����㉺�ړ�
		{
			if (cameraTransform_.rotate_.x >= 60.0f)
			{
				cameraTransform_.rotate_.x = 60.0f;
			}
			else
			{
				cameraTransform_.rotate_.x += 2.5;
			}
		}
		if (Input::IsKey(DIK_S) || Input::GetPadStickR().y >= 0.7)
		{
			if (cameraTransform_.rotate_.x <= -10.0f)
			{
				cameraTransform_.rotate_.x = -10.0f;
			}
			else
			{
				cameraTransform_.rotate_.x -= 2.5;
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
		cameraTransform_.rotate_.x = 90.0f;
	}
	else if (CameraState_ == S_ROCKONCAMERA)
	{

	}

}

void Player::CameraUpdate()
{
	/*switch (CameraState_)
	{
	case Player::S_NORMALCAMERA:
		NormalCamera();
		break;
	case Player::S_DEBUGCAMERA:
		DebugCamera();
		break;
	case Player::S_ROCKONCAMERA:
		RockOnCamra();
		break;
	case Player::S_MAXCAMERA:
		break;
	default:
		break;
	}*/


	//--------------�J�����Ǐ]--------------
	
	//�J�������ʏ�̎��̏���

	//Enemy* pEnemy = (Enemy*)FindObject("Enemy");
	//XMFLOAT3 tmp = pEnemy->GetWorldPosition();

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

void Player::NormalCamera()
{
}

void Player::DebugCamera()
{
	cameraTransform_.rotate_.x = 90.0f;
}

void Player::RockOnCamra()
{
}

void Player::EnemyReflect(XMVECTOR _vector, bool _IsAttack)
{
	
	if (!this->IsInvincibility_)
	{
	XMFLOAT3 f;
	XMStoreFloat3(&f, _vector);
	KnockBack_Direction_ = f;

	if (_IsAttack)
	{
		KnockBack_Velocity_.x = KnockBackPower_ * 1.5;
		KnockBack_Velocity_.z = KnockBackPower_ * 1.5;
	}
	else
	{
		KnockBack_Velocity_.x = KnockBackPower_;
		KnockBack_Velocity_.z = KnockBackPower_;
	}

	PlayerState_ = S_HIT;
	}
}
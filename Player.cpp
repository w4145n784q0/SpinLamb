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

	enum playeronly
	{
		backcameraX = 0,
		backcameraY,
		backcameraZ,
		keyboardrotateY,
		camerashaketime,
		jumpheight,
		camerainitx,
		camerainity,
		camerainitz,
		camerarotate,
		cameraupperlimit,
		cameralowerlimit,
		cameradebugPos,
	};

	XMVECTOR BackCameraPos = { 0.0f,0.0f,0.0f };//BackCamera�̒l�͕ς�邪���t���[�����̒l�ɂ���i�l���ς�葱����̂�h���j
	float KeyBoardRotateY = 0.0f;
	
	float cameraShakeTime = 0.0f;
	float Jumpheight = 0.0f;

	float cameraInitX = 0.0f;
	float cameraInitY = 0.0f;
	float cameraInitZ = 0.0f;

	float cameraRotate = 0.0f;
	float cameraUpperLimit = 0.0f;
	float cameraLowerLimit = 0.0f;
	float cameraDebugPos = 0.0f;

	//const std::string wallName[] = { "UpperWire", "LowerWire", "RightWire", "LeftWire" };
}

Player::Player(GameObject* parent) 
	: Character(parent,"Player"),
	hPlayer_(-1),hShadow_(-1),
	PlayerState_(S_STOP),CameraState_(S_NORMALCAMERA),
	Direction_({ 0,0,0 }),BackCamera_({ 0,0,0 }), CameraPosition_({ 0,0,0 }), CameraTarget_({ 0,0,0 })
{
	
}

Player::~Player()
{
	
}

void Player::Initialize()
{
	std::string path = "CSVdata\\PlayerData.csv";
	SetcsvStatus(path);
	SetCSVPlayer();

	hPlayer_ = Model::Load("Model\\chara2.fbx");
	assert(hPlayer_ >= 0);

	ShadowInit();



	SetStartPosition();
	
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0,0,0),HitParam_. ColliderSize_);
	this->AddCollider(collider);

	XMVECTOR cameraAdd = XMLoadFloat3(&this->transform_.position_);
	BackCamera_ = { BackCameraPos + cameraAdd };//�o�b�N�J�����̏����l���Z�b�g
	cameraTransform_ = this->transform_;
	CameraPosition_ = { this->transform_.position_.x + cameraInitX
	,this->transform_.position_.y + cameraInitY, this->transform_.position_.z + cameraInitZ };
	CameraTarget_ = { this->transform_.position_.x,this->transform_.position_.y, this->transform_.position_.z };
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

	if(!WallHitParam_. IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
	{
		if (IsOutsideStage(this->transform_.position_))
		{
			WallHit();
			PlayerState_ = S_WALLHIT;
		}
	}

	if(!(PlayerState_ == S_WALLHIT))
	{
		InvincibilityTimeCalclation();
	}

	CharacterGravity();

	//�J�����̍X�V
	CameraUpdate();

#ifdef _DEBUG
	if (Input::IsKeyDown(DIK_ESCAPE))
	{
		SetStartPosition();
	}
#endif

}

void Player::Draw()
{
	if (WallHitParam_.IsInvincibility_)
	{			
		if (++WallHitParam_. blinkTimer > WallHitParam_.blinkValue) {

			WallHitParam_.blinkTimer = 0;
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

#ifdef _DEBUG
	ImGui::Text("PositionX:%.3f", this->transform_.position_.x);
	ImGui::Text("PositionY:%.3f", this->transform_.position_.y);
	ImGui::Text("PositionZ:%.3f", this->transform_.position_.z);

	ImGui::Text("PlayerState:.%d", (float)PlayerState_);
	ImGui::Text("CameraPosition:%.3f", (float)CameraPosition_.x);
	ImGui::Text("CameraPositionY:%.3f", CameraPosition_.y);
	ImGui::Text("CameraPositionZ:%.3f", CameraPosition_.z);
	ImGui::Text("CameraTargetX:%.3f", CameraTarget_.x);
	ImGui::Text("CameraTargetY:%.3f", CameraTarget_.y);
	ImGui::Text("CameraTargetZ:%.3f", CameraTarget_.z);

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

	//if (pTarget->GetObjectName() == "Fence")
	//{
	//	if(!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
	//	{
	//		WallHit();
	//		PlayerState_ = S_WALLHIT;
	//	}
	//}

	/*if (pTarget->GetObjectName() == "UpperWire" || pTarget->GetObjectName() == "LowerWire" ||
		pTarget->GetObjectName() == "RightWire" || pTarget->GetObjectName() == "LeftWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallHit();
			PlayerState_ = S_WALLHIT;
		}
	}*/

	if (pTarget->GetObjectName() == "UpperWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.UpperNormal_);
			PlayerState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "LowerWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.LowerNormal_);
			PlayerState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "RightWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.RightNormal_);
			PlayerState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "LeftWire")
	{
		if (!WallHitParam_.IsInvincibility_ && !(PlayerState_ == S_WALLHIT))
		{
			WallReflect(WallHitParam_.LeftNormal_);
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

	if (length > StickMicroTilt)
	{
		//�R���g���[���[�����ƑO�����x�N�g���̊O�ϋ��߂�i�j
		XMVECTOR cross = XMVector3Cross(SetController, InitParam_. FrontDirection_);

		//Y�O�ς��Ƃ�+��-���œ|����]���������߂�
		float crossY = XMVectorGetY(cross);

		//���ʃx�N�g���Ƃ̃��W�A���p���Ƃ�
		XMVECTOR r = XMVector3AngleBetweenVectors(SetController, InitParam_.FrontDirection_);

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
	MoveParam_.ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, InitParam_. FrontDirection_);


	//------------------�U����Ԃֈڍs------------------//

	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
	{
		if (JumpParam_. IsOnGround_)
		{
			cameraTransform_.rotate_.y = this->transform_.rotate_.y;
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

	Direction_ = { 0,0,0 };//�Ō�ɐi�s�����̃��Z�b�g���t���[���s��
}

void Player::UpdateCharge()
{
	SetChargingEffect("PaticleAssets\\circle_B.png");

	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		if (JumpParam_. IsOnGround_)
		{
			AccelerationStop();
			SetJump();
			PlayerState_ = S_IDLE;
		}
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
	Audio::Play(hSoundattack_);
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
		//WallHitParam_.IsInvincibility_ = true;

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
	JumpParam_. IsOnGround_ = false;
	JumpParam_. JumpSpeed_ = Jumpheight;//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫������
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

		if (Input::IsKey(DIK_A) || Input::GetPadStickR().x <= -StickTilt)	//�J�������E�ړ�
		{
			cameraTransform_.rotate_.y -= cameraRotate;
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickR().x >= StickTilt)
		{
			cameraTransform_.rotate_.y += cameraRotate;
		}

		if (Input::IsKey(DIK_W) || Input::GetPadStickR().y <= -StickTilt)	//�J�����㉺�ړ�
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
		if (Input::IsKey(DIK_S) || Input::GetPadStickR().y >= StickTilt)
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
	XMStoreFloat3(&CameraPosition_, MoveParam_. NewPositon_ + BackCamera_);//�v���C���[�̈ړ��x�N�g���ƃo�b�N�J���������Z

	//--------------�J�����U��--------------
	// �S�X�e�[�g���L
	CameraPosition_.x += Camera::CameraShakeFloat3().x;
	CameraPosition_.y += Camera::CameraShakeFloat3().y;
	CameraPosition_.z += Camera::CameraShakeFloat3().z;

	Camera::SetPosition(CameraPosition_);//�J�����̈ʒu���Z�b�g 
	Camera::SetTarget(CameraTarget_);//�J�����̏œ_���Z�b�g

	if (!(PlayerState_ == Player::S_STOP))
	{
		BackCamera_ = { BackCameraPos };//�o�b�N�J�����x�N�g�������Z�b�g
	}
}

void Player::KeyBoradMove()
{
	XMVECTOR move = XMVectorSet(Direction_.x, Direction_.y, Direction_.z, 0.0f);
	CharacterMoveRotate(move, this->transform_.rotate_.y);
}

void Player::SetCSVPlayer()
{
	CsvReader csv;
	csv.Load("CSVdata\\PlayerData.csv");

	std::string only = "PlayerOnlyParam";
	if (csv.IsGetParamName(only))
	{
		std::vector<float> v = csv.GetParam(only);
		BackCameraPos = { v[backcameraX], v[backcameraY], v[backcameraZ] };
		KeyBoardRotateY = v[keyboardrotateY];
		cameraShakeTime = v[camerashaketime];
		Jumpheight = v[jumpheight];
		cameraInitX = v[camerainitx];
		cameraInitY = v[camerainity];
		cameraInitZ = v[camerainitz];
		cameraRotate = v[camerarotate];
		cameraUpperLimit = v[cameraupperlimit];
		cameraLowerLimit = v[cameralowerlimit];
		cameraDebugPos = v[cameradebugPos];
	}
}

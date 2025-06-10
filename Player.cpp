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
	std::string path = "CSVdata\\PlayerData.csv";
	SetcsvStatus(path);
	SetCSVPlayer();

	hPlayer_ = Model::Load("Model\\chara2.fbx");
	assert(hPlayer_ >= 0);

	hArrow_ = Model::Load("Model\\AttackArrow2.fbx");
	assert(hArrow_ >= 0);

	InitArrow();
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
	DrawCharacterModel(hPlayer_, this->transform_);

	ShadowDraw();

	if (PlayerState_ == S_CHARGE)
	{
		Model::SetTransform(hArrow_, this->MoveParam_.ArrowTransform_);
		Model::Draw(hArrow_);
	}

#ifdef _DEBUG
	if (ImGui::TreeNode("PlayerStatus"))
	{
		if (ImGui::TreeNode("Transform.Position"))
		{
			ImGui::SliderFloat("PositionX:%.3f", &this->transform_.position_.x, WestEnd_, EastEnd_);
			ImGui::SliderFloat("PositionZ:%.3f", &this->transform_.position_.z, SouthEnd_, NorthEnd_);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Transform.Rotate"))
		{
			ImGui::Text("RotateX:%.3f", &this->transform_.rotate_.x);
			ImGui::Text("RotateY:%.3f", &this->transform_.rotate_.y);
			ImGui::Text("RotateZ:%.3f", &this->transform_.rotate_.z);
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

	if (length > StickMicroTilt)
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
	/*XMVECTOR frontArrow = XMVectorScale(MoveParam_.ForwardVector_, 3.0);
	XMVECTOR pPosition = XMLoadFloat3(&this->transform_.position_);
	XMVECTOR arrowPosVec = XMVectorAdd(pPosition, frontArrow);
	XMStoreFloat3(&AttackArrowTransform_.position_, arrowPosVec);
	AttackArrowTransform_.rotate_.y = this->transform_.rotate_.y;*/

	SetArrow();
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

	if (Input::IsKey(DIK_LEFT) || Input::GetPadStickL().x < -StickTilt)
	{
		this->transform_.rotate_.y -= KeyBoardRotateY;
	}
	if (Input::IsKey(DIK_RIGHT) || Input::GetPadStickL().x > StickTilt)
	{
		this->transform_.rotate_.y += KeyBoardRotateY;
	}

	if (Input::IsKeyUp(DIK_LSHIFT) || Input::IsKeyUp(DIK_RSHIFT) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B))
	{
		PlayerState_ = S_ATTACK;
	}

	FrontVectorConfirm();
	Charging();
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
		cameraInitX = v[i_camerainitx];
		cameraInitY = v[i_camerainity];
		cameraInitZ = v[i_camerainitz];
		cameraRotate = v[i_camerarotate];
		cameraUpperLimit = v[i_cameraupperlimit];
		cameraLowerLimit = v[i_cameralowerlimit];
		cameraDebugPos = v[i_cameradebugPos];
	}
}

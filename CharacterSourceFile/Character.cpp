#include "Character.h"
#include"../Engine/VFX.h"
#include"../Engine/Model.h"
#include"../Engine/Audio.h"
#include"../Engine/CsvReader.h"

Character::Character(GameObject* parent)
	:GameObject(parent,"Character"),
	params_(nullptr), blink_(nullptr), vfx_(nullptr), shadow_(nullptr), air_(nullptr),
	forward_(nullptr), movement_(nullptr),rotate_(nullptr),charge_(nullptr),
    hit_(nullptr),fence_(nullptr),csvload_(nullptr),observer_(nullptr),debugpanel_(nullptr)
{
}

Character::Character(GameObject* parent, const std::string& name)
	:GameObject(parent, name),
	params_(nullptr), blink_(nullptr), vfx_(nullptr), shadow_(nullptr), air_(nullptr),
	forward_(nullptr), movement_(nullptr), rotate_(nullptr), charge_(nullptr),
	hit_(nullptr), fence_(nullptr), csvload_(nullptr), observer_(nullptr), debugpanel_(nullptr)
{
	//�e���W���[���̐����E������(Instantiate)
	//�K�v�ɉ����ăp�����[�^�̃Z�b�g(SetParams)
	//�K�v�ɉ����Đe�N���X�i���g�j�̃Z�b�g(SetCharacter)
	//�K�v�ɉ����ăC�x���g���X�i�[�̃Z�b�g(SetEventListener)

	if (params_ == nullptr)
	{
		params_ = Instantiate<CharacterParams>(this);
	}

	if (blink_ == nullptr)
	{
		blink_ = Instantiate<CharacterModelBlink>(this);
		blink_->SetParams(params_);
	}
	if (vfx_ == nullptr)
	{
		vfx_ = Instantiate<CharacterVFX>(this);
		vfx_->SetParams(params_);
		vfx_->SetCharacter(this);
	}
	if (shadow_ == nullptr)
	{
		shadow_ = Instantiate<CharacterShadow>(this);
		shadow_->SetParams(params_);
		shadow_->SetCharacter(this);
	}
	if (air_ == nullptr)
	{
		air_ = Instantiate<CharacterAir>(this);
		air_->SetParams(params_);
		air_->SetCharacter(this);
	}
	if (forward_ == nullptr)
	{
		forward_ = Instantiate<CharacterForward>(this);
		forward_->SetParams(params_);
		forward_->SetCharacter(this);
	}
	if (movement_ == nullptr)
	{
		movement_ = Instantiate<CharacterMovement>(this);
		movement_->SetParams(params_);
		movement_->SetCharacter(this);
	}
	if (rotate_ == nullptr)
	{
		rotate_ = Instantiate<CharacterRotate>(this);
		rotate_->SetParams(params_);
		rotate_->SetCharacter(this);
	}
	if (charge_ == nullptr)
	{
		charge_ = Instantiate<CharacterCharge>(this);
		charge_->SetParams(params_);
		charge_->SetCharacter(this);
		charge_->SetEventListener(this);
	}
	if (hit_ == nullptr)
	{
		hit_ = Instantiate<CharacterHit>(this);
		hit_->SetParams(params_);
		hit_->SetCharacter(this);
		hit_->SetEventListener(this, this, this);
	}
	if (fence_ == nullptr)
	{
		fence_ = Instantiate<CharacterFence>(this);
		fence_->SetParams(params_);
		fence_->SetCharacter(this);
		fence_->SetEventListener(this, this, this);
	}
	if(csvload_ == nullptr)
	{
		csvload_ = Instantiate<CharacterCsvLoader>(this);
		csvload_->SetParams(params_);
		csvload_->SetCharacter(this);
	}
	if (observer_ == nullptr)
	{
		observer_ = Instantiate<CharacterObserver>(this);
		observer_->SetParams(params_);
	}
	if (debugpanel_ == nullptr)
	{
		debugpanel_ = Instantiate<CharacterDebugPanel>(this);
		debugpanel_->SetParams(params_);
		debugpanel_->SetCharacter(this);
	}

	//csv����p�����[�^�ǂݍ���
	vfx_->InitCSVEffect();

	//�T�E���h�̓ǂݍ���
	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string SoundSE = "Sound\\SE\\";

	params_->SoundParam_.hSoundcharge_ = Audio::Load(SoundSE + "Charge.wav",false, Audio::GetChargeNum());
	assert(params_->SoundParam_.hSoundcharge_ >= 0);

	params_->SoundParam_.hSoundattack_ = Audio::Load(SoundSE + "Attack.wav", false, Audio::GetAttackNum());
	assert(params_->SoundParam_.hSoundattack_ >= 0);

	params_->SoundParam_.hSoundCollision_ = Audio::Load(SoundSE + "Collision.wav",false, Audio::GetCollisionNum());
	assert(params_->SoundParam_.hSoundCollision_ >= 0);

	params_->SoundParam_.hSoundJump_ = Audio::Load(SoundSE + "Jump.wav", false, Audio::GetJumpNum());
	assert(params_->SoundParam_.hSoundJump_ >= 0);

	//���ꂼ��̍�̖@�����擾
	fence_->GetWireNormal();

	//�e���f����������
	shadow_->InitShadow();
}

Character::~Character()
{
	//���̂͏������A�A�h���X�̂ݖ���������
	for (auto& observer : params_->InitParam_.observers)
	{
		if(observer != nullptr)
		{
			//�Ď��Ώۂ̔z�񂩂�폜
			//�ʒm��(Character)���Ď���(BattleScene)�̏��ɉ�������̂ŁA
			//Character�̃f�X�g���N�^��RemoveObserver���Ă�
			observer_->RemoveObserver(observer);

			//���S��Ƃ��ăA�h���X�𖳌���
			observer = nullptr;
		}
	}

	//�e�Ɏg�p����n�ʃN���X�̃|�C���^����ɂ���
	if(params_->ShadowParam_.pGround_ != nullptr)
	{
		params_->ShadowParam_.pGround_ = nullptr;
	}
}

void Character::Update()
{
	//Character�N���X���p������N���X�ŌĂԋ��ʍX�V

	//���t���[���e�̈ʒu���X�V
	shadow_->ShadowSet();

	//���t���[���d�͂�����������
	air_->CharacterGravity();

	//�����̑O���x�N�g��(��]���������܂�)���X�V
	forward_->FrontVectorConfirm();
}

void Character::Draw()
{
	//Character�N���X���p������N���X�ŌĂԋ��ʕ`��

	//�L�����N�^�[�̊ۉe��`��
	shadow_->ShadowDraw();
}

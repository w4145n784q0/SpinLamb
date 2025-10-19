#include "CharacterVFX.h"
#include"../Character.h"
#include"../../Engine/VFX.h"

CharacterVFX::CharacterVFX(GameObject* parent)
	:GameObject(parent, "CharacterVFX"), params_(nullptr), character_(nullptr)
{
}

void CharacterVFX::InitCSVEffect()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVData\\EffectData\\VFXData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾
	std::string Effects[] = { "Charge","FullCharge" ,"Locus" , "Hit" , "FenceHit" };

	//ChargeParam_����n�܂�VFX�̃p�����[�^(vector<float>�^�̔z��)�̎Q�Ƃ�
	//�|�C���^�z��Ɋi�[
	std::vector<float>* Param[] = { 
		
		&params_->EffectParam_.ChargeParam_,
		&params_->EffectParam_.FullChargeParam,
		&params_->EffectParam_.AttackLocusParam_,
		&params_->EffectParam_.HitEffectParam_,
		&params_->EffectParam_.FenceHitEffectParam_
	};


	for (int i = 0; i < sizeof(Effects) / sizeof(Effects[0]); i++)
	{
		//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
		std::vector<float> v = GetCSVReadData(csv, Effects[i]);

		//���̎��_�ł͑���̂ݍs����
		// SetEmitter�Ŏ��ۂ�VFX�̃p�����[�^�ɃZ�b�g����� 
		*Param[i] = v;

	}
}

void CharacterVFX::SetChargingEffect(std::string _path)
{
	//csv����ǂݍ���,�`���[�W���G�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData charge;
	VFX::SetEmitter(charge, params_->EffectParam_.ChargeParam_);

	//�g�p����摜�̃p�X���Z�b�g����
	charge.textureFileName = _path;

	//���ˈʒu���Z�b�g
	charge.position = character_->GetPosition();

	//�G�t�F�N�g���J�n
	VFX::Start(charge);
}

void CharacterVFX::SetFullChargeEffect()
{
	//csv����ǂݍ���,�ő�`���[�W�G�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData fullcharge;
	VFX::SetEmitter(fullcharge, params_->EffectParam_.FullChargeParam);

	//�g�p����摜�̃p�X���Z�b�g����
	fullcharge.textureFileName = "ParticleAssets\\circle_W.png";

	//���ˈʒu���Z�b�g
	fullcharge.position = character_->GetPosition();

	//�G�t�F�N�g���J�n
	VFX::Start(fullcharge);
}

void CharacterVFX::SetAttackLocusEffect()
{
	//csv����ǂݍ���,�U�����̋O�ՃG�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData locus;
	VFX::SetEmitter(locus, params_->EffectParam_.AttackLocusParam_);

	//�g�p����摜�̃p�X���Z�b�g����
	locus.textureFileName = "ParticleAssets\\flashB_Y.png";

	//���ˈʒu���Z�b�g
	locus.position = character_->GetPosition();

	//�G�t�F�N�g���J�n
	VFX::Start(locus);
}

void CharacterVFX::SetHitEffect()
{
	//csv����ǂݍ���,��e�G�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData hit;
	VFX::SetEmitter(hit, params_->EffectParam_.HitEffectParam_);

	//�g�p����摜�̃p�X���Z�b�g����
	hit.textureFileName = "ParticleAssets\\flashB_W.png";

	//���ˈʒu���Z�b�g
	hit.position = character_->GetPosition();

	//�G�t�F�N�g���J�n
	VFX::Start(hit);
}

void CharacterVFX::SetFenceHitEffect()
{
	//csv����ǂݍ���,��ɐڐG���G�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData  fencehit;
	VFX::SetEmitter(fencehit, params_->EffectParam_.FenceHitEffectParam_);

	//�g�p����摜�̃p�X���Z�b�g����
	fencehit.textureFileName = "ParticleAssets\\flashB_W.png";

	//���ˈʒu���Z�b�g
	fencehit.position = character_->GetPosition();

	//�G�t�F�N�g���J�n
	VFX::Start(fencehit);
}

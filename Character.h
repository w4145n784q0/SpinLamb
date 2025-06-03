#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"
#include "Ground.h"

#include"UpperWire.h"
#include"LowerWire.h"
#include"LeftWire.h"
#include"RightWire.h"

//�v���C���[,�G�N���X�̋��ʎ����N���X
class Character :
    public GameObject
{
private:
	//----------�X�e�[�W�̒[----------
   float NorthEnd_ = 0.0f;
   float SouthEnd_ = 0.0f;
   float EastEnd_ = 0.0f;
   float WestEnd_ = 0.0f;
protected:
    
	//----------�T�E���h�n���h��----------
	int hSoundcharge_ = -1; //�`���[�W���̃n���h��
	int hSoundattack_ = -1; //�ˌ����̃n���h��
    int hSoundCollision_ = -1;//�ڐG��

    //----------�������----------
    struct InitializeParam
    {
        XMFLOAT3 StartPosition_ = {0,0,0};//�����ʒu
        XMVECTOR FrontDirection_ = {0,0,1};//���ʂ̏����l(���[�J�����W�n) ��������ǂꂾ����]������
    };
    InitializeParam InitParam_;

    //----------�ړ�----------
    struct MoveParam   
    {   
        float Velocity_ = 0.0f;//�����x ���̑��x�ɉ����x�����Z�����
        float Acceleration_ = 0.0f;//�����x
        float AcceleValue_ = 0.0f;//Acceleration_�㏸���A1f������̑�����
        float FullAccelerate_ = 0.0f;//�����x�̍ő�
        XMVECTOR ForwardVector_ = { 0,0,0 };//�L�����N�^�[���猩�����ʂ̕���(���[���h���W�n) ���g��y����]�ʂƂ����Čv�Z ���K�������l������
        XMVECTOR MoveDirection_ = { 0,0,0 };//�ړ����� ���̒l�ɑ����̗v�f�������Ĉړ��x�N�g��������
        XMVECTOR NewPositon_ = { 0,0,0 };//�ړ���̈ʒu�x�N�g��
    };
    MoveParam MoveParam_;

    //----------��]----------
    struct RotateParam
    {
        float MoveRotateX = 0.0f;//�ړ�����1f�̉�]��
        float FastRotateX = 0.0f;//(�`���[�W���Ȃ�)������]����1f�̉�]��
    };
    RotateParam RotateParam_;

    //----------��----------
    struct JumpParam
    {
        float Gravity_ = 0.0f; //�d�� �L�����N�^�[�̉������ɂ������
        bool IsOnGround_ = false;//�n�ʂɂ��邩
        float JumpSpeed_ = 0.0f; //�v���C���[�̏�����Ɍ����� +�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����
        float HeightLowerLimit_ = 0.0f;//�����̉���
        float HeightUpperLimit_ = 0.0f;//�����̏��
    };
    JumpParam JumpParam_;


    //----------��e----------
    struct HitParam
    {
        float ColliderSize_ = 0.0f; //�����蔻��(����)�̃T�C�Y
		float OriginaRangeMin_ = 0.0f; //�ϊ����̃m�b�N�o�b�N�ʂ̍ŏ��l
		float OriginaRangeMax_ = 0.0f;  //�ϊ����̃m�b�N�o�b�N�ʂ̍ő�l
		float ConvertedRangeMin_ = 0.0f; //�ϊ���̃m�b�N�o�b�N�ʂ̍ŏ��l
		float ConvertedRangeMax_ = 0.0f; //�ϊ���̃m�b�N�o�b�N�ʂ̍ő�l
        XMFLOAT3 KnockBack_Direction_ = { 0,0,0 };//�m�b�N�o�b�N�������
        XMFLOAT3 KnockBack_Velocity_ = {0,0,0};//�m�b�N�o�b�N���鑬�x
        float KnockBackPower_ = 0.0f; //�m�b�N�o�b�N���鋭���i�ω��Ȃ��j
        float DecelerationRate_ = 0.0f;//�m�b�N�o�b�N����1f���Ƃ̌�����
        float KnockBackEnd_ = 0.0f;//�m�b�N�o�b�N���I������l
    };
    HitParam HitParam_;

    //----------�ǂ̐ڐG�_���[�W----------
    struct WallHitParam
    {
        XMVECTOR UpperNormal_;//�X�e�[�W�k�[�̖@���x�N�g��
        XMVECTOR LowerNormal_;//�X�e�[�W��[�̖@���x�N�g��
        XMVECTOR RightNormal_;//�X�e�[�W���[�̖@���x�N�g��
        XMVECTOR LeftNormal_;//�X�e�[�W���[�̖@���x�N�g��

        int InvincibilityTime_ = 0;//�_���[�W��̖��G���� 1f���Ƃɏ㏸
        bool IsInvincibility_ = false;//���G���Ԃ�
        int InvincibilityValue = 0;//���G���Ԃ̒l�@���̒l�𒴂���Ɩ��G���ԏI��
        int blinkTimer = 0;//�_���[�W��̓_�ŃJ�E���g
        int blinkValue = 0;//���̒l��blinkTimer�����B����ƕ`�悷��
    };
    WallHitParam WallHitParam_;
    
    //----------�e�t��----------
    struct ShadowParam
    {
        int hShadow_ = -1;//�e�̃��f���n���h��
        float ShadowHeight_ = 0.0f;//�e�����鍂��
        float ShadowCorrection_ = 0.0f;//�e�̈ʒu�̒����l
        Transform ShadowTrans_;//�e�̕`��g�����X�t�H�[��
        Ground* pGround_ = nullptr;//�n�ʂ̃C���X�^���X
    };
    ShadowParam ShadowParam_;

    //----------�G�t�F�N�g�֘A----------

    std::vector<float> ChargeParam_ = {};//�`���[�W��ԃG�t�F�N�g�̃p�����[�^
	std::vector<float> AttackLocusParam_ = {};//�ˌ��G�t�F�N�g�̃p�����[�^
	std::vector<float> HitEffectParam_ = {};//�ڐG���̏Ռ��G�t�F�N�g�̃p�����[�^
	std::vector<float> WallHitEffectParam_ = {};//�ǂɐڐG���̏Ռ��G�t�F�N�g�̃p�����[�^
	std::vector<std::vector<float>> EffectArray_ = {};//�G�t�F�N�g�p�����[�^���܂Ƃ߂�z��

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    /// <summary>
    /// CSV����e�X�e�[�^�X��ǂݍ��݂���
    /// Character�N���X����
    /// </summary>
    /// <param name="_path">csv�t�@�C���̃p�X</param>
    void SetcsvStatus(std::string _path);

    void GetWireNormal() {
        UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
        WallHitParam_.UpperNormal_ = pUpperWire->GetNormal();

        LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
		WallHitParam_.LowerNormal_ = pLowerWire->GetNormal();

        LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
		WallHitParam_.LeftNormal_ = pLeftWire->GetNormal();

        RightWire* pRightWire = (RightWire*)FindObject("RightWire");
		WallHitParam_.RightNormal_ = pRightWire->GetNormal();
    }

    //----------��{����----------

    /// <summary>
    /// �����ʒu�̐ݒ�
    /// </summary>
    void SetStartPosition() { this->transform_.position_ = InitParam_.StartPosition_; }

    /// <summary>
    /// �d�͏���
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// �e���f���̏�����
    /// </summary>
    void ShadowInit();

    /// <summary>
    /// �e�t���i���t���[���X�V�j
    /// </summary>
    void ShadowSet();

    /// <summary>
    /// �e���f���`��
    /// </summary>
    void ShadowDraw();

    /// <summary>
    /// �L�����N�^�[�̈ړ�����(��]���s��)
    /// </summary>
    /// <param name="_direction">�����������x�N�g��</param>
    void CharacterMoveRotate(XMVECTOR _direction, float rotateY);

    /// <summary>
    /// �L�����N�^�[�̈ړ�����
    /// </summary>
    /// <param name="_direction">�����������x�N�g��</param>
    void CharacterMove(XMVECTOR _direction);

    /// <summary>
    /// �ړ��x�N�g��������
    /// </summary>
    void CreateMoveVector();

    /// <summary>
    /// ��O����
    /// </summary>
    /// <param name="_position">�X�V�\��̈ʒu</param>
    /// <returns>�ړ��\��</returns>
    bool IsOutsideStage(XMFLOAT3 _position);

    /// <summary>
    /// �ړ��m�菈��
    /// </summary>
    void MoveConfirm();

    /// <summary>
    /// ���ʃx�N�g�����X�V
    /// </summary>
    void FrontVectorConfirm(){ 
        MoveParam_.ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, InitParam_.FrontDirection_);
    }

    /// <summary>
    /// ���ˏ���
    /// </summary>
    /// <param name="myVector">���g�̈ʒu�x�N�g��</param>
    /// <param name="eVector">����̈ʒu�x�N�g��</param>
    /// <param name="myVelocity">���g�̉����x</param>
    /// <param name="eVelocity">����̉����x</param>
    void Reflect(XMVECTOR myVector, XMVECTOR eVector, float myVelocity, float eVelocity);

    /// <summary>
    /// �m�b�N�o�b�N�ړ�����
    /// </summary>
    void KnockBack();

    /// <summary>
    /// �ǂɐڐG�����ۂ̌v�Z����
    /// </summary>
    void WallHit();


    void WallReflect(XMVECTOR pos);

    /// <summary>
    /// �m�b�N�o�b�N�I������
    /// </summary>
    /// <returns>�m�b�N�o�b�N���x���I���l�ȉ���</returns>
    bool IsKnockBackEnd();

    /// <summary>
    /// �_���[�W��̖��G���Ԃ̌v�Z
    /// </summary>
    void InvincibilityTimeCalclation();

    /// <summary>
    /// �ʏ�X����]
    /// </summary>
    void MoveRotate(){ this->transform_.rotate_.x += RotateParam_.MoveRotateX; }

    /// <summary>
    /// �ʏ�X����](-x��])
    /// </summary>
    void MoveRotateReverse(){ this->transform_.rotate_.x -= RotateParam_.MoveRotateX; }

    /// <summary>
    /// ����X����]
    /// </summary>
    void FastRotate(){ this->transform_.rotate_.x += RotateParam_.FastRotateX; }

    /// <summary>
    /// ����X����](-x��])
    /// </summary>
    void FastRotateReverse() { this->transform_.rotate_.x -= RotateParam_.FastRotateX; }

    /// <summary>
    /// X��]���~�߂�
    /// </summary>
    void RotateStop(){ this->transform_.rotate_.x = 0.0f; }

    /// <summary>
    /// �����x�̉��Z
    /// </summary>
    void Charging();

    /// <summary>
    /// ��������
    /// </summary>
    void Deceleration() { MoveParam_.Acceleration_ -= MoveParam_.AcceleValue_; }

    /// <summary>
    /// �����x���Z�b�g
    /// </summary>
    void AccelerationStop(){ MoveParam_.Acceleration_ = 0.0f; }

    /// <summary>
    /// ��~����
    /// </summary>
    /// <returns>�����ʂ�0.0�ȉ����ǂ���</returns>
    bool IsDashStop() { if (MoveParam_.Acceleration_ <= 0.0f) return true; else return false; }

    /// <summary>
    /// Y���̉�]�s����x�N�g���ɕϊ�
    /// </summary>
    /// <param name="rotY">Y���ɉ�]�������p�x�iDegree�j</param>
    /// <param name="front">���ʃx�N�g��(���[�J�����)</param>
    /// <returns>�ό`�����x�N�g���i���[���h��ԁj</returns>
    XMVECTOR RotateVecFront(float rotY, XMVECTOR front);


    //----------�G�t�F�N�g����----------
    
    /// <summary>
    /// CSV�p�f�[�^�̏�����
    /// </summary>
    void InitCSVEffect();

    /// <summary>
    /// �`���[�W��ԃG�t�F�N�g������
    /// </summary>
    /// <param name="_path">csv��ǂݍ��ރp�X</param>
    void SetChargingEffect(std::string _path);

    /// <summary>
    /// �ˌ��G�t�F�N�g����(�v���C���[�̔w��Ɍ��̗��q)
    /// </summary>
    void SetAttackLocusEffect();

    /// <summary>
    /// ����ƐڐG���̏Ռ��G�t�F�N�g����
    /// </summary>
    void SetHitEffect();

    /// <summary>
    /// �ǂɐڐG���̏Ռ��G�t�F�N�g����
    /// </summary>
    void SetWallHitEffect();

    //setter,getter�֐�

    void SetAcceleration(float _acceleration) { MoveParam_.Acceleration_ = _acceleration; }
    float GetAcceleration() { return MoveParam_.Acceleration_; }

    //�L�����N�^�[�̈ړ��������Z�b�g
    void SetEnd(float upper, float lower, float left, float right) {
        NorthEnd_ = upper;
        SouthEnd_ = lower;
        WestEnd_ = left;
        EastEnd_ = right;
    }
};
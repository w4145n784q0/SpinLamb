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
protected:

    //----------�X�e�[�W�̒[----------
	float NorthEnd_ = 0.0f;//�X�e�[�W�k�[(�O��)�̈ʒu
	float SouthEnd_ = 0.0f;//�X�e�[�W��[(���)�̈ʒu
	float EastEnd_ = 0.0f;//�X�e�[�W���[(�E��)�̈ʒu
	float WestEnd_ = 0.0f;//�X�e�[�W���[(����)�̈ʒu

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
        float TmpAccele_ = 0.0f;//�����x�㏸���Ɏg�����̒l
        float AcceleValue_ = 0.0f;//Acceleration_�㏸���A1f������̑�����
        float FullAccelerate_ = 0.0f;//�����x�̍ő�
		float Friction_ = 0.0f;//���C�W��(������) 1f������̌�����
        XMVECTOR ForwardVector_ = { 0,0,0 };//�L�����N�^�[���猩�����ʂ̕���(���[���h���W�n) ���g��y����]�ʂƂ����Čv�Z ���K�������l������
        XMVECTOR MoveDirection_ = { 0,0,0 };//�ړ����� ���̒l�ɑ����̗v�f�������Ĉړ��x�N�g��������
        XMVECTOR NewPositon_ = { 0,0,0 };//�ړ���̈ʒu�x�N�g��
		Transform ArrowTransform_;//�`���[�W/�U���������̖��̃g�����X�t�H�[��
        XMFLOAT3 ArrowRotate_ = { 0,0,0 };//���̏�����]
        XMFLOAT3 ArrowScale_ = { 0,0,0 };//���̑傫��
		float AddArrowDepth_ = 0.0f;//���̉��s��(�O����)�̒����l
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
        float Gravity_ = 0.0f; //�d�� �L�����N�^�[�̉������ɂ������ 9.8/60(1�b)���y�����Ă���
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
        float DecelerationRate_ = 0.0f;//�m�b�N�o�b�N����1f���Ƃ̌�����
        float KnockBackEnd_ = 0.0f;//�m�b�N�o�b�N���I������l
    };
    HitParam HitParam_;

    //----------�ǂ̐ڐG�_���[�W----------
    struct WallHitParam
    {
        XMVECTOR UpperNormal_ = {0,0,0};//�X�e�[�W�k�[(�O��)�̖@���x�N�g��
        XMVECTOR LowerNormal_ = { 0,0,0 };//�X�e�[�W��[(���)�̖@���x�N�g��
        XMVECTOR RightNormal_ = { 0,0,0 };//�X�e�[�W���[(�E��)�̖@���x�N�g��
        XMVECTOR LeftNormal_ = { 0,0,0 };//�X�e�[�W���[(����)�̖@���x�N�g��
        std::vector<XMVECTOR> NormalArray_ = {};//�e�@���x�N�g�����i�[�����z��
        std::vector<std::string> WireArray_ = { "UpperWire", "LowerWire", "RightWire" ,"LeftWire"};//�e�S���̖��O�̔z��

        float KnockBackPower_ = 0.0f; //�ǃq�b�g�Ńm�b�N�o�b�N���鋭���i�ω��Ȃ��j
        int InvincibilityTime_ = 0;//�_���[�W��̖��G���� 1f���Ƃɏ㏸
        bool IsInvincibility_ = false;//���G���Ԃ�
        int InvincibilityValue_ = 0;//���G���Ԃ̒l�@���̒l�𒴂���Ɩ��G���ԏI��
        int blinkTimer_ = 0;//�_���[�W��̓_�ŃJ�E���g
        int blinkValue_ = 0;//���̒l��blinkTimer�����B����ƕ`�悷��
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
    std::vector<float> FullChargeParam = {};
	std::vector<float> AttackLocusParam_ = {};//�ˌ��G�t�F�N�g�̃p�����[�^
	std::vector<float> HitEffectParam_ = {};//�ڐG���̏Ռ��G�t�F�N�g�̃p�����[�^
	std::vector<float> WallHitEffectParam_ = {};//�ǂɐڐG���̏Ռ��G�t�F�N�g�̃p�����[�^

    //----------�T�E���h�֘A----------
    int ChargeSoundCount_ = 0;//�`���[�W����炷��
    int AttackSoundCount_ = 0;//�ˌ�����炷��

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    //�X�V(�p����̋��ʏ����̂ݍs��)
    void Update() override;

    //�`��(�p����̋��ʕ`��̂ݍs��)
    void Draw() override;

    //----------������----------

    /// <summary>
    /// CSV����e�X�e�[�^�X��ǂݍ��݂���
    /// Character�N���X����
    /// </summary>
    /// <param name="_path">csv�t�@�C���̃p�X</param>
    void SetcsvStatus(std::string _path);

    /// <summary>
    /// �e�����̍򂩂�@���x�N�g�����擾��NormalArray��������
    /// </summary>
    void GetWireNormal();

    /// <summary>
    /// �����ʒu�̐ݒ�
    /// </summary>
    void SetStartPosition() { this->transform_.position_ = InitParam_.StartPosition_; }

    /// <summary>
    /// ���g�����X�t�H�[���̏�����
    /// </summary>
    void InitArrow();
  
    //----------�`��----------

    /// <summary>
    /// �L�����N�^�[���f���`��(�_���[�W���̓_�ŕ\�����s��)
    /// </summary>
    /// <param name="_handle">���f���n���h��</param>
    /// <param name="_transform">�`��ʒu�̃g�����X�t�H�[��</param>
    void DrawCharacterModel(int _handle, Transform _transform);

    /// <summary>
    /// �L�����N�^�[���ʂ�ImGui��`�� �ʗv�f�͔h����ŋL�q
    /// </summary>
    void DrawCharacterImGui();

    //----------�ړ�----------

    /// <summary>
    /// �L�����N�^�[�̈ړ�����
    /// </summary>
    /// <param name="_direction">�����������x�N�g��</param>
    void CharacterMove(XMVECTOR _direction);

    /// <summary>
    /// �ړ��x�N�g��������(�����x�N�g�����K�v)
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
    /// Y���̉�]�s����x�N�g���ɕϊ�
    /// </summary>
    /// <param name="rotY">Y���ɉ�]�������p�x�iDegree�j</param>
    /// <param name="front">���ʃx�N�g��(���[�J�����)</param>
    /// <returns>�ό`�����x�N�g���i���[���h��ԁj</returns>
    XMVECTOR RotateVecFront(float rotY, XMVECTOR front);

    /// <summary>
    /// ���ʃx�N�g�����X�V
    /// </summary>
    void FrontVectorConfirm(){

        //���[�J�����ʃx�N�g�������݂�y����]�ʂŕό`����ƁA���ʂ���ǂꂾ����]���������v�Z�����
        //���̒l�����[���h���ʃx�N�g���ƂȂ�
        MoveParam_.ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, InitParam_.FrontDirection_);
    }

    /// <summary>
    /// �󂯎�����x�N�g������L�����N�^�[�̉�]�ʂ��v�Z
    /// </summary>
    /// <param name="_MoveVector">�i�s�����������x�N�g��</param>
    /// <returns>��]����p�x(Y����])</returns>
    float RotateDirectionVector(XMVECTOR _MoveVector);

    //----------�`���[�W----------

    /// <summary>
    /// �����x�̉��Z
    /// </summary>
    void Charging();

    /// <summary>
    /// �~�ς���TmpAccele_�����ۂɉ����x�ɑ��
    /// </summary>
    void ChargeRelease();

    /// <summary>
    /// TmpAccele_��0�ɂ���
    /// </summary>
    void ChargeReset();

    /// <summary>
    /// �`���[�W���̖��ʒu���Z�b�g
    /// </summary>
    void SetArrow();

    //----------�U��----------

    /// <summary>
    /// ��������(�������̑����ʎg�p)
    /// </summary>
    void Deceleration() { MoveParam_.Acceleration_ -= MoveParam_.AcceleValue_; }

    /// <summary>
    /// ���C�ɂ�錸������
    /// </summary>
    void FrictionDeceleration() { MoveParam_.Acceleration_ -= MoveParam_.Friction_; }

    /// <summary>
    /// �����x���Z�b�g
    /// </summary>
    void AccelerationStop() { MoveParam_.Acceleration_ = 0.0f; }

    /// <summary>
    /// ��~����
    /// </summary>
    /// <returns>�����ʂ�0.0�ȉ����ǂ���</returns>
    bool IsDashStop() { if (MoveParam_.Acceleration_ <= 0.0f) return true; else return false; }


    //----------��----------

    /// <summary>
    /// �d�͏���
    /// </summary>
    void CharacterGravity();

    //----------��]----------

    /// <summary>
    /// �ʏ�X����]
    /// </summary>
    void MoveRotate() { this->transform_.rotate_.x += RotateParam_.MoveRotateX; }

    /// <summary>
    /// �ʏ�X����](-x��])
    /// </summary>
    void MoveRotateReverse() { this->transform_.rotate_.x -= RotateParam_.MoveRotateX; }

    /// <summary>
    /// ����X����]
    /// </summary>
    void FastRotate() { this->transform_.rotate_.x += RotateParam_.FastRotateX; }

    /// <summary>
    /// ����X����](-x��])
    /// </summary>
    void FastRotateReverse() { this->transform_.rotate_.x -= RotateParam_.FastRotateX; }

    /// <summary>
    /// X��]���~�߂�
    /// </summary>
    void RotateStop() { this->transform_.rotate_.x = 0.0f; }


    //----------��e----------

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
    /// �w�肳�ꂽ�S���̖��O�ɉ������e�@���x�N�g�����擾����
    /// </summary>
    /// <param name="_normal">�L�����N�^�[���ڐG�����S���̖��O</param>
    /// <returns>�S���̖��O�ɑΉ������@���x�N�g��</returns>
    XMVECTOR HitNormal(std::string _normal);

    /// <summary>
    /// �ǂɐڐG�����ۂ̌v�Z�����@�ǂ̖@���Ōv�Z
    /// </summary>
    /// <param name="normal">���˂�������(�ڐG������̖@���x�N�g��)</param>
    void WallReflect(XMVECTOR normal);

    /// <summary>
    /// �m�b�N�o�b�N�I������
    /// </summary>
    /// <returns>�m�b�N�o�b�N���x���I���l�ȉ���</returns>
    bool IsKnockBackEnd();

    /// <summary>
    /// �_���[�W��̖��G���Ԃ̌v�Z
    /// </summary>
    void InvincibilityTimeCalclation();

    //----------�e�t��----------

    /// <summary>
    /// �e���f���̏�����
    /// </summary>
    void InitShadow();

    /// <summary>
    /// �e�t���i���t���[���X�V�j
    /// </summary>
    void ShadowSet();

    /// <summary>
    /// �e���f���`��
    /// </summary>
    void ShadowDraw();


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
    /// �ő�`���[�W��ԃG�t�F�N�g(���̊g�U)������
    /// </summary>
    void SetFullChargeEffect();

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

    //----------�T�E���h�֌W----------

    //CSV����p�����[�^��ǂݍ��݂���
    void InitCSVSound();

    //----------�Z�b�^�[�E�Q�b�^�[�֐�----------
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
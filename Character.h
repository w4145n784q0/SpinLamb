#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"
#include "Ground.h"

//�v���C���[,�G�N���X�̋��ʎ����N���X
class Character :
    public GameObject
{
private:
   float NorthEnd;
   float SouthEnd;
   float EastEnd;
   float WestEnd;
protected:
    //----------�������----------
    XMFLOAT3 StartPosition_;//�����ʒu
    XMVECTOR FrontDirection_;//���ʂ̏����l(���[�J�����W�n) ��������ǂꂾ����]������

    //----------�ړ�----------
    float Velocity_;//�����x ���̑��x�ɉ����x�����Z�����
    float Acceleration_;//�����x
    float AcceleValue_;//Acceleration_�㏸���A1f������̑�����
    float FullAccelerate_;//�����x�̍ő�
    XMVECTOR ForwardVector_;//�L�����N�^�[���猩�����ʂ̕���(���[���h���W�n) ���g��y����]�ʂƂ����Čv�Z ���K�������l������
    XMVECTOR MoveDirection_;//�ړ����� ���̒l�ɑ����̗v�f�������Ĉړ��x�N�g��������
    XMVECTOR NewPositon_;//�ړ���̈ʒu�x�N�g��

    //----------��]----------
    float MoveRotateX;//�ړ�����1f�̉�]��
    float FastRotateX;//(�`���[�W���Ȃ�)������]����1f�̉�]��

    //----------��----------
    float Gravity_; //�d�� �L�����N�^�[�̉������ɂ������
    bool IsOnGround_;//�n�ʂɂ��邩
    float JumpSpeed_; //�v���C���[�̏�����Ɍ����� +�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����
    float HeightLowerLimit_;//�����̉���
    float HeightUpperLimit_;//�����̏��


    //----------��e----------
    float ColliderSize_; //�����蔻��(����)�̃T�C�Y
    XMFLOAT3 KnockBack_Direction_;//�m�b�N�o�b�N�������
    XMFLOAT3 KnockBack_Velocity_;//�m�b�N�o�b�N���鑬�x
    float KnockBackPower_; //�m�b�N�o�b�N���鋭��
    float DecelerationRate_;//�m�b�N�o�b�N����1f���Ƃ̌�����
    float KnockBackEnd_;//�m�b�N�o�b�N���I������l

    //�m�b�N�o�b�N���鑬�x= �m�b�N�o�b�N���鋭��(�萔) * �m�b�N�o�b�N�������

    //----------�ǂ̐ڐG�_���[�W----------
    int InvincibilityTime_;//�_���[�W��̖��G���� 1f���ƂɌ���
    bool IsInvincibility_;//���G���Ԃ�
    int InvincibilityValue;//���G���Ԃ̒l�@���̒l�𒴂���Ɩ��G���ԏI��

    
    //----------�e�t��----------
    int hShadow_;//�e�̃��f���n���h��
    float ShadowHeight_;//�e�����鍂��
    float ShadowCorrection_;//�e�̈ʒu�̒����l
    Transform ShadowTrans_;//�e�̕`��g�����X�t�H�[��
    Ground* pGround_;//�n�ʂ̃C���X�^���X

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

    //----------��{����----------

    /// <summary>
    /// �����ʒu�̐ݒ�
    /// </summary>
    void SetStartPosition() { this->transform_.position_ = StartPosition_; }

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
    void MoveRotate(){ this->transform_.rotate_.x += MoveRotateX; }

    /// <summary>
    /// �ʏ�X����](�G���f���p)
    /// </summary>
    void MoveRotateReverse(){ this->transform_.rotate_.x -= MoveRotateX; }

    /// <summary>
    /// ����X����]
    /// </summary>
    void FastRotate(){ this->transform_.rotate_.x += FastRotateX; }

    /// <summary>
    /// ����X����](�G���f���p)
    /// </summary>
    void FastRotateReverse() { this->transform_.rotate_.x -= FastRotateX; }

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
    void Deceleration() { Acceleration_ -= AcceleValue_; }

    /// <summary>
    /// ��~����
    /// </summary>
    /// <returns>�����ʂ�0.0�ȉ����ǂ���</returns>
    bool IsDashStop() { if (Acceleration_ <= 0.0f) return true; else return false; }

    /// <summary>
    /// Y���̉�]�s����x�N�g���ɕϊ�
    /// </summary>
    /// <param name="rotY">Y���ɉ�]�������p�x�iDegree�j</param>
    /// <param name="front">���ʃx�N�g��(���[�J�����)</param>
    /// <returns>�ό`�����x�N�g���i���[���h��ԁj</returns>
    XMVECTOR RotateVecFront(float rotY, XMVECTOR front);


    //----------�G�t�F�N�g����----------
    
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
    /// �Ռ��G�t�F�N�g����
    /// </summary>
    void SetHitEffect();


    //setter,getter�֐�

    void SetAcceleration(float _acceleration) { Acceleration_ = _acceleration; }
    float GetAcceleration() { return Acceleration_; }

    //�L�����N�^�[�̈ړ��������Z�b�g
    void SetEnd(float upper, float lower, float left, float right) {
        NorthEnd = upper;
        SouthEnd = lower;
        WestEnd = left;
        EastEnd = right;
    }
};
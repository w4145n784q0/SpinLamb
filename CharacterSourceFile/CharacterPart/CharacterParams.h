#pragma once
#include"../../InterfaceSourceFile/IGameObserver.h"
#include"../../StageSourceFile/Ground.h"

//�v���C���[,�G�N���X�̋��ʃp�����[�^�݂̂������t�@�C��

    //----------�X�e�[�W�̒[----------
    struct EndParam 
    {
        //�L�����N�^�[�����ړ������̒l
        float NorthEnd_ = 0.0f;     //�X�e�[�W�k�[(�O��)�̈ʒu
        float SouthEnd_ = 0.0f;     //�X�e�[�W��[(���)�̈ʒu
        float EastEnd_ = 0.0f;      //�X�e�[�W���[(�E��)�̈ʒu
        float WestEnd_ = 0.0f;      //�X�e�[�W���[(����)�̈ʒu
    };
    //EndParam EndParam_;

    //----------�T�E���h�n���h��----------
    struct SoundParam
    {

        int hSoundcharge_ = -1;     //�`���[�W���̃n���h��
        int hSoundattack_ = -1;     //�ˌ����̃n���h��
        int hSoundCollision_ = -1;  //�ڐG���̃n���h��
        int hSoundJump_ = -1;       //�W�����v���̃n���h��
    };
    //SoundParam SoundParam_;

    //----------�������----------
    struct InitializeParam
    {
        int CharacterID = -1;                   //�Q�[���ɎQ������L�����N�^�[��id
        XMFLOAT3 StartPosition_ = { 0,0,0 };    //�����ʒu
        XMVECTOR FrontDirection_ = { 0,0,1 };   //���ʂ̏����l(���[�J�����W�n) ��������ǂꂾ����]������
        std::vector<IGameObserver*> observers;  //�Ď������Ώۂ̔z��
    };
    //InitializeParam InitParam_;

    //----------�ړ�----------
    struct MoveParam
    {
        float Velocity_ = 0.0f;                 //�����x ���̑��x�ɉ����x�����Z�����
        float Acceleration_ = 0.0f;             //�����x �_�b�V���U�����Ɏg�p����l
        float TmpAccele_ = 0.0f;                //�����x�㏸���Ɏg�����̒l
        float AcceleValue_ = 0.0f;              //Acceleration_�㏸���A1f������̑�����
        float FullAccelerate_ = 0.0f;           //�����x�̍ő�
        float Friction_ = 0.0f;                 //���C�W��(������) 1f������̌�����
        int hMoveArrow_ = -1;                   //�`���[�W���ɕ\�������󃂃f��
        XMVECTOR ForwardVector_ = { 0,0,0 };    //�L�����N�^�[���猩�����ʂ̕���(���[���h���W�n) ���g��y����]�ʂƂ����Čv�Z ���K�������l������
        XMVECTOR MoveDirection_ = { 0,0,0 };    //�ړ����� ���̒l�ɑ����̗v�f�������Ĉړ��x�N�g��������
        XMVECTOR NewPosition_ = { 0,0,0 };      //�ړ���̈ʒu�x�N�g��
        Transform ArrowTransform_;              //�`���[�W/�U���������̖��̃g�����X�t�H�[��
        XMFLOAT3 ArrowRotate_ = { 0,0,0 };      //���̏�����]
        XMFLOAT3 ArrowScale_ = { 0,0,0 };       //���̑傫��
        float AddArrowDepth_ = 0.0f;            //���̉��s��(�O����)�̒����l
    };
   // MoveParam MoveParam_;

    //----------��]----------
    struct RotateParam
    {
        float MoveRotateX = 0.0f;   //�ړ�����1f�̉�]��
        float FastRotateX = 0.0f;   //(�`���[�W���Ȃ�)������]����1f�̉�]��
    };
    //RotateParam RotateParam_;

    //----------��----------
    struct JumpParam
    {
        float Gravity_ = 0.0f;          //�d�� �L�����N�^�[�̉������ɂ������ ���ۂ̏d�͂��(9.8/60 m/s)���y�����Ă���
        bool IsOnGround_ = false;       //�n�ʂɂ��邩
        float JumpSpeed_ = 0.0f;        //�L�����N�^�[�̏�����Ɍ����� +�Ȃ�W�����v���Ă����� -�Ȃ牺�~�`�n�ʂɂ�����
        float JumpHeight = 0.0f;        //�W�����v���̈ꎞ�I�ɑ������l(=�W�����v�̍���)
        float HeightLowerLimit_ = 0.0f; //�����̉���
        float HeightUpperLimit_ = 0.0f; //�����̏��
        float MinusLimit_ = 0.0f;       //JumpSpeed�̍Œ�l(�O�̂��߃I�[�o�[�t���[��h�~����)
    };
    //JumpParam JumpParam_;


    //----------��e----------
    struct HitParam
    {
        float ColliderSize_ = 0.0f;                 //�����蔻��(����)�̃T�C�Y
        float OriginalRangeMin_ = 0.0f;             //�ϊ����̃m�b�N�o�b�N�ʂ̍ŏ��l
        float OriginalRangeMax_ = 0.0f;             //�ϊ����̃m�b�N�o�b�N�ʂ̍ő�l
        float ConvertedRangeMin_ = 0.0f;            //�ϊ���̃m�b�N�o�b�N�ʂ̍ŏ��l
        float ConvertedRangeMax_ = 0.0f;            //�ϊ���̃m�b�N�o�b�N�ʂ̍ő�l
        XMFLOAT3 KnockBack_Direction_ = { 0,0,0 };  //�m�b�N�o�b�N�������
        XMFLOAT3 KnockBack_Velocity_ = { 0,0,0 };   //�m�b�N�o�b�N���鑬�x
        float DecelerationRate_ = 0.0f;             //�m�b�N�o�b�N����1f���Ƃ̌�����
        float KnockBackEnd_ = 0.0f;                 //�m�b�N�o�b�N���I������l
        std::string AttackedName_ = "";             //�ڐG��������̖��O
    };
    //HitParam HitParam_;

    //----------��̐ڐG----------
    struct FenceHitParam
    {
        XMVECTOR UpperNormal_ = { 0,0,0 };                      //�X�e�[�W�k�[(�O��)�̖@���x�N�g��
        XMVECTOR LowerNormal_ = { 0,0,0 };                      //�X�e�[�W��[(���)�̖@���x�N�g��
        XMVECTOR RightNormal_ = { 0,0,0 };                      //�X�e�[�W���[(�E��)�̖@���x�N�g��
        XMVECTOR LeftNormal_ = { 0,0,0 };                       //�X�e�[�W���[(����)�̖@���x�N�g��
        std::vector<XMVECTOR> NormalArray_ = {};                //�e�@���x�N�g�����i�[�����z��
        std::vector<std::string> WireArray_ =
        { "UpperWire", "LowerWire", "RightWire" ,"LeftWire" };  //�e�S���̖��O�̔z��

        float KnockBackPower_ = 0.0f;                           //��q�b�g�Ńm�b�N�o�b�N���鋭���i�ω��Ȃ��j
        int InvincibilityTime_ = 0;                             //�_���[�W��̖��G���� 1f���Ƃɏ㏸
        bool IsInvincibility_ = false;                          //���G���Ԃ�
        int InvincibilityValue_ = 0;                            //���G���Ԃ̒l�@���̒l�𒴂���Ɩ��G���ԏI��
        int BlinkTimer_ = 0;                                    //�_���[�W��̓_�ŃJ�E���g
        int BlinkValue_ = 0;                                    //���̒l��blinkTimer�����B����ƒʏ�`�悷��(����܂ł͓_��)
    };
    //FenceHitParam FenceHitParam_;

    //----------�e�t��----------
    struct ShadowParam
    {
        int hShadow_ = -1;                  //�e�̃��f���n���h��
        float ShadowHeight_ = 0.0f;         //�e�����鍂��
        float ShadowCorrection_ = 0.0f;     //�e�̈ʒu�̒����l
        Transform ShadowTrans_;             //�e�̕`��g�����X�t�H�[��
        Ground* pGround_ = nullptr;         //�n�ʂ̃C���X�^���X
    };
    //ShadowParam ShadowParam_;

    //----------�G�t�F�N�g�֘A----------

    struct EffectParam
    {
        std::vector<float> ChargeParam_ = {};        //�`���[�W��ԃG�t�F�N�g�̃p�����[�^
        std::vector<float> FullChargeParam = {};     //�ő�`���[�W��ԃG�t�F�N�g�̃p�����[�^
        std::vector<float> AttackLocusParam_ = {};   //�ˌ��G�t�F�N�g�̃p�����[�^
        std::vector<float> HitEffectParam_ = {};     //�ڐG���̏Ռ��G�t�F�N�g�̃p�����[�^
        std::vector<float> FenceHitEffectParam_ = {};//��ɐڐG���̏Ռ��G�t�F�N�g�̃p�����[�^
    };
   // EffectParam EffectParam_;

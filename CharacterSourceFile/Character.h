#pragma once
#include "../Engine/GameObject.h"

//���i���������w�b�_�t�@�C��
#include "CharacterPart/CharacterParams.h"
#include "CharacterPart/CharacterModelBlink.h"
#include "CharacterPart/CharacterVfx.h"
#include "CharacterPart/CharacterShadow.h"
#include "CharacterPart/CharacterAir.h"
#include "CharacterPart/CharacterForward.h"
#include "CharacterPart/CharacterMovement.h"
#include "CharacterPart/CharacterRotate.h"
#include "CharacterPart/CharacterCharge.h"
#include "CharacterPart/CharacterHit.h"
#include "CharacterPart/CharacterFence.h"
#include "CharacterPart/CharacterCsvLoader.h"
#include "CharacterPart/CharacterObserver.h"

//�C���^�[�t�F�[�X

#include"../InterfaceSourceFile/IVFXEventListener.h"
#include"../InterfaceSourceFile/IChargeEventListner.h"
#include"../InterfaceSourceFile/IRotateEventListner.h"
#include"../InterfaceSourceFile/IMovementEventListener.h"


//�v���C���[,�G�N���X�̋��ʎ����N���X
class Character :
    public GameObject, 
    public IVFXEventListener, public IChargeEventListener,
    public IRotateEventListener, public IMovementEventListener
{
public:
    //----------���W���[���Q----------

    CharacterModelBlink* blink_;
    CharacterVFX*        vfx_;
    CharacterShadow*     shadow_;
    CharacterAir*        air_;
    CharacterForward*    forward_;
    CharacterMovement*   movement_;
    CharacterRotate*     rotate_;
    CharacterCharge*     charge_;
    CharacterHit*        hit_;
    CharacterFence*      fence_;
    CharacterCsvLoader*  csvload_;
    CharacterObserver*   observer_;

protected:

    //----------���ʃp�����[�^�Q----------
    EndParam EndParam_;
    SoundParam SoundParam_;
    InitializeParam InitParam_;
    MoveParam MoveParam_;
    RotateParam RotateParam_;
    JumpParam JumpParam_;
    HitParam HitParam_;
    FenceHitParam FenceHitParam_;
    ShadowParam ShadowParam_;
    EffectParam EffectParam_;

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    //�X�V(�p����̋��ʏ����̂ݍs��)
    void Update() override;

    //�`��(�p����̋��ʕ`��̂ݍs��)
    void Draw() override;

    //�`���[�W���G�t�F�N�g���o���C�x���g
    void OnChargeVFX() override
    {
        vfx_->SetFullChargeEffect(); 
    }

    //�ő�`���[�W�G�t�F�N�g���o���C�x���g
    void OnFullChargeVFX() override
    {
        vfx_->SetFullChargeEffect();
    }

    //�O�ՃG�t�F�N�g���o���C�x���g
    void OnAttackLocusVFX() override
    {
        vfx_->SetAttackLocusEffect();
    }

    //�ڐG�G�t�F�N�g���o���C�x���g
    void OnHitVFX() override
    {
        vfx_->SetHitEffect();
    }

    //��ڐG�G�t�F�N�g���o���C�x���g
    void OnFenceHitVFX() override
    {
        vfx_->SetFenceHitEffect();
    }

    //�`���[�W��(TmpAccele_)��0�ɂ���C�x���g
    void OnChargeReset() override
    {
        charge_->ChargeReset();
    }

    //X����]���s���C�x���g
    void OnMoveRotateX() override
    {
        rotate_->MoveRotateX();
    }

    //����X����]���s���C�x���g
    void OnFastRotateX() override
    {
        rotate_->FastRotateX();
    }

    //X����]��~���s���C�x���g
    void OnRotateXStop() override
    {
        rotate_->RotateXStop();
    }

    //�ړ��m�蔻����s���C�x���g
    bool OnIsOutsideStage(DirectX::XMFLOAT3 _position) override
    {
        return movement_->IsOutsideStage(_position);
    }

    //�����x��0�ɂ���C�x���g
    void OnAccelerationStop() override
    {
        movement_->AccelerationStop();
    }

    //�ړ��m�菈�����s���C�x���g
    void OnMoveConfirm() override
    {
		movement_->MoveConfirm();
    }

    //----------������----------

    /// <summary>
    /// CSV����e�X�e�[�^�X��ǂݍ��݂���
    /// Character�N���X����
    /// </summary>
    /// <param name="_path">csv�t�@�C���̃p�X</param>
    //void SetCSVStatus(std::string _path);

    /// <summary>
    /// �e�����̍򂩂�@���x�N�g�����擾��NormalArray��������
    /// </summary>
    /// void GetWireNormal();

    /// <summary>
    /// ���g�̈ʒu�������ʒu�ɐݒ�
    /// </summary>
    //void SetStartPosition() { this->transform_.position_ = InitParam_.StartPosition_; }

    /// <summary>
    /// ���g�����X�t�H�[���̏�����
    /// </summary>
    // void InitArrow();

    /// <summary>
    /// ���g���Ď�����Ώۂ�z��ɒǉ�
    /// BattleScene�ł̂ݍs����
    /// </summary>
    /// <param name="_observer">�ǉ�����Ď��Ώ�</param>
    //void AddObserver(IGameObserver* _observer);

    /// <summary>
    /// ���g���Ď�����Ώۂ�z�񂩂�폜
    /// </summary>
    /// <param name="_observer">�폜����Ď��Ώ�</param>
    //void RemoveObserver(IGameObserver* _observer);

    //----------�`��----------

    /// <summary>
    /// �L�����N�^�[���f���`��(�_���[�W���̓_�ŕ\�����s��)
    /// </summary>
    /// <param name="_handle">���f���n���h��</param>
    /// <param name="_transform">�`��ʒu�̃g�����X�t�H�[��</param>
    /// void DrawCharacterModel(int _handle, Transform _transform);

    /// <summary>
    /// �L�����N�^�[���ʂ�ImGui��`�� �ʗv�f�͔h����ŋL�q
    /// </summary>
    ///void DrawCharacterImGui();

    //----------�ړ�----------

    ///// <summary>
    ///// �L�����N�^�[�̈ړ�����
    ///// </summary>
    ///// <param name="_direction">����������</param>
    //void CharacterMove(XMVECTOR _direction);

    ///// <summary>
    ///// �ړ��x�N�g��������(�����x�N�g�����K�v)
    ///// </summary>
    //void CreateMoveVector();

    ///// <summary>
    ///// ��O����
    ///// </summary>
    ///// <param name="_position">�X�V�\��̈ʒu</param>
    ///// <returns>�ړ��\��</returns>
    //bool IsOutsideStage(XMFLOAT3 _position);

    ///// <summary>
    ///// �ړ��m�菈��
    ///// </summary>
    //void MoveConfirm();

    /// <summary>
    /// Y���̉�]�s����x�N�g���ɕϊ�
    /// </summary>
    /// <param name="_rotY">Y���ɉ�]�������p�x�iDegree�j</param>
    /// <param name="_front">���ʃx�N�g��(���[�J�����)</param>
    /// <returns>�ό`�����x�N�g���i���[���h��ԁj</returns>
    /// XMVECTOR RotateVecFront(float _rotY, XMVECTOR _front);

    /// <summary>
    /// ���ʃx�N�g�����X�V
    /// </summary>
    // void FrontVectorConfirm();

    /// <summary>
    /// �󂯎�����x�N�g������L�����N�^�[�̉�]�ʂ��v�Z
    /// </summary>
    /// <param name="_MoveVector">�i�s�����������x�N�g��</param>
    /// <returns>��]����p�x(Y����])</returns>
    /// float RotateDirectionVector(XMVECTOR _MoveVector);

    //----------�`���[�W----------

    ///// <summary>
    ///// �����x�̉��Z
    ///// </summary>
    //void Charging();

    ///// <summary>
    ///// �~�ς���TmpAccele_�����ۂɉ����x�ɑ��
    ///// </summary>
    //void ChargeRelease();

    ///// <summary>
    ///// TmpAccele_��0�ɂ���
    ///// </summary>
    //void ChargeReset();

    ///// <summary>
    ///// �`���[�W���̖��ʒu���Z�b�g
    ///// </summary>
    //void SetArrow();

    ///// <summary>
    ///// ��󃂃f���̕`��
    ///// </summary>
    //void DrawArrow();

    //----------�U��----------

 /*   /// <summary>
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
    bool IsDashStop() { if (MoveParam_.Acceleration_ <= 0.0f) return true; else return false; }*/


    //----------��----------

 /*   /// <summary>
    /// �d�͏���
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// �W�����v�J�n
    /// </summary>
    void SetJump();*/

    //----------��]----------

    ///// <summary>
    ///// �ʏ�X����]
    ///// </summary>
    //void MoveRotateX() { this->transform_.rotate_.x += RotateParam_.MoveRotateX; }

    ///// <summary>
    ///// �ʏ�X����](-x��])
    ///// </summary>
    //void MoveRotateXReverse() { this->transform_.rotate_.x -= RotateParam_.MoveRotateX; }

    ///// <summary>
    ///// ����X����]
    ///// </summary>
    //void FastRotateX() { this->transform_.rotate_.x += RotateParam_.FastRotateX; }

    ///// <summary>
    ///// ����X����](-x��])
    ///// </summary>
    //void FastRotateReverse() { this->transform_.rotate_.x -= RotateParam_.FastRotateX; }

    ///// <summary>
    ///// X��]���~�߂�
    ///// </summary>
    //void RotateXStop() { this->transform_.rotate_.x = 0.0f; }

    //----------��e----------

    /// <summary>
    /// �L�����N�^�[���m�̔��ˏ���
    /// </summary>
    /// <param name="_myVector">���g�̈ʒu�x�N�g��</param>
    /// <param name="_targetVector">����̈ʒu�x�N�g��</param>
    /// <param name="_myVelocity">���g�̉����x</param>
    /// <param name="_targetVelocity">����̉����x</param>
    /// <param name="_attackName">�U�������L�����N�^�[�̖��O</param>
    //void Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity, 
    //    std::string _attackName);

    ///// <summary>
    ///// �m�b�N�o�b�N����Y����]�p�̌v�Z �m�b�N�o�b�N���O�ɍs��
    ///// </summary>
    ///// <param name="_KnockBackVector">�m�b�N�o�b�N�������(���K������Ă��邱�ƑO��)</param>
    ///// <param name="_KnockBackValue">�m�b�N�o�b�N�����</param>
    //void KnockBackAngleY(XMFLOAT3 _KnockBackVector, float _KnockBackValue);

    ///// <summary>
    ///// �m�b�N�o�b�N�ړ�����
    ///// </summary>
    //void KnockBack();

    ///// <summary>
    ///// �w�肳�ꂽ�S���̖��O�ɉ������e�@���x�N�g�����擾����
    ///// </summary>
    ///// <param name="_normal">�L�����N�^�[���ڐG�����S���̖��O</param>
    ///// <returns>�S���̖��O�ɑΉ������@���x�N�g��</returns>
    //XMVECTOR HitNormal(std::string _normal);

    /// <summary>
    /// ��ɐڐG�����ۂ̌v�Z�����@��̖@���Ōv�Z
    /// </summary>
    /// <param name="_normal">���˂�������(�ڐG������̖@���x�N�g��)</param>
    //void FenceReflect(XMVECTOR _normal);

    /// <summary>
    /// �m�b�N�o�b�N�I������
    /// </summary>
    /// <returns>�m�b�N�o�b�N���x���I���l�ȉ���</returns>
    ///bool IsKnockBackEnd();

    /// <summary>
    /// �m�b�N�o�b�N���x��0�ɖ߂�
    /// </summary>
    /// void KnockBackVelocityReset() { HitParam_.KnockBack_Velocity_ = { 0,0,0 };}

    /// <summary>
    /// �_���[�W��̖��G���Ԃ̌v�Z
    /// </summary>
    //void InvincibilityTimeCalculation();

    /// <summary>
    /// �Ď�����Ώ�(�z��)�ɍ�Ƀq�b�g�������Ƃ�ʒm
    /// </summary>
    //void NotifyFenceHit();

    //----------�e�t��----------

    ///// <summary>
    ///// �e���f���̏�����
    ///// </summary>
    //void InitShadow();

    ///// <summary>
    ///// �e�t���i���t���[���X�V�j
    ///// </summary>
    //void ShadowSet();

    ///// <summary>
    ///// �e���f���`��
    ///// </summary>
    //void ShadowDraw();


    //----------�G�t�F�N�g����----------

  /*  /// <summary>
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
    /// ��ɐڐG���̏Ռ��G�t�F�N�g����
    /// </summary>
    void SetFenceHitEffect();*/

    //----------�Z�b�^�[�E�Q�b�^�[�֐�----------

    //�L�����N�^�[�̈ړ��������Z�b�g
    void SetEnd(float _upper, float _lower, float _left, float _right) {
        EndParam_.NorthEnd_ = _upper;
        EndParam_.SouthEnd_ = _lower;
        EndParam_.WestEnd_ = _left;
        EndParam_.EastEnd_ = _right;
    }

    //�������

    void SetID(int _id) { InitParam_.CharacterID = _id; }
    int GetID() const { return InitParam_.CharacterID; }

    void SetStartPosition(XMFLOAT3 _pos) { InitParam_.StartPosition_ = _pos; }
    XMFLOAT3 GetStartPosition() const { return InitParam_.StartPosition_; }

    //�ړ�

    void SetVelocity(float _velocity) { MoveParam_.Velocity_ = _velocity; }
    float GetVelocity() const { return MoveParam_.Velocity_; }

    void SetAcceleration(float _acceleration) { MoveParam_.Acceleration_ = _acceleration; }
    float GetAcceleration() const { return MoveParam_.Acceleration_; }

    void SetTmpAccele(float _tmpAccele) { MoveParam_.TmpAccele_ = _tmpAccele; }
    float GetTmpAccele() const { return MoveParam_.TmpAccele_; }

    void SetAcceleValue(float _acceleValue) { MoveParam_.AcceleValue_ = _acceleValue; }
    float GetAcceleValue() const { return MoveParam_.AcceleValue_; }

    void SetFullAccelerate(float _fullAccelerate) { MoveParam_.FullAccelerate_; }
    float GetFullAccelerate() { return MoveParam_.FullAccelerate_; }

    void SetFriction(float _friction) { MoveParam_.Friction_; }
    float GetFriction() { return  MoveParam_.Friction_; }

    //��]
    void SetMoveRotateX(float _moveRotate) { RotateParam_.MoveRotateX = _moveRotate; }
    float GetMoveRotateX() const { return RotateParam_.MoveRotateX ; }

    void SetFastRotateX(float _fastRotate) { RotateParam_.FastRotateX = _fastRotate; }
    float GetFastRotateX() const { return RotateParam_.FastRotateX; }

    //��
    void SetGravity(float _gravity) { JumpParam_.Gravity_ = _gravity; }
    float GetGravity() const { return JumpParam_.Gravity_ ; }

    void SetJumpHeight(float _jumpHeight) { JumpParam_.JumpHeight = _jumpHeight; }
    float GetJumpHeight() const { return JumpParam_.JumpHeight; }

    //��e
    void SetOriginalRangeMin(float _originalRangeMin) { HitParam_.OriginalRangeMin_ = _originalRangeMin; }
    float GetOriginalRangeMin() const { return HitParam_.OriginalRangeMin_; }

    void SetOriginalRangeMax(float _originalRangeMax) { HitParam_.OriginalRangeMax_ = _originalRangeMax; }
    float GetOriginalRangeMax() const { return HitParam_.OriginalRangeMax_; }

    void SetConvertedRangeMin(float _convertedRangeMin) { HitParam_.ConvertedRangeMin_ = _convertedRangeMin; }
    float GetConvertedRangeMin() const { return  HitParam_.ConvertedRangeMin_; }

    void SetConvertedRangeMax(float _convertedRangeMax) { HitParam_.ConvertedRangeMax_ = _convertedRangeMax; }
    float GetConvertedRangeMax() const { return  HitParam_.ConvertedRangeMax_; }

    void SetKnockBackDirection(XMFLOAT3 _knockbackDirection) { HitParam_.KnockBack_Direction_ = _knockbackDirection; }
    XMFLOAT3 GetKnockBackDirection()  const { return  HitParam_.KnockBack_Direction_; }

    void SetKnockBackVelocity(XMFLOAT3 _knockbackVelocity) { HitParam_.KnockBack_Velocity_ = _knockbackVelocity; }
    XMFLOAT3 GetKnockBackVelocity() const { return HitParam_.KnockBack_Velocity_; }

    void SetDecelerationRate(float _decelerationRate) { HitParam_.DecelerationRate_ = _decelerationRate; }
    float GetDecelerationRate()  const { return  HitParam_.DecelerationRate_; }

    void SetKnockBackEnd(float _knockbackEnd) { HitParam_.KnockBackEnd_ = _knockbackEnd; }
    float GetKnockBackEnd() const { return HitParam_.KnockBackEnd_; }

    void SetAttackedName(std::string _AttackedName) { HitParam_.AttackedName_ = _AttackedName; }
    std::string GetAttackedName() const { return HitParam_.AttackedName_; }

    //��̐ڐG
    void SetKnockBackPower(float _knockbackPower) { FenceHitParam_.KnockBackPower_ = _knockbackPower; }
    float GetKnockBackPower() const { return FenceHitParam_.KnockBackPower_; }

    void SetInvincibilityTime(int _invincibilityTime) { FenceHitParam_.InvincibilityTime_ = _invincibilityTime; }
    int GetInvincibilityTime() const { return FenceHitParam_.InvincibilityTime_; }

    void SetInvincibilityValue(int _invincibilityValue) { FenceHitParam_.InvincibilityValue_ = _invincibilityValue; }
    int GetInvincibilityValue() const { return FenceHitParam_.InvincibilityValue_; }

    void SetBlinkTimer(int _blinkTimer) { FenceHitParam_.BlinkTimer_ = _blinkTimer; }
    int GetBlinkTimer() const { return  FenceHitParam_.BlinkTimer_; }

    void SetBlinkValue(int _blinkValue) { FenceHitParam_.BlinkValue_ = _blinkValue; }
    int GetBlinkValue() const { return FenceHitParam_.BlinkValue_; }
};
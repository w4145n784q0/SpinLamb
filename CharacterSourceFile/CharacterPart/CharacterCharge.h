#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
#include "../../InterfaceSourceFile/IChargeEventListener.h"

class Character;

class CharacterCharge :
    public GameObject
{
private:
    //Character* owner_;

    IChargeEventListener* ChargeListener_;
public:
 
    //----------�`���[�W----------
    CharacterCharge(GameObject* parent);
    virtual ~CharacterCharge() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    // ���X�i�[���ォ��ݒ肷��
    void SetEventListener(IChargeEventListener* listener);

    /// <summary>
    /// ���g�����X�t�H�[���̏�����
    /// </summary>
    void InitArrow();

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

    /// <summary>
    /// ��󃂃f���̕`��
    /// </summary>
    void DrawArrow();
};


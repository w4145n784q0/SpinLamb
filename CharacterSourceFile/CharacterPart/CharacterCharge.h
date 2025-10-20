#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
#include "../../InterfaceSourceFile/IVFXEventListener.h"

//Character�N���X�̃`���[�W�֌W�̏������s�����W���[��

class Character;

class CharacterCharge :
    public GameObject
{
private:
    //�e�N���X(Character)�̃|�C���^
    Character* character_;

    //�g�p����p�����[�^(CharacterParams)�̃|�C���^
    CharacterParams* params_;

    //VFX�C�x���g��ʒm���郊�X�i�[
    IVFXEventListener* VFXListener_;
public:
 
    CharacterCharge(GameObject* parent);
    virtual ~CharacterCharge() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParams�̃Z�b�^�[�֐�
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //�e�N���X(Character)�̃Z�b�^�[�֐�
    void SetCharacter(Character* _character) {
        character_ = _character;
    }

    //���X�i�[�̃Z�b�^�[�֐�
    void SetEventListener(IVFXEventListener* listener);

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


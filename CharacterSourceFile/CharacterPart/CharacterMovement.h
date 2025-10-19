#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

class Character;

class CharacterMovement :
    public GameObject
{
private:
    Character* character_;
    CharacterParams* params_;

public:
    CharacterMovement(GameObject* parent);
    virtual ~CharacterMovement() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    void SetCharacter(Character* _character) {
        character_ = _character;
    }

    //----------�ړ�----------

    /// <summary>
    /// ���g�̈ʒu�������ʒu�ɐݒ�
    /// </summary>
    void InitStartPosition();

    /// <summary>
    /// �L�����N�^�[�̈ړ�����
    /// </summary>
    /// <param name="_direction">����������</param>
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

    //----------�U��----------

    /// <summary>
    /// ��������(�������̑����ʎg�p)
    /// </summary>
    void Deceleration();

    /// <summary>
    /// ���C�ɂ�錸������
    /// </summary>
    void FrictionDeceleration();

    /// <summary>
    /// �����x���Z�b�g
    /// </summary>
    void AccelerationStop();

    /// <summary>
    /// ��~����
    /// </summary>
    /// <returns>�����ʂ�0.0�ȉ����ǂ���</returns>
    bool IsDashStop();

};


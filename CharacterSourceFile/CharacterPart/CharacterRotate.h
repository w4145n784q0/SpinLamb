#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

class CharacterRotate :
    public GameObject
{
private:
    CharacterParams* params_;

public:
    CharacterRotate(GameObject* parent);
    virtual ~CharacterRotate() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //----------��]----------

    /// <summary>
    /// �󂯎�����x�N�g������L�����N�^�[�̉�]�ʂ��v�Z
    /// </summary>
    /// <param name="_MoveVector">�i�s�����������x�N�g��</param>
    /// <returns>��]����p�x(Y����])</returns>
    float RotateDirectionVector(XMVECTOR _MoveVector);

    /// <summary>
    /// �ʏ�X����]
    /// </summary>
    void MoveRotateX();

    /// <summary>
    /// ����X����]
    /// </summary>
    void FastRotateX();

    /// <summary>
    /// X��]���~�߂�
    /// </summary>
    void RotateXStop();
};


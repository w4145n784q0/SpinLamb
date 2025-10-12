#pragma once
#include"../../Engine/GameObject.h"

class CharacterRotate :
    public GameObject
{
public:
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
    /// �ʏ�X����](-x��])
    /// </summary>
    void MoveRotateXReverse();

    /// <summary>
    /// ����X����]
    /// </summary>
    void FastRotateX();

    /// <summary>
    /// ����X����](-x��])
    /// </summary>
    void FastRotateReverse();

    /// <summary>
    /// X��]���~�߂�
    /// </summary>
    void RotateXStop();

};


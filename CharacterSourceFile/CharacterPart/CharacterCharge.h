#pragma once
#include"../../Engine/GameObject.h"
class CharacterCharge :
    public GameObject
{
public:
 
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

    /// <summary>
    /// ��󃂃f���̕`��
    /// </summary>
    void DrawArrow();
};


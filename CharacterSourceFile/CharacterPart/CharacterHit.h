#pragma once
#include"../../Engine/GameObject.h"

class CharacterHit :
    public GameObject
{
public:
    //----------��e----------

    /// <summary>
    /// �L�����N�^�[���m�̔��ˏ���
    /// </summary>
    /// <param name="_myVector">���g�̈ʒu�x�N�g��</param>
    /// <param name="_targetVector">����̈ʒu�x�N�g��</param>
    /// <param name="_myVelocity">���g�̉����x</param>
    /// <param name="_targetVelocity">����̉����x</param>
    /// <param name="_attackName">�U�������L�����N�^�[�̖��O</param>
    void Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity, 
        std::string _attackName);

    /// <summary>
    /// �m�b�N�o�b�N����Y����]�p�̌v�Z �m�b�N�o�b�N���O�ɍs��
    /// </summary>
    /// <param name="_KnockBackVector">�m�b�N�o�b�N�������(���K������Ă��邱�ƑO��)</param>
    /// <param name="_KnockBackValue">�m�b�N�o�b�N�����</param>
    void KnockBackAngleY(XMFLOAT3 _KnockBackVector, float _KnockBackValue);

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
    /// �m�b�N�o�b�N�I������
    /// </summary>
    /// <returns>�m�b�N�o�b�N���x���I���l�ȉ���</returns>
    bool IsKnockBackEnd();

    /// <summary>
    /// �m�b�N�o�b�N���x��0�ɖ߂�
    /// </summary>
    void KnockBackVelocityReset();

    /// <summary>
    /// �_���[�W��̖��G���Ԃ̌v�Z
    /// </summary>
    void InvincibilityTimeCalculation();

};


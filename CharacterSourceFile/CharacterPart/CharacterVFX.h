#pragma once

#include"../../Engine/GameObject.h"
class CharacterVFX :
    public GameObject
{
public:
    //----------�G�t�F�N�g����----------

    /// <summary>
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
    void SetFenceHitEffect();
};


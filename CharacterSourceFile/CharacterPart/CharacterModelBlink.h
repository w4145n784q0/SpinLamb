#pragma once
#include "../../Engine/GameObject.h"
class CharacterModelBlink :
    public GameObject
{
public:

    //----------�`��----------

    /// <summary>
    /// �L�����N�^�[���f���`��(�_���[�W���̓_�ŕ\�����s��)
    /// </summary>
    /// <param name="_handle">���f���n���h��</param>
    /// <param name="_transform">�`��ʒu�̃g�����X�t�H�[��</param>
    void DrawCharacterModel(int _handle, Transform _transform);
};

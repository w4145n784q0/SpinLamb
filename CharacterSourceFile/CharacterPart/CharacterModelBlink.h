#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterModelBlink :
    public GameObject
{
public:
    CharacterModelBlink(GameObject* parent);
    virtual ~CharacterModelBlink() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //----------�`��----------

    /// <summary>
    /// �L�����N�^�[���f���`��(�_���[�W���̓_�ŕ\�����s��)
    /// </summary>
    /// <param name="_handle">���f���n���h��</param>
    /// <param name="_transform">�`��ʒu�̃g�����X�t�H�[��</param>
    void DrawCharacterModel(int _handle, Transform _transform);
};

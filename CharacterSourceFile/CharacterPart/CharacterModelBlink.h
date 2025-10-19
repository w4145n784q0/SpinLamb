#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterModelBlink :
    public GameObject
{
private:
    CharacterParams* params_;

public:
    CharacterModelBlink(GameObject* parent);
    virtual ~CharacterModelBlink() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //----------�`��----------

    /// <summary>
    /// �L�����N�^�[���f���`��(�_���[�W���̓_�ŕ\�����s��)
    /// </summary>
    /// <param name="_handle">���f���n���h��</param>
    /// <param name="_transform">�`��ʒu�̃g�����X�t�H�[��</param>
    void DrawCharacterModel(int _handle, Transform _transform);
};

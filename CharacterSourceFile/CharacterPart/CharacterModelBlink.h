#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

//Character�N���X�̃��f���`��֌W�̏������s�����W���[��

class CharacterModelBlink :
    public GameObject
{
private:
    //�g�p����p�����[�^(CharacterParams)�̃|�C���^
    CharacterParams* params_;

public:
    CharacterModelBlink(GameObject* parent);
    virtual ~CharacterModelBlink() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParams�̃Z�b�^�[�֐�
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    /// <summary>
    /// �L�����N�^�[���f���`��(�_���[�W���̓_�ŕ\�����s��)
    /// </summary>
    /// <param name="_handle">���f���n���h��</param>
    /// <param name="_transform">�`��ʒu�̃g�����X�t�H�[��</param>
    void DrawCharacterModel(int _handle, Transform _transform);
};

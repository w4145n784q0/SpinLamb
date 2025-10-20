#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

//Character�N���X�̋󒆊֌W�̏������s�����W���[��

class Character;

class CharacterAir :
    public GameObject
{
private:
    //�e�N���X(Character)�̃|�C���^
    Character* character_;

    //�g�p����p�����[�^(CharacterParams)�̃|�C���^
    CharacterParams* params_;

public:
    CharacterAir(GameObject* parent);
    virtual ~CharacterAir() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParams�̃Z�b�^�[�֐�
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //�e�N���X(Character)�̃Z�b�^�[�֐�
    void SetCharacter(Character* _character) {
        character_ = _character;
    }

    /// <summary>
    /// �d�͏���
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// �W�����v�J�n
    /// </summary>
    void SetJump();
};


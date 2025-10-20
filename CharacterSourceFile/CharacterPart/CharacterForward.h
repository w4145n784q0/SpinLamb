#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

//Character�N���X�̑O�������֌W�̏������s�����W���[��

class Character;

class CharacterForward :
    public GameObject
{
private:
    //�e�N���X(Character)�̃|�C���^
    Character* character_;

    //�g�p����p�����[�^(CharacterParams)�̃|�C���^
    CharacterParams* params_;

public:
    CharacterForward(GameObject* parent);
    virtual ~CharacterForward() = default;

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
    /// Y���̉�]�s����x�N�g���ɕϊ�
    /// </summary>
    /// <param name="_rotY">Y���ɉ�]�������p�x�iDegree�j</param>
    /// <param name="_front">���ʃx�N�g��(���[�J�����)</param>
    /// <returns>�ό`�����x�N�g���i���[���h��ԁj</returns>
    XMVECTOR RotateVecFront(float _rotY, XMVECTOR _front);

    /// <summary>
    /// ���ʃx�N�g�����X�V
    /// </summary>
    void FrontVectorConfirm();
};


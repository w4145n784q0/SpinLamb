#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

//Character�N���X��-CSV�ǂݍ��݊֌W�̏������s�����W���[��

class Character;

class CharacterCsvLoader :
    public GameObject
{
private:
    //�e�N���X(Character)�̃|�C���^
    Character* character_;

    //�g�p����p�����[�^(CharacterParams)�̃|�C���^
    CharacterParams* params_;

public:
    CharacterCsvLoader(GameObject* parent);
    virtual ~CharacterCsvLoader() = default;

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
    /// CSV����e�X�e�[�^�X��ǂݍ��݂���
    /// Character�N���X����
    /// </summary>
    /// <param name="_path">csv�t�@�C���̃p�X</param>
    void SetCSVStatus(std::string _path);
};



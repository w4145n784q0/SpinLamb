#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
class CharacterCsvLoader :
    public GameObject
{
private:
    CharacterParams* params_;

public:
    CharacterCsvLoader(GameObject* parent);
    virtual ~CharacterCsvLoader() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //----------CSV�ǂݍ��ݏ���----------

    /// <summary>
    /// CSV����e�X�e�[�^�X��ǂݍ��݂���
    /// Character�N���X����
    /// </summary>
    /// <param name="_path">csv�t�@�C���̃p�X</param>
    void SetCSVStatus(std::string _path);
};



#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
class CharacterCsvLoader :
    public GameObject
{
public:
    //----------CSV�ǂݍ��ݏ���----------

    /// <summary>
    /// CSV����e�X�e�[�^�X��ǂݍ��݂���
    /// Character�N���X����
    /// </summary>
    /// <param name="_path">csv�t�@�C���̃p�X</param>
    void SetCSVStatus(std::string _path);
};



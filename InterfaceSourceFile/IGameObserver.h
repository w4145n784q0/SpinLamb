#pragma once
#include<string>
//�I�u�U�[�o�[�C���^�[�t�F�[�X
//BattleScene�݂̂��̃C���^�[�t�F�[�X���p��


//�L�����N�^�[(Player,Enemy)����Ƀq�b�g�������̊Ď���
class IGameObserver {
public:

    //��Ƀq�b�g�����ۂ̏���(�p����Ŏ���)
    virtual void OnCharacterFenceHit(std::string _AttackedName, std::string _HitName) = 0;
    virtual ~IGameObserver() = default;
};
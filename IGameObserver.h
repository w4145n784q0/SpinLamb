#pragma once

//�I�u�U�[�o�[�C���^�[�t�F�[�X
//BattleScene�݂̂��̃C���^�[�t�F�[�X���p��


//�L�����N�^�[(Player,Enemy)���ǂɃq�b�g�������̊Ď���
class IGameObserver {
public:

    //��Ƀq�b�g�����ۂ̏���(�p����Ŏ���)
    virtual void OnCharacterFenceHit(int HitCharacterId) = 0;
    virtual ~IGameObserver() = default;
};
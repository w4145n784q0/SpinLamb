#pragma once
#include"../../Engine/GameObject.h"
#include"../../InterfaceSourceFile/IGameObserver.h"

class CharacterObserver :
    public GameObject
{
public:
    //----------�Ď�(�I�u�U�[�o�[�p�^�[��)����----------

    /// <summary>
    /// ���g���Ď�����Ώۂ�z��ɒǉ�
    /// BattleScene�ł̂ݍs����
    /// </summary>
    /// <param name="_observer">�ǉ�����Ď��Ώ�</param>
    void AddObserver(IGameObserver* _observer);

    /// <summary>
    /// ���g���Ď�����Ώۂ�z�񂩂�폜
    /// </summary>
    /// <param name="_observer">�폜����Ď��Ώ�</param>
    void RemoveObserver(IGameObserver* _observer);
};


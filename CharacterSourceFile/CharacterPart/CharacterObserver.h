#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
#include"../../InterfaceSourceFile/IGameObserver.h"

class CharacterObserver :
    public GameObject
{
private:
    CharacterParams* params_;

public:
    CharacterObserver(GameObject* parent);
    virtual ~CharacterObserver() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

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


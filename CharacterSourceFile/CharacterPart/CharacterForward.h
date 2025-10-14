#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterForward :
    public GameObject
{
public:
    CharacterForward(GameObject* parent);
    virtual ~CharacterForward() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};


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


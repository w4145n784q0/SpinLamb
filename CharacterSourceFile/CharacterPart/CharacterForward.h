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
    /// Y軸の回転行列をベクトルに変換
    /// </summary>
    /// <param name="_rotY">Y軸に回転したい角度（Degree）</param>
    /// <param name="_front">正面ベクトル(ローカル空間)</param>
    /// <returns>変形したベクトル（ワールド空間）</returns>
    XMVECTOR RotateVecFront(float _rotY, XMVECTOR _front);

    /// <summary>
    /// 正面ベクトルを更新
    /// </summary>
    void FrontVectorConfirm();
};


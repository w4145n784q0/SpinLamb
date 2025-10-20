#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

//Characterクラスの前方方向関係の処理を行うモジュール

class Character;

class CharacterForward :
    public GameObject
{
private:
    //親クラス(Character)のポインタ
    Character* character_;

    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:
    CharacterForward(GameObject* parent);
    virtual ~CharacterForward() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParamsのセッター関数
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //親クラス(Character)のセッター関数
    void SetCharacter(Character* _character) {
        character_ = _character;
    }

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


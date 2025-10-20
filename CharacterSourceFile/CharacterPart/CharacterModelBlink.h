#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

//Characterクラスのモデル描画関係の処理を行うモジュール

class CharacterModelBlink :
    public GameObject
{
private:
    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:
    CharacterModelBlink(GameObject* parent);
    virtual ~CharacterModelBlink() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParamsのセッター関数
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    /// <summary>
    /// キャラクターモデル描画(ダメージ時の点滅表現等行う)
    /// </summary>
    /// <param name="_handle">モデルハンドル</param>
    /// <param name="_transform">描画位置のトランスフォーム</param>
    void DrawCharacterModel(int _handle, Transform _transform);
};

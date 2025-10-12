#pragma once
#include "../../Engine/GameObject.h"
class CharacterModelBlink :
    public GameObject
{
public:

    //----------描画----------

    /// <summary>
    /// キャラクターモデル描画(ダメージ時の点滅表現等行う)
    /// </summary>
    /// <param name="_handle">モデルハンドル</param>
    /// <param name="_transform">描画位置のトランスフォーム</param>
    void DrawCharacterModel(int _handle, Transform _transform);
};

#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterShadow :
    public GameObject
{
public:
    CharacterShadow(GameObject* parent);
    virtual ~CharacterShadow() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};


    //----------影付け----------

   /// <summary>
   /// 影モデルの初期化
   /// </summary>
    void InitShadow();

    /// <summary>
    /// 影付け（毎フレーム更新）
    /// </summary>
    void ShadowSet();

    /// <summary>
    /// 影モデル描画
    /// </summary>
    void ShadowDraw();
};


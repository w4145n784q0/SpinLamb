#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

class Character;

class CharacterShadow :
    public GameObject
{
private:
    Character* character_;
    CharacterParams* params_;

public:
    CharacterShadow(GameObject* parent);
    virtual ~CharacterShadow() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    void SetCharacter(Character* _character) {
        character_ = _character;
    }

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


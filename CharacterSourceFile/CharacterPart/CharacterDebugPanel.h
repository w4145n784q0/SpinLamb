#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

//Characterクラスのデバッグ情報(ImGui)表示を行うモジュール

class Character;

class CharacterDebugPanel :
    public GameObject
{
private:
    //親クラス(Character)のポインタ
    Character* character_;

    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:
    CharacterDebugPanel(GameObject* parent);
    virtual ~CharacterDebugPanel() = default;

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
    /// キャラクター共通のImGuiを描画 個別要素は派生先で記述
    /// </summary>
    void DrawCharacterImGui();
};


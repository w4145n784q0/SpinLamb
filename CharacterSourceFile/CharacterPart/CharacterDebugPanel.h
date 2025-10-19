#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
class CharacterDebugPanel :
    public GameObject
{
private:
    CharacterParams* params_;

public:
    CharacterDebugPanel(GameObject* parent);
    virtual ~CharacterDebugPanel() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //----------デバッグ情報----------

    /// <summary>
    /// キャラクター共通のImGuiを描画 個別要素は派生先で記述
    /// </summary>
    void DrawCharacterImGui();
};


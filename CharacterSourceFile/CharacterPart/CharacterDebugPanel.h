#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
class CharacterDebugPanel :
    public GameObject
{
    CharacterDebugPanel(GameObject* parent);
    virtual ~CharacterDebugPanel() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //----------�f�o�b�O���----------

    /// <summary>
    /// �L�����N�^�[���ʂ�ImGui��`�� �ʗv�f�͔h����ŋL�q
    /// </summary>
    void DrawCharacterImGui();
};


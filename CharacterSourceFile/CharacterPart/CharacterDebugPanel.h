#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
class CharacterDebugPanel :
    public GameObject
{
    //----------�f�o�b�O���----------

    /// <summary>
    /// �L�����N�^�[���ʂ�ImGui��`�� �ʗv�f�͔h����ŋL�q
    /// </summary>
    void DrawCharacterImGui();
};


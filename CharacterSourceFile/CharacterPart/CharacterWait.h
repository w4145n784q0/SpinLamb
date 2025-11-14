#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

//Characterクラスの待機時間関係の処理を行うモジュール

class Character;

class CharacterWait :
    public GameObject
{
private:
    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:

    CharacterWait(GameObject* parent);
    virtual ~CharacterWait() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParamsのセッター関数
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //WaitTimeを増加
    void WaitTimerAdd();

    //WaitTimeをリセット
    void WaitTimeReset();

    //待機時間を過ぎたか
    bool IsTimeOverWaitTime();

};


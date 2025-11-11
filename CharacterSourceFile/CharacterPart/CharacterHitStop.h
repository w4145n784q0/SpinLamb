#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

class Character;

//Characterクラスのヒットストップ関係の処理を行うモジュール
class CharacterHitStop :
    public GameObject
{
private:
    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:
    CharacterHitStop(GameObject* parent);
    virtual ~CharacterHitStop() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParamsのセッター関数
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    /// <summary>
    /// HitStopTimerを増加
    /// </summary>
    /// <param name="_add">一度に増加させる値 デフォルトは0</param>
    void HitStopTimerAdd(int _add = 0);

    //HitStopTimerをリセット
    void HitStopTimerReset();

    //ヒットストップ時間を過ぎたか
    bool IsTimeOverHitStopTime();
};


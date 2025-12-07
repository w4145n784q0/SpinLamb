#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

//Characterクラスのサウンド関係の処理を行うモジュール

class  CharacterSound :
    public GameObject
{
private:
    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:
    CharacterSound(GameObject* parent);
    virtual ~CharacterSound() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParamsのセッター関数
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //サウンドの初期化(読み込み)
    void LoadSound();

    //チャージ音再生
    void PlayChargeSound();

    //攻撃音再生
    void PlayAttackSound();

    //相手とのヒット音再生
    void PlayCharacterHitSound();

    //柵とのヒット音再生
    void PlayFenceHitSound();

    //ジャンプ音再生
    void PlayJumpSound();

    //着地音再生
    void PlayLandingSound();
};
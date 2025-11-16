#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

//Characterクラスの柵関係の処理を行うモジュール

class Character;

class CharacterFence :
    public GameObject
{    
private:
    //親クラス(Character)のポインタ
    Character* character_;

    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:

    CharacterFence(GameObject* parent);
    virtual ~CharacterFence() = default;

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
    /// 各方向の柵から法線ベクトルを取得しNormalArrayを初期化
    /// </summary>
    void GetWireNormal();

    /// <summary>
    /// 柵に接触した際の計算処理　柵の法線で計算
    /// </summary>
    /// <param name="_normal">反射される方向(接触した柵の法線ベクトル)</param>
    void FenceReflect(XMVECTOR _normal);

    /// <summary>
    /// 監視する対象(配列)に柵にヒットしたことを通知
    /// </summary>
    void NotifyFenceHit();

    /// <summary>
    /// ダメージ後の無敵時間の計算
    /// </summary>
    void InvincibilityTimeCalculation();
};


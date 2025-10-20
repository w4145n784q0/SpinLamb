#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
#include "../../InterfaceSourceFile/IVFXEventListener.h"

//Characterクラスのチャージ関係の処理を行うモジュール

class Character;

class CharacterCharge :
    public GameObject
{
private:
    //親クラス(Character)のポインタ
    Character* character_;

    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

    //VFXイベントを通知するリスナー
    IVFXEventListener* VFXListener_;
public:
 
    CharacterCharge(GameObject* parent);
    virtual ~CharacterCharge() = default;

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

    //リスナーのセッター関数
    void SetEventListener(IVFXEventListener* listener);

    /// <summary>
    /// 矢印トランスフォームの初期化
    /// </summary>
    void InitArrow();

    /// <summary>
    /// 加速度の加算
    /// </summary>
    void Charging();

    /// <summary>
    /// 蓄積したTmpAccele_を実際に加速度に代入
    /// </summary>
    void ChargeRelease();

    /// <summary>
    /// TmpAccele_を0にする
    /// </summary>
    void ChargeReset();

    /// <summary>
    /// チャージ中の矢印位置をセット
    /// </summary>
    void SetArrow();

    /// <summary>
    /// 矢印モデルの描画
    /// </summary>
    void DrawArrow();
};


#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

class Character;

class CharacterCharge :
    public GameObject
{
private:
    Character* owner_;
public:
 
    //----------チャージ----------
    CharacterCharge(GameObject* parent);
    virtual ~CharacterCharge() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

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


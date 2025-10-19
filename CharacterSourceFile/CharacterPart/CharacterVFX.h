#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

class Character;

class CharacterVFX :
    public GameObject
{
private:
    Character* character_;
    CharacterParams* params_;

public:
    CharacterVFX(GameObject* parent);
	virtual ~CharacterVFX() = default;

    void Initialize() override {};
	void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    void SetCharacter(Character* _character) {
        character_ = _character;
    }

    //----------エフェクト処理----------

    /// <summary>
    /// CSV用データの初期化
    /// </summary>
    void InitCSVEffect();

    /// <summary>
    /// チャージ状態エフェクトをつける
    /// </summary>
    /// <param name="_path">csvを読み込むパス</param>
    void SetChargingEffect(std::string _path);

    /// <summary>
    /// 最大チャージ状態エフェクト(光の拡散)をつける
    /// </summary>
    void SetFullChargeEffect();

    /// <summary>
    /// 突撃エフェクトつける(プレイヤーの背後に光の粒子)
    /// </summary>
    void SetAttackLocusEffect();

    /// <summary>
    /// 相手と接触時の衝撃エフェクトつける
    /// </summary>
    void SetHitEffect();

    /// <summary>
    /// 柵に接触時の衝撃エフェクトつける
    /// </summary>
    void SetFenceHitEffect();
};


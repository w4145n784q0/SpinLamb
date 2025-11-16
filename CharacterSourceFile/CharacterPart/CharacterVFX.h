#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

//Characterクラスのエフェクト関係の処理を行うモジュール

class Character;

class CharacterVFX :
    public GameObject
{
private:
    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:
    CharacterVFX(GameObject* parent);
	virtual ~CharacterVFX() = default;

    void Initialize() override {};
	void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParamsのセッター関数
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    /// <summary>
    /// CSV用データの初期化
    /// </summary>
    void InitCSVEffect();

    /// <summary>
    /// チャージ状態エフェクトをつける
    /// </summary>
    /// <param name="_path">csvを読み込むパス</param>
    void SetChargingEffect(std::string _path, XMFLOAT3 _pos);

    /// <summary>
    /// 最大チャージ状態エフェクト(光の拡散)をつける
    /// </summary>
    void SetFullChargeEffect(XMFLOAT3 _pos);

    /// <summary>
    /// 突撃エフェクトつける(プレイヤーの背後に光の粒子)
    /// </summary>
    void SetAttackLocusEffect(XMFLOAT3 _pos);

    /// <summary>
    /// 相手と接触時の衝撃エフェクトつける
    /// </summary>
    void SetHitEffect(XMFLOAT3 _pos);

    /// <summary>
    /// 柵に接触時の衝撃エフェクトつける
    /// </summary>
    void SetFenceHitEffect(XMFLOAT3 _pos);
};
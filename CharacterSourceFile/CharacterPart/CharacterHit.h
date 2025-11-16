#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

//Characterクラスの被弾関係の処理を行うモジュール

class Character;

class CharacterHit :
    public GameObject
{
private:
    //親クラス(Character)のポインタ
    Character* character_;

    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:

    CharacterHit(GameObject* parent);
    virtual ~CharacterHit() = default;

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
    /// キャラクター同士の反射処理
    /// </summary>
    /// <param name="_myVector">自身の位置ベクトル</param>
    /// <param name="_targetVector">相手の位置ベクトル</param>
    /// <param name="_myVelocity">自身の加速度</param>
    /// <param name="_targetVelocity">相手の加速度</param>
    /// <param name="_attackName">攻撃したキャラクターの名前</param>
    void Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity, 
        std::string _attackName);

    /// <summary>
    /// ノックバック中のY軸回転角の計算 ノックバック直前に行う
    /// </summary>
    /// <param name="_KnockBackVector">ノックバックする方向(正規化されていること前提)</param>
    /// <param name="_KnockBackValue">ノックバックする量</param>
    void KnockBackAngleY(XMFLOAT3 _KnockBackVector, float _KnockBackValue);

    /// <summary>
    /// ノックバック移動処理
    /// </summary>
    void KnockBack();

    /// <summary>
    /// 指定された鉄線の名前に応じた各法線ベクトルを取得する
    /// </summary>
    /// <param name="_normal">キャラクターが接触した鉄線の名前</param>
    /// <returns>鉄線の名前に対応した法線ベクトル</returns>
    XMVECTOR HitNormal(std::string _normal);

    /// <summary>
    /// ノックバック終了判定
    /// </summary>
    /// <returns>ノックバック速度が終了値以下か</returns>
    bool IsKnockBackEnd();

    /// <summary>
    /// ノックバック速度を0に戻す
    /// </summary>
    void KnockBackVelocityReset();

};


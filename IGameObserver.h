#pragma once

//オブザーバーインターフェース
//BattleSceneのみこのインターフェースを継承


//キャラクター(Player,Enemy)が壁にヒットした時の監視役
class IGameObserver {
public:

    //柵にヒットした際の処理(継承先で実装)
    virtual void OnCharacterFenceHit(int HitCharacterId) = 0;
    virtual ~IGameObserver() = default;
};
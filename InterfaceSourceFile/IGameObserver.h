#pragma once
#include<string>
//オブザーバーインターフェース
//BattleSceneのみこのインターフェースを継承


//キャラクター(Player,Enemy)が柵にヒットした時の監視役
class IGameObserver {
public:

    //柵にヒットした際の処理(継承先で実装)
    virtual void OnCharacterFenceHit(std::string _AttackedName, std::string _HitName) = 0;
    virtual ~IGameObserver() = default;
};
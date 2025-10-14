#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
#include"../../InterfaceSourceFile/IGameObserver.h"

class CharacterObserver :
    public GameObject
{
public:
    CharacterObserver(GameObject* parent);
    virtual ~CharacterObserver() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};


    //----------監視(オブザーバーパターン)処理----------

    /// <summary>
    /// 自身を監視する対象を配列に追加
    /// BattleSceneでのみ行われる
    /// </summary>
    /// <param name="_observer">追加する監視対象</param>
    void AddObserver(IGameObserver* _observer);

    /// <summary>
    /// 自身を監視する対象を配列から削除
    /// </summary>
    /// <param name="_observer">削除する監視対象</param>
    void RemoveObserver(IGameObserver* _observer);
};


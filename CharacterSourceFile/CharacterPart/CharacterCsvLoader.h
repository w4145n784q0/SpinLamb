#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

//Characterクラスの-CSV読み込み関係の処理を行うモジュール

class Character;

class CharacterCsvLoader :
    public GameObject
{
private:
    //親クラス(Character)のポインタ
    Character* character_;

    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:
    CharacterCsvLoader(GameObject* parent);
    virtual ~CharacterCsvLoader() = default;

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
    /// CSVから各ステータスを読み込みする
    /// Characterクラス限定
    /// </summary>
    /// <param name="_path">csvファイルのパス</param>
    void SetCSVStatus(std::string _path);
};



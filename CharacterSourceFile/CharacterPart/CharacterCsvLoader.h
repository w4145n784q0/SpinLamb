#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

class Character;

class CharacterCsvLoader :
    public GameObject
{
private:
    Character* character_;
    CharacterParams* params_;

public:
    CharacterCsvLoader(GameObject* parent);
    virtual ~CharacterCsvLoader() = default;

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

    //----------CSV読み込み処理----------

    /// <summary>
    /// CSVから各ステータスを読み込みする
    /// Characterクラス限定
    /// </summary>
    /// <param name="_path">csvファイルのパス</param>
    void SetCSVStatus(std::string _path);
};



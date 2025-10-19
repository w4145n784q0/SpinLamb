#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
class CharacterCsvLoader :
    public GameObject
{
private:
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

    //----------CSV読み込み処理----------

    /// <summary>
    /// CSVから各ステータスを読み込みする
    /// Characterクラス限定
    /// </summary>
    /// <param name="_path">csvファイルのパス</param>
    void SetCSVStatus(std::string _path);
};



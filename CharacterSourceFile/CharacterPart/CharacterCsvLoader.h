#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
class CharacterCsvLoader :
    public GameObject
{
public:
    //----------CSV読み込み処理----------

    /// <summary>
    /// CSVから各ステータスを読み込みする
    /// Characterクラス限定
    /// </summary>
    /// <param name="_path">csvファイルのパス</param>
    void SetCSVStatus(std::string _path);
};



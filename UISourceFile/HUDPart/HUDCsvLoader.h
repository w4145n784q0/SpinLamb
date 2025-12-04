#pragma once
#include "../../Engine/GameObject.h"
#include"HUDParam.h"

class HUDCsvLoader :
    public GameObject
{
private:
    //使用するパラメータ(HUDParams)のポインタ
    HUDParam* hudParam_;

public:
    HUDCsvLoader() = default;
    HUDCsvLoader(GameObject* parent);
    virtual ~HUDCsvLoader() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CSVファイルから必要パラメータを読み込みする
    void SetHUDCSV();
};


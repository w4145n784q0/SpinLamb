#pragma once
#include "../../Engine/GameObject.h"
#include"HUDParam.h"

//HUDクラスの画像読み込み関係の処理を行うモジュール

class HUDImageLoader :
    public GameObject
{
private:
    //使用するパラメータ(HUDParams)のポインタ
    HUDParam* hudParam_;

public:
    HUDImageLoader() = default;
    HUDImageLoader(GameObject* parent);
    virtual ~HUDImageLoader() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //HUDParamをセットする
    void SetHUDParam(HUDParam* _param) {
        hudParam_ = _param;
    }

    //画像の初期化
    void ImageInitialize();
};


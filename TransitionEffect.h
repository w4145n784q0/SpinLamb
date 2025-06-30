#pragma once
#include "Engine/GameObject.h"
#include"Engine/Image.h"

//画面遷移時のエフェクト用クラス
class TransitionEffect :
    public GameObject
{
private:
    //----------画像ハンドル----------

    //フェードイン/アウト演出用画像
    int hFadeBlack_;
    int hFadeWhite_;

    //ズームイン/アウト用画像
    int hZoomSheep_;

    //----------画面遷移エフェクトの種類----------
    enum EffectType {

        NoneEffect = 0,
        S_FadeOutBlack,//黒くフェードアウト
        S_FadeInBlack,//白くフェードイン
        S_FadeOutWhite,//フェードイン
        S_FadeInWhite,//白くフェードイン
        S_SlideInLTR,//スライドイン
        S_ZoomIn,//ズームイン
        S_ZoomOut,//ズームアウト
    };
    EffectType EffectType_;

    //遷移時間 他クラスから代入される
    int TransitionTime_;

    //----------各画面遷移の演出用の構造体----------

    //フェードイン/アウト用
    struct FadeInOut
    {
        Transform FadeTransform_;//フェードイン/アウト用トランスフォーム
        int AlphaValue_;//画像の透明度
    };
    FadeInOut FadeEffect_;

    //スライドイン/アウト用
    struct SlideInOut
    {
        Transform SlideTransform_;//スライドイン/アウト用トランスフォーム
    };
    SlideInOut SlideEffect_;

    //ズームイン/アウト処理
    struct ZoomInOut
    {
        Transform ZoomTransform_;//ズームイン/アウト用トランスフォーム
        float MaxZoomValue_;//ズーム拡大最大値
    };
    ZoomInOut ZoomEffect_;


public:
    TransitionEffect(GameObject* parent);
    ~TransitionEffect();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    //画面遷移エフェクトの種類を指示
    //フェードイン時は初めに画像の透明度を最低値にする

    void FadeOutStartBlack() { EffectType_ = S_FadeOutBlack; }
    void FadeInStartBlack(){ EffectType_ = S_FadeInBlack; SetTransitionAlpha(); }
    void FadeOutStartWhite(){ EffectType_ = S_FadeOutWhite; }
    void FadeInStartWhite() { EffectType_ = S_FadeInWhite; SetTransitionAlpha(); }
    void SlideInLTRStart() { EffectType_ = S_SlideInLTR; }
    void ZoomInStart() { EffectType_ = S_ZoomIn; }
    void ZoomOutStart(){ EffectType_ = S_ZoomOut; SetTransitionZoom();}

    //----------EffectType_に応じて内容が変わるUpdate関数----------
    //フェードアウト処理
    void UpdateFadeOut();

    //フェードイン処理
    void UpdateFadeIn();

    //スライドイン処理
    void UpdateSlideInLTR();

    //ズームイン処理
    void UpdateZoomIn();

    //ズームアウト処理
    void UpdateZoomOut();

    //CSVファイルから必要パラメータを読み込みする
    void SetSCVTransitionEffect();


    /// <summary>
    /// 画面遷移時間を設定
    /// </summary>
    /// <param name="_time">設定する遷移までの時間</param>
    void SetTransitionTime(int _time) { TransitionTime_ = _time; }

    /// <summary>
    /// フェードイン/アウト用の画像の透明度を最低値に設定(フェードイン時に使用)
    /// </summary>
    void SetTransitionAlpha() { FadeEffect_.AlphaValue_ = Image::AlphaMin; }

    /// <summary>
    /// ズームイン/アウト用の画像の拡大量を最大に設定(ズームアウト時に使用)
    /// </summary>
    void SetTransitionZoom() {
        ZoomEffect_.ZoomTransform_.scale_.x = ZoomEffect_.MaxZoomValue_;
        ZoomEffect_.ZoomTransform_.scale_.y = ZoomEffect_.MaxZoomValue_;
    }

    /// <summary>
    /// フェードイン/アウト用の画像の透明度をリセット(透明度を最大にする)
    /// </summary>
    void ResetTransitionAlpha() { FadeEffect_.AlphaValue_ = 0; }
};


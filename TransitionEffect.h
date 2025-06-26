#pragma once
#include "Engine/GameObject.h"

//画面遷移時のエフェクト用クラス
class TransitionEffect :
    public GameObject
{
private:
    //----------画像ハンドル----------

    //フェードイン/アウト演出用画像
    int hFadeBlack_;
    int hFadeWhite_;

    //----------画面遷移エフェクトの種類----------
    enum EffectType {

        NoneEffect = 0,
        S_FadeOutBlack,//フェードアウト
        S_FadeInBlack,
        S_FadeOutWhite,//フェードイン
        S_FadeInWhite,
        S_SlideInLTR,//スライドイン
    };
    EffectType EffectType_;

    //遷移時間
    int TransitionTime_;

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

    void FadeOutStartBlack() { EffectType_ = S_FadeOutBlack; }
    void FadeInStartBlack(){ EffectType_ = S_FadeInBlack; }
    void FadeOutStartWhite(){ EffectType_ = S_FadeOutWhite; }
    void FadeInStartWhite() { EffectType_ = S_FadeInWhite; }
    void SlideInLTRStart() { EffectType_ = S_SlideInLTR; }

    //----------EffectType_に応じて内容が変わるUpdate関数----------
    //フェードアウト処理
    void UpdateFadeOut();

    //フェードイン処理
    void UpdateFadeIn();

    //スライドイン処理
    void UpdateSlideInLTR();

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
    void SetTransitionAlpha();

    /// <summary>
    /// フェードイン/アウト用の画像の透明度をリセット(透明度を最大にする)
    /// </summary>
    void ResetTransitionAlpha();
};


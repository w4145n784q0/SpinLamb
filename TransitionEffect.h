#pragma once
#include "Engine/GameObject.h"

//画面遷移時のエフェクト用クラス
class TransitionEffect :
    public GameObject
{
private:
    //フェードイン/アウト演出用画像
    int hFadeBlack_;
    int hFadeWhite_;

    //画面遷移エフェクトの種類
    enum EffectType {

        NoneEffect = 0,
        S_FadeOutBlack,
        S_FadeInBlack,
        S_FadeOutWhite,
        S_FadeInWhite,
        S_SlideInLTR,
    };
    EffectType EffectType_;

    //遷移時間
    int TransitionTime_;

public:
    TransitionEffect(GameObject* parent);
    ~TransitionEffect();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void FadeOutStartBlack() { EffectType_ = S_FadeOutBlack; }
    void FadeInStartBlack(){ EffectType_ = S_FadeInBlack; }
    void FadeOutStartWhite(){ EffectType_ = S_FadeOutWhite; }
    void FadeInStartWhite() { EffectType_ = S_FadeInWhite; }
    void SlideInLTRStart() { EffectType_ = S_SlideInLTR; }

    void UpdateFadeOut();
    void UpdateFadeIn();
    void UpdateSlideInLTR();

    //csv設定行う
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
    /// フェードイン/アウト用の画像の透明度をリセット
    /// </summary>
    void ResetTransitionAlpha();
};


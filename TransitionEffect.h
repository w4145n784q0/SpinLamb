#pragma once
#include "Engine/GameObject.h"

//画面遷移時のエフェクト用クラス
class TransitionEffect :
    public GameObject
{
private:
    //フェードイン/アウト演出用画像
    int hFade_;

    //画面遷移エフェクトの種類
    enum EffectType {

        NoneEffect = 0,
        S_FadeOut,
        S_FadeIn,
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

    void FadeOutStart() { EffectType_ = S_FadeOut; }
    void FadeInStart(){ EffectType_ = S_FadeIn; }
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
};


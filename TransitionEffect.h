#pragma once
#include "Engine/GameObject.h"

//��ʑJ�ڎ��̃G�t�F�N�g�p�N���X
class TransitionEffect :
    public GameObject
{
private:
    //�t�F�[�h�C��/�A�E�g���o�p�摜
    int hFade_;

    //��ʑJ�ڃG�t�F�N�g�̎��
    enum EffectType {

        NoneEffect = 0,
        S_FadeOut,
        S_FadeIn,
        S_SlideInLTR,
    };
    EffectType EffectType_;

    //�J�ڎ���
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

    //csv�ݒ�s��
    void SetSCVTransitionEffect();


    /// <summary>
    /// ��ʑJ�ڎ��Ԃ�ݒ�
    /// </summary>
    /// <param name="_time">�ݒ肷��J�ڂ܂ł̎���</param>
    void SetTransitionTime(int _time) { TransitionTime_ = _time; }

    /// <summary>
    /// �t�F�[�h�C��/�A�E�g�p�̉摜�̓����x���Œ�l�ɐݒ�(�t�F�[�h�C�����Ɏg�p)
    /// </summary>
    void SetTransitionAlpha();
};


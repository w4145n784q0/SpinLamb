#pragma once
#include "Engine/GameObject.h"

//��ʑJ�ڎ��̃G�t�F�N�g�p�N���X
class TransitionEffect :
    public GameObject
{
private:
    //----------�摜�n���h��----------

    //�t�F�[�h�C��/�A�E�g���o�p�摜
    int hFadeBlack_;
    int hFadeWhite_;

    //----------��ʑJ�ڃG�t�F�N�g�̎��----------
    enum EffectType {

        NoneEffect = 0,
        S_FadeOutBlack,//�t�F�[�h�A�E�g
        S_FadeInBlack,
        S_FadeOutWhite,//�t�F�[�h�C��
        S_FadeInWhite,
        S_SlideInLTR,//�X���C�h�C��
    };
    EffectType EffectType_;

    //�J�ڎ���
    int TransitionTime_;

public:
    TransitionEffect(GameObject* parent);
    ~TransitionEffect();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void FadeOutStartBlack() { EffectType_ = S_FadeOutBlack; }
    void FadeInStartBlack(){ EffectType_ = S_FadeInBlack; }
    void FadeOutStartWhite(){ EffectType_ = S_FadeOutWhite; }
    void FadeInStartWhite() { EffectType_ = S_FadeInWhite; }
    void SlideInLTRStart() { EffectType_ = S_SlideInLTR; }

    //----------EffectType_�ɉ����ē��e���ς��Update�֐�----------
    //�t�F�[�h�A�E�g����
    void UpdateFadeOut();

    //�t�F�[�h�C������
    void UpdateFadeIn();

    //�X���C�h�C������
    void UpdateSlideInLTR();

    //CSV�t�@�C������K�v�p�����[�^��ǂݍ��݂���
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

    /// <summary>
    /// �t�F�[�h�C��/�A�E�g�p�̉摜�̓����x�����Z�b�g(�����x���ő�ɂ���)
    /// </summary>
    void ResetTransitionAlpha();
};


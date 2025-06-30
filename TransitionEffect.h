#pragma once
#include "Engine/GameObject.h"
#include"Engine/Image.h"

//��ʑJ�ڎ��̃G�t�F�N�g�p�N���X
class TransitionEffect :
    public GameObject
{
private:
    //----------�摜�n���h��----------

    //�t�F�[�h�C��/�A�E�g���o�p�摜
    int hFadeBlack_;
    int hFadeWhite_;

    //�Y�[���C��/�A�E�g�p�摜
    int hZoomSheep_;

    //----------��ʑJ�ڃG�t�F�N�g�̎��----------
    enum EffectType {

        NoneEffect = 0,
        S_FadeOutBlack,//�����t�F�[�h�A�E�g
        S_FadeInBlack,//�����t�F�[�h�C��
        S_FadeOutWhite,//�t�F�[�h�C��
        S_FadeInWhite,//�����t�F�[�h�C��
        S_SlideInLTR,//�X���C�h�C��
        S_ZoomIn,//�Y�[���C��
        S_ZoomOut,//�Y�[���A�E�g
    };
    EffectType EffectType_;

    //�J�ڎ��� ���N���X�����������
    int TransitionTime_;

    //----------�e��ʑJ�ڂ̉��o�p�̍\����----------

    //�t�F�[�h�C��/�A�E�g�p
    struct FadeInOut
    {
        Transform FadeTransform_;//�t�F�[�h�C��/�A�E�g�p�g�����X�t�H�[��
        int AlphaValue_;//�摜�̓����x
    };
    FadeInOut FadeEffect_;

    //�X���C�h�C��/�A�E�g�p
    struct SlideInOut
    {
        Transform SlideTransform_;//�X���C�h�C��/�A�E�g�p�g�����X�t�H�[��
    };
    SlideInOut SlideEffect_;

    //�Y�[���C��/�A�E�g����
    struct ZoomInOut
    {
        Transform ZoomTransform_;//�Y�[���C��/�A�E�g�p�g�����X�t�H�[��
        float MaxZoomValue_;//�Y�[���g��ő�l
    };
    ZoomInOut ZoomEffect_;


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

    //��ʑJ�ڃG�t�F�N�g�̎�ނ��w��
    //�t�F�[�h�C�����͏��߂ɉ摜�̓����x���Œ�l�ɂ���

    void FadeOutStartBlack() { EffectType_ = S_FadeOutBlack; }
    void FadeInStartBlack(){ EffectType_ = S_FadeInBlack; SetTransitionAlpha(); }
    void FadeOutStartWhite(){ EffectType_ = S_FadeOutWhite; }
    void FadeInStartWhite() { EffectType_ = S_FadeInWhite; SetTransitionAlpha(); }
    void SlideInLTRStart() { EffectType_ = S_SlideInLTR; }
    void ZoomInStart() { EffectType_ = S_ZoomIn; }
    void ZoomOutStart(){ EffectType_ = S_ZoomOut; SetTransitionZoom();}

    //----------EffectType_�ɉ����ē��e���ς��Update�֐�----------
    //�t�F�[�h�A�E�g����
    void UpdateFadeOut();

    //�t�F�[�h�C������
    void UpdateFadeIn();

    //�X���C�h�C������
    void UpdateSlideInLTR();

    //�Y�[���C������
    void UpdateZoomIn();

    //�Y�[���A�E�g����
    void UpdateZoomOut();

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
    void SetTransitionAlpha() { FadeEffect_.AlphaValue_ = Image::AlphaMin; }

    /// <summary>
    /// �Y�[���C��/�A�E�g�p�̉摜�̊g��ʂ��ő�ɐݒ�(�Y�[���A�E�g���Ɏg�p)
    /// </summary>
    void SetTransitionZoom() {
        ZoomEffect_.ZoomTransform_.scale_.x = ZoomEffect_.MaxZoomValue_;
        ZoomEffect_.ZoomTransform_.scale_.y = ZoomEffect_.MaxZoomValue_;
    }

    /// <summary>
    /// �t�F�[�h�C��/�A�E�g�p�̉摜�̓����x�����Z�b�g(�����x���ő�ɂ���)
    /// </summary>
    void ResetTransitionAlpha() { FadeEffect_.AlphaValue_ = 0; }
};


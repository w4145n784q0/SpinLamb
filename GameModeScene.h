#pragma once
#include "Engine/GameObject.h"
#include<array>
#include<list>
class GameModeScene :
    public GameObject
{
private:
	//----------�w�i----------
	int hBackScreen_;//�w�i�F
	int hBackChara_;//�w�i�L����
	int hExplanation_;//�Q�[������
	Transform BackChara_;

	//----------�{�^��----------
	int hBattle_;//�o�g���X�^�[�g�{�^��
	int hPractice_;//���K���[�h�{�^��
	int hHowtoPlay_;//�����т��������{�^��
	int hBackTitle_;//�^�C�g���̃{�^��
	int hFrameLine_;//�I��g�̘g��
	std::array<Transform, 5> ModeSetTrans;//�e�{�^���̃g�����X�t�H�[���Ǘ��z��

	//----------���S�E�e�L�X�g----------
	int hModeSelect_;//���[�h�Z���N�g
	int hBattleText_;//��ʉ����̃e�L�X�g(�o�g���p)
	int hFreePlayText_;//��ʉ����̃e�L�X�g(�t���[�v���C�p)
	int hHowtoPlayText_;//��ʉ����̃e�L�X�g(�����т����p)
	int hTitleText_;//��ʉ����̃e�L�X�g(�^�C�g���p)
	std::array<int, 4> TextArray_;//�e�e�L�X�g�̃n���h���z��

	//----------�T�E���h�n���h��----------
	int hModeSound_;

	enum Mode
	{
		Battle = 0,
		Practice,
		HowToPlay,
		Title,
		Max
	};
	Mode SelectMode_;

	std::list<Mode> ModeList_;//�e���[�h�̃��X�g
	std::list<Mode>::iterator itr;//ModeList_�̃C���f�b�N�X���w���l
	
	Transform Trans_Select_;//�Q�[�����[�h���S�̃g�����X�t�H�[��
	Transform Trans_Text_;//�e�L�X�g�̌Œ�ʒu

	
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	GameModeScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void SetSCV();
};


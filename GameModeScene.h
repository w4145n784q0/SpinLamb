#pragma once
#include "Engine/GameObject.h"
#include<array>
#include<list>
class GameModeScene :
    public GameObject
{
private:

	enum Decide
	{
		Selected,
		Decided,
		MaxSelect
	};
	Decide ModeDecide_;

	enum Mode
	{
		Battle = 0,
		Practice,
		HowToPlay,
		Title,
		MaxCount //���[�h�̍ő�l
	};
	Mode SelectMode_;

	//----------�w�i----------
	int hBackScreen_;//�w�i�F
	int hBackChara_;//�w�i�L����
	Transform BackChara_;

	//----------�{�^��----------
	int hBattle_;//�o�g���X�^�[�g�{�^��
	int hPractice_;//���K���[�h�{�^��
	int hHowtoPlay_;//�����т��������{�^��
	int hBackTitle_;//�^�C�g���̃{�^��
	int hFrameLine_;//�I��g�̘g��
	std::array<Transform, MaxCount> ModeArray_;//�e�{�^���̃g�����X�t�H�[���Ǘ��z��
	std::array<int, MaxCount> ButtonArray_;//�e�{�^���̃n���h���z��
	

	//----------���S�E�e�L�X�g----------
	int hModeSelect_;//���[�h�Z���N�g
	int hBattleText_;//��ʉ����̃e�L�X�g(�o�g���p)
	int hFreePlayText_;//��ʉ����̃e�L�X�g(�t���[�v���C�p)
	int hHowtoPlayText_;//��ʉ����̃e�L�X�g(�����т����p)
	int hTitleText_;//��ʉ����̃e�L�X�g(�^�C�g���p)
	std::array<int, MaxCount> TextArray_;//�e�e�L�X�g�̃n���h���z��

	//----------�T�E���h�n���h��----------
	int hSoundGameMode_;//�Q�[�����[�h�V�[����BGM
	int hSoundSelect_;//���[�h�I������SE
	int hSoundDecide_;//���[�h���莞��SE



	std::list<Mode> ModeList_;//�e���[�h�̃��X�g
	std::list<Mode>::iterator itr;//ModeList_�̃C���f�b�N�X���w���l
	
	Transform Trans_Frame_;//�I��g�̃g�����X�t�H�[��
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

	//csv�t�@�C���̓ǂݍ���
	void SetSCV();

	void UpdateSelect();
	void UpdateDecide();
};


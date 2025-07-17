#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Engine/Direct3D.h"

#include"Engine/Camera.h"
//-----------------------------------------------------------
//��ʕ����E�`��
//-----------------------------------------------------------
namespace GameView
{
	//��ʕ`�惂�[�h
	enum GameViewMode
	{
		S_NormalScreen = 0,//�^�C�g��,�Q�[�����[�h�Ȃǂ̑S�̉��
		S_Single,//��l�v���C���̕`��
		S_Dual,//��l�v���C���̕`��
		MaxScreen
	};

	void Initialize();

	void ViewNormal();
	void ViewPvE();
	void ViewPlayer1();
	void ViewPlayer2();

	bool IsSingle();
	bool IsDual();

	//�`�惂�[�h��ݒ�
	void SetGameViewMode(GameViewMode mode);

	//�v���C���[1�̃C���X�^���X��ݒ�(Battle,Practice�ŌĂ�)
	void SetPlayer1(Player* _pPlayer);

	//�v���C���[2�̃C���X�^���X��ݒ�(Battle,Practice�ŌĂ�)
	void SetPlayer2(Player* _pPlayer);
};


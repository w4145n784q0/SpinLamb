#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"TransitionEffect.h"
#include<array>

//�Q�[���I����̃��U���g�V�[��
class ResultScene :
	public BaseScene
{
private:
	//----------�摜�n���h��----------

	//�^�C�g���摜
	int hBackScreen_;

	//"you win"�̃��S
	int hYouWin_;

	//"cpu win"�̃��S
	int hCpuWin_;

	//"player1 win"
	int hPlayer1Win_;

	//"player2 win"
	int hPlayer2Win_;

	//"Draw"�̃��S
	int hDraw_;

	//"push title"�̃��S
	int hPushTitle_;

	//----------�T�E���h�n���h��----------

	//���U���g��ʃT�E���h
	int hSoundResult_;

	//�^�C�g���ɖ߂�T�E���h
	int hSoundBackTitle_;

	//----------�g�����X�t�H�[��----------

	//���ʉ�ʂ̃��S�̈ʒu
	Transform Result_;

	//���ʉ�ʂ̉��̃��S�̈ʒu
	Transform PushTitle_;

	//���s���(��Ԃɉ����ď�����ω�������)
	enum WinnerState {
		YOU_WIN = 0,//Player�̏���
		CPU_WIN,//Enemy�̏���
		DRAW,//��������
		RESULTMAX
	};
	WinnerState winner_;

	//�e���ʃ��S�̉摜�n���h���z��
	std::array<int, RESULTMAX> ResultArray_;

	//----------�C���X�^���X----------
	TransitionEffect* pTransitionEffect_;
public:
	ResultScene(GameObject* parent);
	~ResultScene();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//CSV����K�v�p�����[�^��ǂݍ��݂���
	void SetCSVResult();

	//----------BaseScene�̌p���֐�----------
	//�ʏ�̏���
	void UpdateActive() override;

	//�V�[���J�ڒ��̏���
	void UpdateTransition() override;
};


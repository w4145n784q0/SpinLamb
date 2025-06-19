#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
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

	//"Draw"�̃��S
	int hDraw_;

	//"push title"�̃��S
	int hlogoTitle_;

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

	enum WinnerState {
		YOU_WIN = 0,
		CPU_WIN,
		DRAW,
		RESULTMAX
	};
	WinnerState winner_;

	//�e���ʃ��S�̉摜�n���h���z��
	std::array<int, RESULTMAX> ResultArray_;
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

	//csv�ǂݍ���
	void SetCSVResult();

	void UpdateActive() override;
	void UpdatePassive() override {};
	void UpdateTransition() override;
};


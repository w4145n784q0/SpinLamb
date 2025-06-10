#pragma once
#include "Engine/GameObject.h"
class GameTimer :
    public GameObject
{
private:

	//----------�摜�n���h��----------

	//���Ԃ̐����摜�n���h��
	int hNumber0_;
	int hNumber1_;
	int hNumber2_;
	int hNumber3_;
	int hNumber4_;
	int hNumber5_;
	int hNumber6_;
	int hNumber7_;
	int hNumber8_;
	int hNumber9_;

	//----------���ԕ`��----------

	//���ԋL�^
	int TimeNumber_;

	//���ԃJ�E���g(10�̈�)
	int Timeten_;

	//���ԃJ�E���g(1�̈�)
	int Timeone_;

	//���Ԃ̏��
	enum TimeState {
		STOP = 0,  //�ҋ@��
		COUNTING, //�J�E���g��
		MAXTIME 
	};
	TimeState TimeState_;


public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	GameTimer(GameObject* parent);

	~GameTimer();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void UpdateTimeStop();
	void UpdateTimeCount();
};


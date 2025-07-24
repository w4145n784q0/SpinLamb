#pragma once
#include "Engine/GameObject.h"

//���Ԃ̌v�Z���s���N���X
//�������Ԃ͕ʃN���X����󂯎��
class GameTimer :
    public GameObject
{
private:

	//----------���ԕ`��----------

	//���݂̎���
	int CurrentGameTime_;

	//10�̈ʂ̒l
	int Timeten_;

	//1�̈ʂ̒l
	int Timeone_;

	//���Ԃ̏��
	//�����̒l�ɉ����ĊeUpdate�֐����Ăяo��
	enum TimeState
	{
		STOP = 0,  //�ҋ@��
		COUNTING, //�J�E���g��
		MAXTIME 
	};
	TimeState TimeState_;

public:
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

	//----------TimeState_�ɉ����ē��e���ς��Update�֐�----------
	//���Ԍv���X�g�b�v
	void UpdateTimeStop();

	//���Ԍv����
	void UpdateTimeCount();

	//�Q�[�����Ԃ̌v�Z
	void TimeCalclation();

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------
	int GetTimeTen() const { return Timeten_; }
	int GetTimeOne() const { return Timeone_; }
	int GetCurrentGameTime() const { return CurrentGameTime_; }
	void SetCurrentGameTime(int _time) { CurrentGameTime_ = _time; }

	//���Ԓ�~���w��
	void StartTimer() { TimeState_ = COUNTING; }

	//���Ԍv�����w��
	void StopTimer() { TimeState_ = STOP; }
};

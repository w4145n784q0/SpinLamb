#pragma once
#include "Engine/GameObject.h"

//�X�R�A�̌v�Z����������N���X
class Score :
    public GameObject
{
private:
	//Player1�̃X�R�A
	int Score1_; 

	//Player2�iCPU�j�̃X�R�A
	int Score2_; 
public:
	Score(GameObject* parent);
	~Score();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void ScoreMinus();
};


#pragma once
#include "global.h"
#include "GameObject.h"

//�Q�[���ɓo�ꂷ��V�[��
enum SCENE_ID
{
	SCENE_ID_TITLE = 0,//�^�C�g��
	SCENE_ID_GAMEMODE,//�Q�[�����[�h�I��
	SCENE_ID_BATTLE,//�o�g��
	SCENE_ID_PRACTICE,//�t���[�v���C(���K)
	SCENE_ID_HOWTOPLAY,//�����т���
	SCENE_ID_RESULT,//���U���g
};

//-----------------------------------------------------------
//�V�[���؂�ւ���S������I�u�W�F�N�g
//-----------------------------------------------------------
class SceneManager : public GameObject
{
private:
	SCENE_ID currentSceneID_;	//���݂̃V�[��
	SCENE_ID nextSceneID_;		//���̃V�[��

	int FirstCharaScore_;		//�v���C���[1�̃X�R�A
	int SecondCharaScore_;		//�v���C���[2or�G�̃X�R�A

	enum PlayMode {
		PlayerVSEnemy,//��l�v���C�̎�(PvE)
		PlayerVSPlayer,//��l�v���C�̎�(PvP)
		MaxPlay
	};
	PlayMode PlayMode_;
public:

	//�R���X�g���N�^
	//�����Fparent	�e�I�u�W�F�N�g�i��{�I�ɃQ�[���}�l�[�W���[�j
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
	//�����Fnext	���̃V�[����ID
	void ChangeScene(SCENE_ID next);

	void SetFirstCharaScore(int score) { FirstCharaScore_ = score; }
	void SetSecondCharaScore(int score) { SecondCharaScore_ = score; }

	int GetFirstCharaScore() const { return FirstCharaScore_; }
	int GetSecondCharaScore() const { return SecondCharaScore_; }

	PlayMode GetPlayMode() const { return PlayMode_; }
	void SetPvEMode() { PlayMode_ = PlayerVSEnemy; }
	void SetPvPMode() { PlayMode_ = PlayerVSPlayer; }
	bool IsPlayerVSEnemy() const { return PlayMode_ == PlayerVSEnemy; }
	bool IsPlayerVSPlayer() const { return PlayMode_ == PlayerVSPlayer; }
};
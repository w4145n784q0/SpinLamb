#include "sceneManager.h"

#include "Model.h"
#include "Image.h"
#include "Audio.h"
#include"../GameView.h"

#include"../TitleScene.h"
#include"../ResultScene.h"
#include"../GameModeScene.h"
#include"../BattleScene.h"
#include"../PracticeScene.h"
#include"../HowToPlayScene.h"


//�R���X�g���N�^
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager"), currentSceneID_(SCENE_ID_TITLE), nextSceneID_(SCENE_ID_TITLE),
	FirstCharaScore_(0), SecondCharaScore_(0),PlayMode_(PlayerVSPlayer)
{
}

//������
void SceneManager::Initialize()
{
	//�ŏ��̃V�[��������
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

//�X�V
void SceneManager::Update()
{
	//���̃V�[�������݂̃V�[���ƈႤ�@���@�V�[����؂�ւ��Ȃ���΂Ȃ�Ȃ�
	if (currentSceneID_ != nextSceneID_)
	{
		//���̃V�[���̃I�u�W�F�N�g��S�폜
		KillAllChildren();

		//���[�h�����f�[�^��S�폜
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

		//GameView�̊e�|�C���^�̃A�h���X�𖳌������Ă��玟�̃V�[�����쐬
		//���̂��̂��̂�KillAllChildren()�ŏ����Ă���
		GameView::Release();

		//���̃V�[�����쐬
		switch (nextSceneID_)
		{
		case SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
		case SCENE_ID_GAMEMODE: Instantiate<GameModeScene>(this); break;
		case SCENE_ID_BATTLE: Instantiate<BattleScene>(this); break;
		case SCENE_ID_PRACTICE: Instantiate<PracticeScene>(this); break;
		case SCENE_ID_HOWTOPLAY: Instantiate<HowToPlayScene>(this); break;
		case SCENE_ID_RESULT: Instantiate<ResultScene>(this); break;
		}
		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	}
}

//�`��
void SceneManager::Draw()
{
}

//�J��
void SceneManager::Release()
{
}

//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}
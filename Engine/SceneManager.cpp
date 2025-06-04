#include "sceneManager.h"

#include "../TestScene.h"
#include "Model.h"
#include "Image.h"
#include "Audio.h"

#include"../TitleScene.h"
#include"../ResultScene.h"
#include"../GameModeScene.h"
#include"../BattleScene.h"
#include"../PracticeScene.h"
#include"../HowToPlayScene.h"


//�R���X�g���N�^
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager"), currentSceneID_(SCENE_ID_TITLE), nextSceneID_(SCENE_ID_TITLE),
	PlayerScore_(0), EnemyScore_(0)
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
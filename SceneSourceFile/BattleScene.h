#pragma once
#include "../Engine/GameObject.h"
#include"BaseScene.h"
#include"../CharacterSourceFile/Player.h"
#include"../CharacterSourceFile/Enemy.h"
#include"../UISourceFile/GameTimer.h"
#include"../UISourceFile/MiniMap.h"
#include"../UISourceFile/HUD.h"
#include"../EffectSourceFile/TransitionEffect.h"
#include"PlayScene.h"

//"�o�g��"����n�܂郂�[�h�V�[��
class BattleScene :
	public PlayScene, public IGameObserver
{
private:

	//----------�T�E���h�n���h��----------

	//�o�g�����T�E���h
	int hSoundBattle_;

	//�J�n�E�I���z�C�b�X���T�E���h
	int hSoundWhistle_;

	//----------�C���X�^���X----------
	Player* pPlayer1_;
	Player* pPlayer2_;
	Enemy* pEnemy_;
	HUD* pHUD_;
	TransitionEffect* pTransitionEffect_;

	//HUD�N���X�ɓn���|�C���^
	GameTimer* pGameTimer_;//BattleScene�����瑀�삷��ꍇ�̃^�C�}�[�N���X�|�C���^
	MiniMap* pMiniMap_;//HUD�N���X�ɓn���|�C���^

	//���ۂɑ��삷��v���C���[��o�^����z��
	std::vector<Player*> ActivePlayers_;

	//�������ꂽ�G(CPU)��o�^����z��
	std::vector<Enemy*> ActiveEnemys_;

	//----------��ԑJ��----------
	 
	//�o�g���V�[���̏�ԑJ��
	//�����̒l�ɉ����ĊeUpdate�֐����Ăяo��
	//���̂ق��`��w���ɂ��g�p����
	enum BattleState
	{
		S_Before = 0,//�J�n�O����
		S_Ready,     //�J�n���O
		S_Now,       //�o�g����
		S_Pause,     //�|�[�Y��
		S_After,     //�I����
		MaxBattleMode
	};
	BattleState BattleState_;

	//----------�X�R�A----------

	//�v���C���[1�̃X�R�A
	int FirstScore_;

	//�G(CPU)�E�v���C���[2�̃X�R�A
	int SecondScore_;

public:
	BattleScene(GameObject* parent);
	~BattleScene();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//----------BaseScene�̌p���֐�----------
	//�ʏ�̏���
	void UpdateActive() override;

	//�~�߂Ă���Ƃ��̏���
	void UpdateInActive() override;

	//�V�[���J�ڒ��̏���
	void UpdateTransition() override;

	//----------PlayScene�̌p���֐�----------

	//�|�[�Y��ʂɈړ�����
	void GotoPause() override;

	//�ʏ��ԂɈړ�����
	void GotoPlay() override;

	//�^�C�g���Ɉړ�����
	void GotoTitle() override;

	//�I���A�C�R���̈ʒu���Z�b�g
	void SetPauseIconY() override;

	//----------BattleState_�ɉ����ē��e���ς��Update�֐�----------
	//�o�g���J�n�O
	void UpdateBattleBefore();

	//�o�g���J�n���O
	void UpdateBattleReady();

	//�o�g����
	void UpdateBattle();

	//�o�g���I����
	void UpdateBattleAfter();


	//CSV����K�v�p�����[�^��ǂݍ��݂���
	void SetCSVBattle();

	//�v���C���[1�̃X�R�A�㏸
	void PlusFirstScore() { FirstScore_++; }

	//CPU�E�v���C���[2�̃X�R�A�㏸
	void PlusSecondScore() { SecondScore_++; }

	/// <summary>
	/// ��Ƀq�b�g�����ۂ̏���
	/// </summary>
	/// <param name="HitCharaID">�q�b�g�����L�����N�^�[��ID</param>
	void OnCharacterFenceHit(int _HitCharaID) override;

};


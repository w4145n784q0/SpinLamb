#pragma once
#include "../Engine/GameObject.h"
#include"BaseScene.h"
#include"../CharacterSourceFile/Player.h"
#include"../CharacterSourceFile/Enemy.h"
#include"../UISourceFile/GameTimer.h"
#include"../UISourceFile/MiniMap.h"
#include"../UISourceFile/HUD.h"
#include"../EffectSourceFile/TransitionEffect.h"
#include"PlayBaseScene.h"

//"�o�g��"����n�܂郂�[�h�V�[��
class BattleScene :
	public PlayBaseScene, public IGameObserver
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

	//-----HUD�N���X�ɓn���|�C���^-----

	//BattleScene�����瑀�삷��ꍇ�̃^�C�}�[�N���X�|�C���^
	GameTimer* pGameTimer_;	

	//HUD�N���X�ɓn���~�j�}�b�v�N���X�̃|�C���^
	MiniMap* pMiniMap_;		

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
		S_Before = 0,	//�J�n�O����
		S_Ready,		//�J�n���O
		S_Now,			//�o�g����
		S_Pause,		//�|�[�Y��
		S_After,		//�I����
		MaxBattleMode
	};
	BattleState BattleState_;

	//----------�X�R�A----------

	//�v���C���[1�̃X�R�A
	int FirstScore_;

	//�G(CPU)�E�v���C���[2�̃X�R�A
	int SecondScore_;

	//�X�R�A���Q�ƌ^�Ŋi�[�����|�C���^�z��
	//ActiveCharacters�Ɠ������ԂŒǉ�����
	std::vector<int*> ScoreArray_;

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

	//----------PlayBaseScene�̌p���֐�----------

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
	/// �X�R�A�̏㏸�E���~�v�Z
	/// </summary>
	/// <param name="_score">�ω�������X�R�A</param>
	/// <param name="_IsSelfDestruction">�����ɂ���̐ڐG���ǂ���,�w�肪�Ȃ����false</param>
	void ScoreChange(int& _score, bool _IsSelfDestruction = false);

	/// <summary>
	/// ��Ƀq�b�g�����ۂ̏���
	/// </summary>
	/// <param name="_AttackedName">��Ƀq�b�g�����L�����N�^�[�ɁA�Ō�ɐG�ꂽ�L�����N�^�[�̖��O</param>
	/// <param name="_HitName">��Ƀq�b�g�����L�����N�^�[�̖��O</param>
	void OnCharacterFenceHit(std::string _AttackedName, std::string _HitName) override;
};


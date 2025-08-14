#include "GameView.h"
#include"Engine/Direct3D.h"
#include"Engine/Camera.h"

namespace GameView
{
	//�Q���ł���v���C���[��
	enum pplayerIndex
	{
		i_player1 = 0,
		i_player2,
		MaxPlayersNum//�Q���ł���v���C���[�̍ő�l
	};

	//�`�惂�[�h�̏�Ԃ�ێ�
	GameView::GameViewMode GameViewMode_;

	//�v���C���[1,2�̃C���X�^���X
	Player* pPlayer1_;
	Player* pPlayer2_;

	//Enemy�̃C���X�^���X
	Enemy* pEnemy_;

	//HUD�̃C���X�^���X
	HUD* pHUD_;

	//�n�ʃN���X�̃C���X�^���X
	Ground* pGround_;

	//��N���X�̃C���X�^���X
	Fence* pFence_;

	//�X�e�[�W�O�I�u�W�F�N�g�N���X�̃C���X�^���X
	OutStageThing* pOutStageThing_;


	void GameView::Initialize()
	{
		GameViewMode_ = S_NormalScreen;
		pPlayer1_ = nullptr;
		pPlayer2_ = nullptr;
		pHUD_ = nullptr;
		pEnemy_ = nullptr;
		pGround_ = nullptr;
		pFence_ = nullptr;
		pOutStageThing_ = nullptr;
	}

	void GameView::ViewNormal()
	{
		//�S�̉�ʕ`��
		Direct3D::viewScreenNormal();

		//�J�����ʒu�E�œ_�͏����l�̂܂�
		Camera::SetPosition(Camera::GetInitPos());
		Camera::SetTarget(Camera::GetInitTar());

		//�J�����X�V
		Camera::Update();
	}

	void GameView::Release()
	{
		//���̂̉����SceneManager(�V�[���؂�ւ���)
		//�܂���main(�v���O�����I����)�ōs����̂�
		//���̂͏������A�A�h���X�l�𖳌�������̂�(�_���O�����O�|�C���^�h�~)

		if (pPlayer1_ != nullptr)
		{
			pPlayer1_ = nullptr;
		}
		if (pPlayer2_ != nullptr)
		{
			pPlayer2_ = nullptr;
		}
		if (pEnemy_ != nullptr)
		{
			pEnemy_ = nullptr;
		}
		if (pHUD_ != nullptr)
		{
			pHUD_ = nullptr;
		}
		if (pGround_ != nullptr)
		{
			pGround_ = nullptr;
		}
		if (pFence_ != nullptr)
		{
			pFence_ = nullptr;
		}
		if (pOutStageThing_ != nullptr)
		{
			pOutStageThing_ = nullptr;
		}
	}

	void GameView::ViewPvE()
	{
		if (pPlayer1_ == nullptr)
		{
			return;
		}

		//�S�̉�ʕ`��
		Direct3D::viewScreenNormal();

		//�J�����ʒu���v���C���[1�ɍ��킹��
		Camera::SetPosition(pPlayer1_->GetCameraPosition());

		//�J�����œ_���v���C���[1�ɍ��킹��
		Camera::SetTarget(pPlayer1_->GetCameraTarget());

		//�J�����X�V
		Camera::Update();
	}

	void GameView::ViewPlayer1()
	{
		if (pPlayer1_ == nullptr)
		{
			return;
		}

		//����ʕ`��
		Direct3D::viewScreenLeft();

		//�J�����ʒu���v���C���[1�ɍ��킹��
		Camera::SetPosition(pPlayer1_->GetCameraPosition());

		//�J�����œ_���v���C���[1�ɍ��킹��
		Camera::SetTarget(pPlayer1_->GetCameraTarget());

		//�J�����X�V
		Camera::Update();
	}

	void GameView::ViewPlayer2()
	{
		if (pPlayer2_ == nullptr)
		{
			return;
		}

		//�E��ʕ`��
		Direct3D::viewScreenRight();

		//�J�����ʒu���v���C���[2�ɍ��킹��
		Camera::SetPosition(pPlayer2_->GetCameraPosition());

		//�J�����œ_���v���C���[2�ɍ��킹��
		Camera::SetTarget(pPlayer2_->GetCameraTarget());

		//�J�����X�V
		Camera::Update();
	}

	void ViewImGui()
	{
		//�n���ꂽ�|�C���^����ImGui��`��
		//ImGui�̕`��͈�x�����ł����̂ŁA�eDraw�֐��ł͂Ȃ�
		//GameView�ŏ������Amain�ň�x�����Ă�
		//BattleScene,PracticeScene�݂̂��̏������s��
		//(����A����ȊO�̃V�[���ł͉�ʕ������Ȃ�����)

		if (pGround_ != nullptr)
		{
			pGround_->DrawImGui();
		}
		if (pFence_ != nullptr)
		{
			pFence_->DrawImGui();
		}
		if (pOutStageThing_ != nullptr)
		{
			pOutStageThing_->DrawImGui();
		}

		std::vector<Player*> players = { pPlayer1_,pPlayer2_ };
		for (auto player : players)
		{
			if (player != nullptr)
			{
				player->DrawImGui();
			}
		}

		if (pEnemy_ != nullptr)
		{
			pEnemy_->DrawImGui();
		}

		if (pHUD_ != nullptr)
		{
			pHUD_->DrawImGui();
		}
	}

	void ViewHUDNormal()
	{
		//�n���ꂽ�|�C���^����HUD��`��
		//�����`�悷��K�v������(��ʂɈ�����`�悷��)���̂�`��
		//�^�C�}�[��e�L�X�g�摜�Ȃ�

		//�S�̉�ʕ`��
		Direct3D::viewScreenNormal();

		if (pHUD_ == nullptr)
		{
			return;
		}

		//�J�����ʒu�E�œ_�͏����l�̂܂�
		Camera::SetPosition(Camera::GetInitPos());
		Camera::SetTarget(Camera::GetInitTar());
		Camera::Update();

		//���ʏ�ԂȂ��ʕ����p�̘g����`��
		//�ʏ��HUD�����O�ɕ`�悷��(���̃I�u�W�F�N�g�Ɣ��̂�h������)
		if (IsDual())
		{
			pHUD_->DrawSplitLine();
		}

		//HUD��`��
		pHUD_->DrawFullScreen();
	}

	//��l�v���C��Ԃ��ǂ���
	bool GameView::IsSingle()
	{
		if (GameViewMode_ == S_Single)
			return true;
		else
			return false;
	}

	//��l�v���C��Ԃ��ǂ���
	bool GameView::IsDual()
	{
		if (GameViewMode_ == S_Dual)
			return true;
		else
			return false;
	}

	//�`�惂�[�h�ݒ�
	void GameView::SetGameViewMode(GameViewMode mode)
	{
		GameViewMode_ = mode;
	}

	void SetPlayers(std::vector<Player*> _players)
	{
		//�v���C���[�z�񂪋󂩍ő�v���C���[���𒴂����珈�����Ȃ�
		if (_players.empty() || _players.size() > MaxPlayersNum)
		{
			return;
		}

		//�v���C���[���ɉ����Ď��̂�������
		//�z�񐔂�����Ȃ��Ȃ�nullptr����
		if (_players.size() > i_player1)
		{
			pPlayer1_ = _players[i_player1];
		}
		else
		{
			pPlayer1_ = nullptr;
		}

		if (_players.size() > i_player2)
		{
			pPlayer2_ = _players[i_player2];
		}
		else
		{
			pPlayer2_ = nullptr;
		}
	}

	//HUD�̃C���X�^���X��ݒ�
	void SetHUD(HUD* _HUD)
	{
		pHUD_ = _HUD;
	}

	//Enemy�̃C���X�^���X��ݒ�
	void SetEnemy(Enemy* _enemy)
	{
		pEnemy_ = _enemy;
	}

	//Ground�̃C���X�^���X��ݒ�
	void SetGround(Ground* _ground)
	{
		pGround_ = _ground;
	}

	//Fence�̃C���X�^���X��ݒ�
	void SetFence(Fence* _fence)
	{
		pFence_ = _fence;
	}

	//OutStageThing�̃C���X�^���X��ݒ�
	void SetOutStageThing(OutStageThing* _stagething)
	{
		pOutStageThing_ = _stagething;
	}
}
#include "GameView.h"
#include "Engine/Global.h"

namespace GameView
{
	enum pplayerIndex
	{
		i_player1 = 0,
		i_player2
	};

	//�`�惂�[�h�̃C���X�^���X
	GameView::GameViewMode GameViewMode_;

	//�v���C���[1,2�̃C���X�^���X
	Player* pPlayer1_;
	Player* pPlayer2_;

	//HUD�̃C���X�^���X
	HUD* pHUD_;

	void GameView::Initialize()
	{
		GameViewMode_ = S_NormalScreen;
		pPlayer1_ = nullptr;
		pPlayer2_ = nullptr;
		pHUD_ = nullptr;
	}

	void GameView::ViewNormal()
	{
		//�S�̉�ʕ`��
		Direct3D::viewScreenNormal();

		//�J�����ʒu�E�œ_�͏����l�̂܂�
		Camera::SetPosition(Camera::GetInitPos());
		Camera::SetTarget(Camera::GetInitTar());

		Camera::Update();
	}

	void GameView::Release()
	{
		pPlayer1_ = nullptr;
		pPlayer2_ = nullptr;
		pHUD_ = nullptr;
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

		Camera::Update();
	}

	void ViewHUDNormal()
	{
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

		//HUD��`��
		pHUD_->DrawFullScreen();
	}

	bool GameView::IsSingle()
	{
		if (GameViewMode_ == S_Single)
			return true;
		else
			return false;
	}

	bool GameView::IsDual()
	{
		if (GameViewMode_ == S_Dual)
			return true;
		else
			return false;
	}

	void GameView::SetGameViewMode(GameViewMode mode)
	{
		GameViewMode_ = mode;
	}

	void GameView::SetPlayer1(Player* _pPlayer)
	{
		pPlayer1_ = _pPlayer;
	}

	void GameView::SetPlayer2(Player* _pPlayer)
	{
		pPlayer2_ = _pPlayer;
	}

	void SetPlayers(std::vector<Player*> _players)
	{
		pPlayer1_ = _players[i_player1];
		pPlayer2_ = _players[i_player2];
	}

	void SetHUD(HUD* _HUD)
	{
		pHUD_ = _HUD;
	}
}
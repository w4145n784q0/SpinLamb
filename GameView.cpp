#include "GameView.h"

//�`�惂�[�h�̃C���X�^���X
GameView::GameViewMode GameViewMode_;

//�v���C���[1,2�̃C���X�^���X
Player* pPlayer1_;
Player* pPlayer2_;

void GameView::Initialize()
{
	GameViewMode_ = S_NormalScreen; 
	pPlayer1_ = nullptr;
	pPlayer2_ = nullptr;
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

void GameView::ViewPvE()
{
	if (pPlayer1_ == nullptr)
	{
		return;
	}

	//�S�̉�ʕ`��
	Direct3D::viewScreenNormal();

	//�J�����ʒu�̓v���C���[1�ɍ��킹��

	Camera::SetPosition(pPlayer1_->GetCameraPosition());

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
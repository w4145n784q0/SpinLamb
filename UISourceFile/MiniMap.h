#pragma once
#include "../Engine/GameObject.h"

//��ʉE���̃~�j�}�b�v��\������N���X
class MiniMap :
	public GameObject
{
private:
	//----------�}�b�v��̈ʒu----------

	//Player1�̈ʒu
	XMFLOAT3 FirstPos_;

	//Player2,Enemy�̈ʒu
	XMFLOAT3 SecondPos_;

	//Player1�̖{���̈ʒu �e�v���C�V�[������n�����
	XMFLOAT3 OriginalFirstPos_;

	//Player2,Enemy�̖{���̈ʒu �e�v���C�V�[������n�����
	XMFLOAT3 OriginalSecondPos_;

public:
	MiniMap(GameObject* parent);
	~MiniMap();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//CSV�t�@�C������K�v�p�����[�^��ǂݍ��݂���
	void SetMiniMapCSV();

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------

	void SetFirstPos(XMFLOAT3 _pos) { FirstPos_ = _pos; }
	void SetSecondPos(XMFLOAT3 _pos) { SecondPos_ = _pos; }
	XMFLOAT3 GetFirstPos() const { return FirstPos_; }
	XMFLOAT3 GetSecondPos() const { return SecondPos_; }

	void SetOriginalFirstPos(XMFLOAT3 _pos) { OriginalFirstPos_ = _pos; }
	void SetOriginalSecondPos(XMFLOAT3 _pos) { OriginalSecondPos_ = _pos; }
	XMFLOAT3 GetOriginalFirstPos() const { return OriginalFirstPos_; }
	XMFLOAT3 GetOriginalSecondPos() const { return OriginalSecondPos_; }

};

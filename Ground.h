#pragma once
#include "Engine/GameObject.h"

//�n�ʂ̕`��E�X�V���s���N���X
class Ground :
    public GameObject
{
private:
	//----------���f���n���h��----------

	//�n�ʃ��f��
	int hGrass_;
public:
	Ground(GameObject* parent);
	~Ground();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//ImGui�̕`��
	void DrawImGui();

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------

	//�X�e�[�W���f���̃��f���ԍ���Ԃ�(���C�L���X�g�p)
	int GetModelHandle() const { return hGrass_; }
};
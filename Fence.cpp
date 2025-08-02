#include "Fence.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"
#include"UpperWire.h"
#include"LowerWire.h"
#include"LeftWire.h"
#include"RightWire.h"


#include<array>
#include<vector>

namespace
{
	//���̐� setpillar�̏������ɗp����
	int PillarNum = 0;

	//�S���̃g�����X�t�H�[��
	Transform WireTransform;

	//���̃g�����X�t�H�[��(��]�E�g�嗦�̂� �ʒu�͕ʂɈ���)
	Transform PillarTransform;

	//���̈ʒu���i�[����Transform�z��
	std::vector<Transform> PillarsTransformArray = {};

	//���̈ʒu(XMFLOAT3)�̔z��
	std::vector<XMFLOAT3> PillarPosArray = {};

	//Imgui�\���p�̕�����z��
	std::string PillarNameArray[] = { "UpperPillar", "LowerPillar", "RightPillar", "LeftPillar" };
}

Fence::Fence(GameObject* parent)
	:GameObject(parent,"Fence"),hPillar_(-1), hFence_(-1),
	Pillar_UpperLeft_({0,0,0}),Pillar_UpperRight_({0,0,0}),Pillar_LowerLeft_({0,0,0}),Pillar_LowerRight_({0,0,0})
{
}

Fence::~Fence()
{
}

void Fence::Initialize()
{
	//�e���f���ǂݍ���
	hPillar_ = Model::Load("Model\\Pillar.fbx");
	assert(hPillar_ >= 0);

	hFence_ = Model::Load("Model\\Wire.fbx");
	assert(hFence_ >= 0);

	//�O�㍶�E�̓S���N���X����
	Instantiate<UpperWire>(this);
	Instantiate<LowerWire>(this);
	Instantiate<RightWire>(this);
	Instantiate<LeftWire>(this);

}

void Fence::Update()
{
}

void Fence::Draw()
{
	//�򃂃f���̕`��
	Model::SetAndDraw(hFence_, WireTransform);

	//�����f���̕`��
	for (int i = 0; i < PillarsTransformArray.size(); i++)
	{
		Model::SetAndDraw(hPillar_, PillarsTransformArray[i]);
	}
}

void Fence::Release()
{
}

void Fence::DrawImGui()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Fence"))
	{
		if (ImGui::TreeNode("wire"))
		{
			ImGui::InputFloat("FencePositionX", &WireTransform.position_.x, ZeroPointOne);
			ImGui::InputFloat("FencePositionY", &WireTransform.position_.y, ZeroPointOne);
			ImGui::InputFloat("FencePositionZ", &WireTransform.position_.z, ZeroPointOne);

			ImGui::InputFloat("FenceRotateX", &WireTransform.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("FenceRotateY", &WireTransform.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("FenceRotateZ", &WireTransform.rotate_.z, ZeroPointOne);

			ImGui::InputFloat("FenceScaleX", &WireTransform.scale_.x, ZeroPointOne);
			ImGui::InputFloat("FenceScaleY", &WireTransform.scale_.y, ZeroPointOne);
			ImGui::InputFloat("FenceScaleZ", &WireTransform.scale_.z, ZeroPointOne);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Pillar"))
		{
			for (int i = 0; i < PillarsTransformArray.size(); i++)
			{
				ImGui::InputFloat((PillarNameArray[i] + " PosisionX").c_str(), &PillarsTransformArray[i].position_.x, ZeroPointOne);
				ImGui::InputFloat((PillarNameArray[i] + " PositionY").c_str(), &PillarsTransformArray[i].position_.y, ZeroPointOne);
				ImGui::InputFloat((PillarNameArray[i] + " PositionZ").c_str(), &PillarsTransformArray[i].position_.z, ZeroPointOne);

				ImGui::InputFloat((PillarNameArray[i] + " RotateX").c_str(), &PillarsTransformArray[i].rotate_.x, ZeroPointOne);
				ImGui::InputFloat((PillarNameArray[i] + " RotateY").c_str(), &PillarsTransformArray[i].rotate_.y, ZeroPointOne);
				ImGui::InputFloat((PillarNameArray[i] + " RotateZ").c_str(), &PillarsTransformArray[i].rotate_.z, ZeroPointOne);

				ImGui::InputFloat((PillarNameArray[i] + " ScaleX").c_str(), &PillarsTransformArray[i].scale_.x, ZeroPointOne);
				ImGui::InputFloat((PillarNameArray[i] + " ScaleY").c_str(), &PillarsTransformArray[i].scale_.y, ZeroPointOne);
				ImGui::InputFloat((PillarNameArray[i] + " ScaleZ").c_str(), &PillarsTransformArray[i].scale_.z, ZeroPointOne);

			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif
}

void Fence::SetPillar(float upper, float lower, float left, float right, float height)
{
	//���̈ʒu(����,�E��,����,�E��)��ݒ�
	Pillar_UpperLeft_ = { left,height,upper };
	Pillar_UpperRight_ = { right,height,upper };
	Pillar_LowerLeft_ = { left, height,lower };
	Pillar_LowerRight_ = { right, height,lower };
	
	//���̎��_��PillarPosArray�̒l��������
	PillarPosArray = { Pillar_UpperLeft_, Pillar_UpperRight_,  Pillar_LowerLeft_, Pillar_LowerRight_ };

	//PillarsTransform�̃T�C�Y�𒌂̐����ɕύX
	PillarsTransformArray.resize(PillarNum);

	//���̈ʒu,��],�g�嗦��z��ɃZ�b�g
	//�ʒu��PillarPosArray�̒l��p����
	//��],�g�嗦��pillarTransform�œ���
	for (int i = 0; i < PillarNum; i++)
	{
		PillarsTransformArray[i].position_ = PillarPosArray[i];
		PillarsTransformArray[i].rotate_ = PillarTransform.rotate_;
		PillarsTransformArray[i].scale_ = PillarTransform.scale_;
	}
}

void Fence::SetPillarNum(int num)
{
	PillarNum = num;
}

void Fence::InitWireTransform(Transform _t)
{
	WireTransform = _t;
}

void Fence::InitPillarTransform(Transform _t)
{
	PillarTransform = _t;
}

void Fence::SetWireCollisionUpper(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	//�O���̍�̏�����
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");

	//�����蔻����Z�b�g
	pUpperWire->InitCollision(pos,size);

	//�@�����Z�b�g
	pUpperWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}

void Fence::SetWireCollisionLower(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	//����̍�̏�����
	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");

	//�����蔻����Z�b�g
	pLowerWire->InitCollision(pos, size);

	//�@�����Z�b�g
	pLowerWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}

void Fence::SetWireCollisionRight(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	//�E���̍�̏�����
	RightWire* pRightWire = (RightWire*)FindObject("RightWire");

	//�����蔻����Z�b�g
	pRightWire->InitCollision(pos, size);

	//�@�����Z�b�g
	pRightWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}

void Fence::SetWireCollisionLeft(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	//�����̍�̏�����
	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");

	//�����蔻����Z�b�g
	pLeftWire->InitCollision(pos, size);

	//�@�����Z�b�g
	pLeftWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}
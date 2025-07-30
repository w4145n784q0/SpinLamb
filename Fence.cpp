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
	//���̐� setpiller�̏������ɗp����
	int pillerNum = 0;

	//�S���̃g�����X�t�H�[��
	Transform wireTransform;

	//���̃g�����X�t�H�[��(��]�E�g�嗦�̂� �ʒu�͕ʂɈ���)
	Transform pillerTransform;

	//���̈ʒu���i�[����Transform�z��
	std::vector<Transform> pillersTransformArray = {};

	//���̈ʒu(XMFLOAT3)�̔z��
	std::vector<XMFLOAT3> PillerPosArray = {};

	//Imgui�\���p�̕�����z��
	std::string PillerNameArray[] = { "UpperPiller", "LowerPiller", "RightPiller", "LeftPiller" };
}

Fence::Fence(GameObject* parent)
	:GameObject(parent,"Fence"),hPiller_(-1), hFence_(-1),
	piller_UpperLeft_({0,0,0}),piller_UpperRight_({0,0,0}),piller_LowerLeft_({0,0,0}),piller_LowerRight_({0,0,0})
{
}

Fence::~Fence()
{
}

void Fence::Initialize()
{
	//�e���f���ǂݍ���
	hPiller_ = Model::Load("Model\\Piller.fbx");
	assert(hPiller_ >= 0);

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
	Model::SetAndDraw(hFence_, wireTransform);

	//�����f���̕`��
	for (int i = 0; i < pillersTransformArray.size(); i++)
	{
		Model::SetAndDraw(hPiller_, pillersTransformArray[i]);
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
			ImGui::InputFloat("FencePositionX", &wireTransform.position_.x, ZeroPointOne);
			ImGui::InputFloat("FencePositionY", &wireTransform.position_.y, ZeroPointOne);
			ImGui::InputFloat("FencePositionZ", &wireTransform.position_.z, ZeroPointOne);

			ImGui::InputFloat("FenceRotateX", &wireTransform.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("FenceRotateY", &wireTransform.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("FenceRotateZ", &wireTransform.rotate_.z, ZeroPointOne);

			ImGui::InputFloat("FenceScaleX", &wireTransform.scale_.x, ZeroPointOne);
			ImGui::InputFloat("FenceScaleY", &wireTransform.scale_.y, ZeroPointOne);
			ImGui::InputFloat("FenceScaleZ", &wireTransform.scale_.z, ZeroPointOne);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Piller"))
		{
			for (int i = 0; i < pillersTransformArray.size(); i++)
			{
				ImGui::InputFloat((PillerNameArray[i] + " PotisionX").c_str(), &pillersTransformArray[i].position_.x, ZeroPointOne);
				ImGui::InputFloat((PillerNameArray[i] + " PotisionY").c_str(), &pillersTransformArray[i].position_.y, ZeroPointOne);
				ImGui::InputFloat((PillerNameArray[i] + " PotisionZ").c_str(), &pillersTransformArray[i].position_.z, ZeroPointOne);

				ImGui::InputFloat((PillerNameArray[i] + " RotateX").c_str(), &pillersTransformArray[i].rotate_.x, ZeroPointOne);
				ImGui::InputFloat((PillerNameArray[i] + " RotateY").c_str(), &pillersTransformArray[i].rotate_.y, ZeroPointOne);
				ImGui::InputFloat((PillerNameArray[i] + " RotateZ").c_str(), &pillersTransformArray[i].rotate_.z, ZeroPointOne);

				ImGui::InputFloat((PillerNameArray[i] + " ScaleX").c_str(), &pillersTransformArray[i].scale_.x, ZeroPointOne);
				ImGui::InputFloat((PillerNameArray[i] + " ScaleY").c_str(), &pillersTransformArray[i].scale_.y, ZeroPointOne);
				ImGui::InputFloat((PillerNameArray[i] + " ScaleZ").c_str(), &pillersTransformArray[i].scale_.z, ZeroPointOne);

			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif
}

void Fence::SetPiller(float upper, float lower, float left, float right, float height)
{
	//���̈ʒu(����,�E��,����,�E��)��ݒ�
	piller_UpperLeft_ = { left,height,upper };
	piller_UpperRight_ = { right,height,upper };
	piller_LowerLeft_ = { left, height,lower };
	piller_LowerRight_ = { right, height,lower };
	
	//���̎��_��PillerPosArray�̒l��������
	PillerPosArray = { piller_UpperLeft_ ,piller_UpperRight_ , piller_LowerLeft_,piller_LowerRight_ };

	//pillersTransform�̃T�C�Y�𒌂̐����ɕύX
	pillersTransformArray.resize(pillerNum);

	//���̈ʒu,��],�g�嗦��z��ɃZ�b�g
	//�ʒu��PillerPosArray�̒l��p����
	//��],�g�嗦��pillerTransform�œ���
	for (int i = 0; i < pillerNum; i++)
	{
		pillersTransformArray[i].position_ = PillerPosArray[i];
		pillersTransformArray[i].rotate_ = pillerTransform.rotate_;
		pillersTransformArray[i].scale_ = pillerTransform.scale_;
	}
}

void Fence::SetPillerNum(int num)
{
	pillerNum = num;
}

void Fence::InitWireTransform(Transform _t)
{
	wireTransform = _t;
}

void Fence::InitPillerTransform(Transform _t)
{
	pillerTransform = _t;
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
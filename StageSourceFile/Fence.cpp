#include "Fence.h"
#include"../Engine/Model.h"
#include"UpperWire.h"
#include"LowerWire.h"
#include"LeftWire.h"
#include"RightWire.h"


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
	PillarUpperLeft_({0,0,0}),PillarUpperRight_({0,0,0}),PillarLowerLeft_({0,0,0}),PillarLowerRight_({0,0,0})
{
}

Fence::~Fence()
{
}

void Fence::Initialize()
{
	//�e���f���ǂݍ���

	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string Model = "Model\\";

	hPillar_ = Model::Load(Model + "Pillar.fbx");
	assert(hPillar_ >= 0);

	hFence_ = Model::Load(Model + "Wire.fbx");
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

	//�S�����f��
	if (ImGui::TreeNode("Fence"))
	{
		if (ImGui::TreeNode("wire"))
		{
			if(ImGui::TreeNode("WirePosition"))
			{
				ImGui::InputFloat("FencePositionX", &WireTransform.position_.x, ZeroPointOne);
				ImGui::InputFloat("FencePositionY", &WireTransform.position_.y, ZeroPointOne);
				ImGui::InputFloat("FencePositionZ", &WireTransform.position_.z, ZeroPointOne);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("WireRotate"))
			{
				ImGui::InputFloat("FenceRotateX", &WireTransform.rotate_.x, ZeroPointOne);
				ImGui::InputFloat("FenceRotateY", &WireTransform.rotate_.y, ZeroPointOne);
				ImGui::InputFloat("FenceRotateZ", &WireTransform.rotate_.z, ZeroPointOne);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("WireScale"))
			{
				ImGui::InputFloat("FenceScaleX", &WireTransform.scale_.x, ZeroPointOne);
				ImGui::InputFloat("FenceScaleY", &WireTransform.scale_.y, ZeroPointOne);
				ImGui::InputFloat("FenceScaleZ", &WireTransform.scale_.z, ZeroPointOne);
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		//�����f��
		if (ImGui::TreeNode("Pillar"))
		{
			if (ImGui::TreeNode("PillerPosition"))
			{
				for (int i = 0; i < PillarsTransformArray.size(); i++)
				{
					ImGui::InputFloat((PillarNameArray[i] + " PosisionX").c_str(), &PillarsTransformArray[i].position_.x, ZeroPointOne);
					ImGui::InputFloat((PillarNameArray[i] + " PositionY").c_str(), &PillarsTransformArray[i].position_.y, ZeroPointOne);
					ImGui::InputFloat((PillarNameArray[i] + " PositionZ").c_str(), &PillarsTransformArray[i].position_.z, ZeroPointOne);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PillerRotate"))
			{
				for (int i = 0; i < PillarsTransformArray.size(); i++)
				{
					ImGui::InputFloat((PillarNameArray[i] + " RotateX").c_str(), &PillarsTransformArray[i].rotate_.x, ZeroPointOne);
					ImGui::InputFloat((PillarNameArray[i] + " RotateY").c_str(), &PillarsTransformArray[i].rotate_.y, ZeroPointOne);
					ImGui::InputFloat((PillarNameArray[i] + " RotateZ").c_str(), &PillarsTransformArray[i].rotate_.z, ZeroPointOne);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PillerScale"))
			{
				for (int i = 0; i < PillarsTransformArray.size(); i++)
				{
					ImGui::InputFloat((PillarNameArray[i] + " ScaleX").c_str(), &PillarsTransformArray[i].scale_.x, ZeroPointOne);
					ImGui::InputFloat((PillarNameArray[i] + " ScaleY").c_str(), &PillarsTransformArray[i].scale_.y, ZeroPointOne);
					ImGui::InputFloat((PillarNameArray[i] + " ScaleZ").c_str(), &PillarsTransformArray[i].scale_.z, ZeroPointOne);
				}
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif
}

void Fence::SetPillar(float _upper, float _lower, float _left, float _right, float _height)
{
	//���̈ʒu(����,�E��,����,�E��)��ݒ�
	PillarUpperLeft_ = { _left,_height,_upper };
	PillarUpperRight_ = { _right,_height,_upper };
	PillarLowerLeft_ = { _left, _height,_lower };
	PillarLowerRight_ = { _right, _height,_lower };
	
	//���̎��_��PillarPosArray�̒l��������
	PillarPosArray = { PillarUpperLeft_, PillarUpperRight_,  PillarLowerLeft_, PillarLowerRight_ };

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

void Fence::SetPillarNum(int _num)
{
	PillarNum = _num;
}

void Fence::InitWireTransform(Transform _t)
{
	WireTransform = _t;
}

void Fence::InitPillarTransform(Transform _t)
{
	PillarTransform = _t;
}

void Fence::SetWireCollisionUpper(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal)
{
	//�O���̍�̏�����
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	assert(pUpperWire != nullptr);

	//�����蔻����Z�b�g
	pUpperWire->InitCollision(_pos, _size);

	//�@�����Z�b�g
	pUpperWire->SetNormal(XMVECTOR({ _normal.x,_normal.y,_normal.z }));
}

void Fence::SetWireCollisionLower(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal)
{
	//����̍�̏�����
	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	assert(pLowerWire != nullptr);

	//�����蔻����Z�b�g
	pLowerWire->InitCollision(_pos, _size);

	//�@�����Z�b�g
	pLowerWire->SetNormal(XMVECTOR({ _normal.x,_normal.y,_normal.z }));
}

void Fence::SetWireCollisionRight(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal)
{
	//�E���̍�̏�����
	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	assert(pRightWire != nullptr);

	//�����蔻����Z�b�g
	pRightWire->InitCollision(_pos, _size);

	//�@�����Z�b�g
	pRightWire->SetNormal(XMVECTOR({ _normal.x,_normal.y,_normal.z }));
}

void Fence::SetWireCollisionLeft(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal)
{
	//�����̍�̏�����
	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	assert(pLeftWire != nullptr);

	//�����蔻����Z�b�g
	pLeftWire->InitCollision(_pos, _size);

	//�@�����Z�b�g
	pLeftWire->SetNormal(XMVECTOR({ _normal.x,_normal.y,_normal.z }));
}
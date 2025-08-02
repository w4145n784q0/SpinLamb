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
	//柱の数 setpillarの初期化に用いる
	int PillarNum = 0;

	//鉄線のトランスフォーム
	Transform WireTransform;

	//柱のトランスフォーム(回転・拡大率のみ 位置は別に扱う)
	Transform PillarTransform;

	//柱の位置を格納するTransform配列
	std::vector<Transform> PillarsTransformArray = {};

	//柱の位置(XMFLOAT3)の配列
	std::vector<XMFLOAT3> PillarPosArray = {};

	//Imgui表示用の文字列配列
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
	//各モデル読み込み
	hPillar_ = Model::Load("Model\\Pillar.fbx");
	assert(hPillar_ >= 0);

	hFence_ = Model::Load("Model\\Wire.fbx");
	assert(hFence_ >= 0);

	//前後左右の鉄線クラス生成
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
	//柵モデルの描画
	Model::SetAndDraw(hFence_, WireTransform);

	//柱モデルの描画
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
	//柱の位置(左上,右上,左下,右下)を設定
	Pillar_UpperLeft_ = { left,height,upper };
	Pillar_UpperRight_ = { right,height,upper };
	Pillar_LowerLeft_ = { left, height,lower };
	Pillar_LowerRight_ = { right, height,lower };
	
	//この時点でPillarPosArrayの値を初期化
	PillarPosArray = { Pillar_UpperLeft_, Pillar_UpperRight_,  Pillar_LowerLeft_, Pillar_LowerRight_ };

	//PillarsTransformのサイズを柱の数分に変更
	PillarsTransformArray.resize(PillarNum);

	//柱の位置,回転,拡大率を配列にセット
	//位置はPillarPosArrayの値を用いる
	//回転,拡大率はpillarTransformで統一
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
	//前方の柵の初期化
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");

	//当たり判定をセット
	pUpperWire->InitCollision(pos,size);

	//法線をセット
	pUpperWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}

void Fence::SetWireCollisionLower(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	//後方の柵の初期化
	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");

	//当たり判定をセット
	pLowerWire->InitCollision(pos, size);

	//法線をセット
	pLowerWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}

void Fence::SetWireCollisionRight(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	//右側の柵の初期化
	RightWire* pRightWire = (RightWire*)FindObject("RightWire");

	//当たり判定をセット
	pRightWire->InitCollision(pos, size);

	//法線をセット
	pRightWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}

void Fence::SetWireCollisionLeft(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	//左側の柵の初期化
	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");

	//当たり判定をセット
	pLeftWire->InitCollision(pos, size);

	//法線をセット
	pLeftWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}
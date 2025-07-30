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
	//柱の数 setpillerの初期化に用いる
	int pillerNum = 0;

	//鉄線のトランスフォーム
	Transform wireTransform;

	//柱のトランスフォーム(回転・拡大率のみ 位置は別に扱う)
	Transform pillerTransform;

	//柱の位置を格納するTransform配列
	std::vector<Transform> pillersTransformArray = {};

	//柱の位置(XMFLOAT3)の配列
	std::vector<XMFLOAT3> PillerPosArray = {};

	//Imgui表示用の文字列配列
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
	//各モデル読み込み
	hPiller_ = Model::Load("Model\\Piller.fbx");
	assert(hPiller_ >= 0);

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
	Model::SetAndDraw(hFence_, wireTransform);

	//柱モデルの描画
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
	//柱の位置(左上,右上,左下,右下)を設定
	piller_UpperLeft_ = { left,height,upper };
	piller_UpperRight_ = { right,height,upper };
	piller_LowerLeft_ = { left, height,lower };
	piller_LowerRight_ = { right, height,lower };
	
	//この時点でPillerPosArrayの値を初期化
	PillerPosArray = { piller_UpperLeft_ ,piller_UpperRight_ , piller_LowerLeft_,piller_LowerRight_ };

	//pillersTransformのサイズを柱の数分に変更
	pillersTransformArray.resize(pillerNum);

	//柱の位置,回転,拡大率を配列にセット
	//位置はPillerPosArrayの値を用いる
	//回転,拡大率はpillerTransformで統一
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
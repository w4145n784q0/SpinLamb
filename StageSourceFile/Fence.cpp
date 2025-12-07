#include "Fence.h"
#include"../Engine/Model.h"
#include"UpperWire.h"
#include"LowerWire.h"
#include"LeftWire.h"
#include"RightWire.h"
#include"../Engine/Global.h"

#include<vector>

namespace
{
	//鉄線のトランスフォーム
	Transform WireTransform;

	//柱のトランスフォーム(回転・拡大率のみ 位置は別に扱う)
	Transform PillarTransform;

	//柱の位置を格納するTransform配列
	std::vector<Transform> PillarsTransformArray = {};

	//柱の位置(XMFLOAT3)の配列
	std::vector<XMFLOAT3> PillarPosArray = {};

	//Imgui表示用の文字列配列
	std::string PillarNameArray[] = { "UpperRightPillar", "UpperLeftPillar", "LowerRightPillar", "LowerLeftPillar" };
}

Fence::Fence(GameObject* parent)
	:GameObject(parent,"Fence"),hPillar_(-1), hFence_(-1),
	PillarUpperLeft_({0,0,0}),PillarUpperRight_({0,0,0}),
	PillarLowerLeft_({0,0,0}),PillarLowerRight_({0,0,0}),
	PillarNum_(0)
{
}

Fence::~Fence()
{
}

void Fence::Initialize()
{
	//各モデル読み込み

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string Model = "Model\\";

	hPillar_ = Model::Load(Model + "Pillar.fbx");
	assert(hPillar_ >= 0);

	hFence_ = Model::Load(Model + "Wire.fbx");
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

	//鉄線モデル
	if (ImGui::TreeNode("Fence"))
	{
		if (ImGui::TreeNode("wire"))
		{
			if(ImGui::TreeNode("WirePosition"))
			{
				ImGui::InputFloat("FencePositionX", &WireTransform.position_.x, ZERO_POINT_ONE);
				ImGui::InputFloat("FencePositionY", &WireTransform.position_.y, ZERO_POINT_ONE);
				ImGui::InputFloat("FencePositionZ", &WireTransform.position_.z, ZERO_POINT_ONE);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("WireRotate"))
			{
				ImGui::SliderFloat("FenceRotateX", &WireTransform.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("FenceRotateY", &WireTransform.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("FenceRotateZ", &WireTransform.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("WireScale"))
			{
				ImGui::InputFloat("FenceScaleX", &WireTransform.scale_.x, ZERO_POINT_ONE);
				ImGui::InputFloat("FenceScaleY", &WireTransform.scale_.y, ZERO_POINT_ONE);
				ImGui::InputFloat("FenceScaleZ", &WireTransform.scale_.z, ZERO_POINT_ONE);
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		//柱モデル
		if (ImGui::TreeNode("Pillar"))
		{
			if (ImGui::TreeNode("PillarPosition"))
			{
				for (int i = 0; i < PillarsTransformArray.size(); i++)
				{
					ImGui::InputFloat((PillarNameArray[i] + " PositionX").c_str(), &PillarsTransformArray[i].position_.x, ZERO_POINT_ONE);
					ImGui::InputFloat((PillarNameArray[i] + " PositionY").c_str(), &PillarsTransformArray[i].position_.y, ZERO_POINT_ONE);
					ImGui::InputFloat((PillarNameArray[i] + " PositionZ").c_str(), &PillarsTransformArray[i].position_.z, ZERO_POINT_ONE);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PillarRotate"))
			{
				for (int i = 0; i < PillarsTransformArray.size(); i++)
				{
					ImGui::SliderFloat((PillarNameArray[i] + " RotateX").c_str(), &PillarsTransformArray[i].rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
					ImGui::SliderFloat((PillarNameArray[i] + " RotateY").c_str(), &PillarsTransformArray[i].rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
					ImGui::SliderFloat((PillarNameArray[i] + " RotateZ").c_str(), &PillarsTransformArray[i].rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PillarScale"))
			{
				for (int i = 0; i < PillarsTransformArray.size(); i++)
				{
					ImGui::InputFloat((PillarNameArray[i] + " ScaleX").c_str(), &PillarsTransformArray[i].scale_.x, ZERO_POINT_ONE);
					ImGui::InputFloat((PillarNameArray[i] + " ScaleY").c_str(), &PillarsTransformArray[i].scale_.y, ZERO_POINT_ONE);
					ImGui::InputFloat((PillarNameArray[i] + " ScaleZ").c_str(), &PillarsTransformArray[i].scale_.z, ZERO_POINT_ONE);
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
	//柱の位置(左上,右上,左下,右下)を設定
	PillarUpperLeft_ = { _left,_height,_upper };
	PillarUpperRight_ = { _right,_height,_upper };
	PillarLowerLeft_ = { _left, _height,_lower };
	PillarLowerRight_ = { _right, _height,_lower };
	
	//この時点でPillarPosArrayの値を初期化
	PillarPosArray = { PillarUpperLeft_, PillarUpperRight_,  PillarLowerLeft_, PillarLowerRight_ };

	//PillarsTransformのサイズを柱の数分に変更
	PillarsTransformArray.resize(PillarNum_);

	//柱の位置,回転,拡大率を配列にセット
	//位置はPillarPosArrayの値を用いる
	//回転,拡大率はpillarTransformで統一
	for (int i = 0; i < PillarNum_; i++)
	{
		PillarsTransformArray[i].position_ = PillarPosArray[i];
		PillarsTransformArray[i].rotate_ = PillarTransform.rotate_;
		PillarsTransformArray[i].scale_ = PillarTransform.scale_;
	}
}

void Fence::SetPillarNum(int _num)
{
	PillarNum_ = _num;
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
	//前方の柵の初期化
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	assert(pUpperWire != nullptr);

	//当たり判定をセット
	pUpperWire->InitCollision(_pos, _size);

	//法線をセット
	pUpperWire->SetNormal(XMVECTOR({ _normal.x,_normal.y,_normal.z }));
}

void Fence::SetWireCollisionLower(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal)
{
	//後方の柵の初期化
	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	assert(pLowerWire != nullptr);

	//当たり判定をセット
	pLowerWire->InitCollision(_pos, _size);

	//法線をセット
	pLowerWire->SetNormal(XMVECTOR({ _normal.x,_normal.y,_normal.z }));
}

void Fence::SetWireCollisionRight(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal)
{
	//右側の柵の初期化
	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	assert(pRightWire != nullptr);

	//当たり判定をセット
	pRightWire->InitCollision(_pos, _size);

	//法線をセット
	pRightWire->SetNormal(XMVECTOR({ _normal.x,_normal.y,_normal.z }));
}

void Fence::SetWireCollisionLeft(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal)
{
	//左側の柵の初期化
	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	assert(pLeftWire != nullptr);

	//当たり判定をセット
	pLeftWire->InitCollision(_pos, _size);

	//法線をセット
	pLeftWire->SetNormal(XMVECTOR({ _normal.x,_normal.y,_normal.z }));
}
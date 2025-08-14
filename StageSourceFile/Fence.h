#pragma once
#include "../Engine/GameObject.h"

//柵の鉄線・柱の位置やサイズを扱うクラス
class Fence :
    public GameObject
{
private:
	//----------モデルハンドル----------

	//柱モデル
	int hPillar_;

	//柵モデル
	int hFence_;

	//----------柵の位置----------

	//左上
	XMFLOAT3 Pillar_UpperLeft_;

	//右上
	XMFLOAT3 Pillar_UpperRight_;

	//左下
	XMFLOAT3 Pillar_LowerLeft_;

	//右下
	XMFLOAT3 Pillar_LowerRight_;
	

public:
	Fence(GameObject* parent);
	~Fence();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//ImGuiの描画
	void DrawImGui();

	/// <summary>
	/// 4つの柵の位置決定
	/// </summary>
	/// <param name="upper">北端</param>
	/// <param name="lower">南端</param>
	/// <param name="left">西端</param>
	/// <param name="right">東端</param>
	/// <param name="height">柱の高さ</param>
	void SetPillar(float upper, float lower, float left, float right, float height);

	/// <summary>
	///	柱の数を設定
	/// </summary>
	/// <param name="num">柱の数</param>
	void SetPillarNum(int num);

	/// <summary>
	/// 鉄線のトランスフォームを初期化
	/// </summary>
	/// <param name="_t">StageManagerで初期化したトランスフォーム</param>
	void InitWireTransform(Transform _t); 
	
	/// <summary>
	/// 柱のトランスフォームを初期化
	/// </summary>
	/// <param name="_t">StageManagerで初期化したトランスフォーム</param>
	void InitPillarTransform(Transform _t);
	
	/// <summary>
	/// 各鉄線の衝突判定を設定
	/// </summary>
	/// <param name="pos">箱形の当たり判定の位置</param>
	/// <param name="size">箱形の当たり判定の大きさ</param>
	/// <param name="normal">各鉄線クラスに渡す法線</param>
	
	void SetWireCollisionUpper(XMFLOAT3 pos, XMFLOAT3 size,XMFLOAT3 normal);

	void SetWireCollisionLower(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal);

	void SetWireCollisionLeft(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal);

	void SetWireCollisionRight(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal);
};

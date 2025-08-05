#pragma once
#include "Engine/GameObject.h"

//ステージ全体管理クラス
class StageManager :
    public GameObject
{
private:

	//----------初期化して渡すトランスフォーム----------
	//地面クラスに渡すデータ
	Transform GroundData_;

	//柵クラス(鉄線)に渡すデータ
	Transform WireData_;

	//柵クラス(柱)に渡すデータ
	Transform PillarData_;

	//ステージ外オブジェクトに渡すデータ(小屋)
	std::vector<Transform>  CabinData_;

	//ステージ外オブジェクトに渡すデータ(木)
	std::vector<Transform>  TreeData_;

	//ステージ外オブジェクトに渡すデータ(丸太)
	std::vector<Transform>  LogsData_;

	//ステージ外オブジェクトに渡すデータ(藁束)
	std::vector<Transform> StrawData_;


	//ステージ北端
	float UpperEnd_;

	//ステージ南端
	float LowerEnd_; 

	//ステージ東端
	float RightEnd_;

	//ステージ西端
	float LeftEnd_;

	//柱の数
	int PillarNum_;

	//----------当たり判定関係----------

	//----------当たり判定の位置----------
	//鉄線の当たり判定の位置（+z方向）
	XMFLOAT3 WirePosUpper_;

	//鉄線の当たり判定の位置（-z方向）
	XMFLOAT3 WirePosLower_;

	//鉄線の当たり判定の位置（+x方向）
	XMFLOAT3 WirePosRight_;

	//鉄線の当たり判定の位置（-x方向）
	XMFLOAT3 WirePosLeft_;

	//----------当たり判定のサイズ----------

	//鉄線の当たり判定の大きさ（+z方向）
	XMFLOAT3 WireSizeUpper_;

	//鉄線の当たり判定の大きさ（-z方向）
	XMFLOAT3 WireSizeLower_;

	//鉄線の当たり判定の大きさ（+x方向）
	XMFLOAT3 WireSizeRight_;

	//鉄線の当たり判定の大きさ（-x方向）
	XMFLOAT3 WireSizeLeft_;

	//----------それぞれの柵の法線----------

	//ステージ北端の法線ベクトル
	XMFLOAT3 UpperNormal_;

	//ステージ南端の法線ベクトル
	XMFLOAT3 LowerNormal_;

	//ステージ東端の法線ベクトル
	XMFLOAT3 RightNormal_;

	//ステージ西端の法線ベクトル
	XMFLOAT3 LeftNormal_;

public:
	StageManager(GameObject* parent);
	~StageManager();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//CSVファイルから必要パラメータを読み込みする
	void SetStageInitCSV();

	/// <summary>
	/// 地面クラスのトランスフォーム初期化
	/// </summary>
	void InitGroundData();

	/// <summary>
	/// 柵クラスのトランスフォームの初期化
	/// </summary>
	void InitFenceData();

	/// <summary>
	/// 柵クラスの柱の本数・位置・鉄線クラスの当たり判定を設定
	/// </summary>
	void InitEndData();

	/// <summary>
	/// ステージ外のオブジェクトの初期化
	/// </summary>
	void InitOutStageThingData();

	//----------セッター・ゲッター関数----------

	float GetNorthEnd() const { return UpperEnd_; }
	float GetSouthEnd() const { return LowerEnd_; }
	float GetEastEnd() const { return RightEnd_; }
	float GetWestEnd() const { return LeftEnd_; }
};


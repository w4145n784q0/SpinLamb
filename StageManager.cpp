#include "StageManager.h"
#include"Engine/CsvReader.h"
#include"Engine/Global.h"

namespace
{
	//csv読み込み時のインデックス(各ステージの端)
	enum GroundEndIndex
	{
		i_up = 0,
		i_down,
		i_right,
		i_left,
		i_pillernum,
	};

	//csv読み込み時のインデックス(当たり判定の読み込み)
	enum StageCollisionIndex
	{
		i_collisionX = 0,
		i_collisionY,
		i_collisionZ,
	};

}

StageManager::StageManager(GameObject* parent)
	:GameObject(parent,"StageManager"),
	UpperEnd_(0.0f), LowerEnd_(0.0f), RightEnd_(0.0f), LeftEnd_(0.0f),PillerNum_(0),
	WirePosUpper_({ 0,0,0 }), WirePosLower_({ 0,0,0 }),WirePosRight_({ 0,0,0 }), WirePosLeft_({ 0,0,0 }), 
	WireSizeUpper_({ 0,0,0 }), WireSizeLower_({ 0,0,0 }), WireSizeRight_({ 0,0,0 }),WireSizeLeft_({ 0,0,0 }),
	UpperNormal_({ 0,0,0 }), LowerNormal_({ 0,0,0 }), RightNormal_({ 0,0,0 }), LeftNormal_({ 0,0,0 })
{
}

StageManager::~StageManager()
{
}

void StageManager::Initialize()
{
	//Stage関係の値は基本StageManagerクラスで読み込み、渡す

	//csvからパラメータ読み込み
	SetStageInitSCV();

	//Groundクラス生成
	Instantiate<Ground>(this);

	//Groundのデータ初期化
	InitGroundData();

	//Fenceクラス生成
	Instantiate<Fence>(this);

	//Fenceのデータ初期化
	InitFenceData();
	InitEndData();
}

void StageManager::Update()
{
}

void StageManager::Draw()
{
}

void StageManager::Release()
{
}

void StageManager::SetStageInitSCV()
{
	//csvファイルを読み込む
	//地面,鉄線,柱の各トランスフォーム初期化
	CsvReader csv;
	csv.Load("CSVdata\\StageData.csv");

	//csvファイルの各0列目の文字列の配列を取得
	std::vector<std::string> ParamNames = {
		"Ground","wire","piller"
	};

	//各トランスフォームを配列に入れる
	std::vector<std::reference_wrapper<Transform>> TransformArray = {
		GroundData_,WireData_,PillerData_
	};

	//まとめて初期化
	InitCSVTransformArray(csv, ParamNames, TransformArray);

	//csvファイルを読み込む
	//ステージの各端の位置、柱の本数
	CsvReader csv_end;
	csv_end.Load("CSVdata\\StageEndData.csv");

	//csvファイルの0列目の文字列を取得
	std::string end = "end";

	//指定した文字列がいずれかの0列目に存在したら
	if (csv_end.IsGetParamName(end))
	{
		//その行を配列として全取得
		std::vector<float> v = csv_end.GetParam(end);
		
		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		UpperEnd_ = v[i_up];
		LowerEnd_ = v[i_down];
		RightEnd_ = v[i_right];
		LeftEnd_ = v[i_left];
		PillerNum_ = static_cast<int>(v[i_pillernum]);
	}

	//csvファイルを読み込む
	//当たり判定の位置、サイズ,法線
	CsvReader csv_wire;
	csv_wire.Load("CSVdata\\StageWireData.csv");

	//csvファイルの各0列目の文字列の配列を取得
	std::string collision[] = { "UpperPos","LowerPos","RightPos","LeftPos",
	"UpperSize","LowerSize","RightSize","LeftSize",
	"UpperNormal","LowerNormal","RightNormal","LeftNormal"};

	//各当たり判定用のXMFLOAT3変数の参照をポインタ配列に格納
	XMFLOAT3* CollisionData[] = { &WirePosUpper_ ,&WirePosLower_,&WirePosRight_,&WirePosLeft_,
	&WireSizeUpper_,&WireSizeLower_,&WireSizeRight_,&WireSizeLeft_,
	&UpperNormal_,&LowerNormal_,&RightNormal_,&LeftNormal_ };

	for (int i = 0; i < sizeof(collision) / sizeof(collision[0]); i++)
	{
		//指定した文字列がいずれかの0列目に存在したら
		if (csv_wire.IsGetParamName(collision[i]))
		{
			//その行を配列として全取得
			std::vector<float> v = csv_wire.GetParam(collision[i]);

			//初期化の順番はcsvの各行の順番に合わせる
			//vの添え字はnamespaceで宣言した列挙型を使用
	        CollisionData[i]->x = v[i_collisionX];
			CollisionData[i]->y = v[i_collisionY];
			CollisionData[i]->z = v[i_collisionZ];
		}
	}
}

void StageManager::InitEndData()
{
	//Fenceクラスの柱の位置、法線の方向を初期化

	//柵クラスのインスタンスを取得
	Fence* pFence = (Fence*)FindObject("Fence");

	//柱の数、柱の位置を初期化
	pFence->SetPillerNum(PillerNum_);
	pFence->SetPiller(UpperEnd_,LowerEnd_,RightEnd_,LeftEnd_, PillerData_.position_.y);

	//各鉄線の衝突判定を初期化(当たり判定の位置,当たり判定のサイズ,鉄線が持つ法線)
	pFence->SetWireCollisionUpper(WirePosUpper_, WireSizeUpper_, UpperNormal_);
	pFence->SetWireCollisionLower(WirePosLower_, WireSizeLower_,LowerNormal_);
	pFence->SetWireCollisionRight(WirePosRight_, WireSizeRight_, RightNormal_);
	pFence->SetWireCollisionLeft(WirePosLeft_, WireSizeLeft_, LeftNormal_);

}

void StageManager::InitGroundData()
{
	//Groundクラスの初期化行う

	//地面クラスのインスタンスを取得
	Ground* pGround = (Ground*)FindObject("Ground");

	//CSVから読み込んだ拡大率,回転,位置を渡す
	pGround->SetScale(GroundData_.scale_);
	pGround->SetRotate(GroundData_.rotate_);
	pGround->SetPosition(GroundData_.position_);
}

void StageManager::InitFenceData()
{
	//Fenceクラスのトランスフォーム関係の初期化行う
	
	//柵クラスのインスタンスを取得
	Fence* pFence = (Fence*)FindObject("Fence");

	//CSVから読み込んだ柵,鉄線のトランスフォームを渡す
	pFence->InitWireTransform(WireData_);
	pFence->InitPillerTransform(PillerData_);
	
}

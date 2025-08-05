#include "StageManager.h"
#include"Engine/CsvReader.h"
#include "GameView.h"
#include"Ground.h"
#include"Fence.h"
#include"OutStageThing.h"

namespace
{
	//csv読み込み時のインデックス(各ステージの端)
	enum GroundEndIndex
	{
		i_up = 0,
		i_down,
		i_right,
		i_left,
		i_pillarnum,
	};

	//csv読み込み時のインデックス(当たり判定の読み込み)
	enum StageCollisionIndex
	{
		i_collisionX = 0,
		i_collisionY,
		i_collisionZ,
	};

	//インスタンス
	//このcppファイル内で一度しか使わないので名前空間で宣言
	Ground* pGround_ = nullptr;
	Fence* pFence_ = nullptr;
	OutStageThing* pOutStageThing_ = nullptr;
}

StageManager::StageManager(GameObject* parent)
	:GameObject(parent,"StageManager"),
	UpperEnd_(0.0f), LowerEnd_(0.0f), RightEnd_(0.0f), LeftEnd_(0.0f),PillarNum_(0),
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
	SetStageInitCSV();

	//Groundクラス生成
	Instantiate<Ground>(this);

	//Groundのデータ初期化
	InitGroundData();

	//Fenceクラス生成
	Instantiate<Fence>(this);

	//Fenceのデータ初期化
	InitFenceData();
	InitEndData();

	//ステージ外オブジェクト生成
	Instantiate<OutStageThing>(this); 
	InitOutStageThingData();
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

void StageManager::SetStageInitCSV()
{

	//----------地面,鉄線,柱の各トランスフォーム初期化----------

	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVdata\\StageData\\StageTransformData.csv");

	//csvファイルの各0列目の文字列の配列を取得
	std::vector<std::string> ParamNames = {
		"Ground","wire","pillar"
	};

	//各トランスフォームを配列に入れる
	std::vector<std::reference_wrapper<Transform>> TransformArray = {
		GroundData_,WireData_,PillarData_
	};

	//まとめて初期化
	InitCSVTransformArray(csv, ParamNames, TransformArray);


	//----------ステージの各端の位置、柱の本数----------

	//csvファイルを読み込む
	CsvReader csv_end;
	csv_end.Load("CSVdata\\StageData\\StageEndData.csv");

	//csvファイルの0列目の文字列を取得
	std::string end = "end";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> enddata = GetCSVReadData(csv_end, end);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	UpperEnd_ = enddata[i_up];
	LowerEnd_ = enddata[i_down];
	RightEnd_ = enddata[i_right];
	LeftEnd_ = enddata[i_left];
	PillarNum_ = static_cast<int>(enddata[i_pillarnum]);


	//----------当たり判定の位置、サイズ,法線の初期化----------

	//csvファイルを読み込む
	CsvReader csv_wire;
	csv_wire.Load("CSVdata\\StageData\\StageWireData.csv");

	//csvファイルの各0列目の文字列の配列を取得
	std::string collisionName[] = { "UpperPos","LowerPos","RightPos","LeftPos",
	"UpperSize","LowerSize","RightSize","LeftSize",
	"UpperNormal","LowerNormal","RightNormal","LeftNormal"};

	//各当たり判定用のXMFLOAT3変数の参照をポインタ配列に格納
	XMFLOAT3* CollisionFloat[] = { &WirePosUpper_ ,&WirePosLower_,&WirePosRight_,&WirePosLeft_,
	&WireSizeUpper_,&WireSizeLower_,&WireSizeRight_,&WireSizeLeft_,
	&UpperNormal_,&LowerNormal_,&RightNormal_,&LeftNormal_ };

	for (int i = 0; i < sizeof(collisionName) / sizeof(collisionName[0]); i++)
	{
		//0列目の文字列を渡し、その行のパラメータを取得
		std::vector<float> Collisiondata = GetCSVReadData(csv_wire, collisionName[i]);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
	    CollisionFloat[i]->x = Collisiondata[i_collisionX];
		CollisionFloat[i]->y = Collisiondata[i_collisionY];
		CollisionFloat[i]->z = Collisiondata[i_collisionZ];
	}

	//----------ステージ外オブジェクトの各トランスフォーム初期化----------
	//csvファイルを読み込む
	CsvReader csv_out;
	csv_out.Load("CSVdata\\StageData\\OutStageData.csv");

	//csvファイルの各0列目の文字列の配列を取得

	std::vector<std::string> Cabins = { "Cabin1" };
	std::vector<std::string> Trees = { "Tree1" , "Tree2" , "Tree3" };
	std::vector<std::string> Logs = { "Logs1" };
	std::vector<std::string> Straws = { "Straw1","Straw2","Straw3"};

	std::vector<std::vector<std::string>> OutStageNames = {
		Cabins, Trees,Logs,Straws
	};

	// 各Transform配列への参照
	std::vector<std::vector<Transform>*> OutStageTrans = {
		&CabinData_,
		&TreeData_,
		&LogsData_,
		&StrawData_
	};

	//すべての種類・個数を一括で初期化
	for (size_t type = 0; type < OutStageNames.size(); ++type) 
	{
		//名前の数とTransform配列数が一致しているなら続ける
		if (OutStageNames.size() != OutStageTrans.size())
		{
			break;
		}

		//必要な数だけTransformを確保
		OutStageTrans[type]->resize(OutStageNames[type].size());

		//各トランスフォーム[トランスフォームの種類][その種類のi番目]
		for (size_t i = 0; i < OutStageNames[type].size(); ++i)
		{
			InitCSVTransform(csv_out, OutStageNames[type][i], (*OutStageTrans[type])[i]);
		}
	}

}

void StageManager::InitGroundData()
{
	//Groundクラスの初期化行う

	//地面クラスのインスタンスを取得
	pGround_ = (Ground*)FindObject("Ground");

	//CSVから読み込んだ拡大率,回転,位置を渡す
	pGround_->SetScale(GroundData_.scale_);
	pGround_->SetRotate(GroundData_.rotate_);
	pGround_->SetPosition(GroundData_.position_);

	//GameViewに地面クラスのポインタを渡す
	//ImGui描画用
	GameView::SetGround(pGround_);
}

void StageManager::InitFenceData()
{
	//Fenceクラスのトランスフォーム関係の初期化行う

	//柵クラスのインスタンスを取得
	pFence_ = (Fence*)FindObject("Fence");

	//CSVから読み込んだ柵,鉄線のトランスフォームを渡す
	pFence_->InitWireTransform(WireData_);
	pFence_->InitPillarTransform(PillarData_);

	//GameViewに柵クラスのポインタを渡す
	//ImGui描画用
	GameView::SetFence(pFence_);
}

void StageManager::InitEndData()
{
	//Fenceクラスの柱の位置、法線の方向を初期化

	//柵クラスのインスタンスを取得
	Fence* pFence = (Fence*)FindObject("Fence");

	//柱の数、柱の位置を初期化
	pFence->SetPillarNum(PillarNum_);
	pFence->SetPillar(UpperEnd_,LowerEnd_,RightEnd_,LeftEnd_, PillarData_.position_.y);

	//各鉄線の衝突判定を初期化(当たり判定の位置,当たり判定のサイズ,鉄線が持つ法線)
	pFence->SetWireCollisionUpper(WirePosUpper_, WireSizeUpper_, UpperNormal_);
	pFence->SetWireCollisionLower(WirePosLower_, WireSizeLower_,LowerNormal_);
	pFence->SetWireCollisionRight(WirePosRight_, WireSizeRight_, RightNormal_);
	pFence->SetWireCollisionLeft(WirePosLeft_, WireSizeLeft_, LeftNormal_);

}

void StageManager::InitOutStageThingData()
{
	//ステージ外オブジェクトを初期化

	//OutStageThingのインスタンスを取得
	pOutStageThing_ = (OutStageThing*)FindObject("OutStageThing");

	//CSVから読み込んだ小屋、木、丸太のトランスフォームを渡す
	pOutStageThing_->SetCabinTransform(CabinData_);
	pOutStageThing_->SetTreeTransform(TreeData_);
	pOutStageThing_->SetLogsTransform(LogsData_);
	pOutStageThing_->SetStrawTransform(StrawData_);

	GameView::SetOutStageThing(pOutStageThing_);
}
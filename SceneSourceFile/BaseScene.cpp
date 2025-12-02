#include "BaseScene.h"
#include"../Engine/Global.h"
#include"../ViewSourceFile/GameView.h"

//遷移時間の実体定義
int BaseScene::SceneShortTransition = 0;
int BaseScene::SceneTransition = 0;
int BaseScene::SceneLongTransition = 0;

//GameObject継承先で汎用的に使用する変数
enum CommonIndex
{
	i_SceneShortTransition = 0,
	i_SceneTransition,
	i_SceneLongTransition,
};

BaseScene::BaseScene(GameObject* parent)
	:GameObject(parent,"BaseScene"),SceneState_(S_Active),SceneTransitionTimer_(0)
{
}

BaseScene::BaseScene(GameObject* parent, const std::string& name)
	:GameObject(parent, name), SceneState_(S_Active), SceneTransitionTimer_(0)
{
}

BaseScene::~BaseScene()
{
}

void BaseScene::Update()
{
	//現在のシーンの状態(通常・停止・遷移中)によって更新を分ける
	//継承先がSceneManagerに登録されているシーンの場合、Updateで必ず呼ぶ
	//何の処理を実装するかは継承先によって分ける

	switch (SceneState_)
	{
	case BaseScene::S_Active:
		UpdateActive();
		break;
	case BaseScene::S_InActive:
		UpdateInActive();
		break;
	case BaseScene::S_Transition:
		UpdateTransition();
		break;
	default:
		break;
	}
}

void BaseScene::CSVSceneDataInitialize()
{
	//GameObjectで共通する汎用データの初期化

	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\BaseSceneData.csv");

	//csvファイルの各0列目の文字列を取得
	std::string base = "SceneData";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> baseSceneData = GetCSVReadData(csv, base);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	SceneShortTransition = static_cast<int>(baseSceneData[i_SceneShortTransition]);
	SceneTransition = static_cast<int>(baseSceneData[i_SceneTransition]);
	SceneLongTransition = static_cast<int>(baseSceneData[i_SceneLongTransition]);
}

void BaseScene::SetPointerGameView()
{
	//GameViewに自身(継承先も含むので基底クラスであるBaseScene)のポインタを渡す
	GameView::SetScene(this);
}

void BaseScene::DrawImGuiBaseScene()
{
	if (ImGui::TreeNode("BaseSceneData"))
	{
		//シーン遷移時間
		ImGui::InputInt("SceneShortTransition", &SceneShortTransition);
		ImGui::InputInt("SceneTransition", &SceneTransition);
		ImGui::InputInt("SceneLongTransition", &SceneLongTransition);

		ImGui::TreePop();
	}
}
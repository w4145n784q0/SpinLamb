#include "SceneData.h"
#include"Engine/CsvReader.h"

namespace
{
	//各シーンで仕様するロゴ等の位置などの変数
	enum scene_pos
	{
		resultposition_x = 0,
		resultposition_y,
		resultposition_z,
		underposition_x,
		underposition_y,
		underposition_z,
	};
}

SceneData::SceneData(GameObject* parent)
	:GameObject(parent, "SceneData")
{
}

SceneData::SceneData(GameObject* parent, const std::string& name)
	:GameObject(parent, name)
{
}

SceneData::~SceneData()
{
}

void SceneData::SetCSVScene()
{
	CsvReader csv;
	csv.Load("CSVdata\\SceneData.csv");

	std::string scene = "Scene";
	if (csv.IsGetParamName(scene))
	{
		std::vector<float> v = csv.GetParam(scene);
		ResultPosition_ = { v[resultposition_x], v[resultposition_y], v[resultposition_z] };
		UnderPosition_ = { v[underposition_x], v[underposition_y], v[underposition_z] };
		
		//SetTransformPRS(Result, v);
		//SetTransformPRS(PushTitle, v);
	}
}

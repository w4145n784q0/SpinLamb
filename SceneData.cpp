#include "SceneData.h"
#include"Engine/CsvReader.h"

namespace
{
	//各シーンで仕様するロゴ等の位置などの変数
	enum scene_pos
	{
		practiceend = 0,
		resultposition_x,
		resultposition_y,
		resultposition_z,
		underposition_x,
		underposition_y,
		underposition_z,
	};
}

SceneData::SceneData(GameObject* parent)
{
}

SceneData::SceneData(GameObject* parent, const std::string& name)
{
}

SceneData::~SceneData()
{
}

void SceneData::SetCSV()
{
	CsvReader csv;
	csv.Load("CSVdata\\SceneData.csv");

	std::string scene = "Scene";
	if (csv.IsGetParamName(scene))
	{
		std::vector<float> v = csv.GetParam(scene);
		practiceEnd_ = static_cast<int>(v[practiceend]);
		ResultPosition_ = { v[resultposition_x], v[resultposition_y], v[resultposition_z] };
		UnderPosition_ = { v[underposition_x], v[underposition_y], v[underposition_z] };

	}
}

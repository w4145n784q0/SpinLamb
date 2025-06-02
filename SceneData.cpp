#include "SceneData.h"
#include"Engine/CsvReader.h"

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

	std::string result = "Result";
	if (csv.IsGetParamName(result))
	{
		std::vector<float> v = csv.GetParam(result);
		SetTransformPRS(Result, v);
	}

	std::string push = "PushTitle";
	if (csv.IsGetParamName(push))
	{
		std::vector<float> v = csv.GetParam(push);
		SetTransformPRS(PushTitle, v);
	}
}

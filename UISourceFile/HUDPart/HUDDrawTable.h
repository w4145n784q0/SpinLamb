#pragma once
#include "../../Engine/GameObject.h"
#include <vector>
#include <unordered_map>
#include "RenderTask.h"
#include"HUDParam.h"

//HUDクラスの描画タスク管理を行うモジュール

class HUDDrawTable
	: public GameObject
{
public:
    HUDDrawTable() = default;
    HUDDrawTable(GameObject* parent);
    virtual ~HUDDrawTable() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

	//モードごとの描画タスク
	std::unordered_map<DrawMode, std::vector<RenderTask>> byMode;

	//常時描画するタスク（ミニマップなど）
	std::vector<RenderTask> always;
};


#pragma once
#include "Engine/GameObject.h"
class OutStageThing :
    public GameObject
{
private:
	//----------モデルハンドル----------

	//小屋モデル
	int hCabin_;

	//木モデル
	int hTree_;

	//丸太モデル
	int hLogs_;

	//----------モデル用トランスフォーム----------
	Transform CabinTransform;

	Transform TreeTransform;

	Transform LogsTransform;
public:
	OutStageThing(GameObject* parent);
	~OutStageThing();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//----------セッター・ゲッター関数----------
	void SetCabinTransform(Transform _transform) { CabinTransform = _transform; }

	void SetTreeTransform(Transform _transform) { TreeTransform = _transform; }

	void SetLogsTransform(Transform _transform) { LogsTransform = _transform; }
};


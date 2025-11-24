#include "CharacterShadow.h"
#include"../Character.h"
#include"../../Engine/Model.h"

CharacterShadow::CharacterShadow(GameObject* parent)
	:GameObject(parent, "CharacterShadow"), params_(nullptr), character_(nullptr)
{
}

void CharacterShadow::InitShadow()
{
	if (params_ == nullptr)
	{
		return;
	}

	//初期化の時点でステージクラスのインスタンスを取得
	params_->ShadowParam_.pGround_ = (Ground*)FindObject("Ground");
	assert(params_->ShadowParam_.pGround_ != nullptr);

	//影モデル読み込み
	params_->ShadowParam_.hShadow_ = Model::Load("Model\\ShadowPoint.fbx");
	assert(params_->ShadowParam_.hShadow_ >= 0);

}

void CharacterShadow::ShadowSet()
{
	if (params_ == nullptr)
	{
		return;
	}

	if (character_ != nullptr)
	{
		//自身の位置を取得
		XMFLOAT3 tmp = character_->GetPosition();

		//ステージのモデル番号を取得
		int hGroundModel = params_->ShadowParam_.pGround_->GetModelHandle();

		//ステージに向かってレイを飛ばす設定をする
		RayCastData data;

		//レイの発射位置を設定
		data.start = character_->GetPosition();

		//レイの方向を設定(0, -1, 0なので下向き)
		data.dir = XMFLOAT3(0, -1, 0);

		//レイを発射
		Model::RayCast(hGroundModel, &data);

		//レイが当たったら
		if (data.hit)
		{
			//現在位置-衝突点までの距離 + 補正値で影の高さを設定
			params_->ShadowParam_.ShadowHeight_ =
				(tmp.y - data.dist) + params_->ShadowParam_.ShadowCorrection_;
		}

		//y座標以外はキャラクターと同じ位置に設定
		params_->ShadowParam_.ShadowTrans_.position_.x = tmp.x;
		params_->ShadowParam_.ShadowTrans_.position_.z = tmp.z;

		//影の高さをトランスフォームに設定する
		params_->ShadowParam_.ShadowTrans_.position_.y = params_->ShadowParam_.ShadowHeight_;
	}

}

void CharacterShadow::ShadowDraw()
{
	if (params_ == nullptr)
	{
		return;
	}

	//ShadowSetで位置を設定した影を描画
	Model::SetAndDraw(params_->ShadowParam_.hShadow_, params_->ShadowParam_.ShadowTrans_);
}
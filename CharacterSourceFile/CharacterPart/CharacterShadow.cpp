#include "CharacterShadow.h"
#include"../../Engine/Model.h"

CharacterShadow::CharacterShadow(GameObject* parent)
	:GameObject(parent, "CharacterShadow")
{
}

void CharacterShadow::InitShadow()
{
	//初期化の時点でステージクラスのインスタンスを取得
	ShadowParam_.pGround_ = (Ground*)FindObject("Ground");
	assert(ShadowParam_.pGround_ != nullptr);

	//影モデル読み込み
	ShadowParam_.hShadow_ = Model::Load("Model\\ShadowPoint.fbx");
	assert(ShadowParam_.hShadow_ >= 0);

}

void CharacterShadow::ShadowSet()
{
	//ステージのモデル番号を取得
	int hGroundModel = ShadowParam_.pGround_->GetModelHandle();

	//ステージに向かってレイを飛ばす設定をする
	RayCastData data;

	//レイの発射位置を設定
	data.start = this->transform_.position_;

	//レイの方向を設定(0, -1, 0なので下向き)
	data.dir = XMFLOAT3(0, -1, 0);

	//レイを発射
	Model::RayCast(hGroundModel, &data);

	//レイが当たったら
	if (data.hit)
	{
		//現在位置-衝突点までの距離 + 補正値で影の高さを設定
		ShadowParam_.ShadowHeight_ = (this->transform_.position_.y - data.dist) + ShadowParam_.ShadowCorrection_;
	}

	//y座標以外はキャラクターと同じ位置に設定
	ShadowParam_.ShadowTrans_.position_.x = this->transform_.position_.x;
	ShadowParam_.ShadowTrans_.position_.z = this->transform_.position_.z;

	//影の高さをトランスフォームに設定する
	ShadowParam_.ShadowTrans_.position_.y = ShadowParam_.ShadowHeight_;

}

void CharacterShadow::ShadowDraw()
{
	//ShadowSetで位置を設定した影を描画
	Model::SetAndDraw(ShadowParam_.hShadow_, ShadowParam_.ShadowTrans_);
}
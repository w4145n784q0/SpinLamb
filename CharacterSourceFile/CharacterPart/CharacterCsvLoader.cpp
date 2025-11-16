#include "CharacterCsvLoader.h"
#include "../Character.h" 

namespace {

	//csv読み込み時のインデックス
	//キャラクタークラスの共通変数を初期化

	//移動関係
	enum MoveIndex
	{
		//通常移動
		i_NormalAcceleValue = 0,
		i_NormalFullAccelerate,
		i_NormalFriction,

		//ダッシュ攻撃
		i_AttackAcceleValue,
		i_AttackFullAccelerate,
		i_AttackFriction,

		//矢印関係
		i_ArrowRotateX,
		i_ArrowRotateY,
		i_ArrowRotateZ,
		i_ArrowScaleX,
		i_ArrowScaleY,
		i_ArrowScaleZ,
		i_AddArrowDepth,
	};	

	//回転関係
	enum RotateIndex
	{
		i_MoveRotate = 0,
		i_FastRotate,
	};

	//空中関係
	enum JumpIndex
	{
		i_Gravity = 0,
		i_JumpHeight,
		i_UpperLimit,
		i_LowerLimit,
		i_MinusLimit,
	};

	//被弾関係
	enum HitIndex
	{
		i_Collider = 0,
		i_HitStopValue,
		i_OriginalRangeMin,
		i_OriginalRangeMax,
		i_ConvertedRangeMin,
		i_ConvertedRangeMax,
		i_DecelerationRate,
		i_KnockBackEnd,
	};

	//柵に接触関係
	enum FenceHitIndex
	{
		i_KnockBackPower = 0,
		i_InvincibilityValue,
		i_BlinkValue,

	};

	//待機関係
	enum WaitIndex
	{
		i_WaitValue = 0,
	};

	//影付け関係
	enum ShadowIndex
	{
		i_ShadowCorrection = 0,
	};

	//アニメーション関連
	enum AnimeIndex
	{
		i_DashOneAnimeSize = 0,
		i_DashSheetSize,
		i_DashFrameCountMax,
		i_DashOneFrameNum,
	};

}

CharacterCsvLoader::CharacterCsvLoader(GameObject* parent)
	:GameObject(parent, "CharacterCsvLoader"), params_(nullptr), character_(nullptr)
{
}

void CharacterCsvLoader::SetCSVStatus(std::string _path)
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load(_path);

	//--------------------初期化関係のパラメータ(自身のtransform_,正面ベクトル)--------------------

	//csvファイルの0列目の文字列を取得
	std::string p_init = "InitializeParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> initData = GetCSVReadData(csv, p_init);

	//自身のトランスフォームを初期化
	Transform tmp;
	SetTransformPRS(tmp, initData);

	//親クラスのトランスフォームに反映
	character_->SetPosition(tmp.position_);
	character_->SetRotate(tmp.rotate_);
	character_->SetScale(tmp.scale_);

	//初期位置を保管する
	params_->InitParam_.StartPosition_ = tmp.position_;

	//--------------------移動関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得
	std::string p_move = "MoveParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> MoveData = GetCSVReadData(csv, p_move);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	params_->MoveParam_.NormalAcceleValue_ = MoveData[i_NormalAcceleValue];
	params_->MoveParam_.NormalFullAccelerate_ = MoveData[i_NormalFullAccelerate];
	params_->MoveParam_.NormalFriction_ = MoveData[i_NormalFriction];

	params_->MoveParam_.AttackAcceleValue_ = MoveData[i_AttackAcceleValue];
	params_->MoveParam_.AttackFullAccelerate_ = MoveData[i_AttackFullAccelerate];
	params_->MoveParam_.AttackFriction_ = MoveData[i_AttackFriction];

	params_->MoveParam_.ArrowRotate_ = { MoveData[i_ArrowRotateX],MoveData[i_ArrowRotateY],MoveData[i_ArrowRotateZ] };
	params_->MoveParam_.ArrowScale_ = { MoveData[i_ArrowScaleX],MoveData[i_ArrowScaleY],MoveData[i_ArrowScaleZ] };
	
	params_->MoveParam_.AddArrowDepth_ = MoveData[i_AddArrowDepth];

	//--------------------回転関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得
	std::string p_rotate = "RotateParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> RotData = GetCSVReadData(csv, p_rotate);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	params_->RotateParam_.MoveRotateX = RotData[i_MoveRotate];
	params_->RotateParam_.FastRotateX = RotData[i_FastRotate];


	//--------------------空中関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_jump = "JumpParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> JumpData = GetCSVReadData(csv, p_jump);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	params_->JumpParam_.Gravity_ = JumpData[i_Gravity];
	params_->JumpParam_.JumpHeight = JumpData[i_JumpHeight];
	params_->JumpParam_.HeightLowerLimit_ = JumpData[i_UpperLimit];
	params_->JumpParam_.HeightUpperLimit_ = JumpData[i_LowerLimit];
	params_->JumpParam_.MinusLimit_ = JumpData[i_MinusLimit];

	//--------------------被弾関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_hit = "HitParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> HitData = GetCSVReadData(csv, p_hit);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	params_->HitParam_.ColliderSize_ = HitData[i_Collider];
	params_->HitParam_.HitStopValue_ = static_cast<int>(HitData[i_HitStopValue]);
	params_->HitParam_.OriginalRangeMin_ = HitData[i_OriginalRangeMin];
	params_->HitParam_.OriginalRangeMax_ = HitData[i_OriginalRangeMax];
	params_->HitParam_.ConvertedRangeMin_ = HitData[i_ConvertedRangeMin];
	params_->HitParam_.ConvertedRangeMax_ = HitData[i_ConvertedRangeMax];
	params_->HitParam_.DecelerationRate_ = HitData[i_DecelerationRate];
	params_->HitParam_.KnockBackEnd_ = HitData[i_KnockBackEnd];

	//--------------------柵に接触関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_fencehit = "FenceHitParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> FenceHitData = GetCSVReadData(csv, p_fencehit);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	params_->FenceHitParam_.KnockBackPower_ = FenceHitData[i_KnockBackPower];
	params_->FenceHitParam_.InvincibilityValue_ = static_cast<int>(FenceHitData[i_InvincibilityValue]);
	params_->FenceHitParam_.BlinkValue_ = static_cast<int>(FenceHitData[i_BlinkValue]);

	//--------------------待機時間関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_wait = "WaitParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> WaitData = GetCSVReadData(csv, p_wait);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	params_->WaitParams_.WaitValue_ = static_cast<int>(WaitData[i_WaitValue]);

	//--------------------影関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_shadow = "ShadowParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> ShadowData = GetCSVReadData(csv, p_shadow);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	params_->ShadowParam_.ShadowCorrection_ = ShadowData[i_ShadowCorrection];

	//--------------------アニメーション関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_anime = "AnimeParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> AnimeData = GetCSVReadData(csv, p_anime);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	params_->AnimeParam_.DashOneAnimeSize_ = static_cast<int>(AnimeData[i_DashOneAnimeSize]);
	params_->AnimeParam_.DashSheetSize_ = static_cast<int>(AnimeData[i_DashSheetSize]);
	params_->AnimeParam_.DashFrameCountMax_ = static_cast<int>(AnimeData[i_DashFrameCountMax]);
	params_->AnimeParam_.DashOneFrameNum_ = static_cast<int>(AnimeData[i_DashOneFrameNum]);
}
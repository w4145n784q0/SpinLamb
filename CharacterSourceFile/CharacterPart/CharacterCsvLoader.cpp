#include "CharacterCsvLoader.h"

namespace {

	//csv読み込み時のインデックス
	//キャラクタークラスの共通変数を初期化

	//移動関係
	enum MoveIndex
	{
		i_Velocity = 0,
		i_AcceleValue,
		i_AcceleMax,
		i_Friction,
		i_ArrowRotateX,
		i_ArrowRotateY,
		i_ArrowRotateZ,
		i_ArrowScaleX,
		i_ArrowScaleY,
		i_ArrowScaleZ,
		i_AddArrowDepth
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

	//影付け関係
	enum ShadowIndex
	{
		i_ShadowCorrection = 0,
	};

}

CharacterCsvLoader::CharacterCsvLoader(GameObject* parent)
	:GameObject(parent, "CharacterCsvLoader")
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
	SetTransformPRS(this->transform_, initData);

	//初期位置を保管する
	InitParam_.StartPosition_ = this->transform_.position_;

	//--------------------移動関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得
	std::string p_move = "MoveParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> MoveData = GetCSVReadData(csv, p_move);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	MoveParam_.Velocity_ = MoveData[i_Velocity];
	MoveParam_.AcceleValue_ = MoveData[i_AcceleValue];
	MoveParam_.FullAccelerate_ = MoveData[i_AcceleMax];
	MoveParam_.Friction_ = MoveData[i_Friction];
	MoveParam_.ArrowRotate_ = { MoveData[i_ArrowRotateX],MoveData[i_ArrowRotateY],MoveData[i_ArrowRotateZ] };
	MoveParam_.ArrowScale_ = { MoveData[i_ArrowScaleX],MoveData[i_ArrowScaleY],MoveData[i_ArrowScaleZ] };
	MoveParam_.AddArrowDepth_ = MoveData[i_AddArrowDepth];

	//--------------------回転関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得
	std::string p_rotate = "RotateParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> RotData = GetCSVReadData(csv, p_rotate);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	RotateParam_.MoveRotateX = RotData[i_MoveRotate];
	RotateParam_.FastRotateX = RotData[i_FastRotate];


	//--------------------空中関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_jump = "JumpParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> JumpData = GetCSVReadData(csv, p_jump);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	JumpParam_.Gravity_ = JumpData[i_Gravity];
	JumpParam_.JumpHeight = JumpData[i_JumpHeight];
	JumpParam_.HeightLowerLimit_ = JumpData[i_UpperLimit];
	JumpParam_.HeightUpperLimit_ = JumpData[i_LowerLimit];
	JumpParam_.MinusLimit_ = JumpData[i_MinusLimit];

	//--------------------被弾関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_hit = "HitParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> HitData = GetCSVReadData(csv, p_hit);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	HitParam_.ColliderSize_ = HitData[i_Collider];
	HitParam_.OriginalRangeMin_ = HitData[i_OriginalRangeMin];
	HitParam_.OriginalRangeMax_ = HitData[i_OriginalRangeMax];
	HitParam_.ConvertedRangeMin_ = HitData[i_ConvertedRangeMin];
	HitParam_.ConvertedRangeMax_ = HitData[i_ConvertedRangeMax];
	HitParam_.DecelerationRate_ = HitData[i_DecelerationRate];
	HitParam_.KnockBackEnd_ = HitData[i_KnockBackEnd];

	//--------------------柵に接触関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_fencehit = "FenceHitParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> FenceHitData = GetCSVReadData(csv, p_fencehit);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	FenceHitParam_.KnockBackPower_ = FenceHitData[i_KnockBackPower];
	FenceHitParam_.InvincibilityValue_ = static_cast<int>(FenceHitData[i_InvincibilityValue]);
	FenceHitParam_.BlinkValue_ = static_cast<int>(FenceHitData[i_BlinkValue]);



	//--------------------影関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_shadow = "ShadowParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> ShadowData = GetCSVReadData(csv, p_shadow);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	ShadowParam_.ShadowCorrection_ = ShadowData[i_ShadowCorrection];
}
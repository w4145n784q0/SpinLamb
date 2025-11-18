#include "CharacterVFX.h"
#include"../../Engine/VFX.h"

CharacterVFX::CharacterVFX(GameObject* parent)
	:GameObject(parent, "CharacterVFX"), params_(nullptr)
{
}

void CharacterVFX::InitCSVEffect()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVData\\EffectData\\VFXData.csv");

	//csvファイルの各0列目の文字列の配列を取得
	std::string Effects[] = { "Charge", "FullCharge", "Locus","Landing", "Hit" ,"FenceHit" };

	//ChargeParam_から始まるVFXのパラメータ(vector<float>型の配列)の参照を
	//ポインタ配列に格納
	std::vector<float>* Param[] = { 
		
		&params_->EffectParam_.ChargeParam_,
		&params_->EffectParam_.FullChargeParam,
		&params_->EffectParam_.AttackLocusParam_,
		&params_->EffectParam_.LandingParam_,
		&params_->EffectParam_.HitEffectParam_,
		&params_->EffectParam_.FenceHitEffectParam_
	};


	for (int i = 0; i < sizeof(Effects) / sizeof(Effects[0]); i++)
	{
		//0列目の文字列を渡し、その行のパラメータを取得
		std::vector<float> v = GetCSVReadData(csv, Effects[i]);

		//この時点では代入のみ行われる
		*Param[i] = v;
	}

	//読み込んだデータをEmitterDataに代入し
	//使用する画像のパスを入れていく
	//ChargeDataは呼び出し元によって使う画像のパスが違うのでここでは代入しない

	std::string str = "ParticleAssets\\";

	//チャージ
	VFX::SetEmitter(params_->EffectParam_.ChargeData_, params_->EffectParam_.ChargeParam_);

	//最大チャージ
	VFX::SetEmitter(params_->EffectParam_.FullChargeData_, params_->EffectParam_.FullChargeParam);
	params_->EffectParam_.FullChargeData_.textureFileName = str + "circle_W.png";

	//攻撃中の軌跡
	VFX::SetEmitter(params_->EffectParam_.LocusData_, params_->EffectParam_.AttackLocusParam_);
	params_->EffectParam_.LocusData_.textureFileName = str + "flashB_Y.png";

	//着地
	VFX::SetEmitter(params_->EffectParam_.LandingData_, params_->EffectParam_.LandingParam_);
	params_->EffectParam_.LandingData_.textureFileName = str + "cloudA.png";

	//被弾
	VFX::SetEmitter(params_->EffectParam_.HitData_, params_->EffectParam_.HitEffectParam_);
	params_->EffectParam_.HitData_.textureFileName = str + "star.png";

	//柵に接触
	VFX::SetEmitter(params_->EffectParam_.FenceHitData_, params_->EffectParam_.FenceHitEffectParam_);
	params_->EffectParam_.FenceHitData_.textureFileName = str + "flashB_Y.png";

}

void CharacterVFX::SetChargingEffect(std::string _path, XMFLOAT3 _pos)
{
	//チャージ中エフェクトを実際に行う

	//使用する画像のパスをセットする
	params_->EffectParam_.ChargeData_.textureFileName = _path;

	//発射位置をセット
	params_->EffectParam_.ChargeData_.position = _pos;

	//エフェクトを開始
	VFX::Start(params_->EffectParam_.ChargeData_);
}

void CharacterVFX::SetFullChargeEffect(XMFLOAT3 _pos)
{
	//最大チャージエフェクトを実際に行う

	//発射位置をセット
	params_->EffectParam_.FullChargeData_.position = _pos;

	//エフェクトを開始
	VFX::Start(params_->EffectParam_.FullChargeData_);
}

void CharacterVFX::SetAttackLocusEffect(XMFLOAT3 _pos)
{
	//攻撃中の軌跡エフェクトを実際に行う

	//発射位置をセット
	params_->EffectParam_.LocusData_.position = _pos;

	//エフェクトを開始
	VFX::Start(params_->EffectParam_.LocusData_);
}

void CharacterVFX::SetLandingEffect(XMFLOAT3 _pos)
{
	//着地エフェクトを実際にセットする

	//発射位置をセット
	params_->EffectParam_.LandingData_.position = _pos;

	//エフェクトを開始
	VFX::Start(params_->EffectParam_.LandingData_);
}

void CharacterVFX::SetHitEffect(XMFLOAT3 _pos)
{
	//被弾エフェクトを実際に行う

	//発射位置をセット
	params_->EffectParam_.HitData_.position = _pos;

	//エフェクトを開始
	VFX::Start(params_->EffectParam_.HitData_);
}

void CharacterVFX::SetFenceHitEffect(XMFLOAT3 _pos)
{
	//柵に接触時エフェクトを実際に行う

	//発射位置をセット
	params_->EffectParam_.FenceHitData_.position = _pos;

	//エフェクトを開始
	VFX::Start(params_->EffectParam_.FenceHitData_);
}

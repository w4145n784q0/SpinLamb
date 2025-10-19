#include "CharacterVFX.h"
#include"../Character.h"
#include"../../Engine/VFX.h"

CharacterVFX::CharacterVFX(GameObject* parent)
	:GameObject(parent, "CharacterVFX"), params_(nullptr), character_(nullptr)
{
}

void CharacterVFX::InitCSVEffect()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVData\\EffectData\\VFXData.csv");

	//csvファイルの各0列目の文字列の配列を取得
	std::string Effects[] = { "Charge","FullCharge" ,"Locus" , "Hit" , "FenceHit" };

	//ChargeParam_から始まるVFXのパラメータ(vector<float>型の配列)の参照を
	//ポインタ配列に格納
	std::vector<float>* Param[] = { 
		
		&params_->EffectParam_.ChargeParam_,
		&params_->EffectParam_.FullChargeParam,
		&params_->EffectParam_.AttackLocusParam_,
		&params_->EffectParam_.HitEffectParam_,
		&params_->EffectParam_.FenceHitEffectParam_
	};


	for (int i = 0; i < sizeof(Effects) / sizeof(Effects[0]); i++)
	{
		//0列目の文字列を渡し、その行のパラメータを取得
		std::vector<float> v = GetCSVReadData(csv, Effects[i]);

		//この時点では代入のみ行われる
		// SetEmitterで実際にVFXのパラメータにセットされる 
		*Param[i] = v;

	}
}

void CharacterVFX::SetChargingEffect(std::string _path)
{
	//csvから読み込んだ,チャージ中エフェクトのパラメータを実際にセットする
	EmitterData charge;
	VFX::SetEmitter(charge, params_->EffectParam_.ChargeParam_);

	//使用する画像のパスをセットする
	charge.textureFileName = _path;

	//発射位置をセット
	charge.position = character_->GetPosition();

	//エフェクトを開始
	VFX::Start(charge);
}

void CharacterVFX::SetFullChargeEffect()
{
	//csvから読み込んだ,最大チャージエフェクトのパラメータを実際にセットする
	EmitterData fullcharge;
	VFX::SetEmitter(fullcharge, params_->EffectParam_.FullChargeParam);

	//使用する画像のパスをセットする
	fullcharge.textureFileName = "ParticleAssets\\circle_W.png";

	//発射位置をセット
	fullcharge.position = character_->GetPosition();

	//エフェクトを開始
	VFX::Start(fullcharge);
}

void CharacterVFX::SetAttackLocusEffect()
{
	//csvから読み込んだ,攻撃中の軌跡エフェクトのパラメータを実際にセットする
	EmitterData locus;
	VFX::SetEmitter(locus, params_->EffectParam_.AttackLocusParam_);

	//使用する画像のパスをセットする
	locus.textureFileName = "ParticleAssets\\flashB_Y.png";

	//発射位置をセット
	locus.position = character_->GetPosition();

	//エフェクトを開始
	VFX::Start(locus);
}

void CharacterVFX::SetHitEffect()
{
	//csvから読み込んだ,被弾エフェクトのパラメータを実際にセットする
	EmitterData hit;
	VFX::SetEmitter(hit, params_->EffectParam_.HitEffectParam_);

	//使用する画像のパスをセットする
	hit.textureFileName = "ParticleAssets\\flashB_W.png";

	//発射位置をセット
	hit.position = character_->GetPosition();

	//エフェクトを開始
	VFX::Start(hit);
}

void CharacterVFX::SetFenceHitEffect()
{
	//csvから読み込んだ,柵に接触時エフェクトのパラメータを実際にセットする
	EmitterData  fencehit;
	VFX::SetEmitter(fencehit, params_->EffectParam_.FenceHitEffectParam_);

	//使用する画像のパスをセットする
	fencehit.textureFileName = "ParticleAssets\\flashB_W.png";

	//発射位置をセット
	fencehit.position = character_->GetPosition();

	//エフェクトを開始
	VFX::Start(fencehit);
}

#include "Character.h"
#include"../Engine/Audio.h"

Character::Character(GameObject* parent)
	:GameObject(parent,"Character"),
	params_(nullptr), modeldraw_(nullptr), vfx_(nullptr), shadow_(nullptr), air_(nullptr),
	forward_(nullptr), movement_(nullptr),rotate_(nullptr),charge_(nullptr),hitstop_(nullptr),
    hit_(nullptr),fence_(nullptr),wait_(nullptr),
	csvload_(nullptr),observer_(nullptr),debugpanel_(nullptr)
{
}

Character::Character(GameObject* parent, const std::string& name)
	:GameObject(parent, name),
	params_(nullptr), modeldraw_(nullptr), vfx_(nullptr), shadow_(nullptr), air_(nullptr),
	forward_(nullptr), movement_(nullptr), rotate_(nullptr), charge_(nullptr), hitstop_(nullptr),
	hit_(nullptr), fence_(nullptr), wait_(nullptr),
	csvload_(nullptr), observer_(nullptr), debugpanel_(nullptr)
{
	//各モジュールの生成・初期化(Instantiate)
	//必要に応じてパラメータのセット(SetParams)
	//必要に応じて親クラス（自身）のセット(SetCharacter

	if (params_ == nullptr)
	{
		params_ = std::make_unique<CharacterParams>(this);
	}

	if (modeldraw_ == nullptr)
	{
		modeldraw_ = std::make_unique<CharacterModelBlink>(this);
		modeldraw_->SetParams(params_.get());
	}
	if (vfx_ == nullptr)
	{
		vfx_ = std::make_unique<CharacterVFX>(this);
		vfx_->SetParams(params_.get());
	}
	if (shadow_ == nullptr)
	{
		shadow_ = std::make_unique<CharacterShadow>(this);
		shadow_->SetParams(params_.get());
		shadow_->SetCharacter(this);
	}
	if (air_ == nullptr)
	{
		air_ = std::make_unique<CharacterAir>(this);
		air_->SetParams(params_.get());
		air_->SetCharacter(this);
	}
	if (forward_ == nullptr)
	{
		forward_ = std::make_unique<CharacterForward>(this);
		forward_->SetParams(params_.get());
		forward_->SetCharacter(this);
	}
	if (movement_ == nullptr)
	{
		movement_ = std::make_unique<CharacterMovement>(this);
		movement_->SetParams(params_.get());
		movement_->SetCharacter(this);
	}
	if (rotate_ == nullptr)
	{
		rotate_ = std::make_unique<CharacterRotate>(this);
		rotate_->SetParams(params_.get());
		rotate_->SetCharacter(this);
	}
	if (charge_ == nullptr)
	{
		charge_ = std::make_unique<CharacterCharge>(this);
		charge_->SetParams(params_.get());
		charge_->SetCharacter(this);
	}
	if (hitstop_ == nullptr)
	{
		hitstop_ = std::make_unique<CharacterHitStop>(this);
		hitstop_->SetParams(params_.get());
	}
	if (hit_ == nullptr)
	{
		hit_ = std::make_unique<CharacterHit>(this);
		hit_->SetParams(params_.get());
		hit_->SetCharacter(this);
	}
	if (fence_ == nullptr)
	{
		fence_ = std::make_unique<CharacterFence>(this);
		fence_->SetParams(params_.get());
		fence_->SetCharacter(this);
	}
	if (wait_ == nullptr)
	{
		wait_ = std::make_unique<CharacterWait>(this);
		wait_->SetParams(params_.get());
	}
	if(csvload_ == nullptr)
	{
		csvload_ = std::make_unique<CharacterCsvLoader>(this);
		csvload_->SetParams(params_.get());
		csvload_->SetCharacter(this);
	}
	if (observer_ == nullptr)
	{
		observer_ = std::make_unique<CharacterObserver>(this);
		observer_->SetParams(params_.get());
	}
	if (debugpanel_ == nullptr)
	{
		debugpanel_ = std::make_unique<CharacterDebugPanel>(this);
		debugpanel_->SetParams(params_.get());
		debugpanel_->SetCharacter(this);
	}

	//csvからパラメータ読み込み
	vfx_->InitCSVEffect();

	//サウンドの読み込み
	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string SoundSE = "Sound\\SE\\";

	params_->SoundParam_.hSoundcharge_ = Audio::Load(SoundSE + "Charge.wav",false, Audio::GetChargeNum());
	assert(params_->SoundParam_.hSoundcharge_ >= 0);

	params_->SoundParam_.hSoundattack_ = Audio::Load(SoundSE + "Attack.wav", false, Audio::GetAttackNum());
	assert(params_->SoundParam_.hSoundattack_ >= 0);

	params_->SoundParam_.hSoundCollision_ = Audio::Load(SoundSE + "Collision.wav",false, Audio::GetCollisionNum());
	assert(params_->SoundParam_.hSoundCollision_ >= 0);

	params_->SoundParam_.hSoundJump_ = Audio::Load(SoundSE + "Jump.wav", false, Audio::GetJumpNum());
	assert(params_->SoundParam_.hSoundJump_ >= 0);

	//それぞれの柵の法線を取得
	fence_->GetWireNormal();

	//影モデルを初期化
	shadow_->InitShadow();
}

Character::~Character()
{
	//実体は消さず、アドレスのみ無効化する
	//スマートポインタにし、params_解放前にobserver配列から削除し
	//その後自動で解放されるようにする
	for (auto& observer : params_->InitParam_.observers)
	{
		if(observer != nullptr)
		{
			//監視対象の配列から削除
			//通知者(Character)→監視者(BattleScene)の順に解放されるので、
			//CharacterのデストラクタでRemoveObserverを呼ぶ
			observer_->RemoveObserver(observer);
		}
	}
}

void Character::Update()
{
	//Characterクラスを継承するクラスで呼ぶ共通更新
	//この関数は明示的に呼び出す(継承先でもUpdateはoverrideされるため)

	//毎フレーム影の位置を更新
	shadow_->ShadowSet();

	//毎フレーム重力をかけ続ける
	air_->CharacterGravity();

	//自分の前方ベクトル(回転した分も含む)を更新
	forward_->FrontVectorConfirm();
}

void Character::Draw()
{
	//Characterクラスを継承するクラスで呼ぶ共通描画

	//キャラクターの丸影を描画
	shadow_->ShadowDraw();
}

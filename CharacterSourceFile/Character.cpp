#include "Character.h"
#include"../Engine/VFX.h"
#include"../Engine/Model.h"
#include"../Engine/Audio.h"
#include"../Engine/CsvReader.h"

Character::Character(GameObject* parent)
	:GameObject(parent,"Character"),
	params_(nullptr), blink_(nullptr), vfx_(nullptr), shadow_(nullptr), air_(nullptr),
	forward_(nullptr), movement_(nullptr),rotate_(nullptr),charge_(nullptr),
    hit_(nullptr),fence_(nullptr),csvload_(nullptr),observer_(nullptr),debugpanel_(nullptr)
{
}

Character::Character(GameObject* parent, const std::string& name)
	:GameObject(parent, name),
	params_(nullptr), blink_(nullptr), vfx_(nullptr), shadow_(nullptr), air_(nullptr),
	forward_(nullptr), movement_(nullptr), rotate_(nullptr), charge_(nullptr),
	hit_(nullptr), fence_(nullptr), csvload_(nullptr), observer_(nullptr), debugpanel_(nullptr)
{
	//各モジュールの生成・初期化(Instantiate)
	//必要に応じてパラメータのセット(SetParams)
	//必要に応じて親クラス（自身）のセット(SetCharacter)
	//必要に応じてイベントリスナーのセット(SetEventListener)

	if (params_ == nullptr)
	{
		params_ = Instantiate<CharacterParams>(this);
	}

	if (blink_ == nullptr)
	{
		blink_ = Instantiate<CharacterModelBlink>(this);
		blink_->SetParams(params_);
	}
	if (vfx_ == nullptr)
	{
		vfx_ = Instantiate<CharacterVFX>(this);
		vfx_->SetParams(params_);
		vfx_->SetCharacter(this);
	}
	if (shadow_ == nullptr)
	{
		shadow_ = Instantiate<CharacterShadow>(this);
		shadow_->SetParams(params_);
		shadow_->SetCharacter(this);
	}
	if (air_ == nullptr)
	{
		air_ = Instantiate<CharacterAir>(this);
		air_->SetParams(params_);
		air_->SetCharacter(this);
	}
	if (forward_ == nullptr)
	{
		forward_ = Instantiate<CharacterForward>(this);
		forward_->SetParams(params_);
		forward_->SetCharacter(this);
	}
	if (movement_ == nullptr)
	{
		movement_ = Instantiate<CharacterMovement>(this);
		movement_->SetParams(params_);
		movement_->SetCharacter(this);
	}
	if (rotate_ == nullptr)
	{
		rotate_ = Instantiate<CharacterRotate>(this);
		rotate_->SetParams(params_);
		rotate_->SetCharacter(this);
	}
	if (charge_ == nullptr)
	{
		charge_ = Instantiate<CharacterCharge>(this);
		charge_->SetParams(params_);
		charge_->SetCharacter(this);
		charge_->SetEventListener(this);
	}
	if (hit_ == nullptr)
	{
		hit_ = Instantiate<CharacterHit>(this);
		hit_->SetParams(params_);
		hit_->SetCharacter(this);
		hit_->SetEventListener(this, this, this);
	}
	if (fence_ == nullptr)
	{
		fence_ = Instantiate<CharacterFence>(this);
		fence_->SetParams(params_);
		fence_->SetCharacter(this);
		fence_->SetEventListener(this, this, this);
	}
	if(csvload_ == nullptr)
	{
		csvload_ = Instantiate<CharacterCsvLoader>(this);
		csvload_->SetParams(params_);
		csvload_->SetCharacter(this);
	}
	if (observer_ == nullptr)
	{
		observer_ = Instantiate<CharacterObserver>(this);
		observer_->SetParams(params_);
	}
	if (debugpanel_ == nullptr)
	{
		debugpanel_ = Instantiate<CharacterDebugPanel>(this);
		debugpanel_->SetParams(params_);
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
	for (auto& observer : params_->InitParam_.observers)
	{
		if(observer != nullptr)
		{
			//監視対象の配列から削除
			//通知者(Character)→監視者(BattleScene)の順に解放されるので、
			//CharacterのデストラクタでRemoveObserverを呼ぶ
			observer_->RemoveObserver(observer);

			//安全策としてアドレスを無効化
			observer = nullptr;
		}
	}

	//影に使用する地面クラスのポインタを空にする
	if(params_->ShadowParam_.pGround_ != nullptr)
	{
		params_->ShadowParam_.pGround_ = nullptr;
	}
}

void Character::Update()
{
	//Characterクラスを継承するクラスで呼ぶ共通更新

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

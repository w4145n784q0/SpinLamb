#include "Character.h"
#include"../Engine/VFX.h"
#include"../Engine/Model.h"
#include"../Engine/Audio.h"
#include"../Engine/CsvReader.h"
#include"../StageSourceFile/UpperWire.h"
#include"../StageSourceFile/LowerWire.h"
#include"../StageSourceFile/LeftWire.h"
#include"../StageSourceFile/RightWire.h"


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


Character::Character(GameObject* parent)
	:GameObject(parent,"Character")
{
}

Character::Character(GameObject* parent, const std::string& name)
	:GameObject(parent, name)
{
	//csvからパラメータ読み込み
	InitCSVEffect();

	//サウンドの読み込み
	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string SoundSE = "Sound\\SE\\";

	hSoundcharge_ = Audio::Load(SoundSE + "Charge.wav",false, Audio::GetChargeNum());
	assert(hSoundcharge_ >= 0);

	hSoundattack_ = Audio::Load(SoundSE + "Attack.wav", false, Audio::GetAttackNum());
	assert(hSoundattack_ >= 0);

	hSoundCollision_ = Audio::Load(SoundSE + "Collision.wav",false, Audio::GetCollisionNum());
	assert(hSoundCollision_ >= 0);

	hSoundJump_ = Audio::Load(SoundSE + "Jump.wav", false, Audio::GetJumpNum());
	assert(hSoundJump_ >= 0);

	//それぞれの柵の法線を取得
	GetWireNormal();

	//影モデルを初期化
	InitShadow();
}

Character::~Character()
{
	//実体は消さず、アドレスのみ無効化する
	for (auto& observer : InitParam_.observers)
	{
		if(observer != nullptr)
		{
			//監視対象の配列から削除
			//通知者(Character)→監視者(BattleScene)の順に解放されるので、
			//CharacterのデストラクタでRemoveObserverを呼ぶ
			RemoveObserver(observer);

			//安全策としてアドレスを無効化
			observer = nullptr;
		}
	}

	if(ShadowParam_.pGround_ != nullptr)
	{
		ShadowParam_.pGround_ = nullptr;
	}
}

void Character::Update()
{
	//Characterクラスを継承するクラスで呼ぶ共通更新

	//毎フレーム影の位置を更新
	ShadowSet();

	//毎フレーム重力をかけ続ける
	CharacterGravity();

	//自分の前方ベクトル(回転した分も含む)を更新
	FrontVectorConfirm();
}

void Character::Draw()
{
	//Characterクラスを継承するクラスで呼ぶ共通描画

	ShadowDraw();
}

void Character::SetCSVStatus(std::string _path)
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
	SetTransformPRS(this->transform_,initData);

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

void Character::GetWireNormal()
{
	//各インスタンスから柵の法線を取得
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	assert(pUpperWire != nullptr);
	FenceHitParam_.UpperNormal_ = pUpperWire->GetNormal();

	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	assert(pLowerWire != nullptr);
	FenceHitParam_.LowerNormal_ = pLowerWire->GetNormal();

	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	assert(pRightWire != nullptr);
	FenceHitParam_.RightNormal_ = pRightWire->GetNormal();

	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	assert(pLeftWire != nullptr);
	FenceHitParam_.LeftNormal_ = pLeftWire->GetNormal();

	//取得した法線を配列に格納
	//これらが柵に接触した際の跳ね返る方向になる
	FenceHitParam_.NormalArray_ = { FenceHitParam_.UpperNormal_,  FenceHitParam_.LowerNormal_,
		 FenceHitParam_.RightNormal_, FenceHitParam_.LeftNormal_, };
}

void Character::InitArrow()
{
	//矢印モデルのトランスフォームを初期化
	MoveParam_.ArrowTransform_.position_ = { 0.0f,0.0f, 0.0f };
	MoveParam_.ArrowTransform_.rotate_ = MoveParam_.ArrowRotate_;
	MoveParam_.ArrowTransform_.scale_ = MoveParam_.ArrowScale_;

	//矢印モデル読み込み
	MoveParam_.hMoveArrow_ = Model::Load("Model\\AttackArrow.fbx");
	assert(MoveParam_.hMoveArrow_ >= 0);
}

void Character::AddObserver(IGameObserver* _observer)
{
	//監視者を配列に追加する
	InitParam_.observers.push_back(_observer);
}

void Character::RemoveObserver(IGameObserver* _observer)
{
	auto it = InitParam_.observers.begin();

	while (it != InitParam_.observers.end())
	{
		if (*it == _observer)
		{
			//監視される対象の配列ループ内で、監視対象を見つけたら削除
			//イテレータ削除後は次の要素を参照する
			it = InitParam_.observers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Character::DrawCharacterModel(int _handle, Transform _transform)
{
	//無敵時間中かどうかでモデルの点滅表現を行う

	if (FenceHitParam_.IsInvincibility_)
	{
		//無敵時間中ならタイマーを使い、一定フレームおきにモデルを描画
		if (++FenceHitParam_.BlinkTimer_ > FenceHitParam_.BlinkValue_) {

			FenceHitParam_.BlinkTimer_ = 0;
			Model::SetAndDraw(_handle, _transform);
		}
	}
	else
	{
		//無敵時間でないなら通常の描画
		Model::SetAndDraw(_handle, _transform);
	}
}

void Character::DrawCharacterImGui()
{
	//ImGuiの描画
	//Slider:スライダーを使って値を調整可能
	//Input:値を直接入力 +-による調整も可能
	//Text:記述のみ

	//キャラクターの位置(position_.x,y,z)
	if (ImGui::TreeNode("Transform.Position"))
	{
		ImGui::SliderFloat("PositionX", &this->transform_.position_.x, WestEnd_, EastEnd_);
		ImGui::SliderFloat("PositionY", &this->transform_.position_.y, JumpParam_.HeightLowerLimit_,
			JumpParam_.HeightUpperLimit_);
		ImGui::SliderFloat("PositionZ", &this->transform_.position_.z, SouthEnd_, NorthEnd_);
		ImGui::TreePop();
	}

	//キャラクターの回転量(rotate_.x,y,z)
	if (ImGui::TreeNode("Transform.Rotate"))
	{
		ImGui::InputFloat("RotateX", &this->transform_.rotate_.x, ZeroPointOne);
		ImGui::InputFloat("RotateY", &this->transform_.rotate_.y, ZeroPointOne);
		ImGui::InputFloat("RotateZ", &this->transform_.rotate_.z, ZeroPointOne);
		ImGui::TreePop();
	}

	//ワールド正面ベクトル
	if (ImGui::TreeNode("Forward"))
	{
		XMFLOAT3 tmp;
		XMStoreFloat3(&tmp, MoveParam_.ForwardVector_);
		ImGui::Text("ForwardX:%.3f", tmp.x);
		ImGui::Text("ForwardY:%.3f", tmp.y);
		ImGui::Text("ForwardZ:%.3f", tmp.z);
		ImGui::TreePop();
	}

	//キャラクターID,初期位置
	if (ImGui::TreeNode("Initialize"))
	{
		ImGui::Text("CharacterID:%.1d", InitParam_.CharacterID);
		ImGui::Text("ForwardX:%.3f", InitParam_.StartPosition_.x);
		ImGui::Text("ForwardY:%.3f", InitParam_.StartPosition_.y);
		ImGui::Text("ForwardZ:%.3f", InitParam_.StartPosition_.z);
		ImGui::TreePop();
	}

	//初速度,加速量,チャージの際に使う仮の加速度,1fごとの増加量,最大加速,摩擦係数,
	//移動方向、移動後の更新位置
	if (ImGui::TreeNode("Move"))
	{
		ImGui::InputFloat("Velocity", &this->MoveParam_.Velocity_, ZeroPointOne);
		ImGui::InputFloat("Acceleration", &this->MoveParam_.Acceleration_, ZeroPointOne);
		ImGui::InputFloat("TmpAccele", &this->MoveParam_.TmpAccele_, ZeroPointOne);
		ImGui::InputFloat("AcceleValue", &this->MoveParam_.AcceleValue_, ZeroPointOne);
		ImGui::InputFloat("FullAccelerate", &this->MoveParam_.FullAccelerate_, ZeroPointOne);
		ImGui::InputFloat("Friction", &this->MoveParam_.Friction_, ZeroPointOne);

		XMFLOAT3 move;
		XMStoreFloat3(&move, MoveParam_.MoveDirection_);
		ImGui::Text("MoveDirectionX:%.3f", move.x);
		ImGui::Text("MoveDirectionY:%.3f", move.y);
		ImGui::Text("MoveDirectionZ:%.3f", move.z);

		XMFLOAT3 NewPos;
		XMStoreFloat3(&NewPos, MoveParam_.NewPosition_);
		ImGui::Text("NewPositionX:%.3f", NewPos.x);
		ImGui::Text("NewPositionY:%.3f", NewPos.y);
		ImGui::Text("NewPositionZ:%.3f", NewPos.z);

		ImGui::TreePop();
	}

	//キャラクター移動時の通常の回転量、チャージ中の回転量
	if (ImGui::TreeNode("Rotate"))
	{
		ImGui::InputFloat("NormalRotate", &this->RotateParam_.MoveRotateX, ZeroPointOne);
		ImGui::InputFloat("FastRotate", &this->RotateParam_.FastRotateX, ZeroPointOne);
		ImGui::TreePop();
	}

	//キャラクターにかかる重力,キャラクターの上方向の力、ジャンプの高さ
	// 高さの上限,下限
	if (ImGui::TreeNode("Jump"))
	{
		ImGui::InputFloat("Gravity", &this->JumpParam_.Gravity_, ZeroPointOne);
		ImGui::InputFloat("JumpSpeed", &this->JumpParam_.JumpSpeed_, ZeroPointOne);
		ImGui::InputFloat("JumpHeight", &this->JumpParam_.JumpHeight, ZeroPointOne);
		ImGui::InputFloat("HeightLowerLimit", &this->JumpParam_.HeightLowerLimit_, ZeroPointOne);
		ImGui::InputFloat("HeightUpperLimit", &this->JumpParam_.HeightUpperLimit_, ZeroPointOne);
		ImGui::TreePop();
	}

	//ノックバック量計算時の変換前の範囲,変換後の範囲,減速率,ノックバックを終了する値
	//ノックバックする方向、ノックバックの速度
	if (ImGui::TreeNode("Hit"))
	{
		ImGui::InputFloat("OriginalRangeMin", &this->HitParam_.OriginalRangeMin_, ZeroPointOne);
		ImGui::InputFloat("OriginalRangeMax", &this->HitParam_.OriginalRangeMax_, ZeroPointOne);
		ImGui::InputFloat("ConvertedRangeMin", &this->HitParam_.ConvertedRangeMin_, ZeroPointOne);
		ImGui::InputFloat("ConvertedRangeMax", &this->HitParam_.ConvertedRangeMax_, ZeroPointOne);
		ImGui::InputFloat("DecelerationRate", &this->HitParam_.DecelerationRate_, ZeroPointOne);
		ImGui::InputFloat("KnockBackEnd", &this->HitParam_.KnockBackEnd_, ZeroPointOne);

		ImGui::Text("KnockBack_DirectionX:%.3f", HitParam_.KnockBack_Direction_.x);
		ImGui::Text("KnockBack_DirectionY:%.3f", HitParam_.KnockBack_Direction_.y);
		ImGui::Text("KnockBack_DirectionZ:%.3f", HitParam_.KnockBack_Direction_.z);

		ImGui::Text("KnockBack_VelocityX:%.3f", HitParam_.KnockBack_Velocity_.x);
		ImGui::Text("KnockBack_VelocityY:%.3f", HitParam_.KnockBack_Velocity_.y);
		ImGui::Text("KnockBack_VelocityZ:%.3f", HitParam_.KnockBack_Velocity_.z);

		ImGui::TreePop();
	}

	//柵ヒット時のノックバック量,無敵時間,無敵時間の終了する値
	// 無敵時間中の描画間隔,描画中に点滅表現をさせる値
	if (ImGui::TreeNode("FenceHit"))
	{
		ImGui::InputFloat("KnockBackPower", &this->FenceHitParam_.KnockBackPower_, ZeroPointOne);
		ImGui::InputInt("InvincibilityTime", &this->FenceHitParam_.InvincibilityTime_);
		ImGui::InputInt("InvincibilityValue", &this->FenceHitParam_.InvincibilityValue_);
		ImGui::InputInt("blinkTimer", &this->FenceHitParam_.BlinkTimer_);
		ImGui::InputInt("blinkValue", &this->FenceHitParam_.BlinkValue_);

		ImGui::TreePop();
	}

	//影の位置の補正
	if (ImGui::TreeNode("Shadow"))
	{
		ImGui::InputFloat("ShadowHeight", &this->ShadowParam_.ShadowHeight_, ZeroPointOne);
		ImGui::InputFloat("ShadowCorrection", &this->ShadowParam_.ShadowCorrection_, ZeroPointOne);
		ImGui::TreePop();
	}

	//矢印モデルの位置・回転・拡大率、位置の補正
	if (ImGui::TreeNode("Arrow"))
	{
		ImGui::InputFloat("ArrowTransform.PositionX", &this->MoveParam_.ArrowTransform_.position_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.PositionY", &this->MoveParam_.ArrowTransform_.position_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.PositionZ", &this->MoveParam_.ArrowTransform_.position_.z, ZeroPointOne);

		ImGui::InputFloat("ArrowTransform.RotateX", &this->MoveParam_.ArrowTransform_.rotate_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.RotateY", &this->MoveParam_.ArrowTransform_.rotate_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.RotateZ", &this->MoveParam_.ArrowTransform_.rotate_.z, ZeroPointOne);

		ImGui::InputFloat("ArrowTransform.ScaleX", &this->MoveParam_.ArrowTransform_.scale_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.ScaleY", &this->MoveParam_.ArrowTransform_.scale_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.ScaleZ", &this->MoveParam_.ArrowTransform_.scale_.z, ZeroPointOne);

		ImGui::InputFloat("AddArrowDepth", &this->MoveParam_.AddArrowDepth_, ZeroPointOne);

		ImGui::TreePop();
	}

	ImGui::TreePop();
}

void Character::CharacterGravity()
{
	//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	JumpParam_.JumpSpeed_ -= JumpParam_.Gravity_;

	//フィールドに乗っているかは関係なく重力はかかり続ける
	this->transform_.position_.y += JumpParam_.JumpSpeed_;

	//プレイヤーめりこみ防止に一定以下のy座標になったら値を固定する
	if (this->transform_.position_.y <= JumpParam_.HeightLowerLimit_)
	{
		this->transform_.position_.y = JumpParam_.HeightLowerLimit_;

		//一定以下のy座標になった場合,着地している判定にする
		JumpParam_.IsOnGround_ = true;
	}

	//マイナスし続けるので、念のためオーバーフロー防止
	//JumpSpeedが最低値以下になったら最低値の値で固定
	if (JumpParam_.JumpSpeed_ <= JumpParam_.MinusLimit_)
	{
		JumpParam_.JumpSpeed_ = JumpParam_.MinusLimit_;
	}
}

void Character::SetJump()
{
	//ジャンプを開始する処理

	//地上判定をfalseにする
	JumpParam_.IsOnGround_ = false;

	//一時的にy方向にマイナスされている値を大きくすることで、キャラクターが飛び上がる
	JumpParam_.JumpSpeed_ = JumpParam_.JumpHeight;

	//チャージ中のSE再生
	Audio::Play(hSoundJump_);
}

void Character::InitShadow()
{
	//初期化の時点でステージクラスのインスタンスを取得
	ShadowParam_.pGround_ = (Ground*)FindObject("Ground");
	assert(ShadowParam_.pGround_ != nullptr);

	//影モデル読み込み
	ShadowParam_.hShadow_ = Model::Load("Model\\ShadowPoint.fbx");
	assert(ShadowParam_.hShadow_ >= 0);
}

void Character::ShadowSet()
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
	this->ShadowParam_.ShadowTrans_.position_.x = this->transform_.position_.x;
	this->ShadowParam_.ShadowTrans_.position_.z = this->transform_.position_.z;

	//影の高さをトランスフォームに設定する
	this->ShadowParam_.ShadowTrans_.position_.y = ShadowParam_.ShadowHeight_;
}

void Character::ShadowDraw()
{
	//ShadowSetで位置を設定した影を描画
	Model::SetAndDraw(ShadowParam_.hShadow_, this->ShadowParam_.ShadowTrans_);
}

void Character::CharacterMove(XMVECTOR _direction)
{
	//受け取った方向が0ベクトルなら処理しない(0ベクトルを正規化はできない)
	if (XMVector3Equal(_direction, XMVectorZero()))
	{
		return;
	}

	//単位ベクトル化し、移動方向を確定
	MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	//移動ベクトルを作成
	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPosition_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void Character::CreateMoveVector()
{
	//移動ベクトル = 移動方向 * ((初速度 + 加速度) * 1fの移動量のスケーリング)
	//移動ベクトル化する
	XMVECTOR MoveVector = XMVectorScale(MoveParam_.MoveDirection_, (MoveParam_.Velocity_ + MoveParam_.Acceleration_) * DeltaTime);

	//現在位置と移動ベクトルを加算し
	//移動後のベクトルを作成(この時点では移動確定していない)
	XMVECTOR PrevPos = XMLoadFloat3(&this->transform_.position_);
	MoveParam_.NewPosition_ = PrevPos + MoveVector;
}

bool Character::IsOutsideStage(XMFLOAT3 _position)
{
	//指定位置が一つでもステージ端を超えるかどうか判定し、真偽を返す
	//ステージ外判定をする際に使用

	if (_position.x > EastEnd_ || _position.x < WestEnd_ || _position.z > NorthEnd_ || _position.z < SouthEnd_)
	{
			return true;
	}
	else 
	{
		return false;
	}
}

void Character::MoveConfirm()
{
	//移動後のベクトルをtransform_.positionに代入し、移動を確定する
	XMStoreFloat3(&this->transform_.position_, MoveParam_.NewPosition_);
}

float Character::RotateDirectionVector(XMVECTOR _MoveVector)
{
	//主にコントローラー・キーボードの入力に使う
	//受け取った方向ベクトルと前向きベクトルの外積求める
	XMVECTOR cross = XMVector3Cross(_MoveVector, InitParam_.FrontDirection_);

	//Y外積をとり+か-かで倒し回転方向を求める
	float crossY = XMVectorGetY(cross);

	//正面ベクトルとのラジアン角をとる
	XMVECTOR r = XMVector3AngleBetweenVectors(_MoveVector, InitParam_.FrontDirection_);

	//ラジアン角度を取得
	float angle = XMVectorGetX(r);

	//ディグリー角に直す
	float angleDeg = XMConvertToDegrees(angle);

	// crossYの正負に応じて回転角度の符号を変える
	if (crossY > 0)
	{
		angleDeg = -angleDeg;
	}

	return angleDeg;
}

void Character::Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity)
{
	//無敵状態なら処理しない
	if (this->FenceHitParam_.IsInvincibility_)
	{
		return;
	}

	//接触相手のベクトルから自身のベクトルを引く
	XMVECTOR subVector = XMVectorSubtract(_targetVector, _myVector);

	//引いたベクトルが0ならお互いの位置が同じなので処理しない(0ベクトルを正規化はできない)
	if (XMVector3Equal(subVector, XMVectorZero()))
	{
		return;
	}

	//単位ベクトルに変換
	subVector = XMVector3Normalize(subVector);
	
	//逆ベクトルにして反射方向を決定
	subVector = XMVectorNegate(subVector);
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp,subVector);

	//反射方向を設定
	HitParam_.KnockBack_Direction_ = tmp;

	//自身の速度と相手の速度の差分をとる
	//速度の差分に応じてノックバック量を変化させる
	float subVelocity = _myVelocity - _targetVelocity;

	//ノックバック量の初期化
	//毎回値が変化するのでローカル変数
	float KnockBackValue = 0.0f;

	//速度差の判定は線形補完元の最大値を適用
	//速度差が自身の方が一定以上なら、自身のノックバック量は0
	if (subVelocity >= HitParam_.OriginalRangeMax_)
	{
		KnockBackValue = 0.0f;
	}
	else 
	{
		//値がマイナスならプラスに変更
		if (signbit(subVelocity)) 
		{
			subVelocity = -subVelocity;
		}

		//ノックバック量の線形補完を行う
		KnockBackValue = LinearInterpolation(subVelocity,
			HitParam_.OriginalRangeMin_, HitParam_.OriginalRangeMax_,
			HitParam_.ConvertedRangeMin_, HitParam_.ConvertedRangeMax_);
	}

	//変換したノックバック量をノックバック時の速度x,zに代入
	HitParam_.KnockBack_Velocity_.x = KnockBackValue;
	HitParam_.KnockBack_Velocity_.z = KnockBackValue;

	//溜めている速度をリセット
	ChargeReset();

	//ノックバック時のY軸回転角の固定
	KnockBackAngleY(HitParam_.KnockBack_Direction_);
}

void Character::KnockBackAngleY(XMFLOAT3 _KnockBackVector)
{
	//ノックバック中のY軸の回転角を求める
	float angleY = static_cast<float>(atan2(_KnockBackVector.x, _KnockBackVector.z));

	//angleYはラジアン角なのでディグリー角に変換し、Y軸回転にセット
	this->transform_.rotate_.y = XMConvertToDegrees(angleY);
}

void Character::KnockBack()
{
	//x軸の+回転を行う
	MoveRotateX();

	//毎フレームノックバック速度を減少
	HitParam_.KnockBack_Velocity_.x *= HitParam_.DecelerationRate_;
	HitParam_.KnockBack_Velocity_.z *= HitParam_.DecelerationRate_;

	//ノックバック後の位置を計算
	//位置 = 位置 + 方向 * 速度
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += HitParam_.KnockBack_Direction_.x * HitParam_.KnockBack_Velocity_.x;
	TmpPos.z += HitParam_.KnockBack_Direction_.z * HitParam_.KnockBack_Velocity_.z;

	//この時点では変更せず、移動後の仮の位置に保管
	MoveParam_.NewPosition_ = XMLoadFloat3(&TmpPos);

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_. NewPosition_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

XMVECTOR Character::HitNormal(std::string _normal)
{
	
	//指定した名前の鉄線がWireArrayから見つかったら対応した法線を返す
	//返した方向が柵に接触した際のノックバック方向となる
	//見つからない場合は0を返す
	for (int i = 0; i < FenceHitParam_.WireArray_.size(); i++)
	{
		if (_normal == FenceHitParam_.WireArray_[i])
		{
			return FenceHitParam_.NormalArray_[i];
		}
	}

	return { 0,0,0 };
}

void Character::FenceReflect(XMVECTOR _normal)
{
	//接触エフェクトを指定
	SetFenceHitEffect();

	//溜めている速度をリセット
	ChargeReset();

	//ダッシュ中の速度リセット
	AccelerationStop();

	//念のため正規化する
	//反射方向が0なら処理しない(0ベクトルを正規化はできない)
	if (XMVector3Equal(_normal, XMVectorZero()))
	{
		return;
	}

	//単位ベクトルに変換
	_normal = XMVector3Normalize(_normal);

	//XMFLOAT3型に変換
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, _normal);

	//受け取った法線をノックバック方向に代入
	HitParam_.KnockBack_Direction_ = { tmp };

	//ノックバック量を速度に代入(一定値)
	HitParam_.KnockBack_Velocity_.x = FenceHitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = FenceHitParam_.KnockBackPower_;

	//無敵状態を設定
	FenceHitParam_.IsInvincibility_ = true;

	//接触通知
	NotifyFenceHit();

	//ノックバック時のY軸回転角の固定
	KnockBackAngleY(HitParam_.KnockBack_Direction_);
}

bool Character::IsKnockBackEnd()
{
	//ノックバック速度が終了値に到達したか判定し、真偽を返す

	if (HitParam_.KnockBack_Velocity_.x <= HitParam_.KnockBackEnd_ ||
		HitParam_.KnockBack_Velocity_.z <= HitParam_.KnockBackEnd_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Character::InvincibilityTimeCalculation()
{
	//無敵時間の計算

	//無敵時間かどうか判定(この関数は毎フレーム呼ばれるため)
	if (FenceHitParam_.IsInvincibility_)
	{
		//タイマーが終了値を超えたら無敵時間を終わる
		if (++FenceHitParam_.InvincibilityTime_ > FenceHitParam_.InvincibilityValue_)
		{
			FenceHitParam_.InvincibilityTime_ = 0;
			FenceHitParam_.IsInvincibility_ = false;
		}
	}
}

void Character::NotifyFenceHit()
{
	//通知を受け取る側がoverrideしていなかった場合は何も起こらない
	//BattleSceneでのみ有効(監視対象がない=AddObserverを呼ばない場合、
	//InitParam_.observers自体が空なのでfor文がスルーされる)
	for (IGameObserver* observer : InitParam_.observers) 
	{

		//監視者へ柵にヒットしたこと（当たったCharacterのID）を通知
		observer->OnCharacterFenceHit(this->InitParam_.CharacterID);
	}
}

void Character::Charging()
{
	//チャージ中のSE再生
	Audio::Play(hSoundcharge_);

	//チャージ中,仮の値に一定の加速量を加算し続ける
	//チャージ解放時に実際にAcceleration_に代入する
	if (MoveParam_.TmpAccele_ < MoveParam_.FullAccelerate_)
	{
		MoveParam_.TmpAccele_ += MoveParam_.AcceleValue_;
	}
	else
	{
		SetFullChargeEffect();
		MoveParam_.TmpAccele_ = MoveParam_.FullAccelerate_;
	}
}

void Character::ChargeRelease()
{
	//実際の加速度に溜めた仮の値を代入
	MoveParam_.Acceleration_ = MoveParam_.TmpAccele_;

	//溜めている速度をリセット
	ChargeReset();
}

void Character::ChargeReset()
{
	MoveParam_.TmpAccele_ = 0.0f;
}

void Character::SetArrow()
{
	//正面ベクトルに前方向の調整値を乗算し、矢印の正面位置を計算
	XMVECTOR FrontArrow = XMVectorScale(this->MoveParam_.ForwardVector_, this->MoveParam_.AddArrowDepth_);
	
	//現在位置を取得
	XMVECTOR PosVec = XMLoadFloat3(&this->transform_.position_);

	//矢印の正面位置と現在位置を加算
	XMVECTOR arrowPosVec = XMVectorAdd(PosVec, FrontArrow);

	//矢印のトランスフォームに代入
	XMStoreFloat3(&this->MoveParam_.ArrowTransform_.position_, arrowPosVec);
}

void Character::DrawArrow()
{
	//矢印モデルを描画
	Model::SetAndDraw(MoveParam_.hMoveArrow_, this->MoveParam_.ArrowTransform_);
}

XMVECTOR Character::RotateVecFront(float _rotY, XMVECTOR _front)
{
	//回転させたいベクトル（方向）を代入
	//基本はローカル正面ベクトル
	XMVECTOR v = _front;

	//Y回転をラジアンにし、回転行列を作成
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(_rotY));

	//方向ベクトルを回転行列で変換し、ワールド座標での向いている方向が出る
	v = XMVector3TransformCoord(v, m);

	return v;
}

void Character::FrontVectorConfirm()
{
	//ローカル正面ベクトルを現在のy軸回転量で変形すると、正面からどれだけ回転したかが計算される
	//その値がワールド正面ベクトルとなる
	MoveParam_.ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, InitParam_.FrontDirection_);
}

void Character::InitCSVEffect()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVData\\EffectData\\VFXData.csv");

	//csvファイルの各0列目の文字列の配列を取得
	std::string Effects[] = { "Charge","FullCharge" ,"Locus" , "Hit" , "FenceHit" };
  
	//ChargeParam_から始まるVFXのパラメータ(vector<float>型の配列)の参照を
	//ポインタ配列に格納
	std::vector<float>* Param[] = { &ChargeParam_,&FullChargeParam,
		&AttackLocusParam_, &HitEffectParam_, &FenceHitEffectParam_ };  


    for (int i = 0; i < sizeof(Effects) / sizeof(Effects[0]); i++)  
    {  
		//0列目の文字列を渡し、その行のパラメータを取得
		std::vector<float> v = GetCSVReadData(csv, Effects[i]);

		//この時点では代入のみ行われる
		// SetEmitterで実際にVFXのパラメータにセットされる 
		*Param[i] = v;  
            
    }
}

void Character::SetChargingEffect(std::string _path)
{
	//csvから読み込んだ,チャージ中エフェクトのパラメータを実際にセットする
	EmitterData charge;
	VFX::SetEmitter(charge, ChargeParam_);

	//使用する画像のパスをセットする
	charge.textureFileName = _path;

	//発射位置をセット
	charge.position = this->transform_.position_;

	//エフェクトを開始
	VFX::Start(charge);
}

void Character::SetFullChargeEffect()
{
	//csvから読み込んだ,最大チャージエフェクトのパラメータを実際にセットする
	EmitterData fullcharge;
	VFX::SetEmitter(fullcharge, FullChargeParam);

	//使用する画像のパスをセットする
	fullcharge.textureFileName = "ParticleAssets\\circle_W.png";

	//発射位置をセット
	fullcharge.position = this->transform_.position_;

	//エフェクトを開始
	VFX::Start(fullcharge);
}

void Character::SetAttackLocusEffect()
{
	//csvから読み込んだ,攻撃中の軌跡エフェクトのパラメータを実際にセットする
	EmitterData locus;
	VFX::SetEmitter(locus, AttackLocusParam_);

	//使用する画像のパスをセットする
	locus.textureFileName = "ParticleAssets\\flashB_Y.png";

	//発射位置をセット
	locus.position = this->transform_.position_;

	//エフェクトを開始
	VFX::Start(locus);
}

void Character::SetHitEffect()
{
	//csvから読み込んだ,被弾エフェクトのパラメータを実際にセットする
	EmitterData hit;
	VFX::SetEmitter(hit, HitEffectParam_);

	//使用する画像のパスをセットする
	hit.textureFileName = "ParticleAssets\\flashB_W.png";

	//発射位置をセット
	hit.position = this->transform_.position_;

	//エフェクトを開始
	VFX::Start(hit);
}

void Character::SetFenceHitEffect()
{
	//csvから読み込んだ,柵に接触時エフェクトのパラメータを実際にセットする
	EmitterData  fencehit;
	VFX::SetEmitter(fencehit, FenceHitEffectParam_);

	//使用する画像のパスをセットする
	fencehit.textureFileName = "ParticleAssets\\flashB_W.png";

	//発射位置をセット
	fencehit.position = this->transform_.position_;

	//エフェクトを開始
	VFX::Start(fencehit);
}

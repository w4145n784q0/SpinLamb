#include "Character.h"
#include"Engine/VFX.h"
#include"Engine/Model.h"
#include"Engine/Audio.h"
#include"Engine/Global.h"

namespace {
	
	//キャラクタークラスの共通変数を初期化するインデックス
	//初期化関係
	enum InitializeIndex
	{
		i_posX = 0,
		i_posY,
		i_posZ,
		i_rotateX,
		i_rotateY,
		i_rotateZ,
		i_scaleX,
		i_scaleY,
		i_scaleZ,
		i_frontX,
		i_frontY,
		i_frontZ,
	};

	//移動関係
	enum moveIndex
	{
		i_vel = 0,
		i_accele_value,
		i_accele_max,
		i_friction,
		i_arrowrotateX,
		i_arrowrotateY,
		i_arrowrotateZ,
		i_arrowscaleX,
		i_arrowscaleY,
		i_arrowscaleZ,
		i_addarrowdepth
	};

	//回転関係
	enum RotateIndex
	{
		i_moverot = 0,
		i_fastrot ,
	};

	//空中関係
	enum JumpIndex
	{
		i_gravity = 0,
		i_upperlimit,
		i_lowerlimit,
	};

	//被弾関係
	enum HitIndex
	{
		i_collider = 0,
		i_originalrangemin,
		i_originalrangemax,
		i_convertedrangemin,
		i_convertedrangemax,

		i_deceleration,
		i_knockbackend,
	};

	//柵に接触関係
	enum WallHitIndex
	{
		i_knockbackpower = 0,
		i_invincibilityvalue,
		i_blinkvalue,

	};

	//影付け関係
	enum ShadowIndex
	{
		i_shadowcorrection = 0,
	};


	//エフェクト初期化時のインデックス
	enum EffectParamIndex
	{
		i_Charge = 0,
		i_Locus,
		i_Hit,
		i_WallHit,
	};

	//サウンド初期化時のインデックス
	enum SoundParamIndex
	{
		i_chargecount = 0,
		i_attackcount,
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
	InitCSVSound();

	//各画像・サウンドの読み込み
	hSoundcharge_ = Audio::Load("Sound\\SE\\charge.wav",false, ChargeSoundCount_);
	assert(hSoundcharge_ >= 0);
	hSoundattack_ = Audio::Load("Sound\\SE\\attack.wav", false, AttackSoundCount_);
	assert(hSoundattack_ >= 0);
	hSoundCollision_ = Audio::Load("Sound\\SE\\collision.wav",false);
	assert(hSoundCollision_ >= 0);

	//それぞれの柵の法線を取得
	GetWireNormal();

	//影モデルを初期化
	InitShadow();
}

Character::~Character()
{
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

void Character::SetcsvStatus(std::string _path)
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load(_path);

	//--------------------初期化関係のパラメータ(自身のtransform_)--------------------

	//csvファイルの0列目の文字列を取得
	std::string p_init = "InitializeParam";

	//指定した文字列がいずれかの0列目に存在したら
	if (csv.IsGetParamName(p_init))
	{
		//その行を配列として全取得
		std::vector<float> v = csv.GetParam(p_init);

		//自身のトランスフォームを初期化
		SetTransformPRS(this->transform_,v);
		InitParam_.FrontDirection_ = {v[i_frontX],v[i_frontY],v[i_frontZ]};

		//初期位置を保管する
		InitParam_.StartPosition_ = this->transform_.position_;
	}

	//--------------------移動関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得
	std::string p_move = "MoveParam";

	//指定した文字列がいずれかの0列目に存在したら
	if (csv.IsGetParamName(p_move))
	{
		//その行を配列として全取得
		std::vector<float> v = csv.GetParam(p_move);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		MoveParam_.Velocity_ = v[i_vel];
		MoveParam_.AcceleValue_ = v[i_accele_value];
		MoveParam_.FullAccelerate_ = v[i_accele_max];
		MoveParam_.Friction_ = v[i_friction];
		MoveParam_.ArrowRotate_ = { v[i_arrowrotateX],v[i_arrowrotateY],v[i_arrowrotateZ] };
		MoveParam_.ArrowScale_ = { v[i_arrowscaleX],v[i_arrowscaleY],v[i_arrowscaleZ] };
		MoveParam_.AddArrowDepth_ = v[i_addarrowdepth];
	}

	//--------------------回転関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得
	std::string p_rotate = "RotateParam";

	//指定した文字列がいずれかの0列目に存在したら
	if (csv.IsGetParamName(p_rotate))
	{
		//その行を配列として全取得
		std::vector<float> v = csv.GetParam(p_rotate);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		RotateParam_.MoveRotateX = v[i_moverot];
		RotateParam_.FastRotateX = v[i_fastrot];

	}

	//--------------------空中関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_jump = "JumpParam";

	//指定した文字列がいずれかの0列目に存在したら
	if (csv.IsGetParamName(p_jump))
	{
		//その行を配列として全取得
		std::vector<float> v = csv.GetParam(p_jump);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		JumpParam_.Gravity_ = v[i_gravity];
		JumpParam_.HeightLowerLimit_ = v[i_upperlimit];
		JumpParam_.HeightUpperLimit_ = v[i_lowerlimit];
	}

	//--------------------被弾関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_hit = "HitParam";

	//指定した文字列がいずれかの0列目に存在したら
	if (csv.IsGetParamName(p_hit))
	{
		//その行を配列として全取得
		std::vector<float> v = csv.GetParam(p_hit);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		HitParam_.ColliderSize_ =v[i_collider];
		HitParam_.OriginaRangeMin_ = v[i_originalrangemin];
		HitParam_.OriginaRangeMax_ = v[i_originalrangemax];
		HitParam_.ConvertedRangeMin_ = v[i_convertedrangemin];
		HitParam_.ConvertedRangeMax_ = v[i_convertedrangemax];
		HitParam_.DecelerationRate_ = v[i_deceleration];
		HitParam_.KnockBackEnd_ = v[i_knockbackend];
	}

	//--------------------柵に接触関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_wallhit = "WallHitParam";

	//指定した文字列がいずれかの0列目に存在したら
	if (csv.IsGetParamName(p_wallhit))
	{
		//その行を配列として全取得
		std::vector<float> v = csv.GetParam(p_wallhit);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		WallHitParam_.KnockBackPower_ = v[i_knockbackpower];
		WallHitParam_.InvincibilityValue_ = static_cast<int>(v[i_invincibilityvalue]);
		WallHitParam_.blinkValue_ = static_cast<int>(v[i_blinkvalue]);
	}


	//--------------------影関係のパラメータ--------------------

	//csvファイルの0列目の文字列を取得	
	std::string p_shadow = "ShadowParam";

	//指定した文字列がいずれかの0列目に存在したら
	if(csv.IsGetParamName(p_shadow))
	{
		//その行を配列として全取得
		std::vector<float> v = csv.GetParam(p_shadow);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		ShadowParam_.ShadowCorrection_ = v[i_shadowcorrection];
	}
}

void Character::GetWireNormal()
{
	//各インスタンスから柵の法線を取得
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	WallHitParam_.UpperNormal_ = pUpperWire->GetNormal();

	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	WallHitParam_.LowerNormal_ = pLowerWire->GetNormal();

	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	WallHitParam_.RightNormal_ = pRightWire->GetNormal();

	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	WallHitParam_.LeftNormal_ = pLeftWire->GetNormal();

	//取得した法線を配列に格納
	//これらが柵に接触した際の跳ね返る方向になる
	WallHitParam_.NormalArray_ = { WallHitParam_.UpperNormal_,  WallHitParam_.LowerNormal_,
		 WallHitParam_.RightNormal_, WallHitParam_.LeftNormal_, };
}

void Character::InitArrow()
{
	MoveParam_.ArrowTransform_.position_ = { 0.0f,0.0f, 0.0f };
	MoveParam_.ArrowTransform_.rotate_ = MoveParam_.ArrowRotate_;
	MoveParam_.ArrowTransform_.scale_ = MoveParam_.ArrowScale_;
}

void Character::DrawCharacterModel(int _handle, Transform _transform)
{
	//無敵時間中かどうかでモデルの点滅表現を行う

	if (WallHitParam_.IsInvincibility_)
	{
		//無敵時間中ならタイマーを使い、一定フレームおきにモデルを描画
		if (++WallHitParam_.blinkTimer_ > WallHitParam_.blinkValue_) {

			WallHitParam_.blinkTimer_ = 0;
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
	//Input:値を直接入力
	//Text:記述のみ

	//キャラクターの位置(position_.x,y,z)
	if (ImGui::TreeNode("Transform.Position"))
	{
		ImGui::SliderFloat("PositionX:%.3f", &this->transform_.position_.x, WestEnd_, EastEnd_);
		ImGui::SliderFloat("PositionZ:%.3f", &this->transform_.position_.z, SouthEnd_, NorthEnd_);
		ImGui::TreePop();
	}

	//キャラクターの回転量(rotate_.x,y,z)
	if (ImGui::TreeNode("Transform.Rotate"))
	{
		ImGui::Text("RotateX:%.3f", &this->transform_.rotate_.x);
		ImGui::Text("RotateY:%.3f", &this->transform_.rotate_.y);
		ImGui::Text("RotateZ:%.3f", &this->transform_.rotate_.z);
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

	//初速度,加速量,最大加速,チャージの際に使う仮の加速度
	if (ImGui::TreeNode("Move"))
	{
		ImGui::InputFloat("velocity", &this->MoveParam_.Velocity_);
		ImGui::InputFloat("AcceleValue", &this->MoveParam_.AcceleValue_);
		ImGui::InputFloat("FullAccelerate", &this->MoveParam_.FullAccelerate_);
		ImGui::InputFloat("TmpAccele", &this->MoveParam_.TmpAccele_);
		ImGui::TreePop();
	}

	//キャラクター移動時の通常の回転量、チャージ中の回転量
	if (ImGui::TreeNode("Rotate"))
	{
		ImGui::InputFloat("normalRotate", &this->RotateParam_.MoveRotateX);
		ImGui::InputFloat("fastRotate", &this->RotateParam_.FastRotateX);
		ImGui::TreePop();
	}

	//キャラクターにかかる重力,高さの上限,下限
	if (ImGui::TreeNode("Jump"))
	{
		ImGui::InputFloat("Gravity", &this->JumpParam_.Gravity_);
		ImGui::InputFloat("HeightLowerLimit", &this->JumpParam_.HeightLowerLimit_);
		ImGui::InputFloat("HeightUpperLimit", &this->JumpParam_.HeightUpperLimit_);
		ImGui::TreePop();
	}

	//ノックバック量計算時の変換前の範囲,変換後の範囲,減速率,ノックバックを終了する値
	if (ImGui::TreeNode("Hit"))
	{
		ImGui::InputFloat("OriginaRangeMin", &this->HitParam_.OriginaRangeMin_);
		ImGui::InputFloat("OriginaRangeMax", &this->HitParam_.OriginaRangeMax_);
		ImGui::InputFloat("ConvertedRangeMin", &this->HitParam_.ConvertedRangeMin_);
		ImGui::InputFloat("ConvertedRangeMax", &this->HitParam_.ConvertedRangeMax_);
		ImGui::InputFloat("DecelerationRate", &this->HitParam_.DecelerationRate_);
		ImGui::InputFloat("KnockBackEnd", &this->HitParam_.KnockBackEnd_);
		ImGui::TreePop();
	}

	//柵ヒット時のノックバック量,無敵時間,無敵時間中の描画間隔
	if (ImGui::TreeNode("WallHit"))
	{
		ImGui::InputFloat("Collider", &this->WallHitParam_.KnockBackPower_);
		ImGui::InputInt("InvincibilityTime", &this->WallHitParam_.InvincibilityValue_);
		ImGui::InputInt("blinkValue", &this->WallHitParam_.blinkValue_);
		ImGui::TreePop();
	}

	//影の位置の補正
	if (ImGui::TreeNode("Shadow"))
	{
		ImGui::InputFloat("Collider", &this->ShadowParam_.ShadowCorrection_);
		ImGui::TreePop();
	}

	ImGui::TreePop();
}

void Character::CharacterGravity()
{
	//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	JumpParam_. JumpSpeed_ -= JumpParam_.Gravity_;

	//フィールドに乗っているかは関係なく重力はかかり続ける
	this->transform_.position_.y += JumpParam_.JumpSpeed_;

	//プレイヤーめりこみ防止に一定以下のy座標になったら値を固定する
	if (this->transform_.position_.y <= JumpParam_.HeightLowerLimit_)
	{
		this->transform_.position_.y = JumpParam_.HeightLowerLimit_;

		//一定以下のy座標になった場合,着地している判定にする
		JumpParam_.IsOnGround_ = true;
	}
}

void Character::InitShadow()
{
	//初期化の時点でステージクラスのインスタンスを取得
	ShadowParam_.pGround_ = (Ground*)FindObject("Ground");

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

	//レイの方向を設定
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
	//単位ベクトル化し、移動方向を確定
	MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	//移動ベクトルを作成
	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPositon_);
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
	MoveParam_.NewPositon_ = PrevPos + MoveVector;
}

bool Character::IsOutsideStage(XMFLOAT3 _position)
{
	//指定位置が一つでもステージ端を超えるかどうか判定し、真偽を返す
	//ステージ外判定をする際に使用

	if (_position.x > EastEnd_ || _position.x < WestEnd_ || _position.z > NorthEnd_ || _position.z < SouthEnd_){
			return true;
	}
	else {
		return false;
	}
}

void Character::MoveConfirm()
{
	//移動後のベクトルをtransform_.positionに代入し、移動を確定する
	XMStoreFloat3(&this->transform_.position_, MoveParam_.NewPositon_);
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

void Character::Reflect(XMVECTOR myVector, XMVECTOR eVector, float myVelocity, float eVelocity)
{
	//無敵状態なら処理しない
	if (this->WallHitParam_.IsInvincibility_)
	{
		return;
	}

	//接触相手のベクトルから自身のベクトルを引き、正規化
	XMVECTOR subVector = XMVector3Normalize(XMVectorSubtract(eVector ,myVector));
	
	//逆ベクトルにして反射方向を決定
	subVector = XMVectorNegate(subVector);
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp,subVector);

	//反射方向を設定
	 HitParam_.KnockBack_Direction_ = tmp;

	//自身の速度と相手の速度の差分をとる
	float subVelocity = myVelocity - eVelocity;

	//ノックバック量の初期化
	//値が変化するのでローカル変数
	float KnockBackValue = 0.0f;

	//速度差の判定は線形補完元の最大値を適用
	//速度差が自身の方が一定以上なら、自身のノックバック量は0
	if (subVelocity >= HitParam_.OriginaRangeMax_)
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
		KnockBackValue = LinearCompletion(subVelocity,
			HitParam_.OriginaRangeMin_, HitParam_.OriginaRangeMax_,
			HitParam_.ConvertedRangeMin_, HitParam_.ConvertedRangeMax_);
	}

	//変換したノックバック量をノックバック時の速度x,zに代入
	HitParam_.KnockBack_Velocity_.x = KnockBackValue;
	HitParam_.KnockBack_Velocity_.z = KnockBackValue;

	//溜めている速度をリセット
	ChargeReset();

	//ダッシュ中の速度リセット
	AccelerationStop();
}

void Character::KnockBack()
{
	MoveRotateReverse();

	//毎フレームノックバック速度を減少
	HitParam_.KnockBack_Velocity_.x *= HitParam_.DecelerationRate_;
	HitParam_.KnockBack_Velocity_.z *= HitParam_.DecelerationRate_;

	//ノックバック後の位置を計算
	//位置 = 位置 + 方向 * 速度
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += HitParam_.KnockBack_Direction_.x * HitParam_.KnockBack_Velocity_.x;
	TmpPos.z += HitParam_.KnockBack_Direction_.z * HitParam_.KnockBack_Velocity_.z;

	//この時点では変更せず、移動後の仮の位置に保管
	MoveParam_.NewPositon_ = XMLoadFloat3(&TmpPos);

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_. NewPositon_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

XMVECTOR Character::HitNormal(std::string _normal)
{
	//wallArray[] = { "UpperWire", "LowerWire", "RightWire" ,"LeftWire" };
	
	//指定した名前の鉄線がWallArrayから見つかったら対応した法線を返す
	//返した方向が柵に接触した際のノックバック方向となる
	//見つからない場合は0を返す
	for (int i = 0; i < WallHitParam_.WireArray_.size(); i++)
	{
		if (_normal == WallHitParam_.WireArray_[i])
		{
			return WallHitParam_.NormalArray_[i];
		}
	}

	return { 0,0,0 };
}

void Character::WallReflect(XMVECTOR normal)
{
	//接触エフェクトを指定
	SetWallHitEffect();

	//溜めている速度をリセット
	ChargeReset();

	//ダッシュ中の速度リセット
	AccelerationStop();

	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, normal);

	//受け取った法線をノックバック方向に代入
	HitParam_.KnockBack_Direction_ = { tmp };

	//ノックバック量を速度に代入(一定値)
	HitParam_.KnockBack_Velocity_.x = WallHitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = WallHitParam_.KnockBackPower_;

	//無敵状態を設定
	WallHitParam_.IsInvincibility_ = true;

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

void Character::InvincibilityTimeCalclation()
{
	//無敵時間の計算

	//無敵時間かどうか判定(この関数は毎フレーム呼ばれるため)
	if (WallHitParam_.IsInvincibility_)
	{
		//タイマーが終了値を超えたら無敵時間を終わる
		if (++WallHitParam_.InvincibilityTime_ > WallHitParam_.InvincibilityValue_)
		{
			WallHitParam_.InvincibilityTime_ = 0;
			WallHitParam_.IsInvincibility_ = false;
		}
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
		//SetFullChargeEffect();
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
	XMVECTOR frontArrow = XMVectorScale(this->MoveParam_.ForwardVector_, this->MoveParam_.AddArrowDepth_);
	
	//現在位置を取得
	XMVECTOR PosVec = XMLoadFloat3(&this->transform_.position_);

	//矢印の正面位置と現在位置を加算
	XMVECTOR arrowPosVec = XMVectorAdd(PosVec, frontArrow);

	//矢印のトランスフォームに代入
	XMStoreFloat3(&this->MoveParam_.ArrowTransform_.position_, arrowPosVec);
}

XMVECTOR Character::RotateVecFront(float rotY, XMVECTOR front)
{
	//回転させたいベクトル（方向）を代入
	//基本はローカル正面ベクトル
	XMVECTOR v = front;

	//Y回転をラジアンにし、回転行列を作成
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(rotY));

	//方向ベクトルを回転行列で変換し、ワールド座標での向いている方向が出る
	v = XMVector3TransformCoord(v, m);

	return v;
}

void Character::InitCSVEffect()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVData\\EffectData\\VFXData.csv");

	//csvファイルの各0列目の文字列の配列を取得

	std::string effects[] = { "Charge","FullCharge" ,"Locus" , "Hit" , "WallHit" };
  
	//ChargeParam_から始まるVFXのパラメータ(vector<float>型の配列)の参照を
	//ポインタ配列に格納
	std::vector<float>* param[] = { &ChargeParam_,&FullChargeParam,
		&AttackLocusParam_, &HitEffectParam_, &WallHitEffectParam_ };  


    for (int i = 0; i < sizeof(effects) / sizeof(effects[0]); i++)  
    {  
		//指定した文字列がいずれかの0列目に存在したら
		if (csv.IsGetParamName(effects[i]))
        {  
			//その行を配列として全取得
			std::vector<float> v = csv.GetParam(effects[i]);  

			//この時点では代入のみ行われる
			// SetEmitterで実際にVFXのパラメータにセットされる 
			*param[i] = v;  
            
        }  
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
	fullcharge.textureFileName = "PaticleAssets\\flashA_W.png";

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
	locus.textureFileName = "PaticleAssets\\flashB_Y.png";

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
	hit.textureFileName = "PaticleAssets\\flashB_W.png";

	//発射位置をセット
	hit.position = this->transform_.position_;

	//エフェクトを開始
	VFX::Start(hit);
}

void Character::SetWallHitEffect()
{
	//csvから読み込んだ,柵に接触時エフェクトのパラメータを実際にセットする
	EmitterData  wallhit;
	VFX::SetEmitter(wallhit, WallHitEffectParam_);

	//使用する画像のパスをセットする
	wallhit.textureFileName = "PaticleAssets\\flashB_W.png";

	//発射位置をセット
	wallhit.position = this->transform_.position_;

	//エフェクトを開始
	VFX::Start(wallhit);
}

void Character::InitCSVSound()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVData\\EngineData\\SoundData.csv");

	//csvファイルの0列目の文字列を取得
	std::string p_sound = "SoundParam";
	
	//指定した文字列がいずれかの0列目に存在したら
	if (csv.IsGetParamName(p_sound))
	{
		//その行を配列として全取得
		std::vector<float> v = csv.GetParam(p_sound);
		
		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		ChargeSoundCount_ = static_cast<int>(v[i_chargecount]);
		AttackSoundCount_ = static_cast<int>(v[i_attackcount]);
	}
}

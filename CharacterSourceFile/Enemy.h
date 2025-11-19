#pragma once
#include "../Engine/GameObject.h"
#include"Character.h"
#include"Player.h"
#include <memory>
#include <unordered_map>
#include"../CharacterSourceFile/EnemyState/BaseEnemyState.h"

//敵(CPU)キャラクターの処理を行うクラス
class Enemy :
    public Character
{
public:
	//----------状態遷移----------

	//敵(CPU)の状態遷移
	//これらの値に応じて各Update関数を呼び出す
	enum EnemyState {
		S_Root = 0,		//判断用
		S_Approach,		//追いかける
		S_Aim,			//プレイヤーを狙う(攻撃準備)
		S_Attack,		//攻撃
		S_HitStop,		//ヒットストップ
		S_Hit,			//弾かれる
		S_FenceHit,		//柵にヒット
		S_Wait,         //待機時間 攻撃後のクールタイム
		S_Look,         //様子見(プレイヤーを見るだけ)
		S_Stop,			//敵を止める
		S_MaxState
	};
	
private:
	//----------モデルハンドル----------

	//敵モデル
    int hEnemy_;

	//----------インスタンス----------

	//プレイヤーのインスタンス(Enemyからは取らず、上位シーンから設定)
	Player* pPlayer_;

	//----------攻撃関係----------
	
	//狙ってから攻撃までの計測タイマー
	int AimTimer_;

	//EnemyAttackTimeArrayの添え字
	int RandomAim_;

	//----------移動関係----------

	//プレイヤーの位置ベクトルを保管
	XMVECTOR TargetVec_;

	//プレイヤーの位置(座標)
	XMFLOAT3 TargetPosition_;

	//プレイヤーの速度
	float TargetAcceleration_;

	//プレイヤーのオブジェクト名
	std::string TargetName_;

	//----------ステートクラス関係----------

	//各ステートの実体を格納するテーブル(連想配列)
	std::unordered_map<EnemyState, std::unique_ptr<BaseEnemyState>> StateTable_;

	//現在アクティブな状態
	BaseEnemyState* CurrentState_;

public:
	Enemy(GameObject* parent);
	~Enemy();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//解放
	void Release() override;

	//当たり判定処理
	void OnCollision(GameObject* pTarget) override;

	//Characterの仮想関数継承
	bool IsCharacterStateHitStop() override { return CurrentState_->IsHitStopState(); }
	bool IsCharacterStateHit() override { return CurrentState_->isHitState(); }
	bool IsCharacterStateFenceHit() override { return CurrentState_->IsFenceHitState(); }

	//動的に呼び出す更新処理
	void EnemyRun();

	//状態遷移を行う(ステートマシン)
	void ChangeState(EnemyState _newState);
	
	//----------Enemy処理関数----------

	//ImGuiの描画
	void DrawImGui();

	//敵に移動を許可(ステートマシン)
	void EnemyStartState() { ChangeState(Enemy::S_Root); }

	//敵を止める(ステートマシン)
	void EnemyStopState() { ChangeState(Enemy::S_Stop); }
	
	//プレイヤーの方向に回転する
	void LookPlayer();

	/// <summary>
	/// 自身のY軸回転量を指定された方向に合わせて更新し、前を向くようにする
	/// </summary>
	/// <param name="_direction">回転量を計算する際のベクトル</param>
	void RotateFromDirection(XMVECTOR _direction);

	//敵とプレイヤーの距離を返す
	float PlayerEnemyDistanceX();

	/// <summary>
	/// 追跡判定距離より近いかどうか返す
	/// </summary>
	/// <param name="_length">相手と自身の距離</param>
	/// <returns>相手と自身の距離の方が近いかどうか</returns>
	bool IsNearChaseLength(float _length);

	//AimTimerを増加
	void AimTimerAdd();

	//AimTimerをリセット
	void AimTimerReset();

	//攻撃するまでの時間を過ぎたか
	bool IsTimeOverAttackTime();

	//攻撃までの時間を再抽選する
	void RandomAimReLottery();

	//敵がプレイヤーに対して攻撃を行うか返す
	//trueなら「攻撃しろ」、falseなら「攻撃するな」 を指示
	bool IsAttackDecision();

	//Enemy限定のデータをCSV読み込み
	void SetCSVEnemy();

	//----------セッター・ゲッター関数----------
	void SetPlayerPointer(Player* _player) { pPlayer_ = _player; }

	void SetTargetPosition(XMFLOAT3 _targetpos) { TargetPosition_ = _targetpos; }
	XMFLOAT3 GetTargetPosition() const {return TargetPosition_; }

	void SetTargetAcceleration(float _acceleration) { TargetAcceleration_ = _acceleration; }
	float GetTargetAcceleration() const { return TargetAcceleration_; }

	void SetChaseLength(float _chaselength);
	float GetChaseLength() const;

	void SetAutoAttackDirection(XMVECTOR _AutoAttackDirection);
	XMVECTOR GetAutoAttackDirection() const;

};

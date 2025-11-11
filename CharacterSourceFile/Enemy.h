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

	//各ステートの実体を格納するテーブル(連想配列)
	std::unordered_map<EnemyState, std::unique_ptr<BaseEnemyState>> stateTable_;

	//現在アクティブな状態
	BaseEnemyState* currentState_ = nullptr;

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

	//動的に呼び出す更新処理
	void EnemyRun();

	//状態遷移を行う(ステートマシン)
	void ChangeState(EnemyState newState);
	
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

	//AimTimerを増加
	void AimTimerAdd();

	//AimTimerをリセット
	void AimTimerReset();

	//攻撃するまでの時間を過ぎたか
	bool IsTimeOverAttackTime();

	//HitStopTimerを増加
	void HitStopTimerAdd();

	//HitStopTimerをリセット
	void HitStopTimerReset();

	//ヒットストップ時間を過ぎたか
	bool IsTimeOverHitStopTime();

	//攻撃までの時間を再抽選する
	void RandomAimReLottery();

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

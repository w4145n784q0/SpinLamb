#pragma once
#include "Engine/GameObject.h"
#include"Player.h"

//敵キャラクターの処理を行うクラス
class Enemy :
    public Character
{
private:
	//----------モデルハンドル----------

	//敵モデル
    int hEnemy_;

	//----------インスタンス----------

	//プレイヤーのインスタンス(Enemyからは取らず、上位シーンから設定)
	Player* pPlayer_;

	//敵(CPU)ステート
	//これらの値に応じて各Update関数を呼び出す
	enum State {
		S_ROOT = 0,//判断用
		S_CHASE,//追いかける
		S_AIM,//プレイヤーを狙う(攻撃準備)
		S_ATTACK,//攻撃
		S_HITSTOP,//ヒットストップ
		S_HIT,//弾かれる
		S_FENCEHIT,//柵にヒット
		S_STOP,//敵を止める
		S_MAX
	};
	State EnemyState_;

	//----------攻撃関係----------
	
	//狙ってから攻撃までのタイマー
	int AimTimer_;

	//----------移動関係----------

	//プレイヤーの位置ベクトルを保管
	XMVECTOR TargetVec_;

	//プレイヤーの位置(座標)
	XMFLOAT3 TargetPosition_;

	//プレイヤーの速度
	float TargetAcceleration_;

	//----------攻撃関係----------

	//EnemyAttackTimeArrayの添え字
	int RandomAim_; 

	///----------ダメージ関係/----------

	//ヒットストップ時間
	int HitStopTimer_;


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

	//----------EnemyState_に応じて内容が変わるUpdate関数----------

	//ここから次のUpdateに移る判断をする
	void UpdateRoot();

	//追跡
	void UpdateChase();

	//攻撃準備
	void UpdateAim();

	//攻撃
	void UpdateAttack();

	//ヒットストップ
	void UpdateHitStop();

	//弾かれた状態
	void UpdateHit();

	//壁に接触状態
	void UpdateFenceHit();
	
	//敵を止める
	void UpdateStop();

	//ImGuiの描画
	void DrawImGui();
	
	//敵に移動を許可
	void EnemyStart() { EnemyState_ = S_ROOT; }

	//敵を止める
	void EnemyStop() { EnemyState_ = S_STOP; }
	
	//プレイヤーの方向に回転する
	void LookPlayer();

	//敵とプレイヤーの距離を返す
	float PlayerEnemyDistanceX();

	//Enemy限定のデータをCSV読み込み
	void SetCSVEnemy();

	//----------セッター・ゲッター関数----------
	void SetPlayerPointer(Player* _player) { pPlayer_ = _player; }

	void SetTargetPosition(XMFLOAT3 _targetpos) { TargetPosition_ = _targetpos; }
	XMFLOAT3 GetTargetPosition() const {return TargetPosition_; }

	void SetTargetAcceleration(float _acceleration) { TargetAcceleration_ = TargetAcceleration_; }
	float GetTargetAcceleration() const { return TargetAcceleration_; }
};

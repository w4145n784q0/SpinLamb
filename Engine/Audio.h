#pragma once
#include <string>

//-----------------------------------------------------------
//サウンドを管理する（XAudio使用）
//-----------------------------------------------------------
namespace Audio
{
	//初期化
	void Initialize();

	//サウンドファイル(.wav）をロード
	//すでに同じ名前のファイルをロード済みの場合は、既存のデータの番号を返す
	//引数：fileName	ファイル名
	//引数：isLoop		ループ再生させたいならtrue
	//引数：svNum　		同時に鳴らす最大数（省略可）
	//戻値：そのデータに割り当てられた番号
	int Load(std::string fileName, bool isLoop = false, int svNum = 1);

	//再生
	//引数：handle	鳴らしたいサウンドの番号
	void Play(int ID);

	//停止
	//引数：handle	止めたいサウンドの番号
	void Stop(int ID);

	//シーンごとの解放（ロードしたサウンドを解放）
	void Release();

	//本体も解放
	void AllRelease();

	//鳴らす回数のCSV読み込み
	void InitCSVAudio();

	//----------SEを鳴らす回数のゲッター関数----------

	//スタート音を同時に鳴らす回数
	int GetStartNum();

	//選択音を同時に鳴らす回数
	int GetSelectNum();

	//決定音を同時に鳴らす回数
	int GetDecideNum();

	//キャンセル音を同時に鳴らす回数
	int GetCancelNum();

	//ホイッスル音を同時に鳴らす回数
	int GetWhistleNum();

	//チャージ音を同時に鳴らす回数
	int GetChargeNum();

	//攻撃音を同時に鳴らす回数
	int GetAttackNum();

	//ヒット音を同時に鳴らす回数
	int GetHitNum();

	//柵に接触する音を同時に鳴らす回数
	int GetFenceHitNum();

	//ジャンプ音を同時に鳴らす回数
	int GetJumpNum();

	//着地音をを同時に鳴らす回数
	int GetLandingNum();
};


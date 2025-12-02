#pragma once
#include <string>

//-----------------------------------------------------------
//サウンドを管理する（XAudio使用）
//-----------------------------------------------------------
namespace Audio
{
	//----------汎用的に使うSE----------
	extern int StartSoundNum_;		//スタート音を同時に鳴らす回数
	extern int SelectSoundNum_;		//選択音を同時に鳴らす回数
	extern int DecideSoundNum_;		//決定音を同時に鳴らす回数
	extern int CancelSoundNum_;		//キャンセル音を同時に鳴らす回数

	//----------ゲームプレイ中のSE----------
	extern int WhistleSoundNum_;	//ホイッスル音を同時に鳴らす回数
	extern int ChargeSoundNum_;		//チャージ音を同時に鳴らす回数
	extern int AttackSoundNum_;		//攻撃音を同時に鳴らす回数
	extern int HitSoundNum_;		//ヒット音を同時に鳴らす回数
	extern int FenceHitSoundNum_;	//柵に接触音を同時に鳴らす回数
	extern int JumpSoundNum_;		//ジャンプ音を同時に鳴らす回数
	extern int LandingSoundNum_;	//着地音を同時に鳴らす回数

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

};


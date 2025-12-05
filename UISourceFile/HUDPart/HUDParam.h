#pragma once
#include "../../Engine/GameObject.h"
#include "../../UISourceFile/GameTimer.h"
#include "../../UISourceFile/MiniMap.h"


namespace
{
	//描画モード（状況に応じて表示/非表示を切り替えたいもの）を指定
	//他クラスからSetDrawModeを通じて指示するため、namespaceに宣言(このクラスからは変更しない)
	enum DrawMode
	{
		Mode_BeforeStart = 0,	//スタート前
		Mode_JustBefore,		//"Ready?","Go!"を出す、ゲーム開始直前
		Mode_Playing,			//バトルシーンのゲーム中
		Mode_PlayPause,			//バトルシーンのポーズ中
		Mode_Finish,			//"Finish"を出す ゲーム終了
		Mode_Practice,			//フリープレイのゲーム中
		Mode_PracticePause,		//フリープレイのポーズ中
		Mode_None,
	};

	//DrawStartで呼ぶ状態遷移
	enum DrawStartMode
	{
		S_StartReady = 0,	//"Ready?"を表示している状態
		S_StartGo,			//"Go!"を表示している状態
		S_MaxStartMode,
	};
}

class HUDParam :
    public GameObject
{
public:
	//----------画像ハンドル----------

	//練習中(練習シーン用)
	int hPracticeNow_ = -1;

	//ゲーム開始前の簡易説明
	int hGameExplanation_ = -1;

	//ゲーム開始"start!"ロゴ(バトルシーン用)
	int hStart_ = -1;

	//ゲーム開始"ready?"ロゴ(バトルシーン用)
	int hReady_ = -1;

	//ゲーム開始"go!"ロゴ(バトルシーン用)
	int hGo_ = -1;

	//フィニッシュのロゴ(バトルシーン用)
	int hFinish_ = -1;

	//画面分割の枠
	int hSplitLine_ = -1;

	//ナンバー
	int hNumber0_ = -1;
	int hNumber1_ = -1;
	int hNumber2_ = -1;
	int hNumber3_ = -1;
	int hNumber4_ = -1;
	int hNumber5_ = -1;
	int hNumber6_ = -1;
	int hNumber7_ = -1;
	int hNumber8_ = -1;
	int hNumber9_ = -1;

	//ナンバーハンドルの配列
	std::vector<int> ArrayHandle = {};

	//マップ全体
	int hMap_ = -1;

	//プレイヤー1のアイコン
	int hFirstIcon_ = -1;

	//敵(CPU)・プレイヤー2のアイコン
	int hSecondIcon_ = -1;

	//ポーズ画面
	int hPauseMenu_ = -1;

	//ポーズ画面の選択アイコン
	int hPauseIcon_ = -1;

	//----------スコア----------

	//プレイヤー1のスコア
	int FirstScore_ = 0;

	//CPU・プレイヤー2のスコア
	int SecondScore_ = 0;

	//スコア表記のナンバーハンドルの添え字
	//Player1またはPlayer2,Enemyそれぞれの十の位、一の位
	int FirstScoreIndexTen = 0;
	int FirstScoreIndexOne = 0;
	int SecondScoreIndexTen = 0;
	int SecondScoreIndexOne = 0;

	//----------タイマー----------
	
	//hud側から操作する場合のタイマークラスポインタ
	GameTimer* pGameTimer_ = nullptr;

	//時間表記のナンバーハンドルの添え字(10の位)
	int TimeIndexTen = 0;

	//時間表記のナンバーハンドルの添え字(1の位)
	int TimeIndexOne = 0;

	//----------ミニマップ----------
	
	//hud側から操作する場合のミニマップクラスポインタ
	MiniMap* pMiniMap_ = nullptr;


	//----------状態遷移----------

	//描画モードを格納する変数
	//直接代入はせず、SetDrawModeから変更される
	DrawMode DrawMode_;

	//DrawStartで呼ぶ状態遷移を格納する変数
	DrawStartMode DrawStart_;

	//----------開始前ロゴ----------

	//開始前ロゴ変更までのカウンター
	float LogoChangeCount = 0;

	//----------イージング----------

	//DrawReady()からDrawGo()に遷る際のタイマー
	//このクラス内では定義せず他クラスから代入される
	int ReadyTimer_ = 0;

	//イージング使用時のカウンター
	float EasingCount_ = 0.0f;

	//----------イージング用定数----------

	//Go!のロゴの最小拡大率(定数)
	float GoMinScale = 0.0f;

	//Go! のロゴの最大拡大率(定数)
	float GoMaxScale = 0.0f;

	//制限時間の最小拡大率(定数)
	float TimeMinScale = 0.0f;

	//制限時間の最大拡大率(定数)
	float TimeMaxScale = 0.0f;

	//制限時間に行うイージング処理の実行時間(定数)
	float TimeDuration = 0.0f;


	//----------画像描画用トランスフォーム----------

	//"練習モード"
	Transform LogoPractice;

	//ゲーム簡易説明
	Transform LogoExplanation;

	//Ready,Go!ロゴ
	Transform LogoStart;

	//Finish!ロゴ
	Transform LogoFinish;

	//画面分割の枠
	Transform SplitLine;

	//時間表記十の位
	Transform TenTime;

	//時間表記一の位
	Transform OneTime;

	//マップのトランスフォーム
	Transform MapIcon;

	//プレイヤーアイコンのトランスフォーム
	Transform FirstIcon;

	//敵アイコンのトランスフォーム
	Transform SecondIcon;

	//スコア表示位置のトランスフォーム
	Transform FirstScoreTen;//プレイヤー1の10の位
	Transform FirstScoreOne;//プレイヤー1の1の位
	Transform SecondScoreTen;//プレイヤー2・Enemyの1の位
	Transform SecondScoreOne;//プレイヤー2・Enemyの1の位

	//ポーズ画面のトランスフォーム
	Transform TransPauseMenu;

	//ポーズ画面の選択アイコンのトランスフォーム
	Transform TransPauseIcon;

	//画像用トランスフォームをすべて入れる配列
	//初期化の際に使用する
	std::vector<std::reference_wrapper<Transform>> ImageArray = {
	LogoPractice,LogoExplanation,LogoStart,
	LogoFinish, SplitLine, TenTime ,OneTime, MapIcon,FirstIcon,SecondIcon,
	FirstScoreTen, FirstScoreOne, SecondScoreTen, SecondScoreOne,
	TransPauseMenu, TransPauseIcon
	};

public:
    HUDParam() = default;
    HUDParam(GameObject* parent);
    virtual ~HUDParam() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

	//画像ハンドル初期化
	void InitImageArray();


	//描画するスコア設定
	void SetFirstScore(int score) { FirstScore_ = score; }
	void SetSecondScore(int score) { SecondScore_ = score; }

	//各ポインタを設定
	void SetTimerPointer(GameTimer* _gametimer) { pGameTimer_ = _gametimer; }
	void SetMiniMapPointer(MiniMap* _minimap) { pMiniMap_ = _minimap; }

	//描画モードを変更 ここで指定した_drawmodeがDraw()にて呼ばれる
	void SetDrawMode(DrawMode _drawmode) { DrawMode_ = _drawmode; }

	//描画モードを渡す 主にImGuiで使用
	DrawMode GetDrawMode() { return DrawMode_; }

	//DrawReady()からDrawGo()に遷る時間設定
	//バラバラに持つことを防ぐため,BattleSceneから指示して設定
	void SetReadyTimer(int _timer) { ReadyTimer_ = _timer; }

	//ポーズ画面選択アイコンの位置設定
	//HUDからは変更せずに、BattleScene,Practiceシーンからセット
	//Y座標だけをトランスフォームに渡す(変わるのはY座標の位置のみ)
	void SetPauseIcon(float	_posY) { TransPauseIcon.position_.y = _posY; }

	//トランスフォームのゲッター関数(ImGui用)
	Transform& GetLogoExplanation();
	Transform& GetLogoStart();
	Transform& GetLogoFinish();
	Transform& GetLogoPractice();
	Transform& GetFirstScoreTen();
	Transform& GetFirstScoreOne();
	Transform& GetSecondScoreTen(); 
	Transform& GetSecondScoreOne();
	Transform& GetTenTime();
	Transform& GetOneTime();
	Transform& GetMapIcon();
	Transform& GetFirstIcon();
	Transform& GetSecondIcon();
	Transform& GetPauseMenu();
	Transform& GetPauseIcon();
};


#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "global.h"
#include "RootObject.h"
#include "Model.h"
#include "Image.h"
#include "Camera.h"
#include "Input.h"
#include "Audio.h"
#include "VFX.h"

#include "../ViewSourceFile/GameView.h"

#include "../imGui/imgui.h" 
#include "../imGui/imgui_impl_dx11.h" 
#include "../imGui/imgui_impl_win32.h"

#pragma comment(lib,"Winmm.lib")


namespace
{
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const int FPS = 60;
	const int MIRI_SEC = 1000;
}

//定数宣言
const char* WIN_CLASS_NAME = "SpinLamb!";	//ウィンドウクラス名

//プロトタイプ宣言
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// エントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif

	srand((unsigned)time(NULL));
	SetCurrentDirectory("Assets");

	//初期化ファイル（setup.ini）から必要な情報を取得
	int screenWidth = GetPrivateProfileInt("SCREEN", "Width", SCREEN_WIDTH, ".\\setup.ini");		//スクリーンの幅
	int screenHeight = GetPrivateProfileInt("SCREEN", "Height", SCREEN_HEIGHT, ".\\setup.ini");	//スクリーンの高さ
	int fpsLimit = GetPrivateProfileInt("GAME", "Fps", FPS, ".\\setup.ini");				//FPS（画面更新速度）
	int isDrawFps = GetPrivateProfileInt("DEBUG", "ViewFps", 0, ".\\setup.ini");		//キャプションに現在のFPSを表示するかどうか




	//ウィンドウを作成
	HWND hWnd = InitApp(hInstance, screenWidth, screenHeight, nCmdShow);

	//Direct3D準備
	Direct3D::Initialize(hWnd, screenWidth, screenHeight);

	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(Direct3D::pDevice_, Direct3D::pContext_);
		ImGui::StyleColorsLight();
	}

	//カメラを準備
	Camera::Initialize();

	//入力処理（キーボード、マウス、コントローラー）の準備
	Input::Initialize(hWnd);

	//オーディオ（効果音）の準備
	Audio::Initialize();

	//ゲームの映し方の準備
	GameView::Initialize();

	//ルートオブジェクト準備
	//すべてのゲームオブジェクトの親となるオブジェクト
	RootObject* pRootObject = new RootObject;
	pRootObject->Initialize();

	//共通データを初期化
	GameObject::CSVCommonDataInitialize();
	Image::SetCSVImage();

	//デルタタイムの計算
	//宣言はGlobalで行われているので実際の初期化をここで行う
	Dt = 1.0f / fpsLimit;

	//メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり（こっちが優先）
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし（ここでゲームの処理）
		else
		{
			//時間計測
			timeBeginPeriod(1);	//時間計測の制度を上げる
			static int FPS = 0;								//画面更新回数のカウンタ
			static DWORD lastFpsResetTime = timeGetTime();	//最後にキャプションにFPSを表示した
			static DWORD lastUpdateTime = timeGetTime();	//最後に画面を更新した時間
			DWORD nowTime = timeGetTime();					//現在の時間

			//キャプションに現在のFPSを表示する
			if (isDrawFps)
			{
				//1秒（1000ミリ秒）経ったら
				if (nowTime - lastFpsResetTime > MIRI_SEC)
				{
					//FPSの値を表示
					char string[16];
					wsprintf(string, "FPS:%d", FPS);
					SetWindowText(GetActiveWindow(), string);
					FPS = 0;
					lastFpsResetTime = nowTime;
				}
			}


			//指定した時間（FPSを60に設定した場合は60分の1秒）経過していたら更新処理
			if ((nowTime - lastUpdateTime) * fpsLimit > 1000.0f)
			{
				//時間計測関連
				lastUpdateTime = nowTime;	//現在の時間（最後に画面を更新した時間）を覚えておく
				FPS++;						//画面更新回数をカウントする

				


				//入力（キーボード、マウス、コントローラー）情報を更新
				Input::Update();

				//全オブジェクトの更新処理
				//ルートオブジェクトのUpdateを呼んだあと、自動的に子、孫のUpdateが呼ばれる
				pRootObject->UpdateSub();

				//カメラを更新
				//Camera::Update();

				//エフェクトの更新
				VFX::Update();


				//このフレームの描画開始
				Direct3D::BeginDraw();

#ifdef _DEBUG
				//ImGuiをセット
				//以降全シーンでImGuiを記述することで使用可能となる
				ImGui::Begin("config");
#endif

				//二画面状態なら、画面分け→カメラを映す→描画の順で処理
				//player1のカメラセット、その他のDrawSub行う
				//player2のカメラセット、その他のDrawSub行う

				//IsDual,IsSingleはBattle,Practiceシーンが該当
				//それ以外はelse文に入る
				if (GameView::IsDual())
				{
					//プレイヤー1用の画面セット
					GameView::ViewPlayer1();

					//全オブジェクトを描画
					//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
					pRootObject->DrawSub();

					//プレイヤー1用のImage専用描画
					GameView::ViewImagePlayer1();

					//エフェクトの描画
					VFX::Draw();

					//プレイヤー2用の画面セット
					GameView::ViewPlayer2();

					//全オブジェクトを描画
					//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
					pRootObject->DrawSub();

					//プレイヤー2用のImage専用描画
					GameView::ViewImagePlayer2();

					//エフェクトの描画
					VFX::Draw();

					///プレイシーン関係のImguiの表示(ImGuiの描画は一度だけ行うので、player1,2の描画が終わった後に行う)
					GameView::ViewPlayImGui();

					//エンジン関係のImGuiの表示(これも一度だけ表示)
					GameView::ViewEngineImGui();

					//HUDの描画(これらも一度だけ描画するので、player1,2の描画が終わった後に行う)
					GameView::ViewHUDNormal();

					//遷移演出の描画(これらも一度だけ描画するので、player1,2の描画が終わった後に行う)
					//遷移演出は必ず最後に一度だけ行う
					GameView::ViewTransitionEffect();
				}
				else if (GameView::IsSingle())
				{
					//プレイヤー1vsCPU用の画面セット
					GameView::ViewPvE();

					//全オブジェクトを描画
					//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
					pRootObject->DrawSub();

					//プレイヤー1用のImage専用描画
					GameView::ViewImagePlayer1();

					//エフェクトの描画
					VFX::Draw();

					//プレイシーン関係のImguiの表示(一度だけ描画するので、player1の描画が終わった後に行う)
					GameView::ViewPlayImGui();

					//エンジン関係のImGuiの表示(これも一度だけ表示)
					GameView::ViewEngineImGui();

					//HUDの描画(一度だけ描画するので、player1の描画が終わった後に行う)
					GameView::ViewHUDNormal();

					//遷移演出の描画(一度だけ描画するので、player1の描画が終わった後に行う)
					//遷移演出は必ず最後に一度だけ行う
					GameView::ViewTransitionEffect();
				}
				else
				{
					//通常(カメラの位置・焦点を初期位置にし全体を描画)の画面セット
					GameView::ViewNormal();

					//全オブジェクトを描画
					//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
					pRootObject->DrawSub();

					//エフェクトの描画
					VFX::Draw();

					//エンジン関係のImGuiの表示(これも一度だけ表示)
					GameView::ViewEngineImGui();

					//遷移演出の描画(一度だけ描画するので、すべての描画が終わった後に行う)
					//遷移演出は必ず最後に一度だけ行う
					GameView::ViewTransitionEffect();
				}

#ifdef _DEBUG

				//ImGuiを終了する
				ImGui::End();
#endif

				//描画終了
				Direct3D::EndDraw();



				
				//ちょっと休ませる
				Sleep(1);
			}
			timeEndPeriod(1);	//時間計測の制度を戻す
		}
	}

	

	//いろいろ解放
	VFX::Release();
	GameView::Release();
	Audio::AllRelease();
	Model::AllRelease();
	Image::AllRelease();
	pRootObject->ReleaseSub();
	SAFE_DELETE(pRootObject);
	Direct3D::Release();

	return 0;
}

//  ImGuiのメッセージ処理
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//ウィンドウの作成
HWND InitApp(HINSTANCE hInstance, int screenWidth, int screenHeight, int nCmdShow)
{
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);					//この構造体のサイズ
	wc.hInstance = hInstance;						//インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;				//ウィンドウクラス名
	wc.lpfnWndProc = WndProc;						//ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;				//スタイル（デフォルト）
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);	//アイコン
	wc.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);	//小さいアイコン
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);	//マウスカーソル
	wc.lpszMenuName = nullptr;						//メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//背景（白）
	RegisterClassEx(&wc);

	//ウィンドウサイズの計算
	RECT winRect = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);

	//タイトルバーに表示する内容
	char caption[64];
	GetPrivateProfileString("SCREEN", "Caption", "***", caption, 64, ".\\setup.ini");

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,					//ウィンドウクラス名
		caption,						//タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW,			//スタイル（普通のウィンドウ）
		CW_USEDEFAULT,					//表示位置左（おまかせ）
		CW_USEDEFAULT,					//表示位置上（おまかせ）
		winRect.right - winRect.left,	//ウィンドウ幅
		winRect.bottom - winRect.top,	//ウィンドウ高さ
		nullptr,						//親ウインドウ（なし）
		nullptr,						//メニュー（なし）
		hInstance,						//インスタンス
		nullptr							//パラメータ（なし）
	);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	return hWnd;
}


//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	//ウィンドウを閉じた
	case WM_DESTROY:
		PostQuitMessage(0);	//プログラム終了
		return 0;

	//マウスが動いた
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
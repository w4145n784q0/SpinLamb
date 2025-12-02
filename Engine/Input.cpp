#include "Input.h"
#include "Global.h"
#include"CsvReader.h"

#include"GameObject.h"

namespace Input
{
	//csv読み込み時のインデックス(入力関連の変数)
	enum InputIndex
	{
		i_StickTilt = 0,
		i_StickMicroTilt,
		i_ControllerVibLeft,
		i_ControllerVibRight,
	};

	float StickTilt = 0.0f;
	float StickMicroTilt = 0.0f;
	int ControllerVibLeft = 0;
	int ControllerVibRight = 0;

	//ウィンドウハンドル
	HWND	hWnd_;

	//DirectInputオブジェクト
	LPDIRECTINPUT8			pDInput_;

	//キーボード
	LPDIRECTINPUTDEVICE8	pKeyDevice_;	//デバイスオブジェクト
	BYTE keyState_[256];					//現在の各キーの状態
	BYTE prevKeyState_[256];				//前フレームでの各キーの状態

	//マウス
	LPDIRECTINPUTDEVICE8	pMouseDevice_;	//デバイスオブジェクト
	DIMOUSESTATE mouseState_;				//マウスの状態
	DIMOUSESTATE prevMouseState_;			//前フレームのマウスの状態
	POINT mousePos_;							//マウスカーソルの位置

	//コントローラー
	const int MAX_PAD_NUM = 4;						//コントローラーの接続最大数
	XINPUT_STATE controllerState_[MAX_PAD_NUM];		//コントローラーの状態
	XINPUT_STATE prevControllerState_[MAX_PAD_NUM];	//前フレームのコントローラーの状態
	float PrevStickTiltL_X = 0.0f;					//前回のLスティック左右の入力保管
	float PrevStickTiltL_Y = 0.0f;					//前回のLスティック上下の入力保管
	float NowStickTiltL_X = 0.0f;					//今回のLスティック左右の入力保管
	float NowStickTiltL_Y = 0.0f;					//今回のLスティック上下の入力保管
	

	//初期化
	void Initialize(HWND hWnd)
	{
		//ウィンドウハンドル
		hWnd_ = hWnd;

		//DirectInput本体
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&pDInput_, nullptr);

		//キーボード
		pDInput_->CreateDevice(GUID_SysKeyboard, &pKeyDevice_, nullptr);
		pKeyDevice_->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice_->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		//マウス
		pDInput_->CreateDevice(GUID_SysMouse, &pMouseDevice_, nullptr);
		pMouseDevice_->SetDataFormat(&c_dfDIMouse);
		pMouseDevice_->SetCooperativeLevel(hWnd_, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		//csv読み込み行う
		SetInputCSV();
	}


	//更新
	void Update()
	{
		//キーボード
		pKeyDevice_->Acquire();
		memcpy(prevKeyState_, keyState_, sizeof(keyState_));
		pKeyDevice_->GetDeviceState(sizeof(keyState_), &keyState_);

		//マウス
		pMouseDevice_->Acquire();
		memcpy(&prevMouseState_, &mouseState_, sizeof(mouseState_));
		pMouseDevice_->GetDeviceState(sizeof(mouseState_), &mouseState_);

		//コントローラー
		for (int i = 0; i < MAX_PAD_NUM; i++)
		{
			memcpy(&prevControllerState_[i], &controllerState_[i], sizeof(controllerState_[i]));
			XInputGetState(i, &controllerState_[i]);
		}

		//前回の傾きに今回の傾きを代入し更新
		PrevStickTiltL_X = NowStickTiltL_X;
		PrevStickTiltL_Y = NowStickTiltL_Y;

		//今の入力状態から今回の傾きを取得
		NowStickTiltL_X = GetPadStickL().x;
		NowStickTiltL_Y = GetPadStickL().y;
	}



	//開放
	void Release()
	{
		SAFE_RELEASE(pMouseDevice_);
		SAFE_RELEASE(pKeyDevice_);
		SAFE_RELEASE(pDInput_);
	}



	/////////////////////////////　キーボード情報取得　//////////////////////////////////

	//キーが押されているか調べる
	bool IsKey(int keyCode)
	{
		//押してる
		if (keyState_[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}


	//キーを今押したか調べる（押しっぱなしは無効）
	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		if (IsKey(keyCode) && !(prevKeyState_[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}


	//キーを今放したか調べる
	bool IsKeyUp(int keyCode)
	{
		//今押してなくて、前回は押してる
		if (!IsKey(keyCode) && prevKeyState_[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}


	/////////////////////////////　マウス情報取得　//////////////////////////////////

	//マウスのボタンが押されているか調べる
	bool IsMouseButton(int buttonCode)
	{
		//押してる
		if (mouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今押したか調べる（押しっぱなしは無効）
	bool IsMouseButtonDown(int buttonCode)
	{
		//今は押してて、前回は押してない
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今放したか調べる
	bool IsMouseButtonUp(int buttonCode)
	{
		//今押してなくて、前回は押してる
		if (!IsMouseButton(buttonCode) && prevMouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//マウスカーソルの位置を取得
	XMFLOAT3 GetMousePosition()
	{
		XMFLOAT3 result = XMFLOAT3((float)mousePos_.x, (float)mousePos_.y, 0);
		return result;
	}

	//マウスカーソルの位置をセット
	void SetMousePosition(int x, int y)
	{
		mousePos_.x = x;
		mousePos_.y = y;
	}


	//そのフレームでのマウスの移動量を取得
	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState_.lX, (float)mouseState_.lY, (float)mouseState_.lZ);
		return result;
	}


	/////////////////////////////　コントローラー情報取得　//////////////////////////////////

	//コントローラーのボタンが押されているか調べる
	bool IsPadButton(int buttonCode, int padID)
	{
		if (controllerState_[padID].Gamepad.wButtons & buttonCode)
		{
			return true; //押してる
		}
		return false; //押してない
	}

	//コントローラーのボタンを今押したか調べる（押しっぱなしは無効）
	bool IsPadButtonDown(int buttonCode, int padID)
	{
		//今は押してて、前回は押してない
		if (IsPadButton(buttonCode, padID) && !(prevControllerState_[padID].Gamepad.wButtons & buttonCode))
		{
			return true;
		}
		return false;
	}

	//コントローラーのボタンを今放したか調べる
	bool IsPadButtonUp(int buttonCode, int padID)
	{
		//今押してなくて、前回は押してる
		if (!IsPadButton(buttonCode, padID) && prevControllerState_[padID].Gamepad.wButtons & buttonCode)
		{
			return true;
		}
		return false;
	}


	float GetAnalogValue(int raw, int max, int deadZone)
	{
		float result = (float)raw;

		if (result > 0)
		{
			//デッドゾーン
			if (result < deadZone)
			{
				result = 0;
			}
			else
			{
				result = (result - deadZone) / (max - deadZone);
			}
		}

		else
		{
			//デッドゾーン
			if (result > -deadZone)
			{
				result = 0;
			}
			else
			{
				result = (result + deadZone) / (max - deadZone);
			}
		}

		return result;
	}


	//左スティックの傾きを取得
	XMFLOAT3 GetPadStickL(int padID)
	{
		float x = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLX, 32767, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		float y = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLY, 32767, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		return XMFLOAT3(x, y, 0);
	}

	//右スティックの傾きを取得
	XMFLOAT3 GetPadStickR(int padID)
	{
		float x = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRX, 32767, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		float y = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRY, 32767, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		return XMFLOAT3(x, y, 0);
	}

	//左トリガーの押し込み具合を取得
	float GetPadTriggerL(int padID)
	{
		return GetAnalogValue(controllerState_[padID].Gamepad.bLeftTrigger, 255, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}

	//右トリガーの押し込み具合を取得
	float GetPadTriggerR(int padID)
	{
		return GetAnalogValue(controllerState_[padID].Gamepad.bRightTrigger, 255, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}

	//振動させる
	void SetPadVibration(int l, int r, int padID)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = l; // 左モーターの強さ
		vibration.wRightMotorSpeed = r;// 右モーターの強さ
		XInputSetState(padID, &vibration);
	}

	//コントローラー振動開始(振動量の固定値)
	void ControllerVibrationStart(int _PadID)
	{
		//コントローラー振動させる(値は固定のものを活用)
		SetPadVibration(ControllerVibLeft, ControllerVibRight, _PadID);
	}

	void ControllerVibrationStart(int l, int r, int _PadID)
	{
		//コントローラー振動させる(引数を活用)
		SetPadVibration(l, r, _PadID);
	}

	//コントローラー振動を止める
	void StopPadVibration(int padID)
	{
		//SetPadVibrationを呼んで引数は0にすることで止まる
		Input::SetPadVibration(0, 0, padID);
	}

	//左、下に倒した場合: 今回倒した値が-sticktiltより小さい && 前回倒した値が-sticktiltより大きい
	//右、上に倒した場合: 今回倒した値がsticktiltより大きい && 前回倒した値がsticktiltより小さい 

	bool IsStickTiltLX_LEFT()
	{
		//今回左スティックを左に一定以上倒した && 前回左スティックを左に一定以上倒していない
		if (NowStickTiltL_X <= -StickTilt && PrevStickTiltL_X >= -StickTilt)
			return true;
		else
			return false;
	}

	bool IsStickTiltLX_RIGHT()
	{
		//今回左スティックを右に一定以上倒した && 前回左スティックを右に一定以上倒していない
		if (NowStickTiltL_X >= StickTilt && PrevStickTiltL_X <= StickTilt)
			return true;
		else
			return false;
	}

	bool IsStickTiltLY_UP()
	{
		//今回左スティックを上に一定以上倒した && 前回左スティックを上に一定以上倒していない
		if (NowStickTiltL_Y >= StickTilt && PrevStickTiltL_Y <= StickTilt)
			return true;
		else
			return false;
	}

	bool IsStickTiltLY_DOWN()
	{
		//今回左スティックを下に一定以上倒した && 前回左スティックを下に一定以上倒していない
		if (NowStickTiltL_Y <= -StickTilt && PrevStickTiltL_Y >= -StickTilt)
			return true;
		else
			return false;
	}

	//csv読み込み
	void SetInputCSV()
	{
		//csvファイルを読み込む
		CsvReader csv;
		csv.Load("CSVdata\\EngineData\\InputData.csv");

		//csvファイルの0列目の文字列を取得
		std::string InputName = "Input";

		//0列目の文字列を渡し、その行のパラメータを取得
		std::vector<float> InputData = GameObject::GetCSVReadData(csv, InputName);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		StickTilt = InputData[i_StickTilt];
		StickMicroTilt = InputData[i_StickMicroTilt];
		ControllerVibLeft = static_cast<int>(InputData[i_ControllerVibLeft]);
		ControllerVibRight = static_cast<int>(InputData[i_ControllerVibRight]);
	}

	//ImGui描画
	void DrawImGui()
	{
#ifdef _DEBUG
		if (ImGui::TreeNode("Stick"))
		{
			//スティックの傾き
			ImGui::InputFloat("StickTilt", &StickTilt, ZERO_POINT_ONE);
			ImGui::InputFloat("StickMicroTilt", &StickMicroTilt, ZERO_POINT_ONE);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ControllerVib"))
		{
			//コントローラーの振動量
			ImGui::InputInt("VibLeft", &ControllerVibLeft);
			ImGui::InputInt("VibRight", &ControllerVibRight);
			ImGui::TreePop();
		}
#endif
	}

}
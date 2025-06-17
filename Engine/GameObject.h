#pragma once
#include <DirectXMath.h>
#include <list>
#include <string>
#include <assert.h>
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Transform.h"
#include"CsvReader.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

namespace
{
	//トランスフォームの初期化値をCSVから読み込む際のインデックス
	enum initTransform
	{
		pos_x = 0,
		pos_y,
		pos_z,
		rot_x,
		rot_y,
		rot_z,
		sca_x,
		sca_y,
		sca_z,
	};

	enum CommonIndex
	{
		i_deltatime = 0,	
		i_onesecond,		
		i_tendivision,
		i_scenetransition,

	};

}

inline XMFLOAT3 operator + (const XMFLOAT3& a, const XMFLOAT3& b) {
	XMFLOAT3 tmp = {0,0,0};
	tmp.x = a.x + b.x;
	tmp.y = a.y + b.y;
	tmp.z = a.z + b.z;
	return tmp;
}

inline XMFLOAT3 operator - (const XMFLOAT3& a, const XMFLOAT3& b) {
	XMFLOAT3 tmp = { 0,0,0 };
	tmp.x = a.x - b.x;
	tmp.y = a.y - b.y;
	tmp.z = a.z - b.z;
	return tmp;
}

inline XMFLOAT3 operator * (const XMFLOAT3& a, const XMFLOAT3& b) {
	XMFLOAT3 tmp = { 0,0,0 };
	tmp.x = a.x * b.x;
	tmp.y = a.y * b.y;
	tmp.z = a.z * b.z;
	return tmp;
}

inline XMFLOAT3 operator / (const XMFLOAT3& a, const XMFLOAT3& b) {
	XMFLOAT3 tmp = { 0,0,0 };
	tmp.x = a.x / b.x;
	tmp.y = a.y / b.y;
	tmp.z = a.z / b.z;
	return tmp;
}


using namespace DirectX;


//-----------------------------------------------------------
//全てのゲームオブジェクト（シーンも含めて）が継承するインターフェース
// ゲームオブジェクトは、親子構造になっていて、
// マトリクスの影響を受けることになる
//-----------------------------------------------------------
class GameObject
{
protected:
	
	//位置や向きなどを管理するオブジェクト
	Transform				transform_;

	//オブジェクトの名前
	std::string				objectName_;

	//衝突判定リスト
	std::list<Collider*>	colliderList_;	

	/// <summary>
	/// 60fpsにおける1フレームの時間
	/// 使用端末によるフレームレート依存防止
	/// </summary>
	static float DeltaTime;

	/// <summary>
	/// 1秒を表す定数
	/// </summary>
	static int oneSecond;

	/// <summary>
	/// 時間表記等に除算する値
	/// </summary>
	static int  TenDivision;

	/// <summary>
	/// 次のシーン遷移までの時間
	/// </summary>
	static int SceneTransition;

public:
	//コンストラクタ
	GameObject();
	GameObject(GameObject* parent);
	GameObject(GameObject* parent, const std::string& name);

	//デストラクタ
	virtual ~GameObject();

	//各オブジェクトで必ず作る関数
	virtual void Initialize(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw() = 0;
	virtual void Release(void) = 0;

	//自分の該当関数を読んだ後、子供の関数も呼ぶ
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();


	////ローカル行列の取得（このオブジェクトの行列）
	////戻値：ローカル行列
	//XMMATRIX GetLocalMatrix();

	//ワールド行列の取得（親の影響を受けた最終的な行列）
	//戻値：ワールド行列
	XMMATRIX GetWorldMatrix();



	//各フラグの制御
	bool IsDead();			// 削除するかどうか
	void KillMe();			// 自分を削除する
	void Enter();			// Updateを許可
	void Leave();			// Updateを拒否
	void Visible();			// Drawを許可
	void Invisible();		// Drawを拒否
	bool IsInitialized();	// 初期化済みかどうか
	void SetInitialized();	// 初期化済みにする
	bool IsEntered();		// Update実行していいか
	bool IsVisibled();		// Draw実行していいか


	//子オブジェクトリストを取得
	//戻値：子オブジェクトリスト
	std::list<GameObject*>* GetChildList();

	//親オブジェクトを取得
	//戻値：親オブジェクトのアドレス
	GameObject* GetParent();

	//名前でオブジェクトを検索（対象は自分の子供以下）
	//引数：name	検索する名前
	//戻値：見つけたオブジェクトのアドレス（見つからなければnullptr）
	GameObject* FindChildObject(const std::string& name);

	//名前でオブジェクトを検索（対象は全体）
	//引数：検索する名前
	//戻値：見つけたオブジェクトのアドレス
	GameObject* FindObject(const std::string& name) { return GetRootJob()->FindChildObject(name); }

	//オブジェクトの名前を取得
	//戻値：名前
	const std::string& GetObjectName(void) const;

	//子オブジェクトを追加（リストの最後へ）
	//引数：追加するオブジェクト
	void PushBackChild(GameObject* obj);

	//子オブジェクトを追加（リストの先頭へ）
	//引数：obj 追加するオブジェクト
	void PushFrontChild(GameObject* obj);

	//子オブジェクトを全て削除
	void KillAllChildren();



	//コライダー（衝突判定）を追加する
	void AddCollider(Collider * collider);

	//何かと衝突した場合に呼ばれる（オーバーライド用）
	//引数：pTarget	衝突した相手
	virtual void OnCollision(GameObject* pTarget) {};

	//コライダー（衝突判定）を削除
	void ClearCollider();

	//衝突判定
	//引数：pTarget	衝突してるか調べる相手
	void Collision(GameObject* pTarget);

	//テスト用の衝突判定枠を表示
	void CollisionDraw();

	//RootJobを取得
	GameObject* GetRootJob();


	//各アクセス関数
	XMFLOAT3 GetPosition() { return transform_.position_; }
	XMFLOAT3 GetRotate() { return transform_.rotate_; }
	XMFLOAT3 GetScale() { return transform_.scale_; }
	XMFLOAT3 GetWorldPosition() { return Transform::Float3Add(GetParent()->transform_.position_ , transform_.position_); }
	XMFLOAT3 GetWorldRotate() { return Transform::Float3Add(GetParent()->transform_.rotate_, transform_.rotate_); }
	XMFLOAT3 GetWorldScale() { return Transform::Float3Add(GetParent()->transform_.scale_, transform_.scale_); }
	void SetPosition(XMFLOAT3 position) { transform_.position_ = position; }
	void SetPosition(float x, float y, float z) { SetPosition(XMFLOAT3( x, y, z )); }
	void SetRotate(XMFLOAT3 rotate) { transform_.rotate_ = rotate; }
	void SetRotate(float x, float y, float z) { SetRotate(XMFLOAT3( x, y, z )); }
	void SetRotateX(float x) { SetRotate(x, transform_.rotate_.y, transform_.rotate_.z); }
	void SetRotateY(float y) { SetRotate(transform_.rotate_.x, y, transform_.rotate_.z); }
	void SetRotateZ(float z) { SetRotate(transform_.rotate_.x, transform_.rotate_.y, z); }
	void SetScale(XMFLOAT3 scale) { transform_.scale_ = scale; }
	void SetScale(float x, float y, float z) { SetScale(XMFLOAT3(x, y, z)); }


	/// <summary>
	/// 線形補完(floatで受け取り返す)
	/// </summary>
	/// <param name="convert">変換したい値(x)</param>
	/// <param name="originalMin">変換元の範囲の最小値(x1)</param>
	/// <param name="originalMax">変換元の範囲の最大値(x2)</param>
	/// <param name="conversionMin">変換後の範囲の最小値(y1)</param>
	/// <param name="conversionMax">変換後の範囲の最大値(y2)</param>
	/// <returns>補正後の値(y)</returns>
	float LinearCompletion(float convert,float originalMin,float originalMax, 
		float conversionMin,float conversionMax)
	{
		//y = y1 + (x - x1) * (y2 - y1) / (x2 - x1);

		if (originalMin == originalMax)
		{
			return conversionMin; // originalMinとoriginalMaxが同じ場合はconversionMinを返す(0除算対策)
		}
		if (convert < originalMin)
		{
			return conversionMin; // convertがoriginalMinより小さい場合はconversionMinを返す(外挿対策)
		}
		if (convert > originalMax)
		{
			return conversionMax; // convertがoriginalMaxより大きい場合はconversionMaxを返す(外挿対策)
		}

		float y =  conversionMin + (convert - originalMin) * (conversionMax - conversionMin) / (originalMax - originalMin);
		return y;
	}

	/// <summary>
	/// csv読み込み時の各トランスフォーム初期化
	/// </summary>
	/// <param name="tr">代入するトランスフォーム変数</param>
	/// <param name="v">受け取った一行分のTransformデータ配列</param>
	void SetTransformPRS(Transform &tr, std::vector<float> v)
	{
		tr.position_ = { v[pos_x],v[pos_y],v[pos_z] };
		tr.rotate_ = { v[rot_x], v[rot_y],v[rot_z] };
		tr.scale_ = { v[sca_x] ,v[sca_y],v[sca_z] };
	}

	/// <summary>
	/// GameObjectの共通データ初期化
	/// </summary>
    static void SCVCommonDataInitialize() {  

		//static bool initialized = false;
		//if (initialized) return;
		//initialized = true;

        CsvReader csv;  
        csv.Load("CSVdata\\CommonData.csv");  

        std::string common = "CommonData";  
        if (csv.IsGetParamName(common))  
        {  
            std::vector<float> v = csv.GetParam(common);  
            DeltaTime = v[i_deltatime];  
            oneSecond = static_cast<int>(v[i_onesecond]);  
            TenDivision = static_cast<int>(v[i_tendivision]);  
            SceneTransition = static_cast<int>(v[i_scenetransition]);  

        }  
    }

private:

	//オブジェクト削除（再帰）
	//引数：obj　削除するオブジェクト
	void KillObjectSub(GameObject* obj);


private:
	//フラグ
	struct OBJECT_STATE
	{
		unsigned initialized : 1;	//初期化済みか
		unsigned entered : 1;		//更新するか
		unsigned visible : 1;		//描画するか
		unsigned dead : 1;			//削除するか
	};
	OBJECT_STATE state_;

	//親オブジェクト
	GameObject* pParent_;

	//子オブジェクトリスト
	std::list<GameObject*> childList_;
};


//オブジェクトを作成するテンプレート
template <class T>
T* Instantiate(GameObject* pParent)
{
	T* pNewObject = new T(pParent);
	if (pParent != nullptr)
	{
		pParent->PushBackChild(pNewObject);
	}
	pNewObject->Initialize();
	return pNewObject;
}


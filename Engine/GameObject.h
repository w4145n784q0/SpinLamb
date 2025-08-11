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

	//汎用的に使用する変数を初期化するインデックス
	enum CommonIndex
	{
		i_deltatime = 0,	
		i_onesecond,		
		i_tendivision,
		i_sceneshorttransition,
		i_scenetransition,
		i_scenelongtransition,
		i_zeropointone,
	};

}

//----------オペレーター----------

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

	//以下はGameObject継承先で汎用的に使う定数

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
	/// 次のシーン遷移までの時間(1.0秒程)
	/// </summary>
	static int SceneShortTransition;

	/// <summary>
	/// 次のシーン遷移までの時間(1.5秒程)
	/// </summary>
	static int SceneTransition;

	/// <summary>
	/// 次のシーン遷移までの時間(2秒程)
	/// </summary>
	static int SceneLongTransition;

	/// <summary>
	/// 0.1 を表す定数
	/// ImGuiの操作などに使う
	/// </summary>
	static float ZeroPointOne;

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

	//オブジェクト名を設定
	//引数：name　設定する名前
	void SetObjectName(std::string name) { objectName_ = name; }

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
	/// 座標ベース線形補完(floatで受け取り返す)
	/// </summary>
	/// <param name="convert">変換したい値(x)</param>
	/// <param name="originalMin">変換元の範囲の最小値(x1)</param>
	/// <param name="originalMax">変換元の範囲の最大値(x2)</param>
	/// <param name="conversionMin">変換後の範囲の最小値(y1)</param>
	/// <param name="conversionMax">変換後の範囲の最大値(y2)</param>
	/// <returns>補正後の値(y) 変換後の範囲に収まる</returns>
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
	/// 正規化(floatで受け取り返す)
	/// </summary>
	/// <param name="_value">変換される値</param>
	/// <param name="_min">下限値</param>
	/// <param name="_max">上限値</param>
	/// <returns></returns>
	float Normalize(float _value, float _min = 0.0f, float _max = 1.0f)
	{
		//値を0から1の範囲に正規化

		//_minと_maxが同じ場合は0を返す(0除算対策)
		if (_min == _max)
		{
			return 0.0f; 
		}

		// minとmaxの順序が逆なら入れ替える
		if (_min > _max)
		{
			std::swap(_min, _max);
		}

		//正規化の計算(対象の値 - 下限値) / (上限値 - 下限値)
		float normalized = (_value - _min) / (_max - _min);

		//正規化された値が1.0f以上~0.0f以下にならないように値を制限
		if (normalized >= 1.0f)
		{
			normalized = 1.0f;
		}
		if (normalized <= 0.0f)
		{
			normalized = 0.0f;
		}

		return normalized;
	}

	/// <summary>
	/// csv読み込み時の各トランスフォーム初期化
	/// </summary>
	/// <param name="tr">代入するトランスフォーム変数</param>
	/// <param name="v">受け取った一行分のTransformデータ配列</param>
	void SetTransformPRS(Transform &_tr, std::vector<float> _v)
	{
		//pos_xは0から始まる整数で、Transformデータ配列の添え字となる
		//Transformの各要素のx,y,zに_vの値を順番に入れていく

		_tr.position_ = { _v[pos_x],_v[pos_y],_v[pos_z] };
		_tr.rotate_ = { _v[rot_x], _v[rot_y],_v[rot_z] };
		_tr.scale_ = { _v[sca_x] ,_v[sca_y],_v[sca_z] };
	}

	/// <summary>
	/// csvから読み込んだデータを配列として取得
	/// </summary>
	/// <param name="_csv">読み込むcsvファイル</param>
	/// <param name="_name">読み込むcsvファイルの0列目の文字列</param>
	/// <returns>読み込んだデータの配列</returns>
	static std::vector<float> GetCSVReadData(CsvReader& _csv, const std::string& _name)
	{
		//指定した文字列がいずれかの0列目に存在したら
		if (_csv.IsGetParamName(_name))
		{
			//その行を配列として全取得
			std::vector<float> v = _csv.GetParam(_name);
			if (!v.empty())
			{
				//配列が空でなければ読み込んだデータ(配列)を返す
				return v;
			}
			else
			{
				//配列が空だった場合はエラーメッセージを出す
				std::string message = "指定された文字列" + _name + "のデータはありませんでした。";
				MessageBox(NULL, message.c_str(), "BaseProjDx9エラー", MB_OK);

				//空の配列を返す
				return {};
			}

		}
		else
		{
			//存在しなかった場合はエラーメッセージを出す
			std::string message = "指定された文字列" + _name + "が見つかりませんでした。";
			MessageBox(NULL, message.c_str(), "BaseProjDx9エラー", MB_OK);

			//空の配列を返す
			return {};
		}

	}

	/// <summary>
	/// Transformを初期化する際の共通処理
	/// </summary>
	/// <param name="_csv">読み込んだCSVインスタンス</param>
	/// <param name="_name">読み込みたいパラメータの名前</param>
	/// <param name="_tr">代入するトランスフォーム変数</param>
	void InitCSVTransform(CsvReader& _csv, const std::string& _name, Transform& _tr)
	{
		//csvからデータを取得
		std::vector<float> v = GetCSVReadData(_csv, _name);
		SetTransformPRS(_tr, v);
	}

	/// <summary>
	/// Transformをまとめて初期化する際の共通処理
	/// </summary>
	/// <param name="_csv">読み込んだCSVインスタンス</param>
	/// <param name="_names">読み込みたいパラメータの名前配列</param>
	/// <param name="_Transforms">代入するトランスフォーム配列</param>
	void InitCSVTransformArray(CsvReader& _csv, const std::vector<std::string>& _names,
		std::vector<std::reference_wrapper<Transform>>& _Transforms)
	{
		//名前配列とトランスフォーム配列数が一致しているなら続ける
		if (_names.size() != _Transforms.size())
		{
			return;
		}

		for (size_t i = 0; i < _names.size(); i++)
		{
			InitCSVTransform(_csv, _names[i], _Transforms[i]);
		}
	}

	/// <summary>
	/// GameObjectの共通データ初期化
	/// </summary>
    static void CSVCommonDataInitialize() {  

		//csvファイルを読み込む
        CsvReader csv;  
        csv.Load("CSVdata\\EngineData\\CommonData.csv");  

		//csvファイルの各0列目の文字列を取得
		std::string common = "CommonData";

		//0列目の文字列を渡し、その行のパラメータを取得
		std::vector<float> commondata = GetCSVReadData(csv,common);
            
		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		DeltaTime = commondata[i_deltatime];
        oneSecond = static_cast<int>(commondata[i_onesecond]);
        TenDivision = static_cast<int>(commondata[i_tendivision]);
		SceneShortTransition = static_cast<int>(commondata[i_sceneshorttransition]);
        SceneTransition = static_cast<int>(commondata[i_scenetransition]);
		SceneLongTransition = static_cast<int>(commondata[i_scenelongtransition]);
		ZeroPointOne = commondata[i_zeropointone];
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


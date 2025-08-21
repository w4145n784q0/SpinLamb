#include "gameObject.h"
#include <assert.h>
#include "global.h"

//複数定義防止のためグローバルスコープで初期化
float GameObject::DeltaTime = 0.0f;
int GameObject::oneSecond = 0;
int GameObject::TenDivision = 0;
int GameObject::SceneShortTransition = 0;
int GameObject::SceneTransition = 0;
int GameObject::SceneLongTransition = 0;
float GameObject::ZeroPointOne = 0.0f;

namespace
{
	//csv読み込み時のインデックス

//トランスフォームの初期化値
	enum InitTransform
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

	//GameObject軽傷先で汎用的に使用する変数
	enum CommonIndex
	{
		i_DeltaTime = 0,
		i_OneSecond,
		i_TenDivision,
		i_SceneShortTransition,
		i_SceneTransition,
		i_SceneLongTransition,
		i_ZeroPointOne,
	};
}

//コンストラクタ（親も名前もなし）
GameObject::GameObject(void) :
	GameObject(nullptr, "")
{

}

//コンストラクタ（名前なし）
GameObject::GameObject(GameObject * parent) :
	GameObject(parent, "")
{
}

//コンストラクタ（標準）
GameObject::GameObject(GameObject * parent, const std::string& name)
	: pParent_(parent),objectName_(name)
{
	childList_.clear();
	state_ = { 0, 1, 1, 0 };

	if(parent)
		transform_.pParent_ = &parent->transform_;

}

//デストラクタ
GameObject::~GameObject()
{
	for (auto it = colliderList_.begin(); it != colliderList_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	colliderList_.clear();
}

// 削除するかどうか
bool GameObject::IsDead()
{
	return (state_.dead != 0);
}

// 自分を削除する
void GameObject::KillMe()
{
	state_.dead = 1;
}

// Updateを許可
void GameObject::Enter()
{
	state_.entered = 1;
}

// Updateを拒否
void GameObject::Leave()
{
	state_.entered = 0;
}

// Drawを許可
void GameObject::Visible()
{
	state_.visible = 1;
}

// Drawを拒否
void GameObject::Invisible()
{
	state_.visible = 0;
}

// 初期化済みかどうか
bool GameObject::IsInitialized()
{
	return (state_.initialized != 0);
}

// 初期化済みにする
void GameObject::SetInitialized()
{
	state_.initialized = 1;
}

// Update実行していいか
bool GameObject::IsEntered()
{
	return (state_.entered != 0);
}

// Draw実行していいか
bool GameObject::IsVisibled()
{
	return (state_.visible != 0);
}

//子オブジェクトリストを取得
std::list<GameObject*>* GameObject::GetChildList()
{
	return &childList_;
}

//親オブジェクトを取得
GameObject * GameObject::GetParent(void)
{
	return pParent_;
}

//名前でオブジェクトを検索（対象は自分の子供以下）
GameObject * GameObject::FindChildObject(const std::string & name)
{
	//子供がいないなら終わり
	if (childList_.empty())
		return nullptr;

	//イテレータ
	auto it = childList_.begin();	//先頭
	auto end = childList_.end();	//末尾

	//子オブジェクトから探す
	while (it != end) {
		//同じ名前のオブジェクトを見つけたらそれを返す
		if ((*it)->GetObjectName() == name)
			return *it;

		//その子供（孫）以降にいないか探す
		GameObject* obj = (*it)->FindChildObject(name);
		if (obj != nullptr)
		{
			return obj;
		}

		//次の子へ
		it++;
	}

	//見つからなかった
	return nullptr;
}

//オブジェクトの名前を取得
const std::string& GameObject::GetObjectName(void) const
{
	return objectName_;
}

//子オブジェクトを追加（リストの最後へ）
void GameObject::PushBackChild(GameObject * obj)
{
	assert(obj != nullptr);

	obj->pParent_ = this;
	childList_.push_back(obj);
}

//子オブジェクトを追加（リストの先頭へ）
void GameObject::PushFrontChild(GameObject * obj)
{
	assert(obj != nullptr);

	obj->pParent_ = this;
	childList_.push_front(obj);
}

//子オブジェクトを全て削除
void GameObject::KillAllChildren(void)
{
	//子供がいないなら終わり
	if (childList_.empty())
		return;

	//イテレータ
	auto it = childList_.begin();	//先頭
	auto end = childList_.end();	//末尾

	//子オブジェクトを1個ずつ削除
	while (it != end)
	{
		KillObjectSub(*it);
		delete *it;
		it = childList_.erase(it);
	}

	//リストをクリア
	childList_.clear();
}

//オブジェクト削除（再帰）
void GameObject::KillObjectSub(GameObject * obj)
{
	if (!childList_.empty())
	{
		auto list = obj->GetChildList();
		auto it = list->begin();
		auto end = list->end();
		while (it != end)
		{
			KillObjectSub(*it);
			delete *it;
			it = list->erase(it);
		}
		list->clear();
	}
	obj->Release();
}



//コライダー（衝突判定）を追加する
void GameObject::AddCollider(Collider* collider)
{
	collider->SetGameObject(this);
	colliderList_.push_back(collider);
}

//コライダー（衝突判定）を削除
void GameObject::ClearCollider()
{
	for (auto it = colliderList_.begin(); it != colliderList_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	colliderList_.clear();
}

//衝突判定
void GameObject::Collision(GameObject * pTarget)
{
	//自分同士の当たり判定はしない
	if (pTarget == nullptr || this == pTarget)
	{
		return;
	}

	//自分とpTargetのコリジョン情報を使って当たり判定
	//1つのオブジェクトが複数のコリジョン情報を持ってる場合もあるので二重ループ
	for (auto i = this->colliderList_.begin(); i != this->colliderList_.end(); i++)
	{
		for (auto j = pTarget->colliderList_.begin(); j != pTarget->colliderList_.end(); j++)
		{
			if ((*i)->IsHit(*j))
			{
				//当たった
				this->OnCollision(pTarget);
			}
		}
	}

	//子供がいないなら終わり
	if (pTarget->childList_.empty())
		return;

	//子供も当たり判定
	for (auto i = pTarget->childList_.begin(); i != pTarget->childList_.end(); i++)
	{
		Collision(*i);
	}
}


//テスト用の衝突判定枠を表示
void GameObject::CollisionDraw()
{
	Direct3D::SetShader(Direct3D::SHADER_UNLIT);

	for (auto i = this->colliderList_.begin(); i != this->colliderList_.end(); i++)
	{
		(*i)->Draw(GetWorldPosition());
	}

	Direct3D::SetShader(Direct3D::SHADER_3D);
}

//RootJobを取得
GameObject * GameObject::GetRootJob()
{
	if (GetParent() == nullptr)
	{
		return this;
	}
	else return GetParent()->GetRootJob();
}

float GameObject::LinearInterpolation(float _convert, float _originalMin, float _originalMax, float _conversionMin, float _conversionMax)
{
	//線形補完の計算
	//y = y1 + (x - x1) * (y2 - y1) / (x2 - x1) に基づく

	if (_originalMin == _originalMax)
	{
		return _conversionMin; // originalMinとoriginalMaxが同じ場合はconversionMinを返す(0除算対策)
	}
	if (_convert < _originalMin)
	{
		return _conversionMin; // convertがoriginalMinより小さい場合はconversionMinを返す(外挿対策)
	}
	if (_convert > _originalMax)
	{
		return _conversionMax; // convertがoriginalMaxより大きい場合はconversionMaxを返す(外挿対策)
	}

	float y = _conversionMin + (_convert - _originalMin) * (_conversionMax - _conversionMin) / (_originalMax - _originalMin);
	return y;
}

float GameObject::Normalize(float _value, float _min, float _max)
{
	//値を0から1の範囲に正規化

	//_minと_maxが同じ場合は0を返す(0除算対策)
	if (_min == _max)
	{
		return 0.0f;
	}

	//minとmaxの順序が逆なら入れ替える
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

void GameObject::SetTransformPRS(Transform& _tr, std::vector<float> _v)
{
	//pos_xは0から始まる整数で、Transformデータ配列の添え字となる
	//Transformの各要素のx,y,zに_vの値を順番に入れていく

	_tr.position_ = { _v[pos_x],_v[pos_y],_v[pos_z] };
	_tr.rotate_ = { _v[rot_x], _v[rot_y],_v[rot_z] };
	_tr.scale_ = { _v[sca_x] ,_v[sca_y],_v[sca_z] };
}

std::vector<float> GameObject::GetCSVReadData(CsvReader& _csv, const std::string& _name)
{
	//csvファイル内から受け取った文字列を読み込み、配列で返す

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

void GameObject::InitCSVTransform(CsvReader& _csv, const std::string& _name, Transform& _tr)
{
	//csvからデータを取得
	std::vector<float> v = GetCSVReadData(_csv, _name);
	SetTransformPRS(_tr, v);
}

void GameObject::InitCSVTransformArray(CsvReader& _csv, const std::vector<std::string>& _names, std::vector<std::reference_wrapper<Transform>>& _Transforms)
{
	//InitCSVTransform(トランスフォームの初期化)を配列単位でまとめて初期化する

	//名前配列とトランスフォーム配列数が一致している必要あり
	if (_names.size() != _Transforms.size())
	{
		return;
	}

	//名前配列とトランスフォーム配列の数だけInitCSVTransformを呼び出す
	for (size_t i = 0; i < _names.size(); i++)
	{
		InitCSVTransform(_csv, _names[i], _Transforms[i]);
	}
}

void GameObject::CSVCommonDataInitialize()
{
	//GameObjectで共通する汎用データの初期化

	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVdata\\EngineData\\CommonData.csv");

	//csvファイルの各0列目の文字列を取得
	std::string Common = "CommonData";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> CommonData = GetCSVReadData(csv, Common);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	DeltaTime = CommonData[i_DeltaTime];
	oneSecond = static_cast<int>(CommonData[i_OneSecond]);
	TenDivision = static_cast<int>(CommonData[i_TenDivision]);
	SceneShortTransition = static_cast<int>(CommonData[i_SceneShortTransition]);
	SceneTransition = static_cast<int>(CommonData[i_SceneTransition]);
	SceneLongTransition = static_cast<int>(CommonData[i_SceneLongTransition]);
	ZeroPointOne = CommonData[i_ZeroPointOne];
}




void GameObject::UpdateSub()
{
	Update();
	Transform();

	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->UpdateSub();
	}

	for (auto it = childList_.begin(); it != childList_.end();)
	{
		if ((*it)->IsDead() == true)
		{
			(*it)->ReleaseSub();
			SAFE_DELETE(*it);
			it = childList_.erase(it);
		}
		else
		{
			//当たり判定
			(*it)->Collision(GetParent());
			it++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();


	//リリース時は削除
#ifdef _DEBUG
		//コリジョンの描画
	if (Direct3D::isDrawCollision_)
	{
		CollisionDraw();
	}
#endif

	//その子オブジェクトの描画処理
	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	//コライダーを削除
	ClearCollider();


	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->ReleaseSub();
		SAFE_DELETE(*it);
	}

	Release();
}

//ワールド行列の取得（親の影響を受けた最終的な行列）
XMMATRIX GameObject::GetWorldMatrix(void)
{
	return transform_.GetWorldMatrix();
}



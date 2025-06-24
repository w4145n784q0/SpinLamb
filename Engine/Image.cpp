#include "Global.h"
#include "Image.h"
#include"../Engine/CsvReader.h"

//3D画像を管理する
namespace Image
{
	enum ImageIndex
	{
		i_leftedge = 0,
		i_rightedge,
		i_upedge,
		i_downedge,
		i_center,
		i_alphamin,
	};

	float LeftEdge = 0.0f;//画像の左端
	float RightEdge = 0.0f;//画像の右端
	float UpEdge = 0.0f;//画像の上端
	float DownEdge = 0.0f;//画像の下端
	float Center = 0.0f;//画像の中央
	int AlphaMin = 0;//透過度の最低値

	//ロード済みの画像データ一覧
	std::vector<ImageData*>	_datas;

	//初期化
	void Initialize()
	{
		AllRelease();
	}

	//画像をロード
	int Load(std::string fileName)
	{
		ImageData* pData = new ImageData;

		//開いたファイル一覧から同じファイル名のものが無いか探す
		bool isExist = false;
		for (int i = 0; i < _datas.size(); i++)
		{
			//すでに開いている場合
			if (_datas[i] != nullptr && _datas[i]->fileName == fileName)
			{
				pData->pSprite = _datas[i]->pSprite;
				isExist = true;
				break;
			}
		}

		//新たにファイルを開く
		if (isExist == false)
		{
			pData->pSprite = new Sprite;
			if (FAILED(pData->pSprite->Load(fileName)))
			{
				//開けなかった
				SAFE_DELETE(pData->pSprite);
				SAFE_DELETE(pData);
				return -1;
			}

			//無事開けた
			pData->fileName = fileName;
		}


		//使ってない番号が無いか探す
		for (int i = 0; i < _datas.size(); i++)
		{
			if (_datas[i] == nullptr)
			{
				_datas[i] = pData;
				return i;
			}
		}

		//新たに追加
		_datas.push_back(pData);

		//画像番号割り振り
		int handle = (int)_datas.size() - 1;

		//切り抜き範囲をリセット
		ResetRect(handle);

		return handle;
	}



	//描画
	void Draw(int handle)
	{
		if (handle < 0 || handle >= _datas.size() || _datas[handle] == nullptr)
		{
			return;
		}
		_datas[handle]->transform.Calclation();
		_datas[handle]->pSprite->Draw(_datas[handle]->transform, _datas[handle]->rect, _datas[handle]->alpha);
	}



	//任意の画像を開放
	void Release(int handle)
	{
		if (handle < 0 || handle >= _datas.size())
		{
			return;
		}

		//同じモデルを他でも使っていないか
		bool isExist = false;
		for (int i = 0; i < _datas.size(); i++)
		{
			//すでに開いている場合
			if (_datas[i] != nullptr && i != handle && _datas[i]->pSprite == _datas[handle]->pSprite)
			{
				isExist = true;
				break;
			}
		}

		//使ってなければモデル解放
		if (isExist == false)
		{
			SAFE_DELETE(_datas[handle]->pSprite);
		}

		SAFE_DELETE(_datas[handle]);
	}



	//全ての画像を解放
	void AllRelease()
	{
		for (int i = 0; i < _datas.size(); i++)
		{
			Release(i);
		}
		_datas.clear();
	}


	//切り抜き範囲の設定
	void SetRect(int handle, int x, int y, int width, int height)
	{
		if (handle < 0 || handle >= _datas.size())
		{
			return;
		}

		_datas[handle]->rect.left = x;
		_datas[handle]->rect.top = y;
		_datas[handle]->rect.right = width;
		_datas[handle]->rect.bottom = height;
	}


	//切り抜き範囲をリセット（画像全体を表示する）
	void ResetRect(int handle)
	{
		if (handle < 0 || handle >= _datas.size())
		{
			return;
		}

		XMFLOAT3 size = _datas[handle]->pSprite->GetTextureSize();

		_datas[handle]->rect.left = 0;
		_datas[handle]->rect.top = 0;
		_datas[handle]->rect.right = (long)size.x;
		_datas[handle]->rect.bottom = (long)size.y;

	}

	//アルファ値設定
	void SetAlpha(int handle, int alpha)
	{
		if (handle < 0 || handle >= _datas.size())
		{
			return;
		}
		_datas[handle]->alpha = (float)alpha / 255.0f;
	}


	//ワールド行列を設定
	void SetTransform(int handle, Transform& transform)
	{
		if (handle < 0 || handle >= _datas.size())
		{
			return;
		}

		_datas[handle]->transform = transform;
	}


	//ワールド行列の取得
	XMMATRIX GetMatrix(int handle)
	{
		if (handle < 0 || handle >= _datas.size())
		{
			return XMMatrixIdentity();
		}
		return _datas[handle]->transform.GetWorldMatrix();
	}

	void SetSCVImage()
	{
		CsvReader csv;
		csv.Load("CSVdata\\ImageData.csv");

		std::string imagedata = "Image";
		if (csv.IsGetParamName(imagedata))
		{
			std::vector<float> v = csv.GetParam(imagedata);
			LeftEdge = v[i_leftedge];
			RightEdge = v[i_rightedge];
			UpEdge = v[i_upedge];
			DownEdge = v[i_downedge];
			Center = v[i_center];
			AlphaMin = static_cast<int>(v[i_alphamin]);
		}
	}
}


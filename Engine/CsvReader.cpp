#include <Windows.h>
#include "CsvReader.h"



//コンストラクタ
CsvReader::CsvReader()
{
	data_.clear();
}

//デストラクタ
CsvReader::~CsvReader()
{
	//全データを開放
	for (int y = 0; y < data_.size(); y++)
	{
		for (int x = 0; x < data_[y].size(); x++)
		{
			data_[y][x].clear();
		}
	}
}

//CSVファイルのロード
bool CsvReader::Load(std::string fileName)
{
	//ファイルを開く
	HANDLE hFile;
	hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//開けなかった
	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::string message = "「" + fileName + "」が開けません。\n開いている場合は閉じてください。";
		MessageBox(NULL, message.c_str(), "BaseProjDx9エラー", MB_OK);

		return false;
	}

	//ファイルのサイズ（文字数）を調べる
	DWORD fileSize = GetFileSize(hFile, NULL);

	//すべての文字を入れられる配列を用意
	char* temp;
	temp = new char[fileSize];

	//ファイルの中身を配列に読み込む
	DWORD dwBytes = 0;
	ReadFile(hFile, temp, fileSize, &dwBytes, NULL);

	//開いたファイルを閉じる
	CloseHandle(hFile);

	//1行のデータを入れる配列
	std::vector<std::string>	line;

	//調べる文字の位置
	DWORD index = 0;

	//最後の文字まで繰り返す
	while (index < fileSize)
	{
		//index文字目から「,」か「改行」までの文字列を取得
		std::string val;
		GetToComma(&val, temp, &index);

		//文字数が0だったということは行末
		if (val.length() - 1 == 0)
		{
			//_dataに1行分追加
			data_.push_back(line);

			//1行データをクリア
			line.clear();

			//index++;
			continue;
		}

		//1行分のデータに追加
		line.push_back(val);
	}

	//読み込んだデータは開放する
	delete[] temp;

	//成功
	return true;
}

//「,」か「改行」までの文字列を取得
void CsvReader::GetToComma(std::string *result, std::string data, DWORD* index)
{
	//「,」まで一文字ずつresultに入れる
	while (data[*index] != ',' && data[*index] != '\n'&& data[*index] != '\r')
	{
		*result += data[*index];
		(*index)++;
	}

	//最後に「\0」を付ける
	*result += '\0';
	(*index)++;
}

//指定した位置のデータを文字列で取得
std::string CsvReader::GetString(DWORD x, DWORD y)
{
	if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight())
		return "";

	return data_[y][x];
}

//指定した位置のデータを整数で取得
int CsvReader::GetValue(DWORD x, DWORD y)
{
	return atoi(GetString(x, y).c_str());
}

float CsvReader::GetValueFloat(DWORD x, DWORD y)
{
	return atof(GetString(x, y).c_str());
}

//ファイルの列数を取得
size_t CsvReader::GetWidth()
{
	return data_[0].size();
}

//ファイルの行数を取得
size_t CsvReader::GetHeight()
{
	return data_.size();
}

//csvの0列目に引数の文字列があれば、その値を配列にして返す
std::vector<float> CsvReader::GetParam(std::string ParamName)
{
	//受け取った文字列に\0を付けておく
	ParamName.push_back('\0');

	for (const auto& arr: data_)
	{
		//読み込んだデータ内の0列目に受け取った文字列があれば
		if (!arr.empty() && arr[0] == ParamName)
		{
			//float型配列にその行のデータを全て入れて返す
			std::vector<float> result;
			for (size_t i = 1; i < arr.size(); ++i) {
				result.push_back(std::stof(arr[i]));
			}
			return result;
		}
	}

	//見つからなかったら空の配列を返す
	return {};
}

//csvの0列目に引数の文字列があるかを判定
bool CsvReader::IsGetParamName(std::string ParamName)
{
	//受け取った文字列に\0を付けておく
	ParamName.push_back('\0');

	//読み込んだデータ内の0列目に受け取った文字列があるかどうかを返す
	for (const auto& arr : data_)
	{
		if (!arr.empty() && arr[0] == ParamName)
		{
			return true;
		}
	}
	return false;
}

#include <xaudio2.h>
#include <vector>
#include "Audio.h"
#include"CsvReader.h"
#include"GameObject.h"

#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p = nullptr;}

namespace Audio
{
	//サウンドを鳴らす回数を初期化するインデックス
	enum SoundNumIndex
	{
		i_start = 0,
		i_select,
		i_decide,
		i_cancel,
		i_whistle,
		i_charge,
		i_attack,
		i_collision,
	};

	//SEを鳴らす回数(効果音は同じものを一度に複数鳴らすことを想定)
	//各クラスではなくAudioで持っておき、余剰な初期化を減らしておく
	//音そのものの読み込みは各クラスで行い、シーン遷移ごとに解放
	
	//----------汎用的に使うSE----------
	int StartSoundNum_ = 0;//スタート音を同時に鳴らす回数
	int SelectSoundNum_ = 0;//選択音を同時に鳴らす回数
	int DecideSoundNum_ = 0;//決定音を同時に鳴らす回数
	int CancelSoundNum_ = 0;//キャンセル音を同時に鳴らす回数

	//----------ゲームプレイ中のSE----------
	int WhistleSoundNum_ = 0;//ホイッスル音を同時に鳴らす回数
	int	ChargeSoundNum_ = 0;//チャージ音を同時に鳴らす回数
	int AttackSoundNum_ = 0;//攻撃音を同時に鳴らす回数
	int CollisionSoundNum_ = 0;//ヒット音を同時に鳴らす回数

	//XAudio本体
	IXAudio2* pXAudio = nullptr;

	//マスターボイス
	IXAudio2MasteringVoice* pMasteringVoice = nullptr;

	//ファイル毎に必要な情報
	struct AudioData
	{
		//サウンド情報
		XAUDIO2_BUFFER buf = {};

		//ソースボイス
		IXAudio2SourceVoice** pSourceVoice = nullptr;

		//同時再生最大数
		int svNum;

		//ファイル名
		std::string fileName;
	};
	std::vector<AudioData>	audioDatas;
}

//初期化
void Audio::Initialize()
{
	CoInitializeEx(0, COINIT_MULTITHREADED);

	XAudio2Create(&pXAudio);
	pXAudio->CreateMasteringVoice(&pMasteringVoice);

	InitCSVAudio();
}

//サウンドファイル(.wav）をロード
int Audio::Load(std::string fileName, bool isLoop, int svNum)
{
	//すでに同じファイルを使ってないかチェック
	for (int i = 0; i < audioDatas.size(); i++)
	{
		if (audioDatas[i].fileName == fileName)
		{
			return i;
		}
	}

	//チャンク構造体
	struct Chunk
	{
		char	id[5] =""; 			// ID
		unsigned int	size = 0;	// サイズ
	};

	//ファイルを開く
	HANDLE hFile;
	hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwBytes = 0;

	Chunk riffChunk = { 0 };
	ReadFile(hFile, &riffChunk.id, 4, &dwBytes, NULL);
	ReadFile(hFile, &riffChunk.size, 4, &dwBytes, NULL);

	char wave[4] = "";
	ReadFile(hFile, &wave, 4, &dwBytes, NULL);

	Chunk formatChunk = { 0 };
	while (formatChunk.id[0] != 'f') {
		ReadFile(hFile, &formatChunk.id, 4, &dwBytes, NULL);
	}
	ReadFile(hFile, &formatChunk.size, 4, &dwBytes, NULL);


	//フォーマットを読み取る
	//https://learn.microsoft.com/ja-jp/windows/win32/api/mmeapi/ns-mmeapi-waveformatex
	WAVEFORMATEX fmt;
	ReadFile(hFile, &fmt.wFormatTag, 2, &dwBytes, NULL);		//形式
	ReadFile(hFile, &fmt.nChannels, 2, &dwBytes, NULL);			//チャンネル（モノラル/ステレオ）
	ReadFile(hFile, &fmt.nSamplesPerSec, 4, &dwBytes, NULL);	//サンプリング数
	ReadFile(hFile, &fmt.nAvgBytesPerSec, 4, &dwBytes, NULL);	//1秒あたりのバイト数
	ReadFile(hFile, &fmt.nBlockAlign, 2, &dwBytes, NULL);		//ブロック配置
	ReadFile(hFile, &fmt.wBitsPerSample, 2, &dwBytes, NULL);	//サンプル当たりのビット数



	//波形データの読み込み
	Chunk data = { 0 };
	while (true)
	{
		//次のデータのIDを調べる
		ReadFile(hFile, &data.id, 4, &dwBytes, NULL);

		//「data」だったらループを抜けて次に進む
		if (strcmp(data.id, "data") == 0)
			break;

		//それ以外の情報ならサイズ調べて読み込む→使わない
		else
		{
			//サイズ調べて
			ReadFile(hFile, &data.size, 4, &dwBytes, NULL);
			char* pBuffer = new char[data.size];

			//無駄に読み込む
			ReadFile(hFile, pBuffer, data.size, &dwBytes, NULL);
		}
	}

	//データチャンクのサイズを取得
	ReadFile(hFile, &data.size, 4, &dwBytes, NULL);

	//波形データを読み込む
	char* pBuffer = new char[data.size];
	ReadFile(hFile, pBuffer, data.size, &dwBytes, NULL);
	CloseHandle(hFile);


	AudioData ad;

	ad.fileName = fileName;

	ad.buf.pAudioData = (BYTE*)pBuffer;
	ad.buf.Flags = XAUDIO2_END_OF_STREAM;

	if (isLoop)	ad.buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	ad.buf.AudioBytes = data.size;


	ad.pSourceVoice = new IXAudio2SourceVoice * [svNum];

	for (int i = 0; i < svNum; i++)
	{
		pXAudio->CreateSourceVoice(&ad.pSourceVoice[i], &fmt);
	}
	ad.svNum = svNum;
	audioDatas.push_back(ad);

	//SAFE_DELETE_ARRAY(pBuffer);

	return (int)audioDatas.size() - 1;
}

//再生
void Audio::Play(int ID)
{
	for (int i = 0; i < audioDatas[ID].svNum; i++)
	{
		XAUDIO2_VOICE_STATE state;
		audioDatas[ID].pSourceVoice[i]->GetState(&state);

		if (state.BuffersQueued == 0)
		{
			audioDatas[ID].pSourceVoice[i]->SubmitSourceBuffer(&audioDatas[ID].buf);
			audioDatas[ID].pSourceVoice[i]->Start();
			break;
		}
	}
}


void Audio::Stop(int ID)
{
	for (int i = 0; i < audioDatas[ID].svNum; i++)
	{
		audioDatas[ID].pSourceVoice[i]->Stop();
		audioDatas[ID].pSourceVoice[i]->FlushSourceBuffers();
	}
}

//シーンごとの解放
void Audio::Release()
{
	for (int i = 0; i < audioDatas.size(); i++)
	{
		for (int j = 0; j < audioDatas[i].svNum; j++)
		{
			audioDatas[i].pSourceVoice[j]->DestroyVoice();
		}
		SAFE_DELETE_ARRAY(audioDatas[i].buf.pAudioData);
	}
	audioDatas.clear();
}

//本体の解放
void Audio::AllRelease()
{
	CoUninitialize();
	if (pMasteringVoice)
	{
		pMasteringVoice->DestroyVoice();
	}
	pXAudio->Release();
}

void Audio::InitCSVAudio()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVData\\EngineData\\SoundData.csv");

	//csvファイルの0列目の文字列を取得
	std::string soundName = "SoundParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> soundData = GameObject::GetCSVReadData(csv, soundName);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	StartSoundNum_ = static_cast<int>(soundData[i_start]);
	SelectSoundNum_ = static_cast<int>(soundData[i_select]);
	DecideSoundNum_ = static_cast<int>(soundData[i_decide]);
	CancelSoundNum_ = static_cast<int>(soundData[i_cancel]);
	WhistleSoundNum_ = static_cast<int>(soundData[i_whistle]);
	ChargeSoundNum_ = static_cast<int>(soundData[i_charge]);
	AttackSoundNum_ = static_cast<int>(soundData[i_attack]);
	CollisionSoundNum_ = static_cast<int>(soundData[i_collision]);
}

int Audio::GetStartNum()
{
	return StartSoundNum_;
}

int Audio::GetSelectNum()
{
	return SelectSoundNum_;
}

int Audio::GetDecideNum()
{
	return 	DecideSoundNum_;
}

int Audio::GetCancelNum()
{
	return CancelSoundNum_;
}

int Audio::GetWhistleNum()
{
	return WhistleSoundNum_;
}

int Audio::GetChargeNum()
{
	return ChargeSoundNum_;
}

int Audio::GetAttackNum()
{
	return AttackSoundNum_;
}

int Audio::GetCollisionNum()
{
	return CollisionSoundNum_;
}

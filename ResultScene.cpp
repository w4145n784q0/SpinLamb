#include "ResultScene.h"
#include"Engine/CsvReader.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"



namespace
{
	int playerscore;
	int enemyscore;
}

ResultScene::ResultScene(GameObject* parent)
	:BaseScene(parent, "ResultScene"), hBackScreen_(-1), hYouWin_(-1),hCpuWin_(-1),
	hDraw_(-1), hlogoTitle_(-1),
	hSoundResult_(-1), hSoundBackTitle_(-1), winner_(RESULTMAX), ResultArray_({})
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	SetCSVResult();

	hBackScreen_ = Image::Load("Image\\GameMode\\back_mode.jpg");
	assert(hBackScreen_ >= 0);

	hYouWin_ = Image::Load("Image\\Result\\YouWin.png");
	assert(hYouWin_ >= 0);

	hCpuWin_ = Image::Load("Image\\Result\\CPUWin.png");
	assert(hCpuWin_ >= 0);

	hDraw_ = Image::Load("Image\\Result\\Draw.png");
	assert(hDraw_ >= 0);

	hlogoTitle_ = Image::Load("Image\\Result\\PushToTitle.png");
	assert(hlogoTitle_ >= 0);

	hSoundResult_ = Audio::Load("Sound\\BGM\\end.wav", true);
	assert(hSoundResult_ >= 0);

	hSoundBackTitle_ = Audio::Load("Sound\\SE\\backtitle.wav", false);
	assert(hSoundBackTitle_ >= 0);

	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	playerscore =  pSceneManager->GetPlayerScore();
	enemyscore = pSceneManager->GetEnemyScore();

	if (playerscore > enemyscore)
	{
		winner_ = YOU_WIN;
	}
	else if (playerscore < enemyscore)
	{
		winner_ = CPU_WIN;
	}
	else if (playerscore == enemyscore)
	{
		winner_ = DRAW;
	}

	ResultArray_ = { hYouWin_, hCpuWin_, hDraw_ };
}

void ResultScene::Update()
{
	BaseScene::Update();
	Audio::Play(hSoundResult_);
}

void ResultScene::Draw()
{
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);

	Image::SetTransform(hlogoTitle_, PushTitle_);
	Image::Draw(hlogoTitle_);

	switch (winner_)
	{
	case ResultScene::YOU_WIN:
	case ResultScene::CPU_WIN:
	case ResultScene::DRAW:
	{
		Image::SetTransform(ResultArray_[winner_], Result_);
		Image::Draw(ResultArray_[winner_]);
	}
		break;
	default:
		break;
	}
	

#ifdef _DEBUG
#endif
}

void ResultScene::Release()
{
}

void ResultScene::SetCSVResult()
{
	CsvReader csv;
	csv.Load("CSVdata\\SceneData.csv");

	std::string result = "Result";
	if (csv.IsGetParamName(result))
	{
		std::vector<float> v = csv.GetParam(result);
		SetTransformPRS(Result_, v);
	}

	std::string push = "PushTitle";
	if (csv.IsGetParamName(push))
	{
		std::vector<float> v = csv.GetParam(push);
		SetTransformPRS(PushTitle_, v);
	}
}

void ResultScene::UpdateActive()
{
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;
		Audio::Play(hSoundBackTitle_);
	}
}

void ResultScene::UpdateTransition()
{
	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
		SceneTransitionTimer_ = 0;
		Audio::Stop(hSoundResult_);
		SceneState_ = S_Active;
	}
}

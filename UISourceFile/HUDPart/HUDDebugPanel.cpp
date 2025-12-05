#include "HUDDebugPanel.h"
#include"../HUD.h"
#include"../../Engine/Image.h"
#include"../../Engine/Global.h"

HUDDebugPanel::HUDDebugPanel(GameObject* parent)
	:GameObject(parent, "HUDDebugPanel")
{
}

void HUDDebugPanel::ImGuiDrawer(HUD* _hud)
{
	//シーンクラスからの指示によって呼ぶ描画関数を変える
	switch (_hud->GetDrawMode())
	{
	case Mode_BeforeStart:
	{
		ImGuiExplanationDraw(_hud);
	}
	break;
	case Mode_JustBefore:
	{
		ImGuiScoreDraw(_hud);
		ImGuiStartLogoDraw(_hud);
	}
	break;
	case Mode_Playing:
	{
		ImGuiScoreDraw(_hud);
		ImGuiTimerDraw(_hud);
	}
	break;
	case Mode_PlayPause:
	{
		ImGuiScoreDraw(_hud);
		ImGuiTimerDraw(_hud);
		ImGuiPauseDraw(_hud);
	}
	break;
	case Mode_Finish:
	{
		ImGuiTimerDraw(_hud);
		ImGuiFinishLogoDraw(_hud);
		ImGuiScoreDraw(_hud);
	}
	break;
	case Mode_Practice:
	{
		ImGuiPracticeLogoDraw(_hud);
	}
	break;
	case Mode_PracticePause:
	{
		ImGuiPracticeLogoDraw(_hud);
		ImGuiPauseDraw(_hud);
	}
	break;
	default:
		break;
	}

	//常に表示するものはswitch文の外で記述
	ImGuiMiniMapDraw(_hud);
}

void HUDDebugPanel::ImGuiExplanationDraw(HUD* _hud)
{
#ifdef _DEBUG
	if (!_hud) return;

	auto& t = _hud->HUDParam_->GetLogoExplanation();

	//バトル開始前の説明
	if (ImGui::TreeNode("Explanation"))
	{
		if (ImGui::TreeNode("ExplanationPosition"))
		{
			ImGui::SliderFloat("Explanation_PositionX", &t.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("Explanation_PositionY", &t.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ExplanationRotate"))
		{
			ImGui::SliderFloat("ExplanationRotateX", &t.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("ExplanationRotateY", &t.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("ExplanationRotateZ", &t.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ExplanationScale"))
		{
			ImGui::InputFloat("ExplanationScaleX", &t.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("ExplanationScaleY", &t.scale_.y, ZERO_POINT_ONE);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif
}

void HUDDebugPanel::ImGuiStartLogoDraw(HUD* _hud)
{
#ifdef _DEBUG
	if (!_hud) return;

	auto& t = _hud->HUDParam_->GetLogoStart();

	//"Ready"と"Go"の文字
	if (ImGui::TreeNode("Start"))
	{
		if (ImGui::TreeNode("StartPosition"))
		{
			ImGui::SliderFloat("StartPositionX", &t.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("StartPositionY", &t.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("StartRotate"))
		{
			ImGui::SliderFloat("StartRotateX", &t.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("StartRotateY", &t.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("StartRotateZ", &t.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("StartScale"))
		{
			ImGui::InputFloat("StartScaleX", &t.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("StartScaleY", &t.scale_.y, ZERO_POINT_ONE);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}

void HUDDebugPanel::ImGuiFinishLogoDraw(HUD* _hud)
{
#ifdef _DEBUG
	if (!_hud) return;

	auto& t = _hud->HUDParam_->GetLogoFinish();

	//"Finish!"の文字
	if (ImGui::TreeNode("Finish"))
	{
		if (ImGui::TreeNode("FinishPosition"))
		{
			ImGui::SliderFloat("FinishPositionX", &t.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("FinishPositionY", &t.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("FinishRotate"))
		{
			ImGui::SliderFloat("FinishRotateX", &t.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("FinishRotateY", &t.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("FinishRotateZ", &t.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("FinishScale"))
		{
			ImGui::InputFloat("FinishScaleX", &t.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("FinishScaleY", &t.scale_.y, ZERO_POINT_ONE);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}

void HUDDebugPanel::ImGuiPracticeLogoDraw(HUD* _hud)
{
#ifdef _DEBUG
	if (!_hud) return;

	auto& t = _hud->HUDParam_->GetLogoPractice();

	//"練習モード"の文字
	if (ImGui::TreeNode("PracticeLogo"))
	{
		if (ImGui::TreeNode("PracticeLogoPosition"))
		{
			ImGui::SliderFloat("Practice_PositionX", &t.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("Practice_PositionY", &t.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("PracticeLogoRotate"))
		{
			ImGui::SliderFloat("Practice_RotateX", &t.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("Practice_RotateY", &t.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("Practice_RotateZ", &t.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("PracticeLogoScale"))
		{
			ImGui::InputFloat("Practice_ScaleX", &t.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("Practice_ScaleY", &t.scale_.y, ZERO_POINT_ONE);
			ImGui::TreePop();
		}


		ImGui::TreePop();
	}
#endif
}

void HUDDebugPanel::ImGuiScoreDraw(HUD* _hud)
{
#ifdef _DEBUG
	if (!_hud) return;

	auto& firstTen = _hud->HUDParam_->GetFirstScoreTen();
	auto& firstOne = _hud->HUDParam_->GetFirstScoreOne();
	auto& secondTen = _hud->HUDParam_->GetSecondScoreTen();
	auto& secondOne = _hud->HUDParam_->GetSecondScoreOne();

	//画面上部のスコア(十の位・一の位)
	if (ImGui::TreeNode("Score"))
	{
		if (ImGui::TreeNode("ScorePosition"))
		{
			ImGui::SliderFloat("FirstScoreTen_PositionX", &firstTen.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("FirstScoreTen_PositionY", &firstTen.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::SliderFloat("FirstScoreOne_PositionX", &firstOne.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("FirstScoreOne_PositionY", &firstOne.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::SliderFloat("SecondScoreTen_PositionX", &secondTen.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("SecondScoreTen_PositionY", &secondTen.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::SliderFloat("SecondScoreOne_PositionX", &secondOne.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("SecondScoreOne_PositionY", &secondOne.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ScoreRotate"))
		{
			ImGui::SliderFloat("FirstScoreTen_RotateX", &firstTen.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("FirstScoreTen_RotateY", &firstTen.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("FirstScoreTen_RotateZ", &firstTen.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);

			ImGui::SliderFloat("FirstScoreOne_RotateX", &firstOne.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("FirstScoreOne_RotateY", &firstOne.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("FirstScoreOne_RotateZ", &firstOne.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);

			ImGui::SliderFloat("SecondScoreTen_RotateX", &secondTen.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("SecondScoreTen_RotateY", &secondTen.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("SecondScoreTen_RotateZ", &secondTen.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);

			ImGui::SliderFloat("SecondScoreOne_RotateX", &secondOne.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("SecondScoreOne_RotateY", &secondOne.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("SecondScoreOne_RotateZ", &secondOne.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ScoreScale"))
		{
			ImGui::InputFloat("FirstScoreTen_ScaleX", &firstTen.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("FirstScoreTen_ScaleY", &firstTen.scale_.y, ZERO_POINT_ONE);

			ImGui::InputFloat("FirstScoreOne_ScaleX", &firstOne.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("FirstScoreOne_ScaleY", &firstOne.scale_.y, ZERO_POINT_ONE);

			ImGui::InputFloat("SecondScoreTen_ScaleX", &secondTen.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("SecondScoreTen_ScaleY", &secondTen.scale_.y, ZERO_POINT_ONE);

			ImGui::InputFloat("SecondScoreOne_ScaleX", &secondOne.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("SecondScoreOne_ScaleY", &secondOne.scale_.y, ZERO_POINT_ONE);

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif

}

void HUDDebugPanel::ImGuiTimerDraw(HUD* _hud)
{
#ifdef _DEBUG
	if (!_hud) return;

	auto& tenTime = _hud->HUDParam_->GetTenTime();
	auto& oneTime = _hud->HUDParam_->GetOneTime();

	//タイマー(十の位・一の位)
	if (ImGui::TreeNode("Timer"))
	{
		if (ImGui::TreeNode("TimerPosition"))
		{
			ImGui::SliderFloat("TenTimePositionX", &tenTime.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("TenTimePositionY", &tenTime.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::SliderFloat("OneTimePositionX", &oneTime.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("OneTimePositionY", &oneTime.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("TimerRotate"))
		{
			ImGui::SliderFloat("TenTimeRotateX", &tenTime.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("TenTimeRotateY", &tenTime.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("TenTimeRotateZ", &tenTime.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);

			ImGui::SliderFloat("OneTimeRotateX", &oneTime.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("OneTimeRotateY", &oneTime.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("OneTimeRotateZ", &oneTime.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("TimerScale"))
		{
			ImGui::InputFloat("TenTimeScaleX", &tenTime.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("TenTimeScaleY", &tenTime.scale_.y, ZERO_POINT_ONE);

			ImGui::InputFloat("OneTimeScaleX", &oneTime.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("OneTimeScaleY", &oneTime.scale_.y, ZERO_POINT_ONE);
			ImGui::TreePop();
		}

		ImGui::TreePop();
		}
#endif
	
}

void HUDDebugPanel::ImGuiMiniMapDraw(HUD* _hud)
{
#ifdef _DEBUG

	auto& mapIcon = _hud->HUDParam_->GetMapIcon();
	auto& firstIcon = _hud->HUDParam_->GetFirstIcon();
	auto& secondIcon = _hud->HUDParam_->GetSecondIcon();

	if (ImGui::TreeNode("MiniMap"))
	{
		//ミニマップ
		if (ImGui::TreeNode("MiniMapPosition"))
		{
			ImGui::SliderFloat("MiniMapPositionX", &mapIcon.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("MiniMapPositionY", &mapIcon.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("MiniMapRotate"))
		{
			ImGui::SliderFloat("MiniMapRotateX", &mapIcon.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("MiniMapRotateY", &mapIcon.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::SliderFloat("MiniMapRotateZ", &mapIcon.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("MiniMapScale"))
		{
			ImGui::InputFloat("MiniMapScaleX", &mapIcon.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("MiniMapScaleY", &mapIcon.scale_.y, ZERO_POINT_ONE);
			ImGui::TreePop();
		}

		//Player1アイコン
		ImGui::Text("FirstIcon: (%.3f, %.3f)", firstIcon.position_.x, firstIcon.position_.y);

		//Player2,Enemyアイコン
		ImGui::Text("SecondIcon: (%.3f, %.3f)", secondIcon.position_.x, secondIcon.position_.y);

		ImGui::TreePop();
	}
#endif

}

void HUDDebugPanel::ImGuiPauseDraw(HUD* _hud)
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Pause"))
	{
		auto& pauseMenu = _hud->HUDParam_->GetPauseMenu();
		auto& pauseIcon = _hud->HUDParam_->GetPauseIcon();

		//ポーズメニュー
		if (ImGui::TreeNode("PauseMenu"))
		{
			if (ImGui::TreeNode("PauseMenuPosition"))
			{
				ImGui::SliderFloat("PauseMenuPositionX", &pauseMenu.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("PauseMenuPositionY", &pauseMenu.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PauseMenuRotate"))
			{
				ImGui::SliderFloat("PauseMenuRotateX", &pauseMenu.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("PauseMenuRotateY", &pauseMenu.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("PauseMenuRotateZ", &pauseMenu.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PauseMenuScale"))
			{
				ImGui::InputFloat("PauseMenuScaleX", &pauseMenu.scale_.x, ZERO_POINT_ONE);
				ImGui::InputFloat("PauseMenuScaleY", &pauseMenu.scale_.y, ZERO_POINT_ONE);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//ポーズアイコン
		if (ImGui::TreeNode("PauseIcon"))
		{
			if (ImGui::TreeNode("PauseIconPosition"))
			{
				ImGui::SliderFloat("PauseIconPositionX", &pauseIcon.position_.x, Image::LeftEdge, Image::RightEdge);
				ImGui::SliderFloat("PauseIconPositionY", &pauseIcon.position_.y, Image::UpEdge, Image::DownEdge);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PauseIconRotate"))
			{
				ImGui::SliderFloat("PauseIconRotateX", &pauseIcon.rotate_.x, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("PauseIconRotateY", &pauseIcon.rotate_.y, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::SliderFloat("PauseIconRotateZ", &pauseIcon.rotate_.z, ANGLE_0_DEG, ANGLE_360_DEG);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("PauseIconScale"))
			{
				ImGui::InputFloat("PauseIconScaleX", &pauseIcon.scale_.x, ZERO_POINT_ONE);
				ImGui::InputFloat("PauseIconScaleY", &pauseIcon.scale_.y, ZERO_POINT_ONE);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}

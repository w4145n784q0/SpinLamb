#include "OutStageThing.h"
#include"Engine/Model.h"

OutStageThing::OutStageThing(GameObject* parent)
	:GameObject(parent, "OutStageThing"), hCabin_(-1), hTree_(-1), hLogs_(-1),hStraw_(-1),
	ModelHandleArray_({}), StageTransArray_({ {} })
{
}

OutStageThing::~OutStageThing()
{
}

void OutStageThing::Initialize()
{

	//各モデルの読み込み
	hCabin_ = Model::Load("Model\\House.fbx");
	assert(hCabin_ >= 0);

	hTree_ = Model::Load("Model\\LittleTree.fbx");
	assert(hTree_ >= 0);

	hLogs_ = Model::Load("Model\\Logs.fbx");
	assert(hLogs_ >= 0);

	hStraw_ = Model::Load("Model\\Straw.fbx");
	assert(hStraw_ >= 0);

	ModelHandleArray_ = { hCabin_, hTree_, hLogs_, hStraw_ };

	//モデル用トランスフォーム配列初期化
	StageTransArray_ = { &CabinData_, &TreeData_, &LogsData_, &StrawData_ };
}

void OutStageThing::Update()
{
}

void OutStageThing::Draw()
{
	//各モデル描画
	for (int i = 0; i < StageTransArray_.size(); i++)
	{
		for (int j = 0; j < (*StageTransArray_[i]).size(); j++)
		{
			Model::SetAndDraw(ModelHandleArray_[i],(*StageTransArray_[i])[j]);
		}
	}

}

void OutStageThing::Release()
{
}

void OutStageThing::DrawImGui()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("OutStageThings"))
	{
		//小屋
		if (ImGui::TreeNode("Cabin"))
		{
			if (ImGui::TreeNode("CabinPosition"))
			{
				for (int i = 0; i < CabinData_.size(); i++)
				{
					std::string labelX = "CabinPositionX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &CabinData_[i].position_.x, ZeroPointOne);

					std::string labelY = "CabinPositionY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &CabinData_[i].position_.y, ZeroPointOne);

					std::string labelZ = "CabinPositionZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &CabinData_[i].position_.z, ZeroPointOne);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("CabinRotate"))
			{
				for (int i = 0; i < CabinData_.size(); i++)
				{
					std::string labelX = "CabinRotateX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &CabinData_[i].rotate_.x, ZeroPointOne);

					std::string labelY = "CabiRotateY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &CabinData_[i].rotate_.y, ZeroPointOne);

					std::string labelZ = "CabinRotateZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &CabinData_[i].rotate_.z, ZeroPointOne);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("CabinScale"))
			{
				for (int i = 0; i < CabinData_.size(); i++)
				{
					std::string labelX = "CabinScaleX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &CabinData_[i].scale_.x, ZeroPointOne);

					std::string labelY = "CabiScaleY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &CabinData_[i].scale_.y, ZeroPointOne);

					std::string labelZ = "CabinScaleZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &CabinData_[i].scale_.z, ZeroPointOne);
				}
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		//木
		if (ImGui::TreeNode("Tree"))
		{
			if (ImGui::TreeNode("TreePosition"))
			{
				for (int i = 0; i < TreeData_.size(); i++)
				{
					std::string labelX = "TreePositionX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &TreeData_[i].position_.x, ZeroPointOne);

					std::string labelY = "TreePositionY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &TreeData_[i].position_.y, ZeroPointOne);

					std::string labelZ = "TreePositionZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &TreeData_[i].position_.z, ZeroPointOne);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("TreeRotate"))
			{
				for (int i = 0; i < TreeData_.size(); i++)
				{
					std::string labelX = "TreeRotateX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &TreeData_[i].rotate_.x, ZeroPointOne);

					std::string labelY = "TreeRotateY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &TreeData_[i].rotate_.y, ZeroPointOne);

					std::string labelZ = "TreeRotateZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &TreeData_[i].rotate_.z, ZeroPointOne);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("TreeScale"))
			{
				for (int i = 0; i < TreeData_.size(); i++)
				{
					std::string labelX = "TreeScaleX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &TreeData_[i].scale_.x, ZeroPointOne);

					std::string labelY = "TreeScaleY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &TreeData_[i].scale_.y, ZeroPointOne);

					std::string labelZ = "TreeScaleZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &TreeData_[i].scale_.z, ZeroPointOne);
				}

				ImGui::TreePop();
			}


			ImGui::TreePop();
		}

		//丸太
		if (ImGui::TreeNode("Logs"))
		{
			if (ImGui::TreeNode("LogsPosition"))
			{
				for (int i = 0; i < LogsData_.size(); i++)
				{
					std::string labelX = "LogsPositionX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &LogsData_[i].position_.x, ZeroPointOne);

					std::string labelY = "LogsPositionY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &LogsData_[i].position_.y, ZeroPointOne);

					std::string labelZ = "LogsPositionZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &LogsData_[i].position_.z, ZeroPointOne);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("LogsRotate"))
			{
				for (int i = 0; i < LogsData_.size(); i++)
				{
					std::string labelX = "LogsRotateX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &LogsData_[i].rotate_.x, ZeroPointOne);

					std::string labelY = "LogsRotateY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &LogsData_[i].rotate_.y, ZeroPointOne);

					std::string labelZ = "LogsRotateZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &LogsData_[i].rotate_.z, ZeroPointOne);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("LogsScale"))
			{
				for (int i = 0; i < LogsData_.size(); i++)
				{
					std::string labelX = "LogsScaleX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &LogsData_[i].scale_.x, ZeroPointOne);

					std::string labelY = "LogsScaleY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &LogsData_[i].scale_.y, ZeroPointOne);

					std::string labelZ = "LogsScaleZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &LogsData_[i].scale_.z, ZeroPointOne);
				}

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		//藁束
		if (ImGui::TreeNode("Straw"))
		{
			if (ImGui::TreeNode("StrawPosition"))
			{
				for (int i = 0; i < StrawData_.size(); i++)
				{
					std::string labelX = "StrawPositionX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &StrawData_[i].position_.x, ZeroPointOne);

					std::string labelY = "StrawPositionY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &StrawData_[i].position_.y, ZeroPointOne);

					std::string labelZ = "StrawPositionZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &StrawData_[i].position_.z, ZeroPointOne);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("StrawRotate"))
			{
				for (int i = 0; i < StrawData_.size(); i++)
				{
					std::string labelX = "StrawRotateX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &StrawData_[i].rotate_.x, ZeroPointOne);

					std::string labelY = "StrawRotateY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &StrawData_[i].rotate_.y, ZeroPointOne);

					std::string labelZ = "StrawRotateZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &StrawData_[i].rotate_.z, ZeroPointOne);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("StrawScale"))
			{
				for (int i = 0; i < StrawData_.size(); i++)
				{
					std::string labelX = "StrawScaleX_" + std::to_string(i);
					ImGui::InputFloat((labelX.c_str()), &StrawData_[i].scale_.x, ZeroPointOne);

					std::string labelY = "StrawScaleY_" + std::to_string(i);
					ImGui::InputFloat((labelY.c_str()), &StrawData_[i].scale_.y, ZeroPointOne);

					std::string labelZ = "StrawScaleZ_" + std::to_string(i);
					ImGui::InputFloat((labelZ.c_str()), &StrawData_[i].scale_.z, ZeroPointOne);
				}

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}

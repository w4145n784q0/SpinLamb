#include "gameObject.h"
#include <assert.h>
#include "global.h"

//������`�h�~�̂��߃O���[�o���X�R�[�v�ŏ�����
float GameObject::DeltaTime = 0.0f;
int GameObject::oneSecond = 0;
int GameObject::TenDivision = 0;
int GameObject::SceneShortTransition = 0;
int GameObject::SceneTransition = 0;
int GameObject::SceneLongTransition = 0;
float GameObject::ZeroPointOne = 0.0f;

namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X

//�g�����X�t�H�[���̏������l
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

	//GameObject�y����Ŕėp�I�Ɏg�p����ϐ�
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

//�R���X�g���N�^�i�e�����O���Ȃ��j
GameObject::GameObject(void) :
	GameObject(nullptr, "")
{

}

//�R���X�g���N�^�i���O�Ȃ��j
GameObject::GameObject(GameObject * parent) :
	GameObject(parent, "")
{
}

//�R���X�g���N�^�i�W���j
GameObject::GameObject(GameObject * parent, const std::string& name)
	: pParent_(parent),objectName_(name)
{
	childList_.clear();
	state_ = { 0, 1, 1, 0 };

	if(parent)
		transform_.pParent_ = &parent->transform_;

}

//�f�X�g���N�^
GameObject::~GameObject()
{
	for (auto it = colliderList_.begin(); it != colliderList_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	colliderList_.clear();
}

// �폜���邩�ǂ���
bool GameObject::IsDead()
{
	return (state_.dead != 0);
}

// �������폜����
void GameObject::KillMe()
{
	state_.dead = 1;
}

// Update������
void GameObject::Enter()
{
	state_.entered = 1;
}

// Update������
void GameObject::Leave()
{
	state_.entered = 0;
}

// Draw������
void GameObject::Visible()
{
	state_.visible = 1;
}

// Draw������
void GameObject::Invisible()
{
	state_.visible = 0;
}

// �������ς݂��ǂ���
bool GameObject::IsInitialized()
{
	return (state_.initialized != 0);
}

// �������ς݂ɂ���
void GameObject::SetInitialized()
{
	state_.initialized = 1;
}

// Update���s���Ă�����
bool GameObject::IsEntered()
{
	return (state_.entered != 0);
}

// Draw���s���Ă�����
bool GameObject::IsVisibled()
{
	return (state_.visible != 0);
}

//�q�I�u�W�F�N�g���X�g���擾
std::list<GameObject*>* GameObject::GetChildList()
{
	return &childList_;
}

//�e�I�u�W�F�N�g���擾
GameObject * GameObject::GetParent(void)
{
	return pParent_;
}

//���O�ŃI�u�W�F�N�g�������i�Ώۂ͎����̎q���ȉ��j
GameObject * GameObject::FindChildObject(const std::string & name)
{
	//�q�������Ȃ��Ȃ�I���
	if (childList_.empty())
		return nullptr;

	//�C�e���[�^
	auto it = childList_.begin();	//�擪
	auto end = childList_.end();	//����

	//�q�I�u�W�F�N�g����T��
	while (it != end) {
		//�������O�̃I�u�W�F�N�g���������炻���Ԃ�
		if ((*it)->GetObjectName() == name)
			return *it;

		//���̎q���i���j�ȍ~�ɂ��Ȃ����T��
		GameObject* obj = (*it)->FindChildObject(name);
		if (obj != nullptr)
		{
			return obj;
		}

		//���̎q��
		it++;
	}

	//������Ȃ�����
	return nullptr;
}

//�I�u�W�F�N�g�̖��O���擾
const std::string& GameObject::GetObjectName(void) const
{
	return objectName_;
}

//�q�I�u�W�F�N�g��ǉ��i���X�g�̍Ō�ցj
void GameObject::PushBackChild(GameObject * obj)
{
	assert(obj != nullptr);

	obj->pParent_ = this;
	childList_.push_back(obj);
}

//�q�I�u�W�F�N�g��ǉ��i���X�g�̐擪�ցj
void GameObject::PushFrontChild(GameObject * obj)
{
	assert(obj != nullptr);

	obj->pParent_ = this;
	childList_.push_front(obj);
}

//�q�I�u�W�F�N�g��S�č폜
void GameObject::KillAllChildren(void)
{
	//�q�������Ȃ��Ȃ�I���
	if (childList_.empty())
		return;

	//�C�e���[�^
	auto it = childList_.begin();	//�擪
	auto end = childList_.end();	//����

	//�q�I�u�W�F�N�g��1���폜
	while (it != end)
	{
		KillObjectSub(*it);
		delete *it;
		it = childList_.erase(it);
	}

	//���X�g���N���A
	childList_.clear();
}

//�I�u�W�F�N�g�폜�i�ċA�j
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



//�R���C�_�[�i�Փ˔���j��ǉ�����
void GameObject::AddCollider(Collider* collider)
{
	collider->SetGameObject(this);
	colliderList_.push_back(collider);
}

//�R���C�_�[�i�Փ˔���j���폜
void GameObject::ClearCollider()
{
	for (auto it = colliderList_.begin(); it != colliderList_.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	colliderList_.clear();
}

//�Փ˔���
void GameObject::Collision(GameObject * pTarget)
{
	//�������m�̓����蔻��͂��Ȃ�
	if (pTarget == nullptr || this == pTarget)
	{
		return;
	}

	//������pTarget�̃R���W���������g���ē����蔻��
	//1�̃I�u�W�F�N�g�������̃R���W�������������Ă�ꍇ������̂œ�d���[�v
	for (auto i = this->colliderList_.begin(); i != this->colliderList_.end(); i++)
	{
		for (auto j = pTarget->colliderList_.begin(); j != pTarget->colliderList_.end(); j++)
		{
			if ((*i)->IsHit(*j))
			{
				//��������
				this->OnCollision(pTarget);
			}
		}
	}

	//�q�������Ȃ��Ȃ�I���
	if (pTarget->childList_.empty())
		return;

	//�q���������蔻��
	for (auto i = pTarget->childList_.begin(); i != pTarget->childList_.end(); i++)
	{
		Collision(*i);
	}
}


//�e�X�g�p�̏Փ˔���g��\��
void GameObject::CollisionDraw()
{
	Direct3D::SetShader(Direct3D::SHADER_UNLIT);

	for (auto i = this->colliderList_.begin(); i != this->colliderList_.end(); i++)
	{
		(*i)->Draw(GetWorldPosition());
	}

	Direct3D::SetShader(Direct3D::SHADER_3D);
}

//RootJob���擾
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
	//���`�⊮�̌v�Z
	//y = y1 + (x - x1) * (y2 - y1) / (x2 - x1) �Ɋ�Â�

	if (_originalMin == _originalMax)
	{
		return _conversionMin; // originalMin��originalMax�������ꍇ��conversionMin��Ԃ�(0���Z�΍�)
	}
	if (_convert < _originalMin)
	{
		return _conversionMin; // convert��originalMin��菬�����ꍇ��conversionMin��Ԃ�(�O�}�΍�)
	}
	if (_convert > _originalMax)
	{
		return _conversionMax; // convert��originalMax���傫���ꍇ��conversionMax��Ԃ�(�O�}�΍�)
	}

	float y = _conversionMin + (_convert - _originalMin) * (_conversionMax - _conversionMin) / (_originalMax - _originalMin);
	return y;
}

float GameObject::Normalize(float _value, float _min, float _max)
{
	//�l��0����1�͈̔͂ɐ��K��

	//_min��_max�������ꍇ��0��Ԃ�(0���Z�΍�)
	if (_min == _max)
	{
		return 0.0f;
	}

	//min��max�̏������t�Ȃ����ւ���
	if (_min > _max)
	{
		std::swap(_min, _max);
	}

	//���K���̌v�Z(�Ώۂ̒l - �����l) / (����l - �����l)
	float normalized = (_value - _min) / (_max - _min);

	//���K�����ꂽ�l��1.0f�ȏ�~0.0f�ȉ��ɂȂ�Ȃ��悤�ɒl�𐧌�
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
	//pos_x��0����n�܂鐮���ŁATransform�f�[�^�z��̓Y�����ƂȂ�
	//Transform�̊e�v�f��x,y,z��_v�̒l�����Ԃɓ���Ă���

	_tr.position_ = { _v[pos_x],_v[pos_y],_v[pos_z] };
	_tr.rotate_ = { _v[rot_x], _v[rot_y],_v[rot_z] };
	_tr.scale_ = { _v[sca_x] ,_v[sca_y],_v[sca_z] };
}

std::vector<float> GameObject::GetCSVReadData(CsvReader& _csv, const std::string& _name)
{
	//csv�t�@�C��������󂯎�����������ǂݍ��݁A�z��ŕԂ�

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (_csv.IsGetParamName(_name))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = _csv.GetParam(_name);
		if (!v.empty())
		{
			//�z�񂪋�łȂ���Γǂݍ��񂾃f�[�^(�z��)��Ԃ�
			return v;
		}
		else
		{
			//�z�񂪋󂾂����ꍇ�̓G���[���b�Z�[�W���o��
			std::string message = "�w�肳�ꂽ������" + _name + "�̃f�[�^�͂���܂���ł����B";
			MessageBox(NULL, message.c_str(), "BaseProjDx9�G���[", MB_OK);

			//��̔z���Ԃ�
			return {};
		}

	}
	else
	{
		//���݂��Ȃ������ꍇ�̓G���[���b�Z�[�W���o��
		std::string message = "�w�肳�ꂽ������" + _name + "��������܂���ł����B";
		MessageBox(NULL, message.c_str(), "BaseProjDx9�G���[", MB_OK);

		//��̔z���Ԃ�
		return {};
	}

}

void GameObject::InitCSVTransform(CsvReader& _csv, const std::string& _name, Transform& _tr)
{
	//csv����f�[�^���擾
	std::vector<float> v = GetCSVReadData(_csv, _name);
	SetTransformPRS(_tr, v);
}

void GameObject::InitCSVTransformArray(CsvReader& _csv, const std::vector<std::string>& _names, std::vector<std::reference_wrapper<Transform>>& _Transforms)
{
	//InitCSVTransform(�g�����X�t�H�[���̏�����)��z��P�ʂł܂Ƃ߂ď���������

	//���O�z��ƃg�����X�t�H�[���z�񐔂���v���Ă���K�v����
	if (_names.size() != _Transforms.size())
	{
		return;
	}

	//���O�z��ƃg�����X�t�H�[���z��̐�����InitCSVTransform���Ăяo��
	for (size_t i = 0; i < _names.size(); i++)
	{
		InitCSVTransform(_csv, _names[i], _Transforms[i]);
	}
}

void GameObject::CSVCommonDataInitialize()
{
	//GameObject�ŋ��ʂ���ėp�f�[�^�̏�����

	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\EngineData\\CommonData.csv");

	//csv�t�@�C���̊e0��ڂ̕�������擾
	std::string Common = "CommonData";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> CommonData = GetCSVReadData(csv, Common);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
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
			//�����蔻��
			(*it)->Collision(GetParent());
			it++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();


	//�����[�X���͍폜
#ifdef _DEBUG
		//�R���W�����̕`��
	if (Direct3D::isDrawCollision_)
	{
		CollisionDraw();
	}
#endif

	//���̎q�I�u�W�F�N�g�̕`�揈��
	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	//�R���C�_�[���폜
	ClearCollider();


	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->ReleaseSub();
		SAFE_DELETE(*it);
	}

	Release();
}

//���[���h�s��̎擾�i�e�̉e�����󂯂��ŏI�I�ȍs��j
XMMATRIX GameObject::GetWorldMatrix(void)
{
	return transform_.GetWorldMatrix();
}



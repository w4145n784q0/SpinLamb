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
	//�g�����X�t�H�[���̏������l��CSV����ǂݍ��ލۂ̃C���f�b�N�X
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

	//�ėp�I�Ɏg�p����ϐ�������������C���f�b�N�X
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

//----------�I�y���[�^�[----------

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
//�S�ẴQ�[���I�u�W�F�N�g�i�V�[�����܂߂āj���p������C���^�[�t�F�[�X
// �Q�[���I�u�W�F�N�g�́A�e�q�\���ɂȂ��Ă��āA
// �}�g���N�X�̉e�����󂯂邱�ƂɂȂ�
//-----------------------------------------------------------
class GameObject
{
protected:
	
	//�ʒu������Ȃǂ��Ǘ�����I�u�W�F�N�g
	Transform				transform_;

	//�I�u�W�F�N�g�̖��O
	std::string				objectName_;

	//�Փ˔��胊�X�g
	std::list<Collider*>	colliderList_;	

	//�ȉ���GameObject�p����Ŕėp�I�Ɏg���萔

	/// <summary>
	/// 60fps�ɂ�����1�t���[���̎���
	/// �g�p�[���ɂ��t���[�����[�g�ˑ��h�~
	/// </summary>
	static float DeltaTime;

	/// <summary>
	/// 1�b��\���萔
	/// </summary>
	static int oneSecond;

	/// <summary>
	/// ���ԕ\�L���ɏ��Z����l
	/// </summary>
	static int  TenDivision;

	/// <summary>
	/// ���̃V�[���J�ڂ܂ł̎���(1.0�b��)
	/// </summary>
	static int SceneShortTransition;

	/// <summary>
	/// ���̃V�[���J�ڂ܂ł̎���(1.5�b��)
	/// </summary>
	static int SceneTransition;

	/// <summary>
	/// ���̃V�[���J�ڂ܂ł̎���(2�b��)
	/// </summary>
	static int SceneLongTransition;

	/// <summary>
	/// 0.1 ��\���萔
	/// ImGui�̑���ȂǂɎg��
	/// </summary>
	static float ZeroPointOne;

public:
	//�R���X�g���N�^
	GameObject();
	GameObject(GameObject* parent);
	GameObject(GameObject* parent, const std::string& name);

	//�f�X�g���N�^
	virtual ~GameObject();

	//�e�I�u�W�F�N�g�ŕK�����֐�
	virtual void Initialize(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw() = 0;
	virtual void Release(void) = 0;

	//�����̊Y���֐���ǂ񂾌�A�q���̊֐����Ă�
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();


	////���[�J���s��̎擾�i���̃I�u�W�F�N�g�̍s��j
	////�ߒl�F���[�J���s��
	//XMMATRIX GetLocalMatrix();

	//���[���h�s��̎擾�i�e�̉e�����󂯂��ŏI�I�ȍs��j
	//�ߒl�F���[���h�s��
	XMMATRIX GetWorldMatrix();



	//�e�t���O�̐���
	bool IsDead();			// �폜���邩�ǂ���
	void KillMe();			// �������폜����
	void Enter();			// Update������
	void Leave();			// Update������
	void Visible();			// Draw������
	void Invisible();		// Draw������
	bool IsInitialized();	// �������ς݂��ǂ���
	void SetInitialized();	// �������ς݂ɂ���
	bool IsEntered();		// Update���s���Ă�����
	bool IsVisibled();		// Draw���s���Ă�����


	//�q�I�u�W�F�N�g���X�g���擾
	//�ߒl�F�q�I�u�W�F�N�g���X�g
	std::list<GameObject*>* GetChildList();

	//�e�I�u�W�F�N�g���擾
	//�ߒl�F�e�I�u�W�F�N�g�̃A�h���X
	GameObject* GetParent();

	//���O�ŃI�u�W�F�N�g�������i�Ώۂ͎����̎q���ȉ��j
	//�����Fname	�������閼�O
	//�ߒl�F�������I�u�W�F�N�g�̃A�h���X�i������Ȃ����nullptr�j
	GameObject* FindChildObject(const std::string& name);

	//���O�ŃI�u�W�F�N�g�������i�Ώۂ͑S�́j
	//�����F�������閼�O
	//�ߒl�F�������I�u�W�F�N�g�̃A�h���X
	GameObject* FindObject(const std::string& name) { return GetRootJob()->FindChildObject(name); }

	//�I�u�W�F�N�g�̖��O���擾
	//�ߒl�F���O
	const std::string& GetObjectName(void) const;

	//�I�u�W�F�N�g����ݒ�
	//�����Fname�@�ݒ肷�閼�O
	void SetObjectName(std::string name) { objectName_ = name; }

	//�q�I�u�W�F�N�g��ǉ��i���X�g�̍Ō�ցj
	//�����F�ǉ�����I�u�W�F�N�g
	void PushBackChild(GameObject* obj);

	//�q�I�u�W�F�N�g��ǉ��i���X�g�̐擪�ցj
	//�����Fobj �ǉ�����I�u�W�F�N�g
	void PushFrontChild(GameObject* obj);

	//�q�I�u�W�F�N�g��S�č폜
	void KillAllChildren();



	//�R���C�_�[�i�Փ˔���j��ǉ�����
	void AddCollider(Collider * collider);

	//�����ƏՓ˂����ꍇ�ɌĂ΂��i�I�[�o�[���C�h�p�j
	//�����FpTarget	�Փ˂�������
	virtual void OnCollision(GameObject* pTarget) {};

	//�R���C�_�[�i�Փ˔���j���폜
	void ClearCollider();

	//�Փ˔���
	//�����FpTarget	�Փ˂��Ă邩���ׂ鑊��
	void Collision(GameObject* pTarget);

	//�e�X�g�p�̏Փ˔���g��\��
	void CollisionDraw();

	//RootJob���擾
	GameObject* GetRootJob();


	//�e�A�N�Z�X�֐�
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
	/// ���W�x�[�X���`�⊮(float�Ŏ󂯎��Ԃ�)
	/// </summary>
	/// <param name="convert">�ϊ��������l(x)</param>
	/// <param name="originalMin">�ϊ����͈̔͂̍ŏ��l(x1)</param>
	/// <param name="originalMax">�ϊ����͈̔͂̍ő�l(x2)</param>
	/// <param name="conversionMin">�ϊ���͈̔͂̍ŏ��l(y1)</param>
	/// <param name="conversionMax">�ϊ���͈̔͂̍ő�l(y2)</param>
	/// <returns>�␳��̒l(y) �ϊ���͈̔͂Ɏ��܂�</returns>
	float LinearCompletion(float convert,float originalMin,float originalMax, 
		float conversionMin,float conversionMax)
	{
		//y = y1 + (x - x1) * (y2 - y1) / (x2 - x1);

		if (originalMin == originalMax)
		{
			return conversionMin; // originalMin��originalMax�������ꍇ��conversionMin��Ԃ�(0���Z�΍�)
		}
		if (convert < originalMin)
		{
			return conversionMin; // convert��originalMin��菬�����ꍇ��conversionMin��Ԃ�(�O�}�΍�)
		}
		if (convert > originalMax)
		{
			return conversionMax; // convert��originalMax���傫���ꍇ��conversionMax��Ԃ�(�O�}�΍�)
		}

		float y =  conversionMin + (convert - originalMin) * (conversionMax - conversionMin) / (originalMax - originalMin);
		return y;
	}

	/// <summary>
	/// ���K��(float�Ŏ󂯎��Ԃ�)
	/// </summary>
	/// <param name="_value">�ϊ������l</param>
	/// <param name="_min">�����l</param>
	/// <param name="_max">����l</param>
	/// <returns></returns>
	float Normalize(float _value, float _min = 0.0f, float _max = 1.0f)
	{
		//�l��0����1�͈̔͂ɐ��K��

		//_min��_max�������ꍇ��0��Ԃ�(0���Z�΍�)
		if (_min == _max)
		{
			return 0.0f; 
		}

		// min��max�̏������t�Ȃ����ւ���
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

	/// <summary>
	/// csv�ǂݍ��ݎ��̊e�g�����X�t�H�[��������
	/// </summary>
	/// <param name="tr">�������g�����X�t�H�[���ϐ�</param>
	/// <param name="v">�󂯎������s����Transform�f�[�^�z��</param>
	void SetTransformPRS(Transform &_tr, std::vector<float> _v)
	{
		//pos_x��0����n�܂鐮���ŁATransform�f�[�^�z��̓Y�����ƂȂ�
		//Transform�̊e�v�f��x,y,z��_v�̒l�����Ԃɓ���Ă���

		_tr.position_ = { _v[pos_x],_v[pos_y],_v[pos_z] };
		_tr.rotate_ = { _v[rot_x], _v[rot_y],_v[rot_z] };
		_tr.scale_ = { _v[sca_x] ,_v[sca_y],_v[sca_z] };
	}

	/// <summary>
	/// csv����ǂݍ��񂾃f�[�^��z��Ƃ��Ď擾
	/// </summary>
	/// <param name="_csv">�ǂݍ���csv�t�@�C��</param>
	/// <param name="_name">�ǂݍ���csv�t�@�C����0��ڂ̕�����</param>
	/// <returns>�ǂݍ��񂾃f�[�^�̔z��</returns>
	static std::vector<float> GetCSVReadData(CsvReader& _csv, const std::string& _name)
	{
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

	/// <summary>
	/// Transform������������ۂ̋��ʏ���
	/// </summary>
	/// <param name="_csv">�ǂݍ���CSV�C���X�^���X</param>
	/// <param name="_name">�ǂݍ��݂����p�����[�^�̖��O</param>
	/// <param name="_tr">�������g�����X�t�H�[���ϐ�</param>
	void InitCSVTransform(CsvReader& _csv, const std::string& _name, Transform& _tr)
	{
		//csv����f�[�^���擾
		std::vector<float> v = GetCSVReadData(_csv, _name);
		SetTransformPRS(_tr, v);
	}

	/// <summary>
	/// Transform���܂Ƃ߂ď���������ۂ̋��ʏ���
	/// </summary>
	/// <param name="_csv">�ǂݍ���CSV�C���X�^���X</param>
	/// <param name="_names">�ǂݍ��݂����p�����[�^�̖��O�z��</param>
	/// <param name="_Transforms">�������g�����X�t�H�[���z��</param>
	void InitCSVTransformArray(CsvReader& _csv, const std::vector<std::string>& _names,
		std::vector<std::reference_wrapper<Transform>>& _Transforms)
	{
		//���O�z��ƃg�����X�t�H�[���z�񐔂���v���Ă���Ȃ瑱����
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
	/// GameObject�̋��ʃf�[�^������
	/// </summary>
    static void CSVCommonDataInitialize() {  

		//csv�t�@�C����ǂݍ���
        CsvReader csv;  
        csv.Load("CSVdata\\EngineData\\CommonData.csv");  

		//csv�t�@�C���̊e0��ڂ̕�������擾
		std::string common = "CommonData";

		//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
		std::vector<float> commondata = GetCSVReadData(csv,common);
            
		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		DeltaTime = commondata[i_deltatime];
        oneSecond = static_cast<int>(commondata[i_onesecond]);
        TenDivision = static_cast<int>(commondata[i_tendivision]);
		SceneShortTransition = static_cast<int>(commondata[i_sceneshorttransition]);
        SceneTransition = static_cast<int>(commondata[i_scenetransition]);
		SceneLongTransition = static_cast<int>(commondata[i_scenelongtransition]);
		ZeroPointOne = commondata[i_zeropointone];
    }

private:

	//�I�u�W�F�N�g�폜�i�ċA�j
	//�����Fobj�@�폜����I�u�W�F�N�g
	void KillObjectSub(GameObject* obj);


private:
	//�t���O
	struct OBJECT_STATE
	{
		unsigned initialized : 1;	//�������ς݂�
		unsigned entered : 1;		//�X�V���邩
		unsigned visible : 1;		//�`�悷�邩
		unsigned dead : 1;			//�폜���邩
	};
	OBJECT_STATE state_;

	//�e�I�u�W�F�N�g
	GameObject* pParent_;

	//�q�I�u�W�F�N�g���X�g
	std::list<GameObject*> childList_;
};


//�I�u�W�F�N�g���쐬����e���v���[�g
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


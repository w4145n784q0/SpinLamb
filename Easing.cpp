#include "Easing.h"
#include"Engine/SceneManager.h"
#include"Engine/CsvReader.h"

namespace
{
    //�C�[�W���O�����S�ʂɎg�p����l������������ۂ̃C���f�b�N�X
    enum CommonEasingIndex
    {
        i_adjust = 0,
        i_squared,
        i_cubed,
    };

    //easeOutBack�Ɏg�p����ۂ̃C���f�b�N�X
    enum easeOutBackIndex
    {
        i_overshoot = 0,
    };

    //�C�[�W���O�̕␳�l(��{1.0)
    double Adjust = 0.0;

    //2�悷��ۂ̎w�� pow()�Ɏg�p
    int Squared = 0;

    //3�悷��ۂ̎w�� pow()�Ɏg�p
    int Cubed = 0;

    //EaseOutBack�̉��o�p�̍\����
    struct EaseOutBack
    {
        double OverShoot = 0.0;//�C�[�W���O�Ȑ��̒��˕Ԃ�
    };
    EaseOutBack EaseOutBackParam;
}

double Easing::easeOutBack(double x)
{
    const double c1 = EaseOutBackParam.OverShoot;
    const double c3 = c1 + Adjust;

    //1 + c3 *  (x - 1)^3 + c1 * (x - 1)^2;
    return Adjust + c3 * pow(x - Adjust, Cubed) + c1 * pow(x - Adjust, Squared);
}

double Easing::easeOutCubic(double x)
{
    //1 - (1 - x, 3)^3;
   return Adjust - pow(Adjust - x, Cubed);
}

double Easing::calculateScale(float _MaxScale, float _EasingCount)
{
    //tmp = 1 + (�ڕW�X�P�[�� - 1) * easeOutBack(x);
    double tmp = Adjust + (_MaxScale - Adjust) * Easing::easeOutBack(_EasingCount);
    return tmp;
}

void Easing::SetCSVEasing()
{
    //csv�t�@�C���ǂݍ���
    CsvReader csveasing;
    csveasing.Load("CSVdata\\EffectData\\EasingData.csv");

    //csv�t�@�C���̊e0��ڂ̕�������擾
    std::string easeInit= "CommonEasing";

    //0��ڂ̕������n���A���̍s�̃p�����[�^���擾
    std::vector<float> easeInitData = csveasing.GetParam(easeInit);

    //�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
    //v�̓Y������namespace�Ő錾�����񋓌^���g�p
    Adjust = static_cast<double>(easeInitData[i_adjust]);
    Squared = static_cast<int>(easeInitData[i_squared]);
    Cubed = static_cast<int>(easeInitData[i_cubed]);

    //csv�t�@�C���̊e0��ڂ̕�������擾
    std::string easeout = "EaseOutBackParam";

    //0��ڂ̕������n���A���̍s�̃p�����[�^���擾
    std::vector<float> easeoutData = csveasing.GetParam(easeout);

    //�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
    //v�̓Y������namespace�Ő錾�����񋓌^���g�p
    EaseOutBackParam.OverShoot = easeoutData[i_overshoot];
}

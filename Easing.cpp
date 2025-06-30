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

void Easing::SetSCVEasing()
{
    //csv�t�@�C���ǂݍ���
    CsvReader csveasing;
    csveasing.Load("CSVdata\\EasingData.csv");

    //csv�t�@�C���̊e0��ڂ̕�������擾
    std::string ease = "CommonEasing";

    //�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
    if (csveasing.IsGetParamName(ease))
    {
        //���̍s��z��Ƃ��đS�擾
        std::vector<float> v = csveasing.GetParam(ease);

        //�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
        //v�̓Y������namespace�Ő錾�����񋓌^���g�p
        Adjust = static_cast<int>(v[i_adjust]);
        Squared = static_cast<int>(v[i_squared]);
        Cubed = static_cast<int>(v[i_cubed]);
    }

    //csv�t�@�C���̊e0��ڂ̕�������擾
    std::string easeout = "EaseOutBackParam";

    //�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
    if (csveasing.IsGetParamName(easeout))
    {
        //���̍s��z��Ƃ��đS�擾
        std::vector<float> v = csveasing.GetParam(easeout);

        //�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
        //v�̓Y������namespace�Ő錾�����񋓌^���g�p
        EaseOutBackParam.OverShoot = v[i_overshoot];
    }
}

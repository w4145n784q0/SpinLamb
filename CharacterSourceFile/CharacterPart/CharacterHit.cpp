#include "CharacterHit.h"

void CharacterHit::Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity,
    std::string _attackName)
{
}

void CharacterHit::KnockBackAngleY(XMFLOAT3 _KnockBackVector, float _KnockBackValue)
{
}

void CharacterHit::KnockBack()
{
}

XMVECTOR CharacterHit::HitNormal(std::string _normal)
{
    return XMVectorZero();//��
}

bool CharacterHit::IsKnockBackEnd()
{
    return false;//��
}

void CharacterHit::KnockBackVelocityReset()
{
}

void CharacterHit::InvincibilityTimeCalculation()
{
}

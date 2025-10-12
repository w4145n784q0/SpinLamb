#pragma once
#include"../../Engine/GameObject.h"

class CharacterRotate :
    public GameObject
{
public:
    //----------回転----------

    /// <summary>
    /// 受け取ったベクトルからキャラクターの回転量を計算
    /// </summary>
    /// <param name="_MoveVector">進行したい方向ベクトル</param>
    /// <returns>回転する角度(Y軸回転)</returns>
    float RotateDirectionVector(XMVECTOR _MoveVector);


    /// <summary>
    /// 通常X軸回転
    /// </summary>
    void MoveRotateX();

    /// <summary>
    /// 通常X軸回転(-x回転)
    /// </summary>
    void MoveRotateXReverse();

    /// <summary>
    /// 高速X軸回転
    /// </summary>
    void FastRotateX();

    /// <summary>
    /// 高速X軸回転(-x回転)
    /// </summary>
    void FastRotateReverse();

    /// <summary>
    /// X回転を止める
    /// </summary>
    void RotateXStop();

};


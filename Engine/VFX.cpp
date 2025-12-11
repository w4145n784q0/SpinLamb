#include "VFX.h"
#include "Camera.h"

namespace VFX
{
    std::list<Emitter*>		emitterList_;	//エミッター達
    std::list<Particle*>	particleList_;	//パーティクル達

    //EmitterDataに順番に値を入れる際のインデックス
    enum VFXIndex
    {
        i_positionX = 0,
        i_positionY,
        i_positionZ,

        i_positionRndX,
        i_positionRndY,
        i_positionRndZ,

        i_directionX,
        i_directionY,
        i_directionZ,

        i_directionRndX,
        i_directionRndY,
        i_directionRndZ,

        i_speed,
        i_speedRnd,
        i_accel,
        i_gravity,

        i_colorX,
        i_colorY,
        i_colorZ,
        i_colorW,

        i_deltaColorX,
        i_deltaColorY,
        i_deltaColorZ,
        i_deltaColorW,

        i_rotateX,
        i_rotateY,
        i_rotateZ,

        i_rotateRndX,
        i_rotateRndY,
        i_rotateRndZ,

        i_spinX,
        i_spinY,
        i_spinZ,

        i_sizeX,
        i_sizeY,

        i_sizeRndX,
        i_sizeRndY,

        i_scaleX,
        i_scaleY,

        i_lifeTime,
        i_delay,
        i_number,
        i_isBillBoard,
    };

};


//更新
void VFX::Update()
{
    //各エミッターの処理
    EmitterUpdate();

    //各パーティクルの処理
    ParticleUpdate();
}


//発生中のパーティクルの更新
void VFX::ParticleUpdate()
{
    for (auto particle = particleList_.begin(); particle != particleList_.end();)
    {
        //寿命が尽きたので消す
        if ((*particle)->life == 0)
        {
            (*particle)->pEmitter->particleNum--;
            delete (*particle);
            particle = particleList_.erase(particle);
        }

        //まだ生きてる
        else
        {
            (*particle)->life--;
            (*particle)->now.position.x += (*particle)->delta.position.x;
            (*particle)->now.position.y += (*particle)->delta.position.y;
            (*particle)->now.position.z += (*particle)->delta.position.z;
            (*particle)->delta.position.x *= (*particle)->accel;
            (*particle)->delta.position.y *= (*particle)->accel;
            (*particle)->delta.position.z *= (*particle)->accel;
            (*particle)->delta.position.y -= (*particle)->gravity;

            (*particle)->now.rotation.x += (*particle)->delta.rotation.x;
            (*particle)->now.rotation.y += (*particle)->delta.rotation.y;
            (*particle)->now.rotation.z += (*particle)->delta.rotation.z;

            (*particle)->now.scale.x *= (*particle)->delta.scale.x;
            (*particle)->now.scale.y *= (*particle)->delta.scale.y;
            (*particle)->now.color.x += (*particle)->delta.color.x; //R
            (*particle)->now.color.y += (*particle)->delta.color.y; //G
            (*particle)->now.color.z += (*particle)->delta.color.z; //B
            (*particle)->now.color.w += (*particle)->delta.color.w; //A

            particle++;
        }
    }
}


//各エミッタの更新
void VFX::EmitterUpdate()
{
    for (auto emitter = emitterList_.begin(); emitter != emitterList_.end();)
    {
        //もう死んでるエミッター
        if ((*emitter)->isDead)
        {
            //そのエミッターから出たパーティクルが全部消えてたらエミッターも削除
            if ((*emitter)->particleNum <= 0)
            {
                (*emitter)->pBillBoard->Release();
                delete ((*emitter)->pBillBoard);
                delete (*emitter);
                emitter = emitterList_.erase(emitter);
            }
            else
            {
                emitter++;
            }
        }

        //まだ生きてる
        else
        {
            //パーティクルを発生させるタイミングなら
            if ((*emitter)->data.delay == 0 || (*emitter)->frameCount % (*emitter)->data.delay == 0)
            {
                //パーティクル発生
                CreateParticle(emitter);
            }

            (*emitter)->frameCount++;


            //delayが0のエミッターは１発出したら削除
            if ((*emitter)->data.delay == 0)
            {
                (*emitter)->isDead = true;

            }

            emitter++;
        }
    }
}

//パーティクル発生
void VFX::CreateParticle(std::list<VFX::Emitter*>::iterator& emitter)
{
    //指定個数分
    for (DWORD i = 0; i < (*emitter)->data.number; i++)
    {
        //パーティクル発生
        Particle* pParticle = new Particle;

        //初期
        {
            //位置
            pParticle->now.position = (*emitter)->data.position;
            float dx = (float)((*emitter)->data.positionRnd.x == 0 ? 0 : rand() % (int)((*emitter)->data.positionRnd.x * 201) - ((*emitter)->data.positionRnd.x * 100)) / 100.0f;
            float dy = (float)((*emitter)->data.positionRnd.y == 0 ? 0 : rand() % (int)((*emitter)->data.positionRnd.y * 201) - ((*emitter)->data.positionRnd.y * 100)) / 100.0f;
            float dz = (float)((*emitter)->data.positionRnd.z == 0 ? 0 : rand() % (int)((*emitter)->data.positionRnd.z * 201) - ((*emitter)->data.positionRnd.z * 100)) / 100.0f;
            pParticle->now.position.x += dx;
            pParticle->now.position.y += dy;
            pParticle->now.position.z += dz;

            //色
            pParticle->now.color = (*emitter)->data.color;

            //拡大率
            dx = (float)((*emitter)->data.sizeRnd.x == 0 ? 0 : rand() % (int)((*emitter)->data.sizeRnd.x * 201) - ((*emitter)->data.sizeRnd.x * 100)) / 100.0f + 1.0f;
            dy = (float)((*emitter)->data.sizeRnd.y == 0 ? 0 : rand() % (int)((*emitter)->data.sizeRnd.y * 201) - ((*emitter)->data.sizeRnd.y * 100)) / 100.0f + 1.0f;
            pParticle->now.scale.x = (*emitter)->data.size.x * dx;
            pParticle->now.scale.y = (*emitter)->data.size.y * dy;

            //回転
            pParticle->now.rotation = (*emitter)->data.rotate;
            dx = (float)((*emitter)->data.rotateRnd.x == 0 ? 0 : rand() % (int)((*emitter)->data.rotateRnd.x * 201) - ((*emitter)->data.rotateRnd.x * 100)) / 100.0f;
            dy = (float)((*emitter)->data.rotateRnd.y == 0 ? 0 : rand() % (int)((*emitter)->data.rotateRnd.y * 201) - ((*emitter)->data.rotateRnd.y * 100)) / 100.0f;
            dz = (float)((*emitter)->data.rotateRnd.z == 0 ? 0 : rand() % (int)((*emitter)->data.rotateRnd.z * 201) - ((*emitter)->data.rotateRnd.z * 100)) / 100.0f;
            pParticle->now.rotation.x += dx;
            pParticle->now.rotation.y += dy;
            pParticle->now.rotation.z += dz;
        }

        //変化量
        {
            //移動方向
            XMVECTOR vecDir = XMLoadFloat3(&(*emitter)->data.direction);
            float dx = (float)((*emitter)->data.directionRnd.x == 0 ? 0 : rand() % (int)((*emitter)->data.directionRnd.x * 201) - ((*emitter)->data.directionRnd.x * 100)) / 100.0f;
            float dy = (float)((*emitter)->data.directionRnd.y == 0 ? 0 : rand() % (int)((*emitter)->data.directionRnd.y * 201) - ((*emitter)->data.directionRnd.y * 100)) / 100.0f;
            float dz = (float)((*emitter)->data.directionRnd.z == 0 ? 0 : rand() % (int)((*emitter)->data.directionRnd.z * 201) - ((*emitter)->data.directionRnd.z * 100)) / 100.0f;
            XMMATRIX matRotX = XMMatrixRotationX(XMConvertToRadians(dx));
            XMMATRIX matRotY = XMMatrixRotationY(XMConvertToRadians(dy));
            XMMATRIX matRotZ = XMMatrixRotationZ(XMConvertToRadians(dz));
            vecDir = XMVector3TransformCoord(vecDir, matRotX * matRotY * matRotZ);

            float s = (float)((*emitter)->data.speedRnd == 0 ? 0 : rand() % (int)((*emitter)->data.speedRnd * 201) - ((*emitter)->data.speedRnd * 100)) / 100.0f + 1.0f;
            vecDir = XMVector3Normalize(vecDir) * ((*emitter)->data.speed * s);
            XMStoreFloat3(&pParticle->delta.position, vecDir);

            //拡大率
            pParticle->delta.scale = (*emitter)->data.scale;

            pParticle->delta.rotation = (*emitter)->data.spin;

            //色
            pParticle->delta.color = (*emitter)->data.deltaColor;


        }

        pParticle->life = (*emitter)->data.lifeTime;    //残り寿命
        pParticle->gravity = (*emitter)->data.gravity;  //重力
        pParticle->accel = (*emitter)->data.accel;      //加速

        pParticle->pEmitter = *emitter; //発生元

                                    
        particleList_.push_back(pParticle);    //発生

        pParticle->pEmitter->particleNum++; //発生元のパーティクル数をカウントアップ

    }
}

//パーティクル描画
void VFX::Draw()
{
    Direct3D::SetShader(Direct3D::SHADER_BILLBOARD);
    Direct3D::SetBlendMode(Direct3D::BLEND_ADD);

 
    for (auto particle = particleList_.begin(); particle != particleList_.end(); particle++)
    {
        XMMATRIX matWorld;
        XMMATRIX matTrans = XMMatrixTranslation((*particle)->now.position.x, (*particle)->now.position.y, (*particle)->now.position.z);

        XMMATRIX matRotateX = XMMatrixRotationX(XMConvertToRadians((*particle)->now.rotation.x));
        XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians((*particle)->now.rotation.y));
        XMMATRIX matRotateZ = XMMatrixRotationZ(XMConvertToRadians((*particle)->now.rotation.z));
        XMMATRIX matRotate = matRotateZ * matRotateX * matRotateY;

        XMMATRIX matScale = XMMatrixScaling((*particle)->now.scale.x, (*particle)->now.scale.y, 1.0f);

        if ((*particle)->pEmitter->data.isBillBoard)
        {
            matWorld = matScale * matRotate * Camera::GetBillboardMatrix() * matTrans;
        }
        else
        {
            matWorld = matScale * matRotate * matTrans;
        }
        (*particle)->pEmitter->pBillBoard->Draw(matWorld,(*particle)->now.color);
    }

    Direct3D::SetShader(Direct3D::SHADER_3D);
    Direct3D::SetBlendMode(Direct3D::BLEND_DEFAULT);
}

//解放
void VFX::Release()
{
    //全パーティクル削除
    for (auto particle = particleList_.begin(); particle != particleList_.end();)
    {
        delete (*particle);
        particle = particleList_.erase(particle);
    }
    particleList_.clear();

    //全エミッター削除
    for (auto emitter = emitterList_.begin(); emitter != emitterList_.end();)
    {
		(*emitter)->pBillBoard->Release();
		delete ((*emitter)->pBillBoard);
		delete (*emitter);
		emitter = emitterList_.erase(emitter);
	}
    emitterList_.clear();
}


//エミッタの作成
int VFX::Start(EmitterData emitterData)
{
    int handle = 0;
    for (auto i = emitterList_.begin(); i != emitterList_.end(); i++)
    {
        handle++;
    }


    Emitter* pEmitter = new Emitter;

    pEmitter->data = emitterData;
    pEmitter->handle = handle;
    pEmitter->frameCount = 0;

    pEmitter->pBillBoard = new BillBoard;
    pEmitter->pBillBoard->Load(emitterData.textureFileName);

    emitterList_.push_back(pEmitter);

    return handle;
}

//エミッタの削除
void VFX::End(int handle)
{
    for (auto i = emitterList_.begin(); i != emitterList_.end(); i++)
    {
        if ((*i)->handle == handle)
        {
            (*i)->isDead = true;
            break;
        }
    }
}

//エミッターデータに各値を入れる
//CSVで読み取れる値の初期化
void VFX::SetEmitter(EmitterData &data, std::vector<float> v)
{
    //CSVで読み取った値を入れた配列を受け取り、EmitterDataに入れていく
    //表示画像のパスはこの処理の後に個別にセットする
    //csvで読み取れる値以外を入れたい場合もこの処理の後に個別にセットする

    data.position = {v[i_positionX],v[i_positionY],v[i_positionZ]};
    data.positionRnd = { v[i_positionRndX],v[i_positionRndY],v[i_positionRndZ] };
	data.direction = { v[i_directionX],v[i_directionY],v[i_directionZ] };
	data.directionRnd = { v[i_directionRndX],v[i_directionRndY],v[i_directionRndZ] };
	data.speed = v[i_speed];
	data.speedRnd = v[i_speedRnd];
	data.accel = v[i_accel];
	data.gravity = v[i_gravity];
	data.color = { v[i_colorX],v[i_colorY],v[i_colorZ],v[i_colorW] };
	data.deltaColor = { v[i_deltaColorX],v[i_deltaColorY],v[i_deltaColorZ],v[i_deltaColorW] };
    data.rotate = { v[i_rotateX],v[i_rotateY],v[i_rotateZ] };
	data.rotateRnd = { v[i_rotateRndX],v[i_rotateRndY],v[i_rotateRndZ] };
	data.spin = { v[i_spinX],v[i_spinY],v[i_spinZ] };
	data.size = { v[i_sizeX],v[i_sizeY] };
	data.sizeRnd = { v[i_sizeRndX],v[i_sizeRndY] };
	data.scale = { v[i_scaleX],v[i_scaleY] };
	data.lifeTime = (DWORD)v[i_lifeTime];
	data.delay = (DWORD)v[i_delay];
	data.number = (DWORD)v[i_number];
	data.isBillBoard = (bool)v[i_isBillBoard];
}

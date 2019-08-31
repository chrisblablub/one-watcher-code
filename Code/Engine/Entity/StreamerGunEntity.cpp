#include <Engine/Entity/StreamerGunEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Engine.h>

namespace env
{
    ENTITY_TYPEID CStreamEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_STREAM;

    CStreamEntity::CStreamEntity()
        : m_currentLifeTime(0.0f),
        m_bEnabled(false),
        m_type(STREAM_TYPE_SMALL),
        m_pSound(nullptr)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CStreamEntity::~CStreamEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pSound);
    }

    void CStreamEntity::InitializeAtCreation()
    {
        CParticleSystemEntity::InitializeAtCreation();

        SetStreamType(m_type);

        m_bGhost = true;

        SetPauseParticles(true);

        SetRenderEditor(false);
        SetRenderGame(false);
        SetupSounds();
    }

    void CStreamEntity::SetStreamType(STREAM_TYPE type)
    {
        m_type = type;

        RemovePolygons();

        CPolygon poly;
        poly.SetNumPoints(4);

        if (m_type == STREAM_TYPE_SMALL)
        {
            poly.AddPoint(Vec3(-18.0f, -4.0f, 0.0f));
            poly.AddPoint(Vec3(18.0f, -4.0f, 0.0f));
            poly.AddPoint(Vec3(14.0f, -110.0f, 0.0f));
            poly.AddPoint(Vec3(-14.0f, -110.0f, 0.0f));

            LoadFromTemplate("STREAM_T1");
        }
        else
        {
            poly.AddPoint(Vec3(-27.0f, -4.0f, 0.0f));
            poly.AddPoint(Vec3(27.0f, -4.0f, 0.0f));
            poly.AddPoint(Vec3(19.0f, -155.0f, 0.0f));
            poly.AddPoint(Vec3(-19.0f, -155.0f, 0.0f));

            LoadFromTemplate("STREAM_T2");
        }

        AddPolygon(poly);
    }

    void CStreamEntity::InitializeAtLoad()
    {
        CParticleSystemEntity::InitializeAtLoad();

        m_bGhost = true;

        SetStreamType(m_type);
        SetupSounds();
    }

    void CStreamEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());

        }
        
        m_pSound->LoadSound("Entities/Stream.wav");
        m_pSound->SetPeakVolumeDistance(150.0f);
        m_pSound->SetAudibleDistance(400);
    }

    void CStreamEntity::SetEnabled(bool bEnable)
    {
        SetPauseParticles(!bEnable);
        SetEmitNewParticles(bEnable);

        m_pNode->SetPosition(Vec3(0.0f, 0.0f, 0.0f));

        m_currentLifeTime = 0.0f;

        m_bEnabled = bEnable;

        m_bGhost = !bEnable;
		
		if (!bEnable)
			m_pSound->SetVolume(0.0f);
    }

    void CStreamEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CStreamEntity::Update);

        CParticleSystemEntity::Update(dt);

		if (m_bEnabled)
		{
			m_currentLifeTime += dt;

			float soundFactor = Clamp(m_currentLifeTime / 1.0f, 0.0f, 1.0f);
			m_pSound->SetVolume(0.4f * soundFactor);
		}
    }

    void CStreamEntity::OnGameModeChanged(bool bGameMode)
    {
        CParticleSystemEntity::OnGameModeChanged(bGameMode);

        SetEnabled(false);
        SetForceUpdate(true);
    }




    ENTITY_TYPEID CStreamerGunEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_STREAMERGUN;

    CStreamerGunEntity::CStreamerGunEntity()
        : m_delay(0.0f),
        m_fireDelay(1.0f),
        m_fireAccum(0.0f),
        m_lifeTime(5.0f),
        m_speed(200.0f),
        m_time(0.0f),
        m_type(STREAM_TYPE_SMALL)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CStreamerGunEntity::~CStreamerGunEntity()
    {
        for (int i = 0; i < m_streams.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_streams[i]->GetName());
        }
    }

    void CStreamerGunEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CStreamerGunEntity::Update);

        CMeshEntity::Update(dt);

        m_time += dt;

        if (m_time < m_delay)
            return;

        m_fireAccum += dt;

        m_rotatationTime -= dt;

        if (m_rotatationTime >= 0.0f)
        {
            SetRotationSpeedY(2.0f * m_rotatationTime / 0.6f);
        }
        else
        {
            SetRotationSpeedY(0.0f);
        }

        for (int i = 0; i < m_currentNumActiveStreams; ++i)
        {
            CStreamEntity* p = m_streams[i];
            if (p->GetCurrentLifeTime() >= m_lifeTime)
            {
                p->SetEnabled(false);
                p->SetRenderParticles(false);

                // Disable stream
                m_streams[i] = m_streams[m_currentNumActiveStreams - 1];
                m_streams[m_currentNumActiveStreams - 1] = p;

                --m_currentNumActiveStreams;
            }
            else if (p->GetCurrentLifeTime() >= (m_lifeTime - 0.5f))
            {
                m_streams[i]->SetEmitNewParticles(false);
            }
        }


        for (int i = 0; i < m_currentNumActiveStreams; ++i)
        {
            m_streams[i]->GetNode()->SetPosition(m_streams[i]->GetNode()->GetPosition() + m_streams[i]->GetNode()->GetUp() * m_speed * dt);
        }

        if (m_fireAccum >= m_fireDelay)
        {
            if (m_currentNumActiveStreams < m_streams.size())
            {
                // Active stream
                m_streams[m_currentNumActiveStreams]->SetRenderParticles(true);
                m_streams[m_currentNumActiveStreams]->SetEnabled(true);
                m_streams[m_currentNumActiveStreams]->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
                m_streams[m_currentNumActiveStreams]->GetNode()->SetOrientation(m_pNode->GetDerivedOrientation());
                ++m_currentNumActiveStreams;

                m_rotatationTime = 0.6f;

                SetRotationSpeedY(2.0f);

                m_fireAccum = 0.0f;
            }
        }
    }

    void CStreamerGunEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("Streamer01.x");

        UpdateStreams();
    }

    void CStreamerGunEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

    }

    void CStreamerGunEntity::OnEntityCloned(CEntity& entity)
    {
        CMeshEntity::OnEntityCloned(entity);

        CStreamerGunEntity& other = static_cast<CStreamerGunEntity&>(entity);
        SetStreamType(other.GetStreamType());
        SetSpeed(other.GetSpeed());
        SetLifeTime(other.GetLifeTime());
        SetFireRate(other.GetFireRate());
    }

    void CStreamerGunEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_currentNumActiveStreams = 0;

        m_fireAccum = 100000.0f;

        m_rotatationTime = -1.0f;

        m_time = 0.0f;

        SetRotationSpeedY(0.0f);
    }

    void CStreamerGunEntity::UpdateStreams()
    {
        int numStreams = (int)(m_lifeTime / m_fireDelay) + 1;

        for (int i = 0; i < m_streams.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_streams[i]->GetName());
        }

        m_streams.resize(numStreams);

        for (int i = 0; i < m_streams.size(); ++i)
        {
            m_streams[i] = static_cast<CStreamEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_STREAM));
            m_streams[i]->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
            m_streams[i]->GetNode()->SetOrientation(m_pNode->GetDerivedOrientation());
            m_streams[i]->SetAutoDelete(false);
            m_streams[i]->SetPickable(false);
            m_streams[i]->SetEnabled(false);
            m_streams[i]->SetStreamType(m_type);
            //m_streams[i]->ApplyTransform(m_streams[i]->GetNode()->GetDerivedRST());

            //m_pNode->AttachChildNode(m_streams[i]->GetNode());

        }

        m_currentNumActiveStreams = 0;
    }


} // env
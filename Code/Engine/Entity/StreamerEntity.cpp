#include "StreamerEntity.h"
#include "EntityMgr.h"
#include "../Core/ParticleSystemMgr.h"

namespace env
{
    ENTITY_TYPEID CStreamerEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_BURNER;

    //-----------------------------------------------------------------------------------
    CStreamerEntity::CStreamerEntity()
        : m_pParticleSys(nullptr)
        , m_currentBurnDuration(0.0f)
        , m_currentBurnPause(0.0f)
        , m_bBurning(false)
        , m_bDisabled(false)
        , m_currentBBoxExpandTime(0.0f)
        , m_currentBBoxShrinkTime(0.0f)
        , m_burnPause(2.0f)
        , m_burnDuration(2.5f)
        , m_type(STREAMER_TYPE_SMALL)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Streamer] SetDisabled", STREAMER_EVENT_SET_DISABLED);
        RegisterEventParameter(STREAMER_EVENT_SET_DISABLED, "Disabled", CBaseParam::PARAMETER_TYPE_BOOL, true);
    }

    //-----------------------------------------------------------------------------------
    CStreamerEntity::~CStreamerEntity()
    {
        if (m_pParticleSys)
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticleSys->GetName());
    }

    //-----------------------------------------------------------------------------------
    void CStreamerEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case STREAMER_EVENT_SET_DISABLED:
            if (eventParameter.IsParameterSet("Disabled"))
                eventParameter.GetParameter("Disabled", m_bDisabled);

            if (!m_bDisabled)
            {
                m_bBurning = true;
                m_currentBBoxExpandTime = m_particleLifetime;
                m_currentBBoxShrinkTime = 0.0f;
                m_currentBurnDuration = 0.0f;
                m_currentBurnPause = 0.0f;
            }
            break;

        default:
            CMeshEntity::OnInput(eventId, eventParameter);
            break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CStreamerEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        CMeshEntity::SetMeshFilename("Streamer01.x");

        SetType(STREAMER_TYPE_SMALL);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CStreamerEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        m_bGhost = false;

        SetType(m_type);
    }

    //-----------------------------------------------------------------------------------
    void CStreamerEntity::SetupSounds()
    {
        m_bGhost = false;

        SetType(m_type);

        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->LoadSound("Entities/Streamer.wav");
        m_pSound->SetVolume(0.0f);
        m_pSound->SetPeakVolumeDistance(200.0f);
        m_pSound->SetAudibleDistance(400);
    }

    //-----------------------------------------------------------------------------------
    void CStreamerEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CStreamerEntity::Update);

        CMeshEntity::Update(dt);

        m_pParticleSys->SetEmitNewParticles(m_bBurning);

        float soundVolume = 1.0f;

        SetGhost(m_bDisabled);

        if (m_bDisabled)
            return;

        if (m_bBurning)
        {
            // Update cells
            m_currentBurnDuration += dt;
            
            float remainingBurnTime = m_burnDuration - m_currentBurnDuration;

            if (m_currentBurnDuration < 0.2)
            {
                soundVolume = m_currentBurnDuration / 0.2f;
            }

            if (remainingBurnTime < 0.2f && m_burnPause > 0.0f)
            {
                soundVolume = remainingBurnTime / 0.2f;
            }

            if (remainingBurnTime <= 0 && m_burnPause > 0.0f)
            {
                //m_pParticleSys->SetEmitNewParticles(false);

                m_currentBBoxShrinkTime = m_particleLifetime;

                m_currentBurnDuration = 0.0f;
                m_currentBurnPause = 0.0f;
                m_bBurning = false;
                soundVolume = 0.0f;
            }

            m_pSound->SetVolume(soundVolume * 0.35f);
        }
        else
        {
            // Start burning
            //
            m_currentBurnPause += dt;
            if (m_currentBurnPause >= m_burnPause)
            {

                m_bBurning = true;
                m_currentBBoxExpandTime = m_particleLifetime;
                m_currentBBoxShrinkTime = 0.0f;
                m_currentBurnDuration = 0.0f;
                m_currentBurnPause = 0.0f;
            }
        }

        if (m_currentBBoxShrinkTime > 0.0f || m_currentBBoxExpandTime > 0.0f)
        {
            RemovePolygons();

            CPolygon p;
            p.SetNumPoints(4);

            float maxY = 0.0f, minY = 0.0f;
            if (m_currentBBoxExpandTime > 0.0f)
            {
                m_currentBBoxExpandTime -= dt;

                maxY = (m_particleLifetime - m_currentBBoxExpandTime) * m_particleSpeed;

                SetRotationSpeedY(2.0f * (1.0f - m_currentBBoxExpandTime / m_particleLifetime));
            }

            if (!m_bBurning)
            {
                if (m_currentBBoxShrinkTime > 0.0f)
                {
                    m_currentBBoxShrinkTime -= dt;
                    maxY = (m_particleLifetime - m_currentBBoxExpandTime) * m_particleSpeed;
                    minY = (m_particleLifetime - m_currentBBoxShrinkTime) * m_particleSpeed;

                    SetRotationSpeedY(2.0f * m_currentBBoxShrinkTime / m_particleLifetime);
                }
                else
                {
                    m_currentBBoxShrinkTime = m_particleLifetime;
                    m_currentBBoxExpandTime = m_particleLifetime;
                }
            }

            if (m_type == STREAMER_TYPE_SMALL)
            {
                p.AddPoint(Vec3(-10, minY, 0.0f));
                p.AddPoint(Vec3(-10, maxY, 0.0f));
                p.AddPoint(Vec3(10, maxY, 0.0f));
                p.AddPoint(Vec3(10, minY, 0.0f));
            }
            else
            {
                p.AddPoint(Vec3(-18, minY, 0.0f));
                p.AddPoint(Vec3(-18, maxY, 0.0f));
                p.AddPoint(Vec3(18, maxY, 0.0f));
                p.AddPoint(Vec3(18, minY, 0.0f));
            }

            AddPolygon(p);
        }
        else if (!m_bBurning)
        {
            RemovePolygons();

            SetRotationSpeedY(0.0f);
        }
        else
        {
            SetRotationSpeedY(2.0f);
        }


        ApplyTransform(m_pNode->GetDerivedRST());
    }

    //-----------------------------------------------------------------------------------
    void CStreamerEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_bBurning = false;
        m_currentBurnDuration = 0.0f;
        m_currentBurnPause = 0.0f;
        m_currentBBoxShrinkTime = 0.0f;
        m_currentBBoxExpandTime = 0.0f;

        SetRotationSpeedY(0.0f);

        //m_pParticleSys->SetEmitNewParticles(false);

        const CParticleSystemEntity::mapEmitters& emitters = m_pParticleSys->GetEmitters();

        for (CParticleSystemEntity::mapEmitters::const_iterator it = emitters.begin(); it != emitters.end(); ++it)
        {
            m_particleLifetime = (it->second->GetLifeTimeMin()) - it->second->GetFadeOutTime() * 0.5f;
            m_particleSpeed = it->second->GetVelocityBegin();
        }

        RemovePolygons();
    }

    //-----------------------------------------------------------------------------------
    void CStreamerEntity::SetBurnPause(float del)
    {
        m_burnPause = del;
    }

    //-----------------------------------------------------------------------------------
    float CStreamerEntity::GetBurnPause()
    {
        return m_burnPause;
    }

    //-----------------------------------------------------------------------------------
    void CStreamerEntity::SetBurnDuration(float dur)
    {
        m_burnDuration = dur;
    }

    //-----------------------------------------------------------------------------------
    float CStreamerEntity::GetBurnDuration()
    {
        return m_burnDuration;
    }

    //-----------------------------------------------------------------------------------
    void CStreamerEntity::SetType(STREAMER_TYPE type)
    {
        m_type = type;

        if (m_pParticleSys)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticleSys->GetName());
            m_pParticleSys = NULL;
        }

        m_pParticleSys = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);

        switch (m_type)
        {
        case STREAMER_TYPE_SMALL:
            m_pParticleSys->LoadFromTemplate("STREAMER_T1");
            break;

        case STREAMER_TYPE_BIG:
            m_pParticleSys->LoadFromTemplate("STREAMER_T2");
            break;
        }


        m_pParticleSys->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        m_pNode->AttachChildNode(m_pParticleSys->GetNode());
        m_pParticleSys->SetPickable(false);
        m_pParticleSys->SetAutoDelete(false);
    }

} // env
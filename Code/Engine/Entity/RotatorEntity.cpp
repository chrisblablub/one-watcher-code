#include <Engine/Entity/RotatorEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Scene/SceneMgr.h>

namespace env
{
    ENTITY_TYPEID CRotatorEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_ROTATOR;

    CRotatorEntity::CRotatorEntity()
        : m_pAttachedNode(nullptr)
        , m_pRetainer(nullptr)
        , m_radius(120.0f)
        , m_speed(-5.0f)
        , m_startPause(0.0f)
        , m_endPause(0.0f)
        , m_angle(PI * 2.0f)
        , m_startAngle(0.0f)
        , m_currentRoll(0.0f)
        , m_currentPause(5.0f)
        , m_bReverse(false)
        , m_bPaused(false)
        , m_bAtStart(true)
        , m_bPausedTriggered(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Rotator] Start", ROTATOR_EVENT_START);
        RegisterEvent("[Rotator] Stop", ROTATOR_EVENT_STOP);
    }

    CRotatorEntity::~CRotatorEntity()
    {
    }

    void CRotatorEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        GetSprite().SetAlphaBlending(true);
        SetTextPosition(Vec2(0.0, 35.0));
        SetMaterialFilename("Sprites/rotator.png");
        SetText(GetName());

        SetRenderGame(false);
    }

    void CRotatorEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        /*if(!m_strAttachedNode.empty())
        {
        CSingletonSceneMgr sceneMgr;
        CSceneNode* pAttachedNode = sceneMgr->GetSceneNode(m_strAttachedNode);
        AttachNode(pAttachedNode);
        }

        for(int i = 0; i < m_retainerMeshNames.size(); ++i)
        {
        if(m_retainerMeshNames[i] == m_strRetainer)
        m_currentRetainer = i;
        }
        if(m_currentRetainer != 0)
        SetRetainerMeshName(m_retainerMeshNames[m_currentRetainer]);*/
    }

    void CRotatorEntity::OnEntityCloned(CEntity& entity)
    {
        CSpriteEntity::OnEntityCloned(entity);

        CRotatorEntity& other = static_cast<CRotatorEntity&>(entity);
        SetAngle(other.GetAngle());
        SetStartAngle(other.GetStartAngle());
        SetStartPause(other.GetStartPause());
        SetEndPause(other.GetEndPause());
        SetRadius(other.GetRadius());
        SetSpeed(other.GetSpeed());
        SetReverse(other.GetReverse());
    }

    void CRotatorEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CRotatorEntity::Update);

        CSpriteEntity::Update(dt);

        if (m_pAttachedNode && !gEngine->GetSceneMgr()->IsValid(m_pAttachedNode))
        {
            m_pAttachedNode = NULL;
        }

        if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
        {
            m_bPaused = true;
        }
        else
        {
            if (gEngine->GetEntityMgr()->GetActiveEntity() &&
                gEngine->GetEntityMgr()->GetActiveEntity()->GetNode() == m_pAttachedNode)
            {
                m_bPaused = true;
            }
            else
            {
                m_bPaused = false;
            }
        }

        if (m_bPaused || m_bPausedTriggered)
            return;

        if ((m_currentPause < m_startPause && m_bAtStart) || (m_currentPause < m_endPause && !m_bAtStart))
        {
            m_currentPause += dt;
            if (m_speed >= 0.0f)
            {
                if (m_bAtStart && m_currentPause >= m_startPause)
                {
                    m_currentSpeed = -m_currentSpeed;
                    m_currentRoll = m_currentStartAngle;
                }
                else if (!m_bAtStart && m_currentPause >= m_endPause)
                {
                    if (m_bReverse)
                    {
                        m_currentSpeed = -m_currentSpeed;
                        m_currentRoll = m_currentEndAngle;
                    }
                    else
                    {
                        m_currentRoll = m_currentStartAngle;
                        // Set start pause!
                        /*if(m_startPause > 0.0f)
                        {
                        m_currentPause = 0.0f;
                        m_bAtStart = true;
                        }
                        m_pNode->SetOrientation(CQuaternion(0.0, 0.0, m_currentRoll));*/
                    }
                }

            }
            else
            {
                if (m_bAtStart && m_currentPause >= m_startPause)
                {
                    if (m_bReverse)
                    {
                        m_currentSpeed = -m_currentSpeed;
                        m_currentRoll = m_currentStartAngle;
                    }
                    else
                    {
                        m_currentRoll = m_currentEndAngle;
                        /*// Set End pause!
                        if(m_endPause > 0.0f)
                        {
                        m_currentPause = 0.0f;
                        m_bAtStart = false;
                        }
                        m_pNode->SetOrientation(CQuaternion(0.0, 0.0, m_currentRoll));*/
                    }
                }
                else if (!m_bAtStart && m_currentPause >= m_endPause)
                {
                    m_currentSpeed = -m_currentSpeed;
                    m_currentRoll = m_currentEndAngle;
                }


                /*if(m_bReverse)
                {
                m_currentSpeed = -m_currentSpeed;
                m_currentRoll = m_currentStartAngle;
                }
                else
                {
                m_currentRoll = m_currentEndAngle;
                }*/
            }
            return;
        }

        m_currentRoll += dt * m_currentSpeed;

        if (m_currentRoll > m_currentEndAngle)
        {
            if (m_endPause > 0.0)
            {
                m_currentPause = 0.0;
                m_bAtStart = false;
            }
            else
            {
                if (m_bReverse)
                {
                    m_currentSpeed = -m_currentSpeed;
                    m_currentRoll = m_currentEndAngle;
                }
                else
                {
                    m_currentRoll = m_currentStartAngle;
                    /*// Set start pause!
                    if(m_startPause > 0.0f)
                    {
                    m_currentPause = 0.0f;
                    m_bAtStart = true;
                    }
                    m_pNode->SetOrientation(CQuaternion(0.0, 0.0, m_currentRoll));*/
                }
            }
        }
        else if (m_currentRoll <= m_currentStartAngle)
        {
            if (m_startPause > 0.0)
            {
                m_currentPause = 0.0;
                m_bAtStart = true;
            }
            else
            {
                if (m_bReverse)
                {
                    m_currentSpeed = -m_currentSpeed;
                    m_currentRoll = m_currentStartAngle;
                }
                else if (!Equals(m_angle, 2 * PI))
                {
                    m_currentRoll = m_currentEndAngle;
                    /*// Set end pause!
                    if(m_endPause > 0.0f)
                    {
                    m_currentPause = 0.0f;
                    m_bAtStart = false;
                    }
                    m_pNode->SetOrientation(CQuaternion(0.0, 0.0, m_currentRoll));*/
                }
            }
        }

        m_pNode->SetOrientation(CQuaternion(0.0, 0.0, m_currentRoll));
    }

    void CRotatorEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_currentRoll = m_startAngle;
        m_bPaused = false;
        m_bPausedTriggered = false;

        m_currentSpeed = m_speed;
        m_currentStartAngle = m_startAngle;

        if (m_speed < 0.0f)
        {
            m_bAtStart = false;
            m_currentEndAngle = m_startAngle - m_angle;
            std::swap(m_currentStartAngle, m_currentEndAngle);

            if (m_endPause > 0.0f)
                m_currentPause = 0.0f;
        }
        else
        {
            m_currentEndAngle = m_startAngle + m_angle;
            m_bAtStart = true;

            if (m_startPause > 0.0f)
                m_currentPause = 0.0f;
        }

        m_pNode->SetOrientation(CQuaternion(0.0, 0.0, m_currentRoll));
    }

    void CRotatorEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case ROTATOR_EVENT_STOP:
            m_bPausedTriggered = true;
            break;
        case ROTATOR_EVENT_START:
            m_bPausedTriggered = false;
            break;
        default:
            CSpriteEntity::OnInput(eventId, eventParameter);
        }
    }


    void CRotatorEntity::AttachNode(CSceneNode* pNode)
    {
        if (m_pAttachedNode)
        {
            m_pNode->DetachChildNode(m_pAttachedNode->GetName());
        }

        m_pAttachedNode = pNode;
        if (m_pAttachedNode)
        {
            m_pAttachedNode->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pNode->AttachChildNode(m_pAttachedNode);

            if (m_radius > 0.0)
                SetRadius(m_radius);
        }
    }

    void CRotatorEntity::DetachNode()
    {
        AttachNode(NULL);
    }

    CSceneNode* CRotatorEntity::GetAttachedNode()
    {
        return m_pAttachedNode;
    }

    void CRotatorEntity::SetSpeed(float speed)
    {
        m_speed = speed;

        OnGameModeChanged(GetGameMode());
    }

    float CRotatorEntity::GetSpeed()
    {
        return m_speed;
    }

    void CRotatorEntity::SetReverse(bool bReverse)
    {
        m_bReverse = bReverse;

        OnGameModeChanged(GetGameMode());
    }

    bool CRotatorEntity::GetReverse()
    {
        return m_bReverse;
    }

    void CRotatorEntity::SetAngle(float angle)
    {
        m_angle = angle;

        OnGameModeChanged(GetGameMode());
    }

    float CRotatorEntity::GetAngle()
    {
        return m_angle;
    }

    void CRotatorEntity::SetStartAngle(float angle)
    {
        m_startAngle = angle;

        m_currentRoll = m_startAngle;

        OnGameModeChanged(GetGameMode());
    }

    float CRotatorEntity::GetStartAngle()
    {
        return m_startAngle;
    }

    void CRotatorEntity::SetRadius(float radius)
    {
        m_radius = radius;


        if (m_pAttachedNode)
        {
            CQuaternion quat = m_pAttachedNode->GetOrientation();
            //quat.SetYawPitchRoll(0.0, 0.0, 0.0);
            //m_pAttachedNode->SetOrientation(quat);

            quat = m_pNode->GetOrientation();
            quat.SetYawPitchRoll(0.0, 0.0, 0.0);
            m_pNode->SetOrientation(quat);
            m_currentRoll = 0.0;

            Vec3 vNodePos = m_pNode->GetPosition();
            Vec3 vAttachedPos = m_pAttachedNode->GetPosition();
            vNodePos += m_radius * Vec3(0.0, 1.0, 0.0);
            m_pAttachedNode->SetPosition(m_radius * Vec3(0.0, 1.0, 0.0));
            vAttachedPos = m_pAttachedNode->GetPosition();

            SetStartAngle(m_startAngle);
        }

        OnGameModeChanged(GetGameMode());
    }

    float CRotatorEntity::GetRadius()
    {
        return m_radius;
    }

    void CRotatorEntity::SetStartPause(float pause)
    {
        m_startPause = pause;

        OnGameModeChanged(GetGameMode());
    }

    float CRotatorEntity::GetStartPause()
    {
        return m_startPause;
    }

    void CRotatorEntity::SetEndPause(float pause)
    {
        m_endPause = pause;

        OnGameModeChanged(GetGameMode());
    }

    float CRotatorEntity::GetEndPause()
    {
        return m_endPause;
    }

} /* End namespace */
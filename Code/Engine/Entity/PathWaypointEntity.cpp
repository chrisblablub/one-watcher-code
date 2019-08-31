#include <Engine/Entity/PathWaypointEntity.h>
#include <Engine/Entity/PathEntity.h>
#include <Engine/Entity/EntityMgr.h>

namespace env
{

    ENTITY_TYPEID CPathWaypointEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PATHWAYPOINT;

    //-----------------------------------------------------------------------------------
    CPathWaypointEntity::CPathWaypointEntity()
        : m_speed(500.0)
        , m_acceleration(800.0)
        , m_breakAcceleration(800.0f)
        , m_pause(0.0)
        , m_pPath(nullptr)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CPathWaypointEntity::~CPathWaypointEntity()
    {
        if (m_pPath)
            m_pPath->RemoveWaypoint(GetName());
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        GetSprite().SetAlphaBlending(true);
        SetTextPosition(Vec2(0.0, 24.0));
        SetMaterialFilename("Sprites/pathwaypoint.png");
        SetRenderGame(false);

        SetAutoDelete(false);
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        SetAutoDelete(false);
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::OnJustPicked()
    {
        if (m_pPath)
            m_pPath->SetRenderDebugWireframe(true);
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::OnJustUnpicked()
    {
        if (m_pPath)
            m_pPath->SetRenderDebugWireframe(false);
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::OnFired(unsigned int eventId)
    {
        if (eventId != 0)
            CSpriteEntity::OnFired(eventId);
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::SetPath(CPathEntity* pPath)
    {
        if (m_pPath == pPath)
            return;

        if (m_pPath)
        {
            m_pPath->RemoveWaypoint(GetName());
        }

        m_pPath = pPath;
        std::string strText = GetName();
        if (m_pPath)
        {
            strText += "(" + m_pPath->GetName() + ")";
        }
        else
        {
            strText += "(No Path)";
        }
        SetText(strText);

        m_pPath->AddWaypoint(this);
        SetAutoDelete(false);
    }

    //-----------------------------------------------------------------------------------
    CPathEntity* CPathWaypointEntity::GetPath()
    {
        return m_pPath;
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::SetSpeed(float speed)
    {
        m_speed = speed;
    }

    //-----------------------------------------------------------------------------------
    float CPathWaypointEntity::GetSpeed()
    {
        return m_speed;
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::SetAcceleration(float acceleration)
    {
        m_acceleration = acceleration;
    }

    //-----------------------------------------------------------------------------------
    float CPathWaypointEntity::GetAcceleration()
    {
        return m_acceleration;
    }

    //-----------------------------------------------------------------------------------
    void CPathWaypointEntity::SetPause(float pause)
    {
        m_pause = pause;
    }

    //-----------------------------------------------------------------------------------
    float CPathWaypointEntity::GetPause()
    {
        return m_pause;
    }


} /* End namespace */
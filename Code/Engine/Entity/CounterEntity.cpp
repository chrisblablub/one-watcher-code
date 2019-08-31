#include <Engine/Entity/CounterEntity.h>

namespace env
{
    ENTITY_TYPEID CCounterEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_COUNTER;

    CCounterEntity::CCounterEntity()
        : m_currentCount(0),
        m_count(2)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Counter] Increment", COUNTER_EVENT_INCREMENT);
        RegisterEvent("[Counter] Decrement", COUNTER_EVENT_DECREMENT);

        SetFireOnce(false);
    }

    CCounterEntity::~CCounterEntity()
    {

    }

    void CCounterEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();
        SetText(GetName());
        SetTextPosition(Vec2(0.0, 35.0));
        SetMaterialFilename("Sprites/counter.png");
        SetRenderGame(false);

        RemovePolygons();
    }

    void CCounterEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        m_count = 2;
    }

    void CCounterEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);
    }

    void CCounterEntity::OnFired(unsigned int eventId)
    {
        /*if(m_currentCount < m_count)
            return;

        CSpriteEntity::OnFired(eventId);*/

        switch (eventId)
        {
        case COUNTER_EVENT_INCREMENT:
            ++m_currentCount;
            if (m_currentCount >= m_count)
                CSpriteEntity::OnFired(eventId);
            break;

        case COUNTER_EVENT_DECREMENT:
            if (m_currentCount > 0)
                --m_currentCount;
            if (m_currentCount >= m_count)
                CSpriteEntity::OnFired(eventId);
            break;

        default:
            if (m_currentCount >= m_count)
                CSpriteEntity::OnFired(eventId);
            break;
        }


    }

    void CCounterEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);

        m_currentCount = 0;
    }

    void CCounterEntity::SetCount(unsigned int count)
    {
        m_count = count;
    }

    unsigned int CCounterEntity::GetCount()
    {
        return m_count;
    }
} // env
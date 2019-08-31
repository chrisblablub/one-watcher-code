#include <Engine/Entity/Entity.h>
#include <Engine/Entity/EntitySlider.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/WorldMgr.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{

    ENTITY_TYPEID CEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_BASE;

    //-----------------------------------------------------------------------------------
    CEntity::CEntity(bool bFireOnce, float firePauseTime)
        : m_bFireOnce(bFireOnce)
        , m_bFireOncePerHit(!bFireOnce)
        , m_fireDelay(firePauseTime)
        , m_bFirePaused(false)
        , m_bPickable(true)
        , m_bDeletable(true)
        , m_bSlidable(true)
        , m_bAutoDelete(true)
        , m_bResetTrigger(true)
        , m_bTriggerEnabled(true)
        , m_bFiredPrevFrame(false)
        , m_bTriggered(false)
        , m_bGameMode(true)
        , m_bPicked(false)
        , m_bGhost(true)
        , m_bPersistent(true)
        , m_bFireOnStart(false)
        , m_bJustCreated(true)
        , m_baseTime(0.0f)
        , m_fireDelayed(0.0f)
        , m_fireDebugOffset(0.0f)
        , m_pNode(NULL)
    {
        SetMetaTypeID(ENTITY_TYPEID_BASE);

        RegisterEvent("[Entity] Fire", ENTITY_EVENT_FIRE);
        RegisterEvent("[Entity] Enable", ENTITY_EVENT_ENABLE);
        RegisterEvent("[Entity] Disable", ENTITY_EVENT_DISABLE);
    }

    //-----------------------------------------------------------------------------------
    bool CEntity::HasEvent(unsigned int id)
    {
        mapEvents::iterator it = m_events.find(id);
        return it != m_events.end();
    }

    //-----------------------------------------------------------------------------------
    void CEntity::ResetTriggered()
    {
        m_baseTime = 0.0f;
        m_bFirePaused = false;
        m_bTriggerEnabled = true;
        m_bTriggered = false;
        m_fireDelayed = 0.0f;  m_triggeredEntities.clear();
    }

    //-----------------------------------------------------------------------------------
    bool CEntity::GetEventName(std::string& strEventName, unsigned int id)
    {
        mapEvents::iterator it = m_events.find(id);
        if (it == m_events.end())
            return false;

        strEventName = it->second.m_strEventName;
        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CEntity::GetEventID(const std::string& strEventName, unsigned int& id)
    {
        for (mapEvents::iterator it = m_events.begin(); it != m_events.end(); ++it)
        {
            if (it->second.m_strEventName == strEventName)
            {
                id = it->first;
                return true;
            }
        }

        id = 0;
        return false;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::RegisterEvent(const std::string& strEventName, unsigned int eventID)
    {
        EventDesc& event = m_events[eventID];
        event.m_strEventName = strEventName;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::RegisterEventParameter(unsigned int eventID,
                                         const std::string& strEventParamName,
                                         CBaseParam::PARAMETER_TYPE paramType,
                                         const CBaseParam& initialValue,
                                         const CBaseParam& minValue,
                                         const CBaseParam& maxValue)
    {
        RegisterEventParameter(eventID,
                               strEventParamName,
                               paramType,
                               std::string(),
                               initialValue,
                               minValue,
                               maxValue);
    }

    //-----------------------------------------------------------------------------------
    void CEntity::RegisterEventParameter(unsigned int eventID,
                                         const std::string& strEventParamName,
                                         CBaseParam::PARAMETER_TYPE paramType,
                                         const std::string& strEventParamDesc,
                                         const CBaseParam& initialValue,
                                         const CBaseParam& minValue,
                                         const CBaseParam& maxValue)
    {
        mapEvents::iterator it = m_events.find(eventID);

        if (it == m_events.end())
            return;

        EventParamDesc desc;
        desc.m_strName = strEventParamName;
        desc.m_strDescription = strEventParamDesc;
        desc.m_paramType = paramType;

        if (initialValue.IsValid())
            desc.m_initialValue = initialValue;

        if (minValue.IsValid())
            desc.m_minValue = minValue;

        if (maxValue.IsValid())
            desc.m_maxValue = maxValue;

        ENV_ASSERT_DBG(initialValue.IsValid() == false ||
                       initialValue.GetType() == paramType);

        ENV_ASSERT_DBG(minValue.IsValid() == false ||
                       minValue.GetType() == paramType);

        ENV_ASSERT_DBG(maxValue.IsValid() == false ||
                       maxValue.GetType() == paramType);

        EventDesc& event = it->second;
        event.m_eventParameter.push_back(desc);
    }

    //-----------------------------------------------------------------------------------
    void CEntity::GetEventParameter(unsigned int eventID, mapEventParameterDescs& eventParamDescs)
    {
        mapEvents::iterator it = m_events.find(eventID);

        if (it == m_events.end())
            return;

        eventParamDescs = it->second.m_eventParameter;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::AddInput(const EntityInput& input, unsigned int key)
    {
        //m_inputs.insert(strName);
        m_inputs[key] = input;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::SetInputSender(unsigned int key, const std::string& strName)
    {
        m_inputs[key].strSender = strName;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::RemoveInput(unsigned int key)
    {
        m_inputs.erase(key);
    }

    //-----------------------------------------------------------------------------------
    void CEntity::OnJustPicked()
    {
        m_bPicked = true;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::OnJustUnpicked()
    {
        m_bPicked = false;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::AddOutput(EntityOutput& output)
    {
        ENV_ASSERT_DBG(output.pReceiver->HasEvent(output.eventId) || output.pReceiver->GetNumEvents() == 0);

        m_outputs[output.pReceiver->GetName()].push_back(output);
    }

    //-----------------------------------------------------------------------------------
    void CEntity::SetOutputSender(CEntity* pReceiver, const std::string& strOldReceiverName)
    {
        mapOutputs::iterator it = m_outputs.find(strOldReceiverName);
        if (it == m_outputs.end())
            return;

        m_outputs[pReceiver->GetName()] = it->second;
        m_outputs.erase(strOldReceiverName);
    }

    //-----------------------------------------------------------------------------------
    void CEntity::RemoveOutputs(const std::string& strName)
    {
        mapOutputs::iterator it = m_outputs.find(strName);
        if (it != m_outputs.end())
            m_outputs.erase(it);

        /* TODO: Remove output from the delayed queue if necessary! */
    }

    //-----------------------------------------------------------------------------------
    void CEntity::RemoveOutput(const std::string& strName, unsigned int key)
    {
        mapOutputs::iterator it = m_outputs.find(strName);
        if (it == m_outputs.end())
            return;

        for (listReceiverOutputs::iterator itReceiver = it->second.begin(); itReceiver != it->second.end(); ++itReceiver)
        {
            EntityOutput& output = *itReceiver;
            if (output.key == key)
            {
                it->second.erase(itReceiver);
                if (it->second.empty())
                    m_outputs.erase(it);
                break;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntity::GetOutput(EntityOutput& output, const std::string& strReceiver, unsigned int key)
    {
        listReceiverOutputs outputs;
        GetOutputs(strReceiver, outputs);

        for (listReceiverOutputs::iterator it = outputs.begin(); it != outputs.end(); ++it)
        {
            EntityOutput& o = *it;
            if (o.key == key)
                output = *it;
        }
    }

    //-----------------------------------------------------------------------------------
    bool CEntity::HasOutput(const std::string& strReceiver, unsigned int key)
    {
        listReceiverOutputs outputs;
        GetOutputs(strReceiver, outputs);

        for (listReceiverOutputs::iterator it = outputs.begin(); it != outputs.end(); ++it)
        {
            EntityOutput& o = *it;
            if (o.key == key)
                return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::SetOutput(const EntityOutput& output, const std::string& strReceiver)
    {
        listReceiverOutputs outputs;
        GetOutputs(strReceiver, outputs);

        for (listReceiverOutputs::iterator it = outputs.begin(); it != outputs.end(); ++it)
        {
            EntityOutput& o = *it;
            if (o.key == output.key)
                o = output;
        }
    }

    //-----------------------------------------------------------------------------------
    CEntity::mapOutputs& CEntity::GetOutputs()
    {
        return m_outputs;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::GetOutputs(const std::string& strReceiver, listReceiverOutputs& outputs)
    {
        outputs = m_outputs[strReceiver];
    }

    //-----------------------------------------------------------------------------------
    void CEntity::OnEntityCloned(CEntity& entity)
    {
        ENV_ASSERT_DBG(entity.GetMetaTypeId() == GetMetaTypeId());

        m_pNode->SetScale(entity.GetNode()->GetScale());
        m_pNode->SetPosition(entity.GetNode()->GetPosition());
        m_pNode->SetOrientation(entity.GetNode()->GetOrientation());
    }

    //-----------------------------------------------------------------------------------
    void CEntity::OnGameModeChanged(bool bGameMode)
    {
        m_bGameMode = bGameMode;
        if (m_bResetTrigger)
        {
            m_baseTime = 0.0f;
            m_bFirePaused = false;
            m_bTriggerEnabled = true;
            m_bTriggered = false;
            m_fireDelayed = 0.0f;
        }

        if (GetResetTrigger())
            m_triggeredEntities.clear();

        m_outputsDelayed.clear();
    }

    //-----------------------------------------------------------------------------------
    void CEntity::OnFired(unsigned int eventId, bool bFireImmediately)
    {
        if (!m_bTriggerEnabled)
            return;

        if (!m_bFirePaused)
        {
            for (mapOutputs::iterator it = m_outputs.begin(); it != m_outputs.end(); ++it)
            {
                listReceiverOutputs& receiverOutputs = it->second;
                for (listReceiverOutputs::iterator itReceiver = receiverOutputs.begin(); itReceiver != receiverOutputs.end(); ++itReceiver)
                {
                    EntityOutput& o = *itReceiver;
                    if (bFireImmediately || Equals(o.delay, 0.0f))
                    {
                        if (o.delay >= m_fireDebugOffset)
                            o.pReceiver->OnInput(o.eventId, EventParameter(o.m_parameter));
                    }
                    else
                    {
                        if (o.delay >= m_fireDebugOffset)
                        {
                            EntityOutput o2 = o;
                            o2.delay -= m_fireDebugOffset;

                            EntityOutputDelayed ot(o2, m_baseTime);
                            m_outputsDelayed.push_back(ot);
                        }
                    }
                }
            }
            if (!m_outputsDelayed.empty())
                m_outputsDelayed.sort();
            //m_bFirePaused = true;
        }

        m_bFiredPrevFrame = true;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case ENTITY_EVENT_FIRE:
            OnFired(eventId);
            break;
        case ENTITY_EVENT_ENABLE:
            m_bTriggerEnabled = true;
            break;
        case ENTITY_EVENT_DISABLE:
            m_bTriggerEnabled = false;
            break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntity::InternalPostUpdate(float dt)
    {
        if (m_bFiredPrevFrame)
        {
            m_bFirePaused = true;
        }

        m_bFiredPrevFrame = false;
    }

    //-----------------------------------------------------------------------------------
    void CEntity::InternalUpdate(float dt)
    {
        m_baseTime += dt;

        while (!m_outputsDelayed.empty())
        {
            EntityOutputDelayed& ot = m_outputsDelayed.front();
            if ((ot.triggerTime + ot.delay) <= m_baseTime)
            {
                if (ot.eventId <= ENTITY_EVENT_FIRE)
                    ot.pReceiver->OnFired(ot.eventId, false);
                else
                    ot.pReceiver->OnInput(ot.eventId, EventParameter(ot.m_parameter));

                m_outputsDelayed.pop_front();
            }
            else
                break;
        }

        if (m_bFirePaused && !m_bFireOnce)
        {
            // Update..
            m_fireDelayed += dt;
            if (m_fireDelayed >= m_fireDelay)
            {
                if (m_bFireOncePerHit && m_bFiredPrevFrame)
                {
                    //m_fireDelayed = 0.0f;
                }
                else
                {
                    m_bFirePaused = false;
                    m_fireDelayed = 0.0f;
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    bool CEntity::HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode)
    {
        *ppNode = m_pNode;
        return CGeometryCollidable::HasCollisionWithRay(vRayOrigin, vRayDirection, distance, ppNode);
    }

} // env
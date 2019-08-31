#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Scene/TerrainMgr.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    CEntityFactory::mapCreators CEntityFactory::m_entityCreators;
    CEntityFactory::mapMetaTypenames CEntityFactory::m_entityMetaTypenames;

    //-----------------------------------------------------------------------------------
    CEntityMgr::CEntityMgr()
        : m_bGameMode(true)
        , m_bLockRemoval(false)
        , m_bSelectionOutline(true)
        , m_pActiveEntity(nullptr)
        , m_pColliderEntity(nullptr)
        , m_pCharacter(nullptr)
        , m_bTickParticles(true) {}

    //-----------------------------------------------------------------------------------
    CEntity* CEntityMgr::CreateEntity(CEntity* pOther)
    {
        CEntity* pEntity = CreateEntity(pOther->GetMetaTypeId());

        pEntity->OnEntityCloned(*pOther);

        return pEntity;
    }

    //-----------------------------------------------------------------------------------
    CEntity* CEntityMgr::CreateEntity(ENTITY_TYPEID strEntityType)
    {
        return CreateEntity(strEntityType, std::string());
    }

    //-----------------------------------------------------------------------------------
    CEntity* CEntityMgr::CreateEntity(ENTITY_TYPEID strType, const std::string& strName)
    {
        CEntity* pEntity = CEntityFactory::CreateEntity(strType);
        std::string strEntityName = strName;
        if (strEntityName.empty())
        {
            unsigned int id = 0;
            mapEntityCounter::iterator it = m_entityCounter.find(strType);
            if (it != m_entityCounter.end())
                id = m_entityCounter[strType]++;
            else
                m_entityCounter[strType] = 1;

            std::stringstream str;

            str << GetEntityMetaTypename(pEntity->GetMetaTypeId()) << id;
            strEntityName = str.str();
        }

        pEntity->SetName(strEntityName);
        pEntity->InitializeAtCreation();
        pEntity->OnResetDevice();
        pEntity->OnGameModeChanged(m_bGameMode);
        m_entities[strEntityName] = pEntity;

        return pEntity;
    }

    //-----------------------------------------------------------------------------------
    CEntity* CEntityMgr::GetEntity(const std::string& strName)
    {
        mapEntities::iterator it = m_entities.find(strName);
        if (it == m_entities.end())
            return nullptr;

        return it->second;
    }

    //-----------------------------------------------------------------------------------
    CEntity* CEntityMgr::GetFirstEntityOfType(ENTITY_TYPEID strEntityType)
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            if (it->second->GetMetaTypeId() == strEntityType)
                return it->second;
        }

        return nullptr;
    }

    //-----------------------------------------------------------------------------------
    bool CEntityMgr::RemoveEntity(CEntity* pEntity)
    {
        if (m_bLockRemoval)
            return false;

        if (pEntity == nullptr)
            return false;

        return RemoveEntity(pEntity->GetName());
    }

    //-----------------------------------------------------------------------------------
    CEntityMgr::mapEntities::iterator CEntityMgr::RemoveEntity(mapEntities::iterator& itEntity)
    {
        if (m_bLockRemoval)
            return itEntity;

        CEntity* pTrigger = itEntity->second;
        if (!pTrigger)
            return itEntity;

        if (pTrigger == m_pActiveEntity)
        {
            m_pActiveEntity = nullptr;
        }

        CEntity::mapOutputs outputs = pTrigger->GetOutputs();
        for (CEntity::mapOutputs::iterator it = outputs.begin(); it != outputs.end(); ++it)
        {
            for (CEntity::listReceiverOutputs::iterator itReceiver = it->second.begin();
                 itReceiver != it->second.end(); ++itReceiver)
            {
                CEntity::EntityOutput& output = *itReceiver;
                output.pReceiver->RemoveInput(output.key);
            }
            //it->second.pReceiver->RemoveInput((unsigned int)(&it->second));
        }

        RemoveInputLinks(pTrigger);

        m_entities.erase(itEntity++);

        ENV_DELETE(pTrigger);

        return itEntity;
    }

    //-----------------------------------------------------------------------------------
    bool CEntityMgr::RemoveEntity(const std::string& strName)
    {
        if (m_bLockRemoval)
            return false;

        CEntity* pTrigger = GetEntity(strName);
        if (!pTrigger)
            return false;

        if (pTrigger == m_pActiveEntity)
        {
            m_pActiveEntity = nullptr;
        }

        CEntity::mapOutputs outputs = pTrigger->GetOutputs();
        for (CEntity::mapOutputs::iterator it = outputs.begin(); it != outputs.end(); ++it)
        {
            for (CEntity::listReceiverOutputs::iterator itReceiver = it->second.begin();
                 itReceiver != it->second.end(); ++itReceiver)
            {
                CEntity::EntityOutput& output = *itReceiver;
                output.pReceiver->RemoveInput(output.key);
            }
            //it->second.pReceiver->RemoveInput((unsigned int)(&it->second));
        }

        RemoveInputLinks(pTrigger);

        m_entities.erase(strName);

        ENV_DELETE(pTrigger);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::SetEntityName(std::string strOldName, const std::string& strNewName)
    {
        mapEntities::iterator it = m_entities.find(strNewName);
        if (it != m_entities.end())
            return;

        it = m_entities.find(strOldName);
        if (it == m_entities.end())
            return;

        CEntity* pEntity = it->second;
        pEntity->SetName(strNewName);
        CEntity::mapOutputs outputs = pEntity->GetOutputs();
        for (CEntity::mapOutputs::iterator itOutputs = outputs.begin(); itOutputs != outputs.end(); ++itOutputs)
        {
            std::string strName = itOutputs->first;

            for (CEntity::listReceiverOutputs::iterator itReceiver = itOutputs->second.begin();
                 itReceiver != itOutputs->second.end(); ++itReceiver)
            {
                CEntity::EntityOutput& o = *itReceiver;
                o.pReceiver->SetInputSender(o.key, strNewName);
            }
        }

        UpdateInputs(pEntity, strOldName);

        m_entities.erase(it);
        m_entities[strNewName] = pEntity;
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::UpdateInputs(CEntity* pEventHandler, const std::string& strOldName)
    {
        CEntity::mapInputs inputs = pEventHandler->GetInputs();
        for (CEntity::mapInputs::iterator itInputs = inputs.begin(); itInputs != inputs.end(); ++itInputs)
        {
            CEntity::EntityInput& input = itInputs->second;
            CEntity* pEntity = GetEntity(input.strSender);
            if (!pEntity)
                continue;

            pEntity->SetOutputSender(pEventHandler, strOldName);
        }
    }

    //-----------------------------------------------------------------------------------
    unsigned int CEntityMgr::AddEntityOutput(CEntity* pTrigger, CEntity::EntityOutput& output)
    {
        static unsigned int ctr = 0;

        while (pTrigger->HasOutput(output.pReceiver->GetName(), ++ctr));

        output.key = ctr;
        pTrigger->AddOutput(output);
        output.pReceiver->AddInput(CEntity::EntityInput(pTrigger->GetName(), output.eventId), output.key);
        return output.key;
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::RemoveEntityOutputs(CEntity* pTrigger, const std::string& strName)
    {
        CEntity::listReceiverOutputs outputs;
        pTrigger->GetOutputs(strName, outputs);
        for (CEntity::listReceiverOutputs::iterator it = outputs.begin(); it != outputs.end(); ++it)
        {
            CEntity::EntityOutput& o = *it;
            o.pReceiver->RemoveInput(o.key);
        }
        pTrigger->RemoveOutputs(strName);
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::RemoveEntityOutput(CEntity* pTrigger, const std::string& strReceiver, unsigned int key)
    {
        pTrigger->RemoveOutput(strReceiver, key);
        CEntity* pTriggerable = GetEntity(strReceiver);
        if (pTriggerable)
            pTriggerable->RemoveInput(key);
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::ResetTriggers()
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            it->second->ResetTriggered();
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::Reset()
    {
#pragma message("--> HACK! BLOCK RemoveEntity() during CEntityMgr::Reset()")
        // Hack: block RemoveEntity(), because an entities destructor might call RemoveEntity,
        // which invalidates the current iterator over m_entities.
        //
        m_bLockRemoval = true;
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            //if(it->second->GetAutoDelete())
            {
                //it = RemoveEntity(it);
                ENV_DELETE(it->second);
            }
        }
        m_bLockRemoval = false;

        m_entities.clear();
        m_pActiveEntity = nullptr;
        m_pColliderEntity = nullptr;
        m_pCharacter = nullptr;
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::InitializeAtLoad()
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            it->second->InitializeAtLoad();
        }

        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            it->second->ApplyTransform(it->second->GetNode()->GetDerivedRST());
        }

        m_pCharacter = GetFirstEntityOfType(ENTITY_TYPEID_CHARACTER3D);
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::InitializeAtCreation()
    {
        m_entityCounter.clear();
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::OnLostDevice()
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            it->second->OnLostDevice();
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::OnResetDevice()
    {
        ENV_LOG_INFO("CEntityMgr::OnResetDevice");

        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            it->second->OnResetDevice();

        ENV_LOG_INFO("CEntityMgr::OnResetDevice SUCCEEDED");
    }

    //-----------------------------------------------------------------------------------
    bool CEntityMgr::Validate()
    {
        ENV_LOG_INFO("Checking validity of entities...");

        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            CEntity* p = it->second;
            CSceneNode* pNode = p->GetNode();
            {
                ENV_LOG_INFO(p->GetName() + " does not have scene node!");
                continue;
            }

            if (!pNode->GetParentNode())
            {
                ENV_LOG_INFO(p->GetName() + " does not have valid parent node!");
            }
        }
        return true;
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::Update(float dt, RenderQueue& renderQueue)
    {
        ENV_SCOPED_CPU_PROFILING(Engine, EntityMgr);


        if (gEngine->GetInput()->IsKeyJustDown('L'))
            m_bTickParticles = !m_bTickParticles;

        {
            ENV_SCOPED_CPU_PROFILING(Engine, EntityMgr::Collision);

            for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                CEntity* p = it->second;
                p->InternalUpdate(dt);

                if (p->GetGhost() || !m_pColliderEntity || !m_bGameMode)
                    continue;

                if (m_pColliderEntity->HasCollision(*p))
                {
                    if (!m_pColliderEntity->GetTriggered(p) && !p->GetFirePaused())
                    {
                        m_pColliderEntity->OnTriggerEntered(*p);
                        p->OnTriggerEntered(*m_pColliderEntity);
                    }

                    p->OnFired(0);
                    p->SetTriggered(true, m_pColliderEntity);
                    m_pColliderEntity->OnFired(0);
                    m_pColliderEntity->SetTriggered(true, p);
                }
                else
                {
                    if (m_pColliderEntity->GetTriggered(p))
                    {
                        m_pColliderEntity->OnTriggerLeft();

                    }
                    m_pColliderEntity->SetTriggered(false, p);
                }
            }

            for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                CEntity* p = it->second;
                p->InternalPostUpdate(dt);
            }
        }

        {
            ENV_SCOPED_CPU_PROFILING(Engine, EntityMgr::Update);

            for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                CEntity* p = it->second;

                if (p->GetMetaTypeId() == ENTITY_TYPEID_CHARACTER3D)
                    continue;

                p->Update(dt);
            }

            if (m_pCharacter)
                m_pCharacter->Update(dt);
        }


        {
            ENV_SCOPED_CPU_PROFILING(Engine, EntityMgr::RemoveEntity);

            for (std::list< CEntity* >::iterator it = m_killEntities.begin(); it != m_killEntities.end(); ++it)
            {
                RemoveEntity(*it);
            }

            m_killEntities.clear();
        }


        {
            ENV_SCOPED_CPU_PROFILING(Engine, EntityMgr::AddToRenderQueue);

            for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                CEntity* p = it->second;

                p->AddToRenderQueue(dt, renderQueue);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::GetCollidables(std::list< CGeometryCollidable* >& collidables)
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            CEntity* p = it->second;
            collidables.push_back(p);
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::PrepareToSave()
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            it->second->OnSerialization(false);
        }

        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            ENV_ASSERT_DBG(!(!it->second->GetPersistent() && it->second->GetAutoDelete()));
            if (!it->second->GetPersistent())
            {
                RemoveEntity(it->second);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::GetEntities(std::list< std::string >& entities, ENTITY_TYPEID strEntityType)
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            if (it->second->GetMetaTypeId() == strEntityType || strEntityType == ENTITY_TYPEID_UNSPECIFIED)
                entities.push_back(it->first);
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::GetEntities(std::list< CEntity* >& entities, ENTITY_TYPEID strEntityType)
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            if (it->second->GetMetaTypeId() == strEntityType || strEntityType == ENTITY_TYPEID_UNSPECIFIED)
                entities.push_back(it->second);
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::GetEntitiesPickable(std::list< CEntity* >& entities)
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            if (it->second->GetPickable())
                entities.push_back(it->second);
        }
    }

    //-----------------------------------------------------------------------------------
    unsigned int CEntityMgr::GetNumEntites(ENTITY_TYPEID strEntityType)
    {
        unsigned int numEntites = 0;
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            if (it->second->GetMetaTypeId() == strEntityType)
                ++numEntites;
        }

        return numEntites;
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::FireEntity(CEntity* pTrigger)
    {
        pTrigger->OnFired(0);
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::SetActiveEntity(CEntity* pEntity)
    {
        if (m_pActiveEntity)
            m_pActiveEntity->OnJustUnpicked();

        m_pActiveEntity = pEntity;

        if (m_pActiveEntity)
        {
            m_pActiveEntity->OnJustPicked();
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntityMgr::SetGameMode(bool bGameMode)
    {
        m_bGameMode = bGameMode;

        mapEntities::iterator jt = m_entities.end();
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end();)
        {
            if (!it->second->GetPersistent() && it->second->GetAutoDelete())
            {
                it = RemoveEntity(it);

                if (jt == m_entities.end())
                    jt = m_entities.begin();

                it = jt;

                if (it != m_entities.end())
                    ++it;
            }
            else
            {
                it->second->OnGameModeChanged(bGameMode);

                jt = it;

                ++it;
            }
        }

        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            CEntity* p = it->second;

            if (p->GetMetaTypeId() == ENTITY_TYPEID_CHARACTER3D)
                continue;

            p->Update(0.0f);
        }

        if (m_pCharacter)
            m_pCharacter->Update(0.0f);


        if (bGameMode)
        {
            for (auto& it : m_entities)
            {
                if (it.second->GetFireOnStart())
                    it.second->OnFired(0);
            }
        }
        
        m_pCharacter = GetFirstEntityOfType(ENTITY_TYPEID_CHARACTER3D);
    }

#ifndef ENV_FINAL

    //-----------------------------------------------------------------------------------
    bool CEntityMgr::RemoveEntitySafe(CEntity* pEntity)
    {
        if (m_bLockRemoval)
            return false;

        if (!IsValid(pEntity))
            return false;

        return RemoveEntity(pEntity->GetName());
    }

    //-----------------------------------------------------------------------------------
    bool CEntityMgr::IsValid(CEntity* p)
    {
        for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            if (it->second == p)
                return true;
        }

        return false;
    }

#endif // ENV_NOT_FINAL

    //-----------------------------------------------------------------------------------
    void CEntityMgr::RemoveInputLinks(CEntity* pTriggerable)
    {
        CEntity::mapInputs inputs = pTriggerable->GetInputs();
        for (CEntity::mapInputs::iterator it = inputs.begin(); it != inputs.end(); ++it)
        {
            CEntity* pTrigger = GetEntity(it->second.strSender);
            if (pTrigger)
            {
                pTrigger->RemoveOutput(pTriggerable->GetName(), it->first);
            }
        }
    }
}
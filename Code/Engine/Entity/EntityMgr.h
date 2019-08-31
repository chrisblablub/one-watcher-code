/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ENTITY_MGR_h
#define ENTITY_MGR_h

#include <Engine/EngineCommon.h>
#include <Engine/Entity/Entity.h>
#include <Engine/Core/Input.h>



namespace env
{
    struct RenderQueue;

    class CEntityFactory
    {
    public:
        typedef std::pair< std::auto_ptr< IFactory< CEntity > >, bool > pairCreator;
        typedef std::map< ENTITY_TYPEID, pairCreator > mapCreators;
        typedef std::map< ENTITY_TYPEID, std::string > mapMetaTypenames;

        template< typename T > static bool RegisterEntityType(const std::string& strEntityTypeName, bool bShowInEditor = true)
        {
            m_entityCreators[T::ENTITY_META_TYPEID] = pairCreator(std::auto_ptr< IFactory< CEntity> >(new CFactory< CEntity, T >()), bShowInEditor);
            m_entityMetaTypenames[T::ENTITY_META_TYPEID] = strEntityTypeName;
            return true;
        }

        void GetEntityMetaTypeIds(std::list< ENTITY_TYPEID >& strMetaTypeIds)
        {
            for (mapCreators::iterator it = m_entityCreators.begin(); it != m_entityCreators.end(); ++it)
            {
                if (it->second.second)
                    strMetaTypeIds.push_back(it->first);
            }
        }

        void GetEntityMetaTypeNames(std::list< std::string >& strMetaTypeNames)
        {
            for (mapMetaTypenames::iterator it = m_entityMetaTypenames.begin(); it != m_entityMetaTypenames.end(); ++it)
                strMetaTypeNames.push_back(it->second);
        }

        std::string GetEntityMetaTypename(ENTITY_TYPEID typeId)
        {
            mapMetaTypenames::iterator it = m_entityMetaTypenames.find(typeId);
            if (it == m_entityMetaTypenames.end())
                return std::string();
            else
                return it->second;
        }

        ENTITY_TYPEID GetEntityMetaTypeId(const std::string& strTypeName)
        {
            for (mapMetaTypenames::iterator it = m_entityMetaTypenames.begin(); it != m_entityMetaTypenames.end(); ++it)
            {
                if (it->second == strTypeName)
                    return it->first;
            }

            return ENTITY_TYPEID_UNSPECIFIED;
        }

        bool GetShowInEditor(ENTITY_TYPEID typeID)
        {
            mapCreators::iterator it = m_entityCreators.find(typeID);

            if (it == m_entityCreators.end())
                return false;

            return it->second.second;
        }

        bool GetShowInEditor(const std::string strTypeName)
        {
            ENTITY_TYPEID typeID = GetEntityMetaTypeId(strTypeName);

            if (typeID == ENTITY_TYPEID_UNSPECIFIED)
                return false;

            mapCreators::iterator it = m_entityCreators.find(typeID);

            if (it == m_entityCreators.end())
                return false;

            return it->second.second;
        }

    protected:
        static CEntity* CreateEntity(ENTITY_TYPEID strType)
        {
            mapCreators::iterator it = m_entityCreators.find(strType);
            if (it == m_entityCreators.end())
                return NULL;

            return it->second.first->Create();
        }


    private:
        static mapCreators m_entityCreators;
        static mapMetaTypenames m_entityMetaTypenames;
    };



    class CEntityMgr : public CEntityFactory, public CMouseListener
    {
    public:
        typedef std::map< std::string, CEntity* > mapEntities;
        typedef std::map< ENTITY_TYPEID, unsigned int > mapEntityCounter;

        CEntityMgr();
        CEntityMgr(const CEntityMgr&) = delete;
        CEntityMgr& operator=(const CEntityMgr&) = delete;

        void Reset();
        void ResetTriggers();
        void InitializeAtLoad();
        void InitializeAtCreation();
        void OnLostDevice();
        void OnResetDevice();
        void Update(float dt, RenderQueue& renderQueue);

        CEntity* CreateEntity(CEntity* pOther);
        CEntity* CreateEntity(ENTITY_TYPEID strEntityType);
        CEntity* CreateEntity(ENTITY_TYPEID strEntityType, const std::string& strEntityName);

        bool RemoveEntity(const std::string& strName);
        bool RemoveEntity(CEntity* pEntity);
        ENV_NOT_FINAL_CODE(bool RemoveEntitySafe(CEntity* pEntity));

        CEntity* GetEntity(const std::string& strName);
        CEntity* GetFirstEntityOfType(ENTITY_TYPEID strEntityType);

        void SetEntityName(std::string strOldName, const std::string& strNewName);
        
        unsigned int AddEntityOutput(CEntity* pTrigger, CEntity::EntityOutput& output);
        void RemoveEntityOutputs(CEntity* pTrigger, const std::string& strName);
        void RemoveEntityOutput(CEntity* pTrigger, const std::string& strReceiver, unsigned int key);

        const mapEntities& GetEntities() const { return m_entities; }
        mapEntities* GetEntitiesPtr() { return &m_entities; }

        void GetEntities(std::list< std::string >& entities, ENTITY_TYPEID strEntityType = ENTITY_TYPEID_UNSPECIFIED);
        void GetEntities(std::list< CEntity* >& entities, ENTITY_TYPEID strEntityType = ENTITY_TYPEID_UNSPECIFIED);

        template<typename Entity>
        void GetEntities(std::list< std::string >& entities)
        {
            for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                if (dynamic_cast<Entity*>(it->second))
                    entities.push_back(it->first);
            }
        }

        template<typename Entity>
        void GetEntities(std::list< CEntity* >& entities)
        {
            for (mapEntities::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                if (dynamic_cast<Entity*>(it->second))
                    entities.push_back(it->second);
            }
        }

        void GetEntitiesPickable(std::list< CEntity* >& entities);
        unsigned int GetNumEntites(ENTITY_TYPEID strEntityType);

        void FireEntity(CEntity* pTrigger);

        void SetActiveEntity(CEntity* pTrigger);
        CEntity* GetActiveEntity() const { return m_pActiveEntity; }

        void KillEntity(CEntity* pEntity) { m_killEntities.push_back(pEntity); }

        void SetGameMode(bool bGameMode);

        ENV_NOT_FINAL_CODE(bool IsValid(CEntity* p));

        bool Validate();

        void GetCollidables(std::list< CGeometryCollidable* >& collidables);

        void PrepareToSave();

        void SetColliderEntity(CEntity* pEntity) { m_pColliderEntity = pEntity; }
        void SetCharacterEntity(CEntity* pEntity) { m_pCharacter = pEntity; }

        CEntity* GetColliderEntity() const { return m_pColliderEntity; }
        CEntity* GetCharacter() const { return m_pCharacter; }

        inline void SetSelectionOutline(bool bSet) { m_bSelectionOutline = bSet; }
        inline bool GetSelectionOutline() const { return m_bSelectionOutline; }

        inline bool GetTickParticles() const { return m_bTickParticles; }
    private:
        mapEntities::iterator RemoveEntity(mapEntities::iterator& it);

        friend class boost::serialization::access;

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & m_entities;
            ar & m_entityCounter;
        }

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            ar & m_entities;
            ar & m_entityCounter;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

    private:

        void UpdateInputs(CEntity* pEventHandler, const std::string& strOldName);
        void RemoveInputLinks(CEntity* pTriggerable);

        std::list< CEntity* > m_killEntities;
        mapEntities m_entities;
        mapEntityCounter m_entityCounter;

        CEntity* m_pColliderEntity;

        CEntity* m_pActiveEntity;
        CEntity* m_pCharacter;

        bool m_bGameMode;
        bool m_bLockRemoval;
        bool m_bSelectionOutline;

        bool m_bTickParticles;
    };


} //env
#endif // ENTITY_MGR_h
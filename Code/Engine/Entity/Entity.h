/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef ENTITY_H
#define ENTITY_H


#include <Engine/EngineCommon.h>
#include <Engine/Scene/GeometryCollidable.h>
#include <Engine/Scene/SceneNode.h>

#include <set>

namespace env
{
    enum ENTITY_TYPEID
    {
        ENTITY_TYPEID_UNSPECIFIED = 0,
        ENTITY_TYPEID_BASE,
        ENTITY_TYPEID_ACCELERATOR,
        ENTITY_TYPEID_BURNER,
        ENTITY_TYPEID_CHARACTER,
        ENTITY_TYPEID_CHECKPOINT,
        ENTITY_TYPEID_COIN,
        ENTITY_TYPEID_CONTACTMINE,
        ENTITY_TYPEID_COUNTER,
        ENTITY_TYPEID_GUN,
        ENTITY_TYPEID_OVERLAYER,
        ENTITY_TYPEID_LIGHT,
        ENTITY_TYPEID_MESH,
        ENTITY_TYPEID_HUNTER,
        ENTITY_TYPEID_EMITTER,
        ENTITY_TYPEID_PARTICLESYSTEM,
        ENTITY_TYPEID_PATH,
        ENTITY_TYPEID_PATHWAYPOINT,
        ENTITY_TYPEID_PLATFORM,
        ENTITY_TYPEID_POLYGON,
        ENTITY_TYPEID_PORTAL,
        ENTITY_TYPEID_ROTATOR,
        ENTITY_TYPEID_SAWBLADE,
        ENTITY_TYPEID_SCYTHE,
        ENTITY_TYPEID_ROCKET,
        ENTITY_TYPEID_SPIKEBALL,
        ENTITY_TYPEID_SPIKE,
        ENTITY_TYPEID_SPRITE,
        ENTITY_TYPEID_SPRITENUMBER,
        ENTITY_TYPEID_TIMER,
        ENTITY_TYPEID_TOLLGATE,
        ENTITY_TYPEID_TRANSLATIONSPRITE,
        ENTITY_TYPEID_VOODOODOLL,
        ENTITY_TYPEID_FADE,
        ENTITY_TYPEID_LASERRADIATOR,
        ENTITY_TYPEID_FORCEFIELD,
        ENTITY_TYPEID_GAMEMANAGER,
        ENTITY_TYPEID_MAGNET,
        ENTITY_TYPEID_MAGNETCLUSTER,
        ENTITY_TYPEID_FIREBALLGUN,
        ENTITY_TYPEID_FIREBALL,
        ENTITY_TYPEID_PLATFORMSAWBLADETOP,
        ENTITY_TYPEID_PLATFORMSAWBLADEBOTTOM,
        ENTITY_TYPEID_METALSHARD,
        ENTITY_TYPEID_RACKWHEEL,
        ENTITY_TYPEID_GATE,
        ENTITY_TYPEID_TRIGGER,
        ENTITY_TYPEID_LIGHTNING,
        ENTITY_TYPEID_BUMPER,
        ENTITY_TYPEID_ROBOTARM,
        ENTITY_TYPEID_BOMB,
        ENTITY_TYPEID_BOMBGUN,
        ENTITY_TYPEID_SPHEREEMITTER,
        ENTITY_TYPEID_BOXEMITTER,
        ENTITY_TYPEID_EXPLODINGHUNTER,
        ENTITY_TYPEID_EXPLOSION,
        ENTITY_TYPEID_EXPLODINGHUNTEREMITTER,
        ENTITY_TYPEID_PLATFORMDRONE,
        ENTITY_TYPEID_SIMPLELIGHT,
        ENTITY_TYPEID_ROBOTARMT2,
        ENTITY_TYPEID_EXPLODINGHUNTEREMITTERT2,
        ENTITY_TYPEID_MESHSWITCH,
        ENTITY_TYPEID_ACCELERATORCLUSTER,
        ENTITY_TYPEID_SKYDOME,
        ENTITY_TYPEID_PLANET,
        ENTITY_TYPEID_SKYPLANE,
        ENTITY_TYPEID_SUN,
        ENTITY_TYPEID_LIGHTSPRITE,
        ENTITY_TYPEID_POSTPROCESSING,
        ENTITY_TYPEID_CAMERA,
        ENTITY_TYPEID_PLAYERCAMERA,
        ENTITY_TYPEID_HALO,
        ENTITY_TYPEID_SKYBOX,
        ENTITY_TYPEID_CHAPTERTITLE,
        ENTITY_TYPEID_STREAM,
        ENTITY_TYPEID_STREAMERGUN,
        ENTITY_TYPEID_MISSILE,
        ENTITY_TYPEID_MISSILEGUN,
        ENTITY_TYPEID_ENERGYFIELD,
        ENTITY_TYPEID_PLANARPOINTEMITTER,
        ENTITY_TYPEID_HUNTINGHUNTER,
        ENTITY_TYPEID_ROBOTARMT3,
        ENTITY_TYPEID_UNIVERSEPLANET,
        ENTITY_TYPEID_MISSILEGUNAIMING,
        ENTITY_TYPEID_WATCHERVIDEO,
        ENTITY_TYPEID_TEXT,
        ENTITY_TYPEID_SAWBLADEBOSS,
        ENTITY_TYPEID_CHARACTER3D,
        ENTITY_TYPEID_CHECKPOINT_ORIGIN,
        ENTITY_TYPEID_CHECKPOINT_DESTINATION,
        ENTITY_TYPEID_ENVMAP,
        ENTITY_TYPEID_STRIPES,
        ENTITY_TYPEID_PLATFORMBASE,
        ENTITY_TYPEID_TIMELAPSE,
        ENTITY_TYPEID_SOUNDSPRITE,
        ENTITY_TYPEID_BOSS,
        ENTITY_TYPEID_WEAPON,
        ENTITY_TYPEID_PROJECTILE,
        ENTITY_TYPEID_PROJECTILE_GUN,
        ENTITY_TYPEID_LASERRADIATOR_AIMING,
        ENTITY_TYPEID_COUNTERV2,
        ENTITY_TYPEID_ACCELERATOR_BOSS
    };

    struct RenderQueue;

    class CEntityMgr;


    class CEntity : public CGeometryCollidable
    {
        friend class CEntityMgr;

    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            ENTITY_EVENT_PLAYER_FIRE = 0,
            ENTITY_EVENT_FIRE,
            ENTITY_EVENT_ENABLE,
            ENTITY_EVENT_DISABLE,
            ENTITY_EVENT_END
        };

        struct EntityInput
        {
            EntityInput()
                : eventID(0) {}

            EntityInput(const std::string& strSender, unsigned int eventID)
                : strSender(strSender), eventID(eventID) {}

            std::string strSender;
            unsigned int eventID;

            template< class Archive > void serialize(Archive& ar, const unsigned int version)
            {
                ar & strSender;
                ar & eventID;
            }
        };

        typedef std::map< std::string, CBaseParam > mapEventParameter;

        struct EntityOutput
        {
            EntityOutput()
                : pReceiver(0)
                , eventId(0)
                , key(0)
                , delay(0.0) {}

            EntityOutput(CEntity* pReceiver, unsigned int eventId, float delay = 0.0)
                : pReceiver(pReceiver)
                , eventId(eventId)
                , key(0)
                , delay(delay) {}

            float delay;
            unsigned int eventId;
            unsigned int key;
            CEntity* pReceiver;
            mapEventParameter m_parameter;

            bool operator<(const EntityOutput& o)
            {
                if (Equals(delay, o.delay))
                    return eventId < o.eventId;
                else
                    return (delay < o.delay);
            }

            template< class Archive > void serialize(Archive& ar, const unsigned int version)
            {
                ar & delay;
                ar & eventId;
                ar & key;
                ar & pReceiver;

                if (version > 0)
                    ar & m_parameter;
            }
        };

        struct EntityOutputDelayed : EntityOutput
        {
            EntityOutputDelayed(EntityOutput& o, float triggerTime)
                : EntityOutput(o)
                , triggerTime(triggerTime) {}

            float triggerTime;

            bool operator<(const EntityOutputDelayed& o)
            {
                if (Equals((triggerTime + delay), (o.triggerTime + o.delay)))
                    return eventId < o.eventId;
                else
                    return ((triggerTime + delay) < (o.triggerTime + o.delay));
            }
        };

        struct EventParamDesc
        {
            std::string m_strName;
            std::string m_strDescription;
            CBaseParam::PARAMETER_TYPE m_paramType;
            CBaseParam m_initialValue;
            CBaseParam m_minValue;
            CBaseParam m_maxValue;
        };

        //typedef std::pair< std::string, CBaseParam::PARAMETER_TYPE > pairParameterDesc;
        typedef std::list< EventParamDesc > mapEventParameterDescs;

        struct EventDesc
        {
            std::string m_strEventName;
            mapEventParameterDescs m_eventParameter;
        };

        class EventParameter
        {
        public:
            explicit EventParameter(const mapEventParameter& params = mapEventParameter())
                : m_params(params) {}

            bool IsParameterSet(const std::string& strParamName) const
            {
                mapEventParameter::const_iterator it = m_params.find(strParamName);

                return it != m_params.end();
            }

            template< class T >
            bool GetParameter(const std::string& strParamName, T& value) const
            {
                mapEventParameter::const_iterator it = m_params.find(strParamName);

                if (it != m_params.end())
                {
                    value = it->second;
                    return true;
                }

                return false;
            }

        private:
            const mapEventParameter& m_params;
        };

        typedef std::map< unsigned int, EventDesc >         mapEvents;
        typedef std::map< unsigned int, EntityInput >       mapInputs;
        typedef std::list< EntityOutput >                   listReceiverOutputs;
        typedef std::map< std::string, listReceiverOutputs> mapOutputs;

        CEntity(bool bFiredOnce = false,
                float firePauseTime = 0.0);

        virtual ~CEntity() {}
        virtual void Update(float dt) {}
        virtual void AddToRenderQueue(float dt, RenderQueue& queue) {}
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnJustPicked();
        virtual void OnJustUnpicked();
        virtual void InitializeAtCreation() { m_bGhost = true; }
        virtual void InitializeAtLoad() { m_bGhost = true; m_pNode->SetDebugName(m_strName); }
        virtual void OnEntityCloned(CEntity& entity);

        virtual void OnLostDevice() {}
        virtual void OnResetDevice() {}
        virtual CSceneNode* GetNode() { return m_pNode; }
        virtual void OnSerialization(bool bLoad) {}
        virtual bool HasCollision(CEntity& e) { return CGeometryCollidable::HasCollision(e); }
        virtual bool HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void OnFired(unsigned int eventId, bool bFireImmediately = false);
        virtual void OnTriggerLeft() {}
        virtual void OnTriggerEntered(CEntity& e) {}
        virtual void SetPickable(bool bPickable) { m_bPickable = bPickable; }
        virtual void SetDeletable(bool bDeletable) { m_bDeletable = bDeletable; }
        virtual void SetSlidable(bool bSlidable) { m_bSlidable = bSlidable; }

        void ResetTriggered();

        void SetTriggered(bool bSet, CEntity* pEntity) { if (bSet) m_triggeredEntities.insert(pEntity); else m_triggeredEntities.erase(pEntity); }
        bool GetTriggered(CEntity* pEntity) const { if (m_triggeredEntities.find(pEntity) != m_triggeredEntities.end()) return true; else return false; }

        inline ENTITY_TYPEID GetMetaTypeId() const { return m_metaTypeId; }

        inline void SetFireOnce(bool bFireOnce) { m_bFireOnce = bFireOnce; }
        inline bool GetFireOnce() const { return m_bFireOnce; }

        inline void SetFireOncePerHit(bool bSet) { m_bFireOncePerHit = bSet; }
        inline bool GetFireOncePerHit() const { return m_bFireOncePerHit; }

        inline void SetFireOnStart(bool bSet) { m_bFireOnStart = bSet; }
        inline bool GetFireOnStart() const { return m_bFireOnStart; }

        inline bool GetFirePaused() const { return m_bFirePaused; }

        inline void SetFireDelay(float time) { m_fireDelay = time; }
        inline float GetFireDelay() const { return m_fireDelay; }

        inline void SetFireDebugOffset(float time) { m_fireDebugOffset = time; }
        inline float GetFireDebugOffset() const { return m_fireDebugOffset; }

        inline bool GetPickable() const { return m_bPickable; }
        inline bool GetDeletable() const { return m_bDeletable; }
        inline bool GetSlidable() const { return m_bSlidable; }

        inline void SetAutoDelete(bool bSet) { m_bAutoDelete = bSet; }
        inline bool GetAutoDelete() const { return m_bAutoDelete; }

        inline void SetResetTrigger(bool bReset) { m_bResetTrigger = bReset; }
        inline bool GetResetTrigger() const { return m_bResetTrigger; }

        inline virtual void SetPersistent(bool bSet) { m_bPersistent = bSet; }
        inline bool GetPersistent() const { return m_bPersistent; }

        inline bool GetGameMode() const { return m_bGameMode; }
        inline bool GetGhost() const { return m_bGhost; }
        inline void SetGhost(bool bGhost) { m_bGhost = bGhost; }

        inline const std::string& GetName() const { return m_strName; }
        inline const mapInputs& GetInputs() const { return m_inputs; }
        inline const mapEvents& GetEvents() const { return m_events; }

        inline unsigned int GetNumEvents() const { return m_events.size(); }

        mapOutputs& GetOutputs();
        void GetOutputs(const std::string& strReceiver, listReceiverOutputs& outputs);
        void GetOutput(EntityOutput& output, const std::string& strReceiver, unsigned int key);
        void SetOutput(const EntityOutput& output, const std::string& strReceiver);
        bool HasOutput(const std::string& strReceiver, unsigned int key);

        bool HasEvent(unsigned int id);
        bool GetEventName(std::string& strEventName, unsigned int id);
        bool GetEventID(const std::string& strEventName, unsigned int& id);
        void RegisterEvent(const std::string& strEventName, unsigned int eventId);
        void RegisterEventParameter(unsigned int eventID,
                                    const std::string& strEventParamName,
                                    CBaseParam::PARAMETER_TYPE paramType,
                                    const CBaseParam& initialValue = CBaseParam(),
                                    const CBaseParam& minValue = CBaseParam(),
                                    const CBaseParam& maxValue = CBaseParam());
        void RegisterEventParameter(unsigned int eventID, 
                                    const std::string& strEventParamName, 
                                    CBaseParam::PARAMETER_TYPE paramType, 
                                    const std::string& strEventParamDesc, 
                                    const CBaseParam& initialValue = CBaseParam(),
                                    const CBaseParam& minValue = CBaseParam(), 
                                    const CBaseParam& maxValue = CBaseParam());
        void GetEventParameter(unsigned int eventID, mapEventParameterDescs& eventParamDescs);

    protected:

        virtual void InternalUpdate(float dt);
        virtual void InternalPostUpdate(float dt);

        void SetName(const std::string& strName) { m_strName = strName; }
        void SetMetaTypeID(ENTITY_TYPEID typeId) { m_metaTypeId = typeId; }
        void AddOutput(EntityOutput& output);
        void SetOutputSender(CEntity* pReceiver, const std::string& strOldReceiverName);
        void RemoveOutputs(const std::string& strName);
        void RemoveOutput(const std::string& strName, unsigned int key);
        void AddInput(const EntityInput& input, unsigned int key);
        void SetInputSender(unsigned int key, const std::string& strName);
        void RemoveInput(unsigned int key);

    protected:

        mapOutputs m_outputs;

        std::list< EntityOutputDelayed > m_outputsDelayed;

    private:

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CGeometryCollidable >(*this);
            ar & m_pNode;
            ar & m_metaTypeId;
            ar & m_bFireOnce;
            ar & m_fireDelay;
            ar & m_strName;
            ar & m_inputs;
            ar & m_outputs;
            ar & m_bPickable;
            ar & m_bAutoDelete;
            ar & m_bGhost;

            if (version > 0)
                ar & m_bPersistent;

            if (version > 1)
                ar & m_bFireOnStart;

            if (version > 2)
            {
                ar & m_bDeletable;
                ar & m_bSlidable;
            }

            if (version > 3)
                ar & m_bResetTrigger;
        }

    private:
        ENTITY_TYPEID m_metaTypeId;

        std::string m_strName;

        bool m_bGameMode;
        bool m_bFireOncePerHit;
        bool m_bFireOnce;
        bool m_bPickable;
        bool m_bDeletable;
        bool m_bSlidable;
        bool m_bAutoDelete;
        bool m_bResetTrigger;
        bool m_bTriggerEnabled;
        bool m_bTriggered;


        float m_fireDelay;
        float m_baseTime;
        float m_fireDelayed;
        float m_fireDebugOffset;

        mapEvents m_events;
        mapInputs m_inputs;

        std::set< CEntity* > m_triggeredEntities;

    protected:
        bool m_bFiredPrevFrame;
        bool m_bFirePaused;
        bool m_bPicked;
        bool m_bGhost;
        bool m_bPersistent;
        bool m_bFireOnStart;
        bool m_bJustCreated;

        CSceneNode* m_pNode;
    };

} // env
#endif // ENTITY_H 
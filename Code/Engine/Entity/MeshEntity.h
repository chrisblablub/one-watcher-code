/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MESH_BASE_ENTITY_H
#define MESH_BASE_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/Entity.h>
#include <Engine/Scene/MeshInstance.h>
#include <Engine/Entity/SoundSpriteEntity.h>

namespace env
{
    class CMeshEntity : public CEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS {
            MESH_EVENT_SHOW = ENTITY_EVENT_END, 
            MESH_EVENT_HIDE, 
            MESH_EVENT_END, 
            MESH_EVENT_SET_DIFFUSE_COLOR = MESH_EVENT_END + 15,
            MESH_EVENT_SET_EMISSIVE_COLOR
        };

        CMeshEntity();
        virtual ~CMeshEntity();

        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter) override;
        virtual bool HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);

        virtual void SetMeshFilename(const std::string& strName);

        const CMeshInstanceContainer& GetMeshInstanceContainer() const { return m_meshInstContainer; }

        CSceneNode* GetMeshInstanceNode() const { return m_pMeshInstanceNode; }

        virtual void SetRotationSpeedZ(float rot);
        virtual void SetRotationSpeedY(float rot);
        virtual void SetRotationSpeedX(float rot);

        void SaveTemplate(const std::string& strFilename, const std::string& strName);
        void LoadTemplate(const std::string& strFilename, bool bInitMaterial = true);

        void SetInitFromMeshTemplate(bool bSet) { m_bInitFromTemplate = bSet; }
        bool GetInitFromMeshTemplate() const { return m_bInitFromTemplate; }

        void SetMeshTemplate(const std::string& strTemplate) { m_strTemplate = strTemplate; }
        const std::string GetMeshTemplate() const { return m_strTemplate; }

        template<typename Lambda>
        void CallForEachNode(Lambda lambda) const { std::for_each(m_meshInstanceNodes.begin(), m_meshInstanceNodes.end(), lambda); }

        CSceneNode* GetMeshInstanceNode(size_t meshInstanceIndex) { return m_meshInstanceNodes[meshInstanceIndex]; }


    protected:
        virtual void SetupSounds() {};
        CSoundSpriteEntity* m_pSound;

    private:
        friend class boost::serialization::access;

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            CMeshInstance* pMeshInst = nullptr;

            ar & boost::serialization::base_object< CEntity >(*this);
            ar & pMeshInst;
            ar & m_pMeshInstanceNode;
            ar & m_rotationSpeedZ;
            ar & m_rotationSpeedX;
            ar & m_rotationSpeedY;

            if (version > 0)
                ar & m_meshInstContainer;

            if (version > 1)
                ar & m_meshInstanceNodes;

            if (version > 2)
            {
                ar & m_bInitFromTemplate;
                ar & m_strTemplate;
            }

            if (version > 3)
                ar & m_pSound;
        }

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            m_version = version;

            CMeshInstance* pMeshInst = nullptr;

            ar & boost::serialization::base_object< CEntity >(*this);
            ar & m_pMeshInstance;
            ar & m_pMeshInstanceNode;
            ar & m_rotationSpeedZ;
            ar & m_rotationSpeedX;
            ar & m_rotationSpeedY;

            if (version > 0)
                ar & m_meshInstContainer;

            if (version > 1)
                ar & m_meshInstanceNodes;

            if (version > 2)
            {
                ar & m_bInitFromTemplate;
                ar & m_strTemplate;
            }

            if (version > 3)
                ar & m_pSound;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER();

        void DestroyMeshInstancesAndNodes();
        void InitDebugName();

    private:

        float m_rotationSpeedZ;
        float m_rotationSpeedY;
        float m_rotationSpeedX;

        unsigned int m_version;

        bool m_bInitFromTemplate;

        std::string m_strTemplate;
        std::vector<CMaterial> m_materialsInitial;
        std::vector<CMaterial> m_materialsBegin;
        std::vector<CMaterial> m_materialsEnd;

        float m_diffuseDuration;
        float m_diffuseTime;
        float m_emissiveDuration;
        float m_emissiveTime;

        CMeshInstance* m_pMeshInstance;

        CSceneNode* m_pMeshInstanceNode;

        CMeshInstanceContainer m_meshInstContainer;
        std::vector<CSceneNode*> m_meshInstanceNodes;
    };


} // env

#endif // MESH_BASE_ENTITY_H 
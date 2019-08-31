/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/AttachableObject.h>
#include <Graphic/Core/MeshContainer.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{

    class CBoundingBoxDataCache;

    class CMeshInstance : public CAttachableObject
    {
    public:

        void AddToRenderQueue(RenderQueue& renderQueue, float dt);
        void Update(float dt);

        CMeshContainer* GetAttachedMeshContainer();
        CMesh* GetAttachedSubMesh();
        const std::string& GetAttachedMeshContainerName();
        const std::string& GetAttachedSubMeshName();

        void AttachMeshContainer(const std::string& strMeshContainer, const std::string& strSubMesh, bool bInitMaterial = true);
        void DetachMeshContainer();

        void ShowBBox(bool bShow = true);

        const std::string& GetName();

        MeshInstanceRenderObject* GetMeshInstanceRenderObject() { return &m_instanceRenderData; }

        void GetInstanceAndMeshesRenderObject(InstanceAndMeshesRenderObject& renderObject);

        inline void SetRotationSpeedZ(float rot) { m_rotSpeedX.z = rot; }
        inline float GetRotationSpeedZ() const { return m_rotSpeedX.z; }

        inline void SetRotationSpeedY(float rot) { m_rotSpeedX.y = rot; }
        inline float GetRotationSpeedY() const { return m_rotSpeedX.y; }

        inline void SetRotationSpeedX(float rot) { m_rotSpeedX.x = rot; }
        inline float GetRotationSpeedX() const { return m_rotSpeedX.x; }

        inline void SetRotationSpeed(const Vec3& rotationSpeed) { m_rotSpeedX = rotationSpeed; }
        inline const Vec3& GetRotationSpeed() const { return m_rotSpeedX; }

        CMaterial& GetMaterial() { return m_material; }
        CMaterial* GetMaterialPtr() { return &m_material; }

    private:

        CMeshInstance();
        CMeshInstance(const std::string& strName);
        CMeshInstance(const CMeshInstance&) = delete;
        CMeshInstance& operator=(const CMeshInstance&) = delete;
        ~CMeshInstance();

        void InitializeAtLoad();

        void UpdateMeshInstanceRenderObject();

    private:
        friend class CSceneMgr;
        friend class CSceneNode;
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            bool bDummy = false;
            float fDummy = 0.0f;
            unsigned long lDummy = 0;
            Vec3 dummyVec3(0, 0, 0);
            MESH_RENDER_CATEGORY meshCatDummy = MESH_RENDER_CATEGORY_DEFERRED;

            ar & boost::serialization::base_object< CAttachableObject >(*this);
            ar & m_strName;
            ar & m_strMeshContainer;
            ar & bDummy;
            ar & bDummy;
            ar & bDummy;
            ar & bDummy;
            ar & bDummy;
            ar & bDummy;
            ar & bDummy;
            ar & dummyVec3;

            if (version > 0)
            {
                ar & bDummy;
                ar & bDummy;
            }

            if (version > 1)
                ar & meshCatDummy;

            if (version > 2)
                ar & dummyVec3;

            if (version > 3)
            {
                ar & fDummy;
                ar & fDummy;
            }

            if (version > 4)
            {
                ar & bDummy;
                ar & fDummy;
            }

            if (version > 5)
            {
                ar & lDummy;
            }

            if (version > 6)
            {
                ar & dummyVec3;
                ar & fDummy;
            }

            if (version > 7)
            {
                ar & m_strSubMesh;
            }

            if (version > 8)
            {
                ar & m_rotSpeedX;
            }

            if (version > 9)
            {
                ar & m_material;
            }
        }

        std::vector<CBoundingBoxDataCache*> m_boundingBoxesDataCache;

        void CleanUp();

        std::string m_strName;
        std::string m_strMeshContainer;
        std::string m_strSubMesh;

        MeshInstanceRenderObject m_instanceRenderData;

        CMeshContainer* m_pMeshContainer;
        
        CMaterial m_material;

        bool m_bShowBBox;

        Matrix4x4 m_mtxPrevTransform;


        Vec3 m_rotSpeedX;


    };

    class CMeshInstanceContainer
    {
        friend class CSceneMgr;

    public:
        ~CMeshInstanceContainer();

        void InitFromMeshContainer(const std::string& strMeshContainer);

        unsigned int GetNumMeshInstances() const { return m_meshInstances.size(); }

        CMeshInstance* GetMeshInstance(const std::string& strSubMesh) const;
        CMeshInstance* GetMeshInstance(unsigned int subMeshInstanceIndex) const;

        const std::string& GetMeshContainer() const { return m_strMeshContainer; }
        const std::vector<CMeshInstance*>& GetMeshInstances() const { return m_meshInstances; }

        void GetMeshInstanceRenderObjects(mapMeshInstanceRenderObjects& instancesAndMesh) const
        {
            for (CMeshInstance* pMeshInstance : m_meshInstances)
            {
                instancesAndMesh[pMeshInstance->GetAttachedSubMesh()->GetMeshRenderObject()].push_back(pMeshInstance->GetMeshInstanceRenderObject());
            }
        }
        template<typename Lambda>
        void CallForEach(Lambda lambda) const { std::for_each(m_meshInstances.begin(), m_meshInstances.end(), lambda); }

        void Destroy();

    private:

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_meshInstances;
            ar & m_indexNameMapping;

            if (version > 1)
                ar & m_strMeshContainer;
        }

        void AddMeshInstance(CMeshInstance* pSubMeshInstance);

    private:
        std::vector<CMeshInstance*> m_meshInstances;
        std::map<std::string, unsigned int> m_indexNameMapping;
        std::string m_strMeshContainer;

    };
} // enf
#endif // MESH_INSTANCE_H
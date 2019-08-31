/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SCENEMGR_H
#define SCENEMGR_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/MeshInstance.h>
#include <Engine/Scene/Light.h>
#include <Engine/Scene/SceneNode.h>
#include <Engine/Core/Engine.h>
#include <Graphic/Core/Mesh.h>

namespace env
{
    class CMeshInstanceContainer;

    class CSceneMgr
    {
    public:
        typedef std::map< std::string, CLight* > mapLights;
        typedef std::map< std::string, CSceneNode* > mapScenNodes;
        typedef std::map< std::string, std::string > mapSceneNodeParents;
        typedef std::map< std::string, CMeshInstance* > mapMeshInstances;


        CSceneMgr();
        CSceneMgr(const CSceneMgr&) = delete;
        CSceneMgr& operator=(const CSceneMgr&) = delete;
        ~CSceneMgr();

        void Reset(bool bCreateRootNode);

        void InitializeAtLoad();

        void AddToRenderQueue(RenderQueue& renderQueue, float dt);
        void Update(float dt);
        void PrepareToSave();

        void SetGameMode(bool bGameMode);

        void RemoveMeshInstanceSelected();

        bool IsValid(CSceneNode* pNode);

        CLight* CreateLight(const std::string& strName);
        CLight* CreateLight();
        CLight* GetLight(const std::string& strName);
        inline const mapLights& GetLights() const { return m_lights; }

        void RemoveLight(const std::string& strName);

        void RemoveLightSelected();
        inline CMeshInstance* GetSelectedMeshInstance() const { return m_pSelectedMeshInstance; }
        inline CLight* GetSelectedLight() const { return m_pSelectedLight; }

        void SetRootNode(CSceneNode* pSceneNode);

        CSceneNode* CreateSceneNode(const std::string& strNodeName, const std::string& strAttachTo = std::string());
        CSceneNode* CreateSceneNode();
        CSceneNode* GetSceneNode(const std::string& strName) const;
        const mapScenNodes& GetSceneNodes() const { return m_sceneNodes; }
        inline CSceneNode* GetRootNode() const { return GetSceneNode("rootNode"); }
        bool RemoveNode(CSceneNode* pNode);
        bool RemoveNode(const std::string& strNode);

        // For editor only, to update mesh instance containers after a hot reload of a mesh container
        void RegisterMeshInstanceContainer(CMeshInstanceContainer* pContainer);
        void UnregisterMeshInstanceContainer(CMeshInstanceContainer* pContainer);
        void UpdateMeshInstanceContainers();

//        void CreateMeshInstances(mapMeshInstances& meshInstances, const std::string& strInstanceName, const std::string& strMesh);
        CMeshInstance* CreateMeshInstances(const std::string& strMeshContainer, const std::string& strMesh);
        CMeshInstance* GetMeshInstance(const std::string& strInstance);
        void RemoveMeshInstance(const std::string& strName);
        void RemoveMeshInstance(CMeshInstance* pMeshInstance);
        void RemoveMeshInstanceWithMesh(const std::string& strMeshName);

        void SetBoundingBoxesVisible(bool bShow = true);
        bool AreBoundingBoxesVisible();

        void Offset(float x, float y);

    private:

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_sceneNodes;
            ar & m_pRootNode;
            ar & m_meshInstances;
            ar & m_lights;
            ar & m_idLights;
            ar & m_idMeshInstances;
            ar & m_idNodes;
        }

    private:

        CSceneNode* m_pRootNode;
        CMeshInstance* m_pSelectedMeshInstance;
        CLight* m_pSelectedLight;

        mapScenNodes m_sceneNodes;
        mapSceneNodeParents m_sceneNodeParents;
        mapLights m_lights;
        mapMeshInstances m_meshInstances;
        std::map<std::string, std::set<CMeshInstanceContainer*>> m_meshInstanceContainers;

        mapScenNodes m_sceneNodesSerialize;
        mapMeshInstances m_meshInstancesSerialize;
        mapLights m_lightsSerialize;
        std::map< std::string, std::string > m_lightNodesSerialize;
        std::map< std::string, std::string > m_meshInstanceNodesSerialize;


        unsigned int m_idMeshInstances;
        unsigned int m_idLights;
        unsigned int m_idNodes;

        bool m_boundingBoxesVisible;
        bool m_bGameMode;
    };

} // env
#endif // SCENEMGR_H
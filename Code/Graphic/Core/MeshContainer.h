/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MESH_CONTAINER_H
#define MESH_CONTAINER_H


#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/Mesh.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/Material.h>
#include <Common/Core/BoundingVolume.h>

struct aiScene;
struct aiNode;

namespace env
{
    typedef std::map< std::string, CMesh* > mapSubMeshes;

    class CMeshContainer
    {
    public:

        typedef CResMgrBase< std::string, CMeshContainer > CMeshMgr;
        friend class CResMgrBase< std::string, CMeshContainer >;

        CMeshContainer() : m_bChanged(false) {}
        ~CMeshContainer();

        CBoundingVolume* GetBoundingVolume() { return &m_boundingVolume; }

        const std::string& GetName() const { return m_strName; }

        mapSubMeshes& GetMeshes() { return m_meshes; }

        CMesh* GetMesh(const std::string& strMesh);

        bool InitResource(const std::string& strFilename, const std::string& strName, _FILETIME* pFt);
        bool HasChanged() const { return m_bChanged; }
        void SetChanged(bool bSet) { m_bChanged = bSet; }

    private:

        bool LoadResource();
        bool LoadMeshesRecursive(const aiScene* pScene, aiNode* pNode, const Matrix4x4& mtxParentTransform);

        void Unload();

        bool ComputeBoundingVolume(const std::vector< PosNormalMapTexVertex >& vertices, Vec3& minPos, Vec3& maxPos);

    private:
        std::string m_strFilename;
        std::string m_strName;

        CBoundingVolume m_boundingVolume;

        mapSubMeshes m_meshes;

        bool m_bChanged;
    };



    typedef CMeshContainer::CMeshMgr CMeshMgr;

    extern CMeshMgr* gMeshMgr;

} // env
#endif // MESH_CONTAINER_H
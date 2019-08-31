/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MESH_RENDER_OBJECT_H
#define MESH_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>

namespace env
{
    class CMaterial;

    class CVertexBuffer;
    class CIndexBuffer;

    struct MeshRenderObject
    {
        CVertexBuffer* m_pVertexBuffer;
        CIndexBuffer* m_pIndexBuffer;

        unsigned int m_numVertices;
        unsigned int m_numFaces;

        //CMaterial* pMaterial;

        Matrix4x4 m_mtxTransform;
    };


    struct MeshInstanceRenderObject
    {
        MeshInstanceRenderObject()
            : m_pMaterial(nullptr) {}

        Matrix4x4 m_mtxTransform;
        Matrix4x4 m_mtxPrevTransform;

        CMaterial* m_pMaterial;
    };

    struct InstanceAndMeshesRenderObject
    {
        MeshInstanceRenderObject* m_InstanceRenderObject;
        MeshRenderObject* m_pMeshRenderObject;
    };

    typedef std::list< MeshInstanceRenderObject* > listMeshInstanceRenderObjects;
    typedef std::map< MeshRenderObject*, listMeshInstanceRenderObjects > mapMeshInstanceRenderObjects;
    typedef std::vector< mapMeshInstanceRenderObjects > vecMeshInstanceRenderObjectCategories;
} // env
#endif // MESH_RENDER_OBJECT_H 
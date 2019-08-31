/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MESH_H
#define MESH_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/RenderObjects/MeshRenderObject.h>
#include <Graphic/Core/VertexBuffer.h>
#include <Graphic/Core/IndexBuffer.h>
#include <Graphic/Core/Material.h>

namespace env
{
    class CMesh final
    {
    public:
        CMesh(CVertexBuffer* pVB, CIndexBuffer* pIB, const CMaterial& pMaterial,
              const Matrix4x4& mtxTransform, CBoundingVolume& boundingVolume,
              unsigned int numFaces, unsigned int numVertices);

        ~CMesh();

        void SetModelTransform(const Matrix4x4& mtxModel);

        CBoundingVolume* GetBoundingVolume() { return &m_boundingVolume; }

        MeshRenderObject* GetMeshRenderObject() { return &m_meshRenderObject; }

        CMaterial* GetMaterial() { return &m_pMaterial; }

    private:

        MeshRenderObject m_meshRenderObject;

        CBoundingVolume m_boundingVolume;

        CMaterial m_pMaterial;
    };

} // env
#endif // MESH_H
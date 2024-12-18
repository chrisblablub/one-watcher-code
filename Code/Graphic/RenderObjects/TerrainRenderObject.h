/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TERRAIN_RENDER_OBJECT_H
#define TERRAIN_RENDER_OBJECT_H

namespace env
{
    class CVertexBuffer;
    class CIndexBuffer;
    class CManagedMaterial;

    typedef short TerrainIndex;

    struct TerrainBufferCell
    {
        TerrainBufferCell()
            : m_pVB(nullptr)
            , m_pIB(nullptr)
            , m_pVBLayer0(nullptr)
            , m_pIBLayer0(nullptr)
            , m_numTerrainTriangles(0)
            , m_numTerrainIndices(0)
            , m_numTerrainTrianglesLayer0(0)
            , m_numTerrainIndicesLayer0(0) {}

        CVertexBuffer* m_pVB;
        CIndexBuffer* m_pIB;
        CVertexBuffer* m_pVBLayer0;
        CIndexBuffer* m_pIBLayer0;

        unsigned int m_numTerrainTriangles;
        unsigned int m_numTerrainIndices;
        unsigned int m_numTerrainTrianglesLayer0;
        unsigned int m_numTerrainIndicesLayer0;

        void Destroy()
        {
            ENV_DELETE(m_pVBLayer0);
            ENV_DELETE(m_pIBLayer0);
            ENV_DELETE(m_pVB);
            ENV_DELETE(m_pIB);

            m_numTerrainIndices = 0;
            m_numTerrainTriangles = 0;
        }
    };

    typedef std::vector< TerrainBufferCell > vecTerrainBufferCells;

    struct TerrainRenderObject
    {
        TerrainRenderObject()
            : m_numPrevTriangles(0)
            , m_numPrevPatternTriangles(0)
            , m_offsetPrevPatternVertices(0)
            , m_pWireframeVB(nullptr)
            , m_pBorderVB(nullptr) {}

        CVertexBuffer* m_pWireframeVB;
        CVertexBuffer* m_pBorderVB;

        const TerrainBufferCell* m_previewBuffer;
        const TerrainBufferCell* m_selectionBuffer;

        vecTerrainBufferCells m_terrainBuffers;

        CManagedMaterial* m_pMaterial;

        Vec4 m_envMtrl;

        unsigned int m_numCellsX;
        unsigned int m_numCellsY;
        unsigned int m_numPrevTriangles;
        unsigned int m_numPrevPatternTriangles;
        unsigned int m_offsetPrevPatternVertices;
        unsigned int m_numBorderTriangles;

        int m_cellSize;
    };

} // env
#endif // TERRAIN_RENDER_OBJECT_H 
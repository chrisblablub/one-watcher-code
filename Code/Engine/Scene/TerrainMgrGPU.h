/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

#include <Engine/Scene/TerrainCell.h>
#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/Material.h>
#include <Graphic/RenderObjects/TerrainRenderObject.h>
#include <Graphic/Core/DataCache.h>

namespace env
{
    struct TerrainVertex;
    struct TerrainBorderVertex;
    struct TerrainPainterVertex;

    class CRenderDevice;
    class CVertexBuffer;
    class CIndexBuffer;

    struct PainterVertex
    {
        PainterVertex()
            : alpha(0.0f, 0.0f),
            color01(1, 1, 1),
            color02(1, 1, 1),
            contrast(0.0f) {}

        Vec2 alpha;
        Vec3 color01;
        Vec3 color02;
        float contrast;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & alpha;

            if (version > 0)
            {
                ar & color01;
                ar & color02;
            }

            if (version > 1)
                ar & contrast;

        }
    };



    typedef std::pair< LevelHalfCell*, LevelHalfCell* > pairHalfCells;
    typedef std::map< unsigned int, pairHalfCells > mapHalfCells;
    typedef std::vector< TerrainCell > mapCells;
    typedef std::map< unsigned int, PainterVertex > mapPainterVertices;
    typedef std::vector< mapPainterVertices > vecPainterVertices;
    typedef std::vector< mapPainterVertices > vecMapPainterVertices;
    typedef std::vector< vecPainterVertices > vecvecPainterVertices;

    struct LevelCellEditParameters
    {
        LevelCellEditParameters()
            : currentEditSequence(0)
            , currentEditLayer(0)
            , currentEditColor(1.0f, 1.0f, 1.0f)
            , currentEditBorderColor(0.0f, 0.0f, 0.0f)
            , currentEditEmissiveColor(1.0f, 1.0f, 1.0f, 1.0f)
            , currentEditCellLayout(CELL_LAYOUT_FULL)
            , currentEditRotation(CELL_ROTATION_0DEG_CW)
            , bRandomRotation(false)
            , brushSize(1) {}

        unsigned int currentEditLayer;
        unsigned int currentEditSequence;
        unsigned int brushSize;

        Vec3 currentEditColor;
        Vec3 currentEditBorderColor;
        Vec4 currentEditEmissiveColor;

        CELL_ROTATION currentEditRotation;
        CELL_LAYOUT currentEditCellLayout;

        bool bRandomRotation;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & currentEditLayer;
            ar & currentEditColor;
            ar & currentEditRotation;
            ar & currentEditCellLayout;
            ar & currentEditSequence;
        }
    };

    class CTerrainCellIndexer
    {
    public:
        CTerrainCellIndexer(const unsigned int& numCellsX, 
                            const unsigned int& numCellsY, 
                            const unsigned int& numCellsPerBuffer,
                            const unsigned int& numBuffersX,
                            const unsigned int& numBuffersY)
            : m_numCellsX(numCellsX)
            , m_numCellsY(numCellsY)
            , m_numCellsPerBuffer(numCellsPerBuffer)
            , m_numVertexBuffersX(numBuffersX)
            , m_numVertexBuffersY(numBuffersY) {}

        void SetCellIndex(unsigned int cellIndex)
        {
            m_cellIndex = cellIndex;
        }

        void SetCellIndices(unsigned int cellX, unsigned int cellY)
        {
            m_cellIndex = cellX * m_numCellsY + cellY;
        }

        void SetBufferCellIndex(unsigned int bufferCellIndex, unsigned int bufferIndex)
        {
            unsigned int bufferCellX = bufferCellIndex / m_numCellsPerBuffer;
            unsigned int bufferCellY = bufferCellIndex % m_numCellsPerBuffer;

            SetBufferCellIndices(bufferCellX, bufferCellY, bufferIndex);
        }

        void GetVertexBufferIndices(unsigned int& bufferX, unsigned int& bufferY) const
        {
            unsigned int cellX, cellY;
            GetCellIndices(cellX, cellY);

            bufferX = cellX / m_numCellsPerBuffer;
            bufferY = cellY / m_numCellsPerBuffer;
        }

        void SetBufferCellIndices(unsigned int bufferCellX, unsigned int bufferCellY, unsigned int bufferIndex)
        {
            unsigned int vertexBufferX = bufferIndex / m_numVertexBuffersY;
            unsigned int vertexBufferY = bufferIndex % m_numVertexBuffersY;

            m_cellIndex = (bufferCellX + vertexBufferX * m_numCellsPerBuffer) * m_numCellsY + (bufferCellY + vertexBufferY * m_numCellsPerBuffer);
        }

        void GetCellIndices(unsigned int& cellX, unsigned int& cellY) const
        {
            cellX = m_cellIndex / m_numCellsY;
            cellY = m_cellIndex % m_numCellsY;
        }

        unsigned int GetCellIndex() const
        {
            return m_cellIndex;
        }

        unsigned int GetVertexBufferIndex() const
        {
            unsigned int cellX, cellY;
            GetCellIndices(cellX, cellY);

            unsigned int t = 5 / 10;
            unsigned int x = cellX / m_numCellsPerBuffer;
            unsigned int y = cellY / m_numCellsPerBuffer;

            return x * m_numVertexBuffersY + y;
        }

        void GetBufferCellIndices(unsigned int& bufferCellX, unsigned int& bufferCellY) const
        {
            unsigned int cellX, cellY;
            GetCellIndices(cellX, cellY);

            unsigned int vertexBufferIndex = GetVertexBufferIndex();
            unsigned int vertexBufferX = vertexBufferIndex / m_numVertexBuffersY;
            unsigned int vertexBufferY = vertexBufferIndex % m_numVertexBuffersY;

            bufferCellX = cellX - vertexBufferX * m_numCellsPerBuffer;
            bufferCellY = cellY - vertexBufferY * m_numCellsPerBuffer;
        }

        unsigned int GetBufferCellX() const
        {
            unsigned int bufferCellX, bufferCellY;
            GetBufferCellIndices(bufferCellX, bufferCellY);

            return bufferCellX;
        }

        unsigned int GetBufferCellY() const
        {
            unsigned int bufferCellX, bufferCellY;
            GetBufferCellIndices(bufferCellX, bufferCellY);

            return bufferCellY;
        }

        unsigned int GetBufferCellIndex() const
        {
            unsigned int bufferCellX, bufferCellY;
            GetBufferCellIndices(bufferCellX, bufferCellY);

            return bufferCellX * m_numCellsPerBuffer + bufferCellY;
        }

    private:
        const unsigned int& m_numCellsX;
        const unsigned int& m_numCellsY;
        const unsigned int& m_numCellsPerBuffer;
        const unsigned int& m_numVertexBuffersX;
        const unsigned int& m_numVertexBuffersY;

        unsigned int m_cellIndex;
    };

    class CTerrainMgrGPU
    {
    public:
        enum CELL_COLOR_TYPE
        {
            CELL_COLOR_TYPE_LAYER = 0,
            CELL_COLOR_TYPE_LIGHTING
        };

        CTerrainMgrGPU();
        ~CTerrainMgrGPU();

        void Initialize(unsigned int numCellsX, unsigned int numCellsY, CManagedMaterial* pMaterial, mapCells* pCells, mapHalfCells* pHalfCells, const Vec3& borderRotationCenter, float borderRotationAngle, const std::vector< Vec3 >& borderColorPresets,
            std::vector<float>& layerDepths, float maxLayerDepth);

        void AddToRenderQueue(RenderQueue& renderQueue);

        void OnLostDevice();
        void OnResetDevice();

        void CreateTerrainVertexBuffer(unsigned int numCellsX, unsigned int numCellsY);
        void CreateAndInitTerrainVertexBuffer(unsigned int numCellsX, unsigned int numCellsY);

        void SetMaterial(CManagedMaterial* pMaterial);

        void SetCellColorType(CELL_COLOR_TYPE type) { m_cellColorType = type; UpdateTerrainBuffers(); }
        CELL_COLOR_TYPE GetCellColorType() const { return m_cellColorType; }

        void SetSize(unsigned int numCellsX, unsigned int numCellsY);
        unsigned int GetNumCellsX() const { return m_numCellsX; }
        unsigned int GetNumCellsY() const { return m_numCellsY; }

        void SetLayerDepths(std::vector<float>& layerDepths, float maxLayerDepth);

        void SetBorderColorPresets(const std::vector< Vec3 >& borderColorPresets);

        void UpdateTerrainBuffers(const std::vector<unsigned int>& cells = std::vector<unsigned int>());

        void UpdateTerrainWireframeBuffer(bool bCreate);
        void UpdateBorderTexture(const std::string& strBorderTexture);
        
        void CreatePreview(bool bPatternPreview, unsigned int cellX, unsigned int cellY, LevelCellEditParameters& cellEditParams, unsigned int brushSize);
        void DestroyPreview();

        void UpdateSelection(mapHalfCells& cells);
        void RemoveSelection();
    
        void SetLayerMaxDepth(float d) { m_maxLayerDepth = d; }
        void SetBorderRotationAngle(float angle) { m_borderRotationAngle = angle; }
        void SetBorderRotationCenter(const Vec3& center) { m_borderRotationCenter = center; }

        void SetBorder(bool bSet) { m_bBorder = bSet; UpdateBorderVertexBuffer(); }
        bool GetBorder() const { return m_bBorder; }

        CVertexBuffer* GetBorderVertexBuffer() const { return m_pVBBorder; }
        const TerrainBufferCell* GetPreviewVertexBuffer() const { return &m_previewBuffer; }
        const TerrainBufferCell* GetSelectionVertexBuffer() const { return &m_selectionBuffer; }
        CVertexBuffer* GetSliderVertexBuffer() const { return m_pVBSlider; }

        CManagedMaterial* GetMaterial() const { return m_pMaterial; }
        CTextureManaged* GetBorderTexture() const { return m_pBorderTexture; }
        
        void SetSelectionOffset(mapHalfCells& cells, int x, int y) { m_selectionOffsetX = x; m_selectionOffsetY = y; UpdateSelection(cells); }
        int GetSelectionOffsetX() const { return m_selectionOffsetX; }
        int GetSelectionOffsetY() const { return m_selectionOffsetY; }

        vecTerrainBufferCells& GetTerrainVertexBuffers() { return m_terrainVertexBuffers; }


        CVertexBuffer* GetTerrainWireframeVertexBuffer() const { return m_pVBWireframe; }

        unsigned int GetNumPreviewTriangles() const { return m_levelGridRenderObject.m_numPrevTriangles; }
        unsigned int GetNumBorderTriangles() const { return m_numBorderTriangles; }

        const std::vector< Matrix4x4 >& GetLayerTransformations() const { return m_layerTransformations; }
        float GetRotation(float depth);

        void TransformVertex(Vec3& vertex, unsigned int layer);

        Vec3 ComputeTransformedPosition(Vec3 vertex);
    
    
    private:
    
        void UpdateBorderVertexBuffer();
        void UpdateTerrainVertexBuffer(unsigned int* pCellX = NULL, unsigned int* pCellY = NULL);
        void UpdateTerrainVertexBuffer(const std::vector<unsigned int>& cells);

        void UpdateLayerTransformations();

        void UpdateTerrainCell(const CTerrainCellIndexer& indexer, pairHalfCells& phc, TerrainVertex* pVertices, TerrainIndex* pIndices);
        void UpdateTerrainCellIndices(const CTerrainCellIndexer& indexer, pairHalfCells& phc, TerrainIndex* pIndices);
        void UpdateTerrainCellVertices(unsigned int x, unsigned int y, unsigned int& numTrianglesAdded, pairHalfCells& phc, TerrainVertex* pVertices);

        void AddCellDataToTerrainBuffer(unsigned int& numTrianglesAdded,
                                        TerrainVertex* pBuffer,
                                        const std::vector< Vec3 >& vertices,
                                        const std::vector< Vec2 >& texcoords,
                                        const std::vector< Vec4 >& lighting,
                                        const Vec3& layerColor,
                                        const Vec4& emissiveColor,
                                        CELL_LAYOUT cellLayout,
                                        unsigned int cellLayer);

        void AddIndexDataToindexBuffer(const CTerrainCellIndexer& indexer, TerrainIndex* w, unsigned int baseIndex, CELL_LAYOUT cellLayout);

        void CreateTangentVectors(TerrainVertex& p0, TerrainVertex& p1, TerrainVertex& p2);
        bool CheckLeft(int x, int y, int layer);
        bool CheckRight(int x, int y, int layer);
        bool CheckTop(int x, int y, int layer);
        bool CheckBottom(int x, int y, int layer);

        void CreateBorderLeft(int x, int y, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, const Vec4& color);
        void CreateBorderRight(int x, int y, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, const Vec4& color);
        void CreateBorderTop(int x, int y, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, const Vec4& color);
        void CreateBorderBottom(int x, int y, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, const Vec4& color);
        void CreateBorder(int x, int y, LevelHalfCell* pCurrHalfCell, LevelHalfCell* pSideHalfCell, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, CELL_LAYOUT layout, const Vec4& color);

        int GetLayerRight(int x, int y);
        int GetLayerLeft(int x, int y);
        int GetLayerTop(int x, int y);
        int GetLayerBottom(int x, int y);

        void GetLightingColorsBorderLeft(int x, int y, Vec4& leftTop, Vec4& leftBot, Vec4& top, Vec4& bot);
        void GetLightingColorsBorderRight(int x, int y, Vec4& rightTop, Vec4& rightBot, Vec4& top, Vec4& bot);
        void GetLightingColorsBorderTop(int x, int y, Vec4& topLeft, Vec4& topRight, Vec4& left, Vec4& right);
        void GetLightingColorsBorderBot(int x, int y, Vec4& botLeft, Vec4& botRight, Vec4& left, Vec4& right);

        void CreateBordersHalfCell(LevelHalfCell* pHalfCell, TerrainBorderVertex* pV, int x, int y, int layer, int& currentTriangle, const Vec4& color);

        void CreateCellData(unsigned int x, unsigned int y, LevelHalfCell* pCell, std::vector< Vec3 >& vertices, std::vector< Vec2 >& texcoords);
        
        void UpdateTerrainBufferCellTriangles();

    private:

        const unsigned int m_numCellsPerVertexBuffer = 50;

        Vec3 m_borderRotationCenter;
        float m_borderRotationAngle;
        CTextureManaged* m_pBorderTexture;

        CRenderDevice* m_pDevice;
        CVertexBuffer* m_pVBWireframe;
        CVertexBuffer* m_pVBBorder;
        CVertexBuffer* m_pVBSlider;
       
        vecTerrainBufferCells m_terrainVertexBuffers;
        TerrainBufferCell m_previewBuffer;
        TerrainBufferCell m_selectionBuffer;

        std::vector<float> m_layerDepths;
        std::vector< Matrix4x4 > m_layerTransformations;

        float m_maxLayerDepth;

        std::vector<Vec3> m_borderColorPresets;

        CELL_COLOR_TYPE m_cellColorType;

        mapCells* m_pCells;
        mapHalfCells* m_pHalfCells;

        int m_selectionOffsetX;
        int m_selectionOffsetY;
        
        unsigned int m_numCellsX;
        unsigned int m_numCellsY;
        unsigned int m_numVertexBuffersX;
        unsigned int m_numVertexBuffersY;
        unsigned int m_numIndices;
        unsigned int m_numBorderTriangles;

        bool m_bBorder;

        CManagedMaterial* m_pMaterial;

        CDataCache<PolygonVertex> m_previewBorder;
        CDataCache<PolygonVertex> m_previewBackground;

        TerrainRenderObject m_levelGridRenderObject;

        CTerrainCellIndexer m_indexer;
    };
} // env


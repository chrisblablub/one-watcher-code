#include <Engine/Scene/TerrainMgrGPU.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/VertexBuffer.h>
#include <Graphic/Core/IndexBuffer.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CTerrainMgrGPU::CTerrainMgrGPU()
        : m_pDevice(nullptr)
        , m_pVBWireframe(nullptr)
        , m_pVBBorder(nullptr)
        , m_pBorderTexture(nullptr)
        , m_pVBSlider(nullptr)
        , m_numCellsX(0)
        , m_numCellsY(0)
        , m_numBorderTriangles(0)
        , m_numIndices(0)
        , m_bBorder(true)
        , m_cellColorType(CELL_COLOR_TYPE_LAYER)
        , m_indexer(m_numCellsX, m_numCellsY, m_numCellsPerVertexBuffer, m_numVertexBuffersX, m_numVertexBuffersY) {}

    //-----------------------------------------------------------------------------------
    CTerrainMgrGPU::~CTerrainMgrGPU()
    {
        for (auto& buffer : m_terrainVertexBuffers)
        {
            buffer.Destroy();
        }

        m_terrainVertexBuffers.clear();

        m_selectionBuffer.Destroy();
        m_previewBuffer.Destroy();

        if (m_pBorderTexture)
            gTextureMgr->Release(m_pBorderTexture->GetName());

        ENV_DELETE(m_pVBBorder);
        
        ENV_DELETE(m_pVBWireframe);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::Initialize(unsigned int numCellsX, unsigned int numCellsY, CManagedMaterial* pMaterial, mapCells* pCells, mapHalfCells* pHalfCells,
                                    const Vec3& borderRotationCenter, float borderRotationAngle, const std::vector< Vec3 >& borderColorPresets,
                                    std::vector<float>& layerDepths, float maxLayerDepth)
    {
        m_pDevice = gRenderSystem->GetRenderDevice();

        SetMaterial(pMaterial);

        m_pCells = pCells;
        m_pHalfCells = pHalfCells;

        SetBorderColorPresets(borderColorPresets);

        m_borderRotationCenter = borderRotationCenter;
        m_borderRotationAngle = borderRotationAngle;

        SetSize(numCellsX, numCellsY);

        SetLayerDepths(layerDepths, maxLayerDepth);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::AddToRenderQueue(RenderQueue& renderQueue)
    {
        m_levelGridRenderObject.m_cellSize = CELL_SIZE;
        m_levelGridRenderObject.m_numBorderTriangles = GetNumBorderTriangles();
        m_levelGridRenderObject.m_numCellsX = GetNumCellsX();
        m_levelGridRenderObject.m_numCellsY = GetNumCellsY();
        m_levelGridRenderObject.m_numPrevTriangles = GetNumPreviewTriangles();
        m_levelGridRenderObject.m_pMaterial = GetMaterial();
        m_levelGridRenderObject.m_previewBuffer = GetPreviewVertexBuffer();
        m_levelGridRenderObject.m_pBorderVB = GetBorderVertexBuffer();
        m_levelGridRenderObject.m_selectionBuffer = GetSelectionVertexBuffer();
        m_levelGridRenderObject.m_terrainBuffers = m_terrainVertexBuffers;
        m_levelGridRenderObject.m_pWireframeVB = GetTerrainWireframeVertexBuffer();

        renderQueue.m_pLevelGridRenderObject = &m_levelGridRenderObject;
        renderQueue.m_vector3Caches.push_back(&m_previewBorder);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::SetSize(unsigned int numCellsX, unsigned int numCellsY)
    {
        m_numCellsX = numCellsX;
        m_numCellsY = numCellsY;

       // m_indexer.SetNumCells(m_numCellsX, m_numCellsY);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::SetLayerDepths(std::vector<float>& layerDepths, float maxLayerDepth)
    {
        m_layerDepths = layerDepths;

        m_maxLayerDepth = maxLayerDepth;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::SetBorderColorPresets(const std::vector< Vec3 >& borderColorPresets)
    {
        m_borderColorPresets = borderColorPresets;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateTerrainBuffers(const std::vector<unsigned int>& cells)
    {
        UpdateTerrainVertexBuffer(cells);
        UpdateBorderVertexBuffer();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateTerrainWireframeBuffer(bool bCreate)
    {
        ENV_DELETE(m_pVBWireframe);

        if (!bCreate)
            return;

        std::vector<TerrainVertex> vertexData;
        vertexData.resize((m_numCellsX + 1) * (m_numCellsY + 1));

        int j = 0;
        for (int i = 0; i <= m_numCellsX; ++i)
        {
            // y lines
            vertexData[j].pos = Vec3(i * CELL_SIZE, 0.0, 0.0);
            vertexData[j + 1].pos = Vec3(i * CELL_SIZE, m_numCellsY * CELL_SIZE, 0.0);
            j += 2;
        }

        for (int i = 0; i <= m_numCellsY; ++i)
        {
            // x lines
            vertexData[j].pos = Vec3(0.0, i * CELL_SIZE, 0.0);
            vertexData[j + 1].pos = Vec3(m_numCellsX * CELL_SIZE, i * CELL_SIZE, 0.0);
            j += 2;
        }


        m_pVBWireframe = m_pDevice->CreateVertexBuffer(((m_numCellsX + 1) + (m_numCellsY + 1)) * 2 * sizeof(TerrainVertex), USAGE_TYPE_GPUREADACCESS, &vertexData[0]);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::OnLostDevice()
    {
        for (int i = 0; i < m_terrainVertexBuffers.size(); ++i)
        {
            ENV_DELETE(m_terrainVertexBuffers[i].m_pVBLayer0);
            ENV_DELETE(m_terrainVertexBuffers[i].m_pIBLayer0);
            ENV_DELETE(m_terrainVertexBuffers[i].m_pVB);
            ENV_DELETE(m_terrainVertexBuffers[i].m_pIB);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::OnResetDevice()
    {
        CreateTerrainVertexBuffer(m_numCellsX, m_numCellsY);

        UpdateTerrainVertexBuffer(0, 0);
    }

    void CTerrainMgrGPU::CreateAndInitTerrainVertexBuffer(unsigned int numCellsX, unsigned int numCellsY)
    {
        UpdateLayerTransformations();

        for (int i = 0; i < m_terrainVertexBuffers.size(); ++i)
        {
            ENV_DELETE(m_terrainVertexBuffers[i].m_pVBLayer0);
            ENV_DELETE(m_terrainVertexBuffers[i].m_pIBLayer0);
            ENV_DELETE(m_terrainVertexBuffers[i].m_pVB);
            ENV_DELETE(m_terrainVertexBuffers[i].m_pIB);
        }

        m_terrainVertexBuffers.clear();

        if (numCellsX == 0 || numCellsY == 0)
            return;

        m_numVertexBuffersX = numCellsX / m_numCellsPerVertexBuffer;
        m_numVertexBuffersY = numCellsY / m_numCellsPerVertexBuffer;

        if (numCellsX % m_numCellsPerVertexBuffer != 0)
            ++m_numVertexBuffersX;

        if (numCellsY % m_numCellsPerVertexBuffer != 0)
            ++m_numVertexBuffersY;

        m_terrainVertexBuffers.resize(m_numVertexBuffersX * m_numVertexBuffersY);


        for (TerrainBufferCell& bufferCell : m_terrainVertexBuffers)
        {
            bufferCell.m_numTerrainIndices = 0;
            bufferCell.m_numTerrainTriangles = 0;
            bufferCell.m_numTerrainIndicesLayer0 = 0;
            bufferCell.m_numTerrainTrianglesLayer0 = 0;
        }

        std::vector<std::vector<TerrainVertex>> terrainVertices;
        terrainVertices.resize(m_terrainVertexBuffers.size());

        std::vector<std::vector<TerrainIndex>> terrainIndices;
        terrainIndices.resize(m_terrainVertexBuffers.size());

        std::vector<std::vector<TerrainVertex>> terrainVerticesLayer0;
        terrainVerticesLayer0.resize(m_terrainVertexBuffers.size());

        std::vector<std::vector<TerrainIndex>> terrainIndicesLayer0;
        terrainIndicesLayer0.resize(m_terrainVertexBuffers.size());

        int cellsX = m_numCellsPerVertexBuffer;
        int cellsY = m_numCellsPerVertexBuffer;

        for (size_t i = 0; i < m_terrainVertexBuffers.size(); ++i)
        {
            terrainVertices[i].resize(cellsX * cellsY * 6);
            terrainVerticesLayer0[i].resize(cellsX * cellsY * 6);
            terrainIndices[i].resize(cellsX * cellsY * 6);
            terrainIndicesLayer0[i].resize(cellsX * cellsY * 6);
        }

        for (mapHalfCells::iterator it = (*m_pHalfCells).begin(); it != (*m_pHalfCells).end(); ++it)
        {
            unsigned int cellX, cellY;

            m_indexer.SetCellIndex(it->first);

            if (m_indexer.GetVertexBufferIndex() >= m_terrainVertexBuffers.size())
                continue;

            TerrainVertex* pTerrainVertices = &terrainVertices[m_indexer.GetVertexBufferIndex()][0];
            TerrainVertex* pTerrainVerticesLayer0 = &terrainVerticesLayer0[m_indexer.GetVertexBufferIndex()][0];
            TerrainIndex* pTerrainIndices = &terrainIndices[m_indexer.GetVertexBufferIndex()][0];
            TerrainIndex* pTerrainIndicesLayer0 = &terrainIndicesLayer0[m_indexer.GetVertexBufferIndex()][0];

            UpdateTerrainCellWrapper(m_indexer, it->second, pTerrainVertices, pTerrainIndices, pTerrainVerticesLayer0, pTerrainIndicesLayer0);
        }

        for (size_t i = 0; i < m_terrainVertexBuffers.size(); ++i)
        {
            m_terrainVertexBuffers[i].m_pVB = m_pDevice->CreateVertexBuffer(cellsX * cellsY * 6 * sizeof(TerrainVertex), USAGE_TYPE_GPUREADACCESS, &terrainVertices[i][0]);
            m_terrainVertexBuffers[i].m_pVBLayer0 = m_pDevice->CreateVertexBuffer(cellsX * cellsY * 6 * sizeof(TerrainVertex), USAGE_TYPE_GPUREADACCESS, &terrainVerticesLayer0[i][0]);
            m_terrainVertexBuffers[i].m_pIB = m_pDevice->CreateIndexBuffer(cellsX * cellsY * 6 * sizeof(TerrainIndex), USAGE_TYPE_GPUREADACCESS, TEXTUREFORMAT_TYPE_INDEX16, &terrainIndices[i][0]);
            m_terrainVertexBuffers[i].m_pIBLayer0 = m_pDevice->CreateIndexBuffer(cellsX * cellsY * 6 * sizeof(TerrainIndex), USAGE_TYPE_GPUREADACCESS, TEXTUREFORMAT_TYPE_INDEX16, &terrainIndicesLayer0[i][0]);
        }


        UpdateBorderVertexBuffer();

    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreateTerrainVertexBuffer(unsigned int numCellsX, unsigned int numCellsY)
    {
        for (int i = 0; i < m_terrainVertexBuffers.size(); ++i)
        {
            ENV_DELETE(m_terrainVertexBuffers[i].m_pVB);
            ENV_DELETE(m_terrainVertexBuffers[i].m_pVBLayer0);
            ENV_DELETE(m_terrainVertexBuffers[i].m_pIB);
            ENV_DELETE(m_terrainVertexBuffers[i].m_pIBLayer0);
        }

        m_terrainVertexBuffers.clear();

        if (numCellsX == 0 || numCellsY == 0)
            return;

        m_numVertexBuffersX = numCellsX / m_numCellsPerVertexBuffer;
        m_numVertexBuffersY = numCellsY / m_numCellsPerVertexBuffer;

        if (numCellsX % m_numCellsPerVertexBuffer != 0)
            ++m_numVertexBuffersX;

        if (numCellsY % m_numCellsPerVertexBuffer != 0)
            ++m_numVertexBuffersY;

        m_terrainVertexBuffers.resize(m_numVertexBuffersX * m_numVertexBuffersY);

        for (int x = 0; x < m_numVertexBuffersX; ++x)
        {
            for (int y = 0; y < m_numVertexBuffersY; ++y)
            {
                int cellsX = m_numCellsPerVertexBuffer;
                int cellsY = m_numCellsPerVertexBuffer;

                m_terrainVertexBuffers[x * m_numVertexBuffersY + y].m_pVB = m_pDevice->CreateVertexBuffer(cellsX * cellsY * 6 * sizeof(TerrainVertex), USAGE_TYPE_CPUWRITEACCESS | USAGE_TYPE_GPUREADACCESS);
                m_terrainVertexBuffers[x * m_numVertexBuffersY + y].m_pIB = m_pDevice->CreateIndexBuffer(cellsX * cellsY * 6 * sizeof(TerrainIndex), USAGE_TYPE_CPUWRITEACCESS | USAGE_TYPE_GPUREADACCESS, TEXTUREFORMAT_TYPE_INDEX16);
                m_terrainVertexBuffers[x * m_numVertexBuffersY + y].m_pVBLayer0 = m_pDevice->CreateVertexBuffer(cellsX * cellsY * 6 * sizeof(TerrainVertex), USAGE_TYPE_CPUWRITEACCESS | USAGE_TYPE_GPUREADACCESS);
                m_terrainVertexBuffers[x * m_numVertexBuffersY + y].m_pIBLayer0 = m_pDevice->CreateIndexBuffer(cellsX * cellsY * 6 * sizeof(TerrainIndex), USAGE_TYPE_CPUWRITEACCESS | USAGE_TYPE_GPUREADACCESS, TEXTUREFORMAT_TYPE_INDEX16);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::SetMaterial(CManagedMaterial* pMaterial)
    {
        m_pMaterial = pMaterial;

        if (m_pMaterial)
        {
            if (m_pMaterial->GetTexture())
            {
                m_pMaterial->GetTexture()->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());
            }

            if (m_pMaterial->GetNormalMap())
            {
                m_pMaterial->GetNormalMap()->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());
                m_pMaterial->GetNormalMap()->SetSRGB(false);
            }

            if (m_pMaterial->GetSpecularMap())
            {
                m_pMaterial->GetSpecularMap()->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());
            }

            if (m_pMaterial->GetEmissiveMap())
            {
                m_pMaterial->GetEmissiveMap()->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());
            }
        }
    }
        
    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::AddCellDataToTerrainBuffer(unsigned int& numTrianglesAdded,
                                                    TerrainVertex* pBuffer,
                                                    const std::vector< Vec3 >& vertices,
                                                    const std::vector< Vec2 >& texcoords,
                                                    const std::vector< Vec4 >& lighting,
                                                    const Vec3& layerColor,
                                                    const Vec4& emissiveColor,
                                                    CELL_LAYOUT cellLayout,
                                                    unsigned int cellLayer)
    {
        if (!pBuffer)
            return;

        for (int i = 0; i < vertices.size(); ++i)
        {
            pBuffer[i].pos = ComputeTransformedPosition(vertices[i]);
            pBuffer[i].texcoords = texcoords[i];
            pBuffer[i].color = Vec4(layerColor, 0.0f);
            pBuffer[i].colorEmissive = emissiveColor;

            if (lighting.size() > i)
            {
                pBuffer[i].colorLighting = lighting[i];
            }
            else
            {
                pBuffer[i].colorLighting = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
            }
        }

        if (cellLayout == CELL_LAYOUT_FULL)
        {
            CreateTangentVectors(pBuffer[0], pBuffer[1], pBuffer[2]);
            CreateTangentVectors(pBuffer[3], pBuffer[4], pBuffer[5]);

            numTrianglesAdded += 2;
        }
        else if (cellLayout != CELL_LAYOUT_UNDEFINED)
        {
            CreateTangentVectors(pBuffer[0], pBuffer[1], pBuffer[2]);

            numTrianglesAdded += 1;
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreateCellData(unsigned int x, unsigned int y, LevelHalfCell* pCell, std::vector< Vec3 >& vertices, std::vector< Vec2 >& texcoords)
    {
        float xMin = (float)(x * CELL_SIZE);
        float xMax = (float)(x * CELL_SIZE + CELL_SIZE);
        float yMin = (float)(y * CELL_SIZE);
        float yMax = (float)(y * CELL_SIZE + CELL_SIZE);

        float depth = m_layerDepths[pCell->depthLayer];

        if (pCell->cellLayout == CELL_LAYOUT_FULL)
        {
            vertices.reserve(6);
            vertices.push_back(Vec3(xMin, yMin, depth));
            vertices.push_back(Vec3(xMin, yMax, depth));
            vertices.push_back(Vec3(xMax, yMin, depth));
            vertices.push_back(Vec3(xMax, yMin, depth));
            vertices.push_back(Vec3(xMin, yMax, depth));
            vertices.push_back(Vec3(xMax, yMax, depth));

            texcoords.reserve(6);
            texcoords.push_back(Vec2(pCell->texCoords02.x, pCell->texCoords02.y));
            texcoords.push_back(Vec2(pCell->texCoords01.x, pCell->texCoords01.y));
            texcoords.push_back(Vec2(pCell->texCoords01.z, pCell->texCoords01.w));
            texcoords.push_back(Vec2(pCell->texCoords01.z, pCell->texCoords01.w));
            texcoords.push_back(Vec2(pCell->texCoords01.x, pCell->texCoords01.y));
            texcoords.push_back(Vec2(pCell->texCoords02.z, pCell->texCoords02.w));
        }
        else if (pCell->cellLayout == CELL_LAYOUT_BOTLEFT)
        {
            vertices.reserve(3);
            vertices.push_back(Vec3(xMin, yMin, depth));
            vertices.push_back(Vec3(xMin, yMax, depth));
            vertices.push_back(Vec3(xMax, yMin, depth));

            texcoords.reserve(3);
            texcoords.push_back(Vec2(pCell->texCoords02.x, pCell->texCoords02.y));
            texcoords.push_back(Vec2(pCell->texCoords01.x, pCell->texCoords01.y));
            texcoords.push_back(Vec2(pCell->texCoords01.z, pCell->texCoords01.w));
        }
        else if (pCell->cellLayout == CELL_LAYOUT_BOTRIGHT)
        {
            vertices.reserve(3);
            vertices.push_back(Vec3(xMin, yMin, depth));
            vertices.push_back(Vec3(xMax, yMax, depth));
            vertices.push_back(Vec3(xMax, yMin, depth));

            texcoords.reserve(3);
            texcoords.push_back(Vec2(pCell->texCoords02.x, pCell->texCoords02.y));
            texcoords.push_back(Vec2(pCell->texCoords02.z, pCell->texCoords02.w));
            texcoords.push_back(Vec2(pCell->texCoords01.z, pCell->texCoords01.w));
        }
        else if (pCell->cellLayout == CELL_LAYOUT_TOPLEFT)
        {
            vertices.reserve(3);
            vertices.push_back(Vec3(xMin, yMin, depth));
            vertices.push_back(Vec3(xMin, yMax, depth));
            vertices.push_back(Vec3(xMax, yMax, depth));

            texcoords.reserve(3);
            texcoords.push_back(Vec2(pCell->texCoords02.x, pCell->texCoords02.y));
            texcoords.push_back(Vec2(pCell->texCoords01.x, pCell->texCoords01.y));
            texcoords.push_back(Vec2(pCell->texCoords02.z, pCell->texCoords02.w));
        }
        else if (pCell->cellLayout == CELL_LAYOUT_TOPRIGHT)
        {
            vertices.reserve(3);
            vertices.push_back(Vec3(xMax, yMin, depth));
            vertices.push_back(Vec3(xMin, yMax, depth));
            vertices.push_back(Vec3(xMax, yMax, depth));

            texcoords.reserve(3);
            texcoords.push_back(Vec2(pCell->texCoords01.z, pCell->texCoords01.w));
            texcoords.push_back(Vec2(pCell->texCoords01.x, pCell->texCoords01.y));
            texcoords.push_back(Vec2(pCell->texCoords02.z, pCell->texCoords02.w));
        }
    }

    void CTerrainMgrGPU::UpdateTerrainCellVertices(unsigned int x, unsigned int y, unsigned int& numTrianglesAdded, pairHalfCells& phc, TerrainVertex* pVertices)
    {
        LevelHalfCell* pFirstCell = phc.first;
        LevelHalfCell* pSecondCell = phc.second;

        if (pFirstCell)
        {
            std::vector< Vec3 > vertices;
            std::vector< Vec2 > texcoords;

            CreateCellData(x, y, pFirstCell, vertices, texcoords);

            AddCellDataToTerrainBuffer(numTrianglesAdded, pVertices, vertices, texcoords,
                                       pFirstCell->lightingColor,
                                       pFirstCell->layerColor,
                                       pFirstCell->emissiveColor,
                                       pFirstCell->cellLayout,
                                       pFirstCell->depthLayer);
        }

        if (pSecondCell)
        {
            std::vector< Vec3 > vertices;
            std::vector< Vec2 > texcoords;

            CreateCellData(x, y, pSecondCell, vertices, texcoords);

            // If the second cell is not a full cell, the first cell must have been set already
            // and cannot be a full cell, otherwise there would be no second cell
            // and therefore the second cell is also a half cell, which means the baseIndex is 3
            // (#vertices first cell)
            //unsigned int baseIndex = pSecondCell->cellLayout == CELL_LAYOUT_FULL ? 0 : 3;

            if (pSecondCell->cellLayout != CELL_LAYOUT_FULL)
                pVertices += 3;

            AddCellDataToTerrainBuffer(numTrianglesAdded, pVertices, vertices, texcoords,
                                       pSecondCell->lightingColor,
                                       pSecondCell->layerColor,
                                       pSecondCell->emissiveColor,
                                       pSecondCell->cellLayout,
                                       pSecondCell->depthLayer);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateTerrainCellIndices(const CTerrainCellIndexer& indexer, pairHalfCells& phc, TerrainIndex* pIndices, unsigned int& numTerrainIndices)
    {
        if (phc.first)
        {
            AddIndexDataToindexBuffer(indexer, numTerrainIndices, pIndices, 0, phc.first->cellLayout);
        }

        if (phc.second)
        {
            AddIndexDataToindexBuffer(indexer, numTerrainIndices, pIndices, 3, phc.second->cellLayout);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateTerrainCellWrapper(const CTerrainCellIndexer& indexer, pairHalfCells& phc, TerrainVertex* pTerrainVertices, TerrainIndex* pTerrainIndices, TerrainVertex* pTerrainVerticesLayer0, TerrainIndex* pTerrainIndicesLayer0)
    {
        unsigned int x, y;

        indexer.GetCellIndices(x, y);

        unsigned int terrainBaseVertexIndex = 6 * indexer.GetBufferCellX() * m_numCellsPerVertexBuffer + indexer.GetBufferCellY() * 6;
        unsigned int numTriangles = 0;

        TerrainVertex* pVertices = &pTerrainVertices[terrainBaseVertexIndex];
        TerrainVertex* pVerticesLayer0 = &pTerrainVerticesLayer0[terrainBaseVertexIndex];

        pairHalfCells phcCopyLayer0;
        pairHalfCells phcCopy;

        if (phc.first)
        {
            if (phc.first->depthLayer == 0)
                phcCopyLayer0.first = phc.first;
            else
                phcCopy.first = phc.first;
        } 
        
        if (phc.second)
        {
            if (phc.second->depthLayer == 0)
                if (phcCopyLayer0.first)
                    phcCopyLayer0.second = phc.second;
                else
                    phcCopyLayer0.first = phc.second;
            else
                if (phcCopy.first)
                    phcCopy.second = phc.second;
                else
                    phcCopy.first = phc.second;
        }

        TerrainBufferCell& bufferCell = m_terrainVertexBuffers[indexer.GetVertexBufferIndex()];

        numTriangles = 0;
        UpdateTerrainCellVertices(x, y, numTriangles, phcCopyLayer0, pVerticesLayer0);
        UpdateTerrainCellIndices(indexer, phcCopyLayer0, pTerrainIndicesLayer0, bufferCell.m_numTerrainIndicesLayer0);
        bufferCell.m_numTerrainTrianglesLayer0 += numTriangles;

        numTriangles = 0;
        UpdateTerrainCellVertices(x, y, numTriangles, phcCopy, pVertices);
        UpdateTerrainCellIndices(indexer, phcCopy, pTerrainIndices, bufferCell.m_numTerrainIndices);
        bufferCell.m_numTerrainTriangles += numTriangles;
    }


    ////-----------------------------------------------------------------------------------
    //void CTerrainMgrGPU::UpdateTerrainCell(const CTerrainCellIndexer& indexer, pairHalfCells& phc, TerrainVertex* pTerrainVertices, TerrainIndex* pTerrainIndices)
    //{
    //    unsigned int x, y;

    //    indexer.GetCellIndices(x, y);

    //    unsigned int terrainBaseVertexIndex = 6 * indexer.GetBufferCellX() * m_numCellsPerVertexBuffer + indexer.GetBufferCellY() * 6;
    //    unsigned int numTriangles = 0;

    //    TerrainVertex* pVertices = &pTerrainVertices[terrainBaseVertexIndex];


    //    UpdateTerrainCellVertices(x, y, numTriangles, phc, pVertices);
    //    UpdateTerrainCellIndices(indexer, phc, pTerrainIndices);

    //    TerrainBufferCell& bufferCell = m_terrainVertexBuffers[indexer.GetVertexBufferIndex()];
    //}

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateTerrainBufferCellTriangles()
    {
        for (unsigned int index = 0; index < m_terrainVertexBuffers.size(); ++index)
        {
            auto& bufferCell = m_terrainVertexBuffers[index];

            bufferCell.m_numTerrainTriangles = 0;

            unsigned int bufferX = 0;
            unsigned int bufferY = 0;

            m_indexer.SetBufferCellIndex(0, index);
            m_indexer.GetVertexBufferIndices(bufferX, bufferY);

            for (unsigned int x = bufferX * m_numCellsPerVertexBuffer; x < std::min(m_numCellsX, (bufferX + 1) * m_numCellsPerVertexBuffer); ++x)
            {
                for (unsigned int y = bufferY * m_numCellsPerVertexBuffer; y < std::min(m_numCellsY, (bufferY + 1) * m_numCellsPerVertexBuffer); ++y)
                {
                    m_indexer.SetCellIndices(x, y);

                    unsigned int cellIndex = m_indexer.GetCellIndex();

                    auto& it = (*m_pCells)[cellIndex];

                    if (it.pFirst)
                    {
                        if (it.pFirst->cellLayout == CELL_LAYOUT_FULL)
                            bufferCell.m_numTerrainTriangles += 2;
                        else
                            bufferCell.m_numTerrainTriangles += 1;
                    }

                    if (it.pSecond)
                    {
                        if (it.pSecond->cellLayout == CELL_LAYOUT_FULL)
                            bufferCell.m_numTerrainTriangles += 2;
                        else
                            bufferCell.m_numTerrainTriangles += 1;
                    }
                }
            }
        }
    }
    
    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateTerrainVertexBuffer(unsigned int* pCellX, unsigned int* pCellY)
    {
        std::vector<unsigned int> cells;

        if (pCellX != nullptr &&
            pCellY != nullptr)
        {
            m_indexer.SetCellIndices(*pCellX, *pCellY);

            cells.emplace_back(m_indexer.GetCellIndex());
        }
        
        UpdateTerrainVertexBuffer(cells);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateTerrainVertexBuffer(const std::vector<unsigned int>& cells)
    {
        if (m_numCellsX == 0 || m_numCellsY == 0)
            return;

        UpdateLayerTransformations();

        if (m_terrainVertexBuffers.empty())
            CreateTerrainVertexBuffer(m_numCellsX, m_numCellsY);

        if (!cells.empty())
        {
            std::set<unsigned int> bufferIndices;

            for (unsigned int i = 0; i < cells.size(); ++i)
            {
                unsigned int cellIndex = cells[i];

                m_indexer.SetCellIndex(cellIndex);

                bufferIndices.insert(m_indexer.GetVertexBufferIndex());
            }

            for(unsigned int bufferIndex : bufferIndices)
            {
                if (m_indexer.GetVertexBufferIndex() >= m_terrainVertexBuffers.size())
                    return;

                m_indexer.SetBufferCellIndex(0, bufferIndex);

                TerrainBufferCell& bufferCell = m_terrainVertexBuffers[m_indexer.GetVertexBufferIndex()];
                bufferCell.m_numTerrainTriangles = 0;
                bufferCell.m_numTerrainIndices = 0;

                unsigned int bufferCellX, bufferCellY;
                m_indexer.GetCellIndices(bufferCellX, bufferCellY);

                TerrainVertex* pTerrainVertices = static_cast<TerrainVertex*>(m_terrainVertexBuffers[bufferIndex].m_pVB->Lock(0, 0, 0));
                TerrainVertex* pTerrainVerticesLayer0 = static_cast<TerrainVertex*>(m_terrainVertexBuffers[bufferIndex].m_pVBLayer0->Lock(0, 0, 0));
                TerrainIndex* pTerrainIndices = static_cast<TerrainIndex*>(m_terrainVertexBuffers[bufferIndex].m_pIB->Lock(0, 0, 0));
                TerrainIndex* pTerrainIndicesLayer0 = static_cast<TerrainIndex*>(m_terrainVertexBuffers[bufferIndex].m_pIBLayer0->Lock(0, 0, 0));

                for (unsigned int x = bufferCellX; x < std::min(bufferCellX + m_numCellsPerVertexBuffer, m_numCellsX); ++x)
                {
                    for (unsigned int y = bufferCellY; y < std::min(bufferCellY + m_numCellsPerVertexBuffer, m_numCellsY); ++y)
                    {
                        m_indexer.SetCellIndices(x, y);

                        TerrainCell& terrainCell = (*m_pCells)[m_indexer.GetCellIndex()];
                        if (terrainCell.pFirst || terrainCell.pSecond)
                        {
                            pairHalfCells hf(terrainCell.pFirst, terrainCell.pSecond);

                            UpdateTerrainCellWrapper(m_indexer, hf, pTerrainVertices, pTerrainIndices, pTerrainVerticesLayer0, pTerrainIndicesLayer0);
                        }
                    }
                }

                m_terrainVertexBuffers[bufferIndex].m_pIB->Unlock();
                m_terrainVertexBuffers[bufferIndex].m_pIBLayer0->Unlock();
                m_terrainVertexBuffers[bufferIndex].m_pVB->Unlock();
                m_terrainVertexBuffers[bufferIndex].m_pVBLayer0->Unlock();
            }
        }
        else
        {
            for (TerrainBufferCell& bufferCell : m_terrainVertexBuffers)
            {
                bufferCell.m_numTerrainIndices = 0;
                bufferCell.m_numTerrainTriangles = 0;
            }

            std::vector<TerrainVertex*> terrainVertices;
            std::vector<TerrainVertex*> terrainVerticesLayer0;
            terrainVertices.resize(m_terrainVertexBuffers.size());
            terrainVerticesLayer0.resize(m_terrainVertexBuffers.size());

            std::vector<TerrainIndex*> terrainIndices;
            std::vector<TerrainIndex*> terrainIndicesLayer0;
            terrainIndices.resize(m_terrainVertexBuffers.size());
            terrainIndicesLayer0.resize(m_terrainVertexBuffers.size());

            for (size_t i = 0; i < m_terrainVertexBuffers.size(); ++i)
            {
                terrainVertices[i] = static_cast<TerrainVertex*>(m_terrainVertexBuffers[i].m_pVB->Lock(0, 0, 0));
                terrainVerticesLayer0[i] = static_cast<TerrainVertex*>(m_terrainVertexBuffers[i].m_pVBLayer0->Lock(0, 0, 0));
                terrainIndices[i] = static_cast<TerrainIndex*>(m_terrainVertexBuffers[i].m_pIB->Lock(0, 0, 0));
                terrainIndicesLayer0[i] = static_cast<TerrainIndex*>(m_terrainVertexBuffers[i].m_pIBLayer0->Lock(0, 0, 0));
            }

            for (mapHalfCells::iterator it = (*m_pHalfCells).begin(); it != (*m_pHalfCells).end(); ++it)
            {
                unsigned int cellX, cellY;

                m_indexer.SetCellIndex(it->first);

                if (m_indexer.GetVertexBufferIndex() >= m_terrainVertexBuffers.size())
                    continue;

                TerrainVertex* pTerrainVertices = terrainVertices[m_indexer.GetVertexBufferIndex()];
                TerrainVertex* pTerrainVerticesLayer0 = terrainVerticesLayer0[m_indexer.GetVertexBufferIndex()];
                TerrainIndex* pTerrainIndices = terrainIndices[m_indexer.GetVertexBufferIndex()];
                TerrainIndex* pTerrainIndicesLayer0 = terrainIndicesLayer0[m_indexer.GetVertexBufferIndex()];

                UpdateTerrainCellWrapper(m_indexer, it->second, pTerrainVertices, pTerrainIndices, pTerrainVerticesLayer0, pTerrainIndicesLayer0);
            }

            for (auto& pTerrainVB : m_terrainVertexBuffers)
            {
                pTerrainVB.m_pVB->Unlock();
                pTerrainVB.m_pVBLayer0->Unlock();
                pTerrainVB.m_pIB->Unlock();
                pTerrainVB.m_pIBLayer0->Unlock();
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateBorderVertexBuffer()
    {
        ENV_DELETE(m_pVBBorder);

        if (m_bBorder)
        {
            UpdateLayerTransformations();

            int numTriangles = 0;
            for (int i = 0; i < (*m_pCells).size(); ++i)
            {
                TerrainCell& cell = (*m_pCells)[i];
                if ((!cell.pFirst && !cell.pSecond) /*|| cell.first.layer != 0*/)
                    continue;

                int gridIndex = i;
                int x = gridIndex / m_numCellsY;
                int y = gridIndex % m_numCellsY;

                if ((cell.pFirst && cell.pFirst->cellLayout == CELL_LAYOUT_FULL) ||
                    (cell.pSecond && cell.pSecond->cellLayout == CELL_LAYOUT_FULL) ||
                    (cell.pFirst && cell.pSecond && cell.pFirst->depthLayer == cell.pSecond->depthLayer))
                {
                    int layer = 0;
                    if (cell.pFirst)
                        layer = cell.pFirst->depthLayer;
                    else
                        layer = cell.pSecond->depthLayer;

                    if (CheckLeft(x, y, layer))
                        numTriangles += 2;

                    if (CheckRight(x, y, layer))
                        numTriangles += 2;

                    if (CheckTop(x, y, layer))
                        numTriangles += 2;

                    if (CheckBottom(x, y, layer))
                        numTriangles += 2;
                }
                else if (cell.pFirst || cell.pSecond)
                {
                    LevelHalfCell* pHalfCell = NULL;
                    if (cell.pFirst)
                        pHalfCell = cell.pFirst;
                    else if (cell.pSecond)
                        pHalfCell = cell.pSecond;

                    if (!pHalfCell)
                        continue;

                    int layer = 0;

                    if (cell.pFirst)
                    {
                        pHalfCell = cell.pFirst;
                        layer = pHalfCell->depthLayer;

                        if (pHalfCell->cellLayout == CELL_LAYOUT_BOTLEFT)
                        {
                            if (CheckLeft(x, y, layer))
                                numTriangles += 2;

                            if (CheckBottom(x, y, layer))
                                numTriangles += 2;

                        }
                        else if (pHalfCell->cellLayout == CELL_LAYOUT_TOPLEFT)
                        {
                            if (CheckLeft(x, y, layer))
                                numTriangles += 2;

                            if (CheckTop(x, y, layer))
                                numTriangles += 2;
                        }
                        else if (pHalfCell->cellLayout == CELL_LAYOUT_TOPRIGHT)
                        {
                            if (CheckTop(x, y, layer))
                                numTriangles += 2;

                            if (CheckRight(x, y, layer))
                                numTriangles += 2;

                        }
                        else if (pHalfCell->cellLayout == CELL_LAYOUT_BOTRIGHT)
                        {
                            if (CheckRight(x, y, layer))
                                numTriangles += 2;

                            if (CheckBottom(x, y, layer))
                                numTriangles += 2;
                        }
                    }

                    if (cell.pSecond)
                    {
                        pHalfCell = cell.pSecond;
                        layer = pHalfCell->depthLayer;

                        if (pHalfCell->cellLayout == CELL_LAYOUT_BOTLEFT)
                        {
                            if (CheckLeft(x, y, layer))
                                numTriangles += 2;

                            if (CheckBottom(x, y, layer))
                                numTriangles += 2;

                        }
                        else if (pHalfCell->cellLayout == CELL_LAYOUT_TOPLEFT)
                        {
                            if (CheckLeft(x, y, layer))
                                numTriangles += 2;

                            if (CheckTop(x, y, layer))
                                numTriangles += 2;
                        }
                        else if (pHalfCell->cellLayout == CELL_LAYOUT_TOPRIGHT)
                        {
                            if (CheckTop(x, y, layer))
                                numTriangles += 2;

                            if (CheckRight(x, y, layer))
                                numTriangles += 2;

                        }
                        else if (pHalfCell->cellLayout == CELL_LAYOUT_BOTRIGHT)
                        {
                            if (CheckRight(x, y, layer))
                                numTriangles += 2;

                            if (CheckBottom(x, y, layer))
                                numTriangles += 2;
                        }
                    }


                    //if(!cell.bFirstCellSet || !cell.bSecondCellSet)
                    numTriangles += 2;
                }
            }

            if (numTriangles > 0)
            {
                std::vector<TerrainBorderVertex> borderVertices;
                borderVertices.resize(numTriangles * 3);

                TerrainBorderVertex* pV = &borderVertices[0]; //static_cast<TerrainBorderVertex*>(m_pVBBorder->Lock(0, 0, 0));
                int current = 0;
                for (int i = 0; i < (*m_pCells).size(); ++i)
                {
                    TerrainCell& cell = (*m_pCells)[i];
                    if ((!cell.pFirst && !cell.pSecond) /*|| cell.first.layer != 0*/)
                        continue;

                    int gridIndex = i;
                    int x = gridIndex / m_numCellsY;
                    int y = gridIndex % m_numCellsY;

                    int maxLayer = m_layerDepths.size() - 1;
                    if (cell.LayoutEquals(CELL_LAYOUT_FULL) ||
                        (cell.pFirst && cell.pSecond && cell.pFirst->depthLayer == cell.pSecond->depthLayer))
                    {
                        int layer = 0;
                        if (cell.pFirst)
                            layer = cell.pFirst->depthLayer;
                        else
                            layer = cell.pSecond->depthLayer;

                        if (CheckLeft(x, y, layer))
                        {
                            maxLayer = GetLayerLeft(x, y);
                            CreateBorderLeft(x, y, pV, current, cell.pFirst->depthLayer, maxLayer, Vec4(cell.pFirst->borderColor, 0));
                            current += 6;
                        }

                        if (CheckRight(x, y, layer))
                        {
                            maxLayer = GetLayerRight(x, y);

                            CreateBorderRight(x, y, pV, current, cell.pFirst->depthLayer, maxLayer, Vec4(cell.pFirst->borderColor, 0));
                            current += 6;
                        }

                        if (CheckTop(x, y, layer))
                        {
                            maxLayer = GetLayerTop(x, y);
                            CreateBorderTop(x, y, pV, current, cell.pFirst->depthLayer, maxLayer, Vec4(cell.pFirst->borderColor, 0));
                            current += 6;
                        }

                        if (CheckBottom(x, y, layer))
                        {
                            maxLayer = GetLayerBottom(x, y);
                            CreateBorderBottom(x, y, pV, current, cell.pFirst->depthLayer, maxLayer, Vec4(cell.pFirst->borderColor, 0));
                            current += 6;
                        }


                    }
                    else if (cell.pFirst || cell.pSecond)
                    {

                        if (cell.pFirst)
                        {
                            CreateBordersHalfCell(cell.pFirst, pV, x, y, cell.pFirst->depthLayer, current, Vec4(cell.pFirst->borderColor, 0));
                        }

                        if (cell.pSecond)
                        {
                            CreateBordersHalfCell(cell.pSecond, pV, x, y, cell.pSecond->depthLayer, current, Vec4(cell.pSecond->borderColor, 0));
                        }

                        if (!cell.pFirst || !cell.pSecond)
                        {
                            if (cell.pFirst)
                                CreateBorder(x, y, cell.pFirst, cell.pSecond, pV, current, cell.pFirst->depthLayer, maxLayer, cell.pFirst->cellLayout, Vec4(cell.pFirst->borderColor, 0));
                            else
                                CreateBorder(x, y, cell.pSecond, cell.pFirst, pV, current, cell.pSecond->depthLayer, maxLayer, cell.pSecond->cellLayout, Vec4(cell.pSecond->borderColor, 0));
                            current += 6;
                        }
                        else
                        {
                            int firstLayer = cell.pFirst->depthLayer;
                            int secondLayer = cell.pSecond->depthLayer;
                            if (cell.pFirst->cellLayout == CELL_LAYOUT_BOTLEFT)
                            {
                                if (firstLayer < secondLayer)
                                    CreateBorder(x, y, cell.pFirst, cell.pSecond, pV, current, cell.pFirst->depthLayer, cell.pSecond->depthLayer, cell.pFirst->cellLayout, Vec4(cell.pFirst->borderColor, 0));
                                else if (firstLayer > secondLayer)
                                    CreateBorder(x, y, cell.pSecond, cell.pFirst, pV, current, cell.pSecond->depthLayer, cell.pFirst->depthLayer, cell.pSecond->cellLayout, Vec4(cell.pSecond->borderColor, 0));

                                current += 6;
                            }
                            else if (cell.pFirst->cellLayout == CELL_LAYOUT_TOPLEFT)
                            {
                                if (firstLayer < secondLayer)
                                    CreateBorder(x, y, cell.pFirst, cell.pSecond, pV, current, cell.pFirst->depthLayer, cell.pSecond->depthLayer, cell.pFirst->cellLayout, Vec4(cell.pFirst->borderColor, 0));
                                else
                                    CreateBorder(x, y, cell.pSecond, cell.pFirst, pV, current, cell.pSecond->depthLayer, cell.pFirst->depthLayer, cell.pSecond->cellLayout, Vec4(cell.pSecond->borderColor, 0));

                                current += 6;
                            }
                            else if (cell.pFirst->cellLayout == CELL_LAYOUT_TOPRIGHT)
                            {
                                if (firstLayer < secondLayer)
                                    CreateBorder(x, y, cell.pFirst, cell.pSecond, pV, current, cell.pFirst->depthLayer, cell.pSecond->depthLayer, cell.pFirst->cellLayout, Vec4(cell.pFirst->borderColor, 0));
                                else
                                    CreateBorder(x, y, cell.pSecond, cell.pFirst, pV, current, cell.pSecond->depthLayer, cell.pFirst->depthLayer, cell.pSecond->cellLayout, Vec4(cell.pSecond->borderColor, 0));

                                current += 6;
                            }
                            else if (cell.pFirst->cellLayout == CELL_LAYOUT_BOTRIGHT)
                            {
                                if (firstLayer < secondLayer)
                                    CreateBorder(x, y, cell.pFirst, cell.pSecond, pV, current, cell.pFirst->depthLayer, cell.pSecond->depthLayer, cell.pFirst->cellLayout, Vec4(cell.pFirst->borderColor, 0));
                                else
                                    CreateBorder(x, y, cell.pSecond, cell.pFirst, pV, current, cell.pSecond->depthLayer, cell.pFirst->depthLayer, cell.pSecond->cellLayout, Vec4(cell.pSecond->borderColor, 0));
                                current += 6;
                            }
                        }
                    }
                }

                m_pVBBorder = m_pDevice->CreateVertexBuffer(numTriangles * 3 * sizeof(TerrainBorderVertex), USAGE_TYPE_GPUREADACCESS, &borderVertices[0]);
            }


            m_numBorderTriangles = numTriangles;
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateLayerTransformations()
    {
        m_layerTransformations.clear();
        m_layerTransformations.resize(m_layerDepths.size());
        for (int i = 0; i < m_layerDepths.size(); ++i)
        {
            float rotation = m_layerDepths[i] / m_maxLayerDepth * m_borderRotationAngle;

            Matrix4x4 mtxRotation = glm::transpose(glm::rotate(Matrix4x4(1.0f), rotation, Vec3(0.0f, 0.0f, 1.0f)));

            Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), m_borderRotationCenter));

            m_layerTransformations[i] = glm::inverse(mtxTranslation) * mtxRotation * mtxTranslation;
        }
    }

    //-----------------------------------------------------------------------------------
    float CTerrainMgrGPU::GetRotation(float depth)
    {
        float rotation = depth / m_maxLayerDepth * m_borderRotationAngle;
        return rotation;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::TransformVertex(Vec3& vertex, unsigned int layer)
    {
        vertex.z = m_layerDepths[layer];

        vertex = Vec3(Vec4(vertex, 1.0f) * m_layerTransformations[layer]);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateBorderTexture(const std::string& strBorderTexture)
    {
        if (m_pBorderTexture)
        {
            gTextureMgr->Release(m_pBorderTexture->GetName());
            m_pBorderTexture = NULL;
        }

        if (!strBorderTexture.empty())
        {
            m_pBorderTexture = gTextureMgr->AddGetPtr(strBorderTexture);
            m_pBorderTexture->SetSRGB(true);
            m_pBorderTexture->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::DestroyPreview()
    {
        m_previewBuffer.Destroy();
        m_previewBuffer.m_numTerrainTriangles = 0;

        m_previewBorder.Reset();

        m_levelGridRenderObject.m_numPrevTriangles = 0;
        m_levelGridRenderObject.m_numPrevPatternTriangles = 0;
        m_levelGridRenderObject.m_offsetPrevPatternVertices = 0;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreatePreview(bool bPatternPreview,
                                       unsigned int cellX,
                                       unsigned int cellY,
                                       LevelCellEditParameters& cellEditParams,
                                       unsigned int brushSize)
    {
        DestroyPreview();

        if (nullptr == m_pMaterial)
            return;

        unsigned int numPreviewTriangles = cellEditParams.currentEditCellLayout == CELL_LAYOUT_FULL ? 2 : 1;
        unsigned int numPatternPreviewTriangles = (bPatternPreview == true) ? 62 : 0;

        UpdateLayerTransformations();

        m_pMaterial->SetSequence(cellEditParams.currentEditSequence);

        TextureRect rect;
        m_pMaterial->GetTextureCoordinates(cellEditParams.currentEditSequence, 0, rect);

        Vec2 v1 = Vec2(rect.left, rect.top);
        Vec2 v2 = Vec2(rect.right, rect.bottom);
        Vec2 v3 = Vec2(v1.x, v2.y);
        Vec2 v4 = Vec2(v2.x, v1.y);

        Vec2 tmp;

        switch (cellEditParams.currentEditRotation)
        {
        case CELL_ROTATION_90DEG_CW:
            tmp = v1;
            v1 = v3;
            v3 = v2;
            v2 = v4;
            v4 = tmp;
            break;
        case CELL_ROTATION_180DEG_CW:
            std::swap(v1, v2);
            std::swap(v3, v4);
            break;
        case CELL_ROTATION_270DEG_CW:
            tmp = v3;
            v3 = v1;
            v1 = v4;
            v4 = v2;
            v2 = tmp;
            break;
        default:
            break;
        }

        m_previewBuffer.m_pVB = m_pDevice->CreateVertexBuffer((numPatternPreviewTriangles + numPreviewTriangles * brushSize * brushSize) * 3 * sizeof(TerrainVertex), USAGE_TYPE_VERTEXINDEXBUFFERS);
        m_previewBuffer.m_numTerrainTriangles = 0;

        TerrainVertex* pVertices = static_cast<TerrainVertex*>(m_previewBuffer.m_pVB->Lock(0, 0, 0));

        int xyMin = brushSize / 2;

        float xMin = (float)(((int)cellX - xyMin) * CELL_SIZE);
        float xMax = (float)(((int)cellX - xyMin) * CELL_SIZE + CELL_SIZE);
        float yMin = (float)(((int)cellY - xyMin) * CELL_SIZE);
        float yMax = (float)(((int)cellY - xyMin) * CELL_SIZE + CELL_SIZE);

        int totalBaseIndex = 0;

        for (int k = 0; k < brushSize; ++k)
        {
            for (int l = 0; l < brushSize; ++l)
            {
                float xMinVertex = xMin + CELL_SIZE * k;
                float xMaxVertex = xMax + CELL_SIZE * k;

                float yMinVertex = yMin + CELL_SIZE * l;
                float yMaxVertex = yMax + CELL_SIZE * l;

                std::vector< Vec3 > vertices;
                std::vector< Vec2 > texcoords;
                std::vector< Vec4 > lighting;

                float depth = m_layerDepths[cellEditParams.currentEditLayer] - 0.1f;

                if (cellEditParams.currentEditCellLayout == CELL_LAYOUT_FULL)
                {
                    vertices.reserve(6);
                    vertices.push_back(Vec3(xMinVertex, yMinVertex, depth));
                    vertices.push_back(Vec3(xMinVertex, yMaxVertex, depth));
                    vertices.push_back(Vec3(xMaxVertex, yMinVertex, depth));
                    vertices.push_back(Vec3(xMaxVertex, yMinVertex, depth));
                    vertices.push_back(Vec3(xMinVertex, yMaxVertex, depth));
                    vertices.push_back(Vec3(xMaxVertex, yMaxVertex, depth));

                    texcoords.reserve(6);
                    texcoords.push_back(Vec2(v3.x, v3.y));
                    texcoords.push_back(Vec2(v1.x, v1.y));
                    texcoords.push_back(Vec2(v2.x, v2.y));
                    texcoords.push_back(Vec2(v2.x, v2.y));
                    texcoords.push_back(Vec2(v1.x, v1.y));
                    texcoords.push_back(Vec2(v4.x, v4.y));
                }
                else if (cellEditParams.currentEditCellLayout == CELL_LAYOUT_BOTLEFT)
                {
                    vertices.reserve(3);
                    vertices.push_back(Vec3(xMinVertex, yMinVertex, depth));
                    vertices.push_back(Vec3(xMinVertex, yMaxVertex, depth));
                    vertices.push_back(Vec3(xMaxVertex, yMinVertex, depth));

                    texcoords.reserve(3);
                    texcoords.push_back(Vec2(v3.x, v3.y));
                    texcoords.push_back(Vec2(v1.x, v1.y));
                    texcoords.push_back(Vec2(v2.x, v2.y));
                }
                else if (cellEditParams.currentEditCellLayout == CELL_LAYOUT_BOTRIGHT)
                {
                    vertices.reserve(3);
                    vertices.push_back(Vec3(xMinVertex, yMinVertex, depth));
                    vertices.push_back(Vec3(xMaxVertex, yMaxVertex, depth));
                    vertices.push_back(Vec3(xMaxVertex, yMinVertex, depth));

                    texcoords.reserve(3);
                    texcoords.push_back(Vec2(v3.x, v3.y));
                    texcoords.push_back(Vec2(v4.x, v4.y));
                    texcoords.push_back(Vec2(v2.x, v2.y));
                }
                else if (cellEditParams.currentEditCellLayout == CELL_LAYOUT_TOPLEFT)
                {
                    vertices.reserve(3);
                    vertices.push_back(Vec3(xMinVertex, yMinVertex, depth));
                    vertices.push_back(Vec3(xMinVertex, yMaxVertex, depth));
                    vertices.push_back(Vec3(xMaxVertex, yMaxVertex, depth));

                    texcoords.reserve(3);
                    texcoords.push_back(Vec2(v3.x, v3.y));
                    texcoords.push_back(Vec2(v1.x, v1.y));
                    texcoords.push_back(Vec2(v4.x, v4.y));
                }
                else if (cellEditParams.currentEditCellLayout == CELL_LAYOUT_TOPRIGHT)
                {
                    vertices.reserve(3);
                    vertices.push_back(Vec3(xMaxVertex, yMinVertex, depth));
                    vertices.push_back(Vec3(xMinVertex, yMaxVertex, depth));
                    vertices.push_back(Vec3(xMaxVertex, yMaxVertex, depth));

                    texcoords.reserve(3);
                    texcoords.push_back(Vec2(v2.x, v2.y));
                    texcoords.push_back(Vec2(v1.x, v1.y));
                    texcoords.push_back(Vec2(v4.x, v4.y));
                }

                int baseIndex = (numPreviewTriangles * 3 * brushSize * k) + l * numPreviewTriangles * 3;

                totalBaseIndex = baseIndex + vertices.size();

                unsigned int numTrianglesAdded = 0;
                AddCellDataToTerrainBuffer(numTrianglesAdded, pVertices,
                                           vertices, texcoords, lighting,
                                           cellEditParams.currentEditColor,
                                           cellEditParams.currentEditEmissiveColor,
                                           cellEditParams.currentEditCellLayout,
                                           cellEditParams.currentEditLayer);

                m_previewBuffer.m_numTerrainTriangles += numTrianglesAdded;

                pVertices += vertices.size();
            }
        }

        if (bPatternPreview)
        {
            xMin = (float)(((int)cellX) * CELL_SIZE);
            xMax = (float)(((int)cellX) * CELL_SIZE + CELL_SIZE);
            yMin = (float)(((int)cellY) * CELL_SIZE);
            yMax = (float)(((int)cellY) * CELL_SIZE + CELL_SIZE);

            for (int i = (int)cellEditParams.currentEditSequence - 16, k = -16; i < (int)cellEditParams.currentEditSequence + 16; ++i, ++k)
            {
                int curSeq = i;
                int currOffset = 1 + (brushSize - 1) / 2;

                if (curSeq == (int)cellEditParams.currentEditSequence)
                    continue;

                if (k < 0)
                    currOffset = -1 - (brushSize) / 2;

                if (curSeq < 0)
                    curSeq = 32 + i;

                curSeq = curSeq % 32;

                m_pMaterial->SetSequence(curSeq);
                m_pMaterial->GetTextureCoordinates(curSeq, 0, rect);

                float xMinVertex = xMin;
                float xMaxVertex = xMax;

                float yMinVertex = yMin + CELL_SIZE * k + CELL_SIZE * currOffset;
                float yMaxVertex = yMax + CELL_SIZE * k + CELL_SIZE * currOffset;

                std::vector< Vec3 > vertices;
                std::vector< Vec2 > texcoords;
                std::vector< Vec4 > lighting;

                Vec2 v1 = Vec2(rect.left, rect.top);
                Vec2 v2 = Vec2(rect.right, rect.bottom);
                Vec2 v3 = Vec2(v1.x, v2.y);
                Vec2 v4 = Vec2(v2.x, v1.y);

                Vec2 tmp;

                switch (cellEditParams.currentEditRotation)
                {
                case CELL_ROTATION_90DEG_CW:
                    tmp = v1;
                    v1 = v3;
                    v3 = v2;
                    v2 = v4;
                    v4 = tmp;
                    break;
                case CELL_ROTATION_180DEG_CW:
                    std::swap(v1, v2);
                    std::swap(v3, v4);
                    break;
                case CELL_ROTATION_270DEG_CW:
                    tmp = v3;
                    v3 = v1;
                    v1 = v4;
                    v4 = v2;
                    v2 = tmp;
                    break;
                default:
                    break;
                }

                float depth = m_layerDepths[cellEditParams.currentEditLayer] - 0.1f;

                vertices.reserve(6);
                vertices.push_back(Vec3(xMinVertex, yMinVertex, depth));
                vertices.push_back(Vec3(xMinVertex, yMaxVertex, depth));
                vertices.push_back(Vec3(xMaxVertex, yMinVertex, depth));
                vertices.push_back(Vec3(xMaxVertex, yMinVertex, depth));
                vertices.push_back(Vec3(xMinVertex, yMaxVertex, depth));
                vertices.push_back(Vec3(xMaxVertex, yMaxVertex, depth));

                texcoords.reserve(6);
                texcoords.push_back(Vec2(v3.x, v3.y));
                texcoords.push_back(Vec2(v1.x, v1.y));
                texcoords.push_back(Vec2(v2.x, v2.y));
                texcoords.push_back(Vec2(v2.x, v2.y));
                texcoords.push_back(Vec2(v1.x, v1.y));
                texcoords.push_back(Vec2(v4.x, v4.y));

                //pVertices += totalBaseIndex;

                unsigned int numTrianglesAdded = 0;
                AddCellDataToTerrainBuffer(numTrianglesAdded, pVertices,
                                           vertices, texcoords, lighting,
                                           cellEditParams.currentEditColor,
                                           cellEditParams.currentEditEmissiveColor,
                                           cellEditParams.currentEditCellLayout,
                                           cellEditParams.currentEditLayer);

                //m_previewBuffer.m_numTerrainTriangles += numTrianglesAdded;

                pVertices += vertices.size();

                totalBaseIndex += 6;
            }

            int currTopOffset = 1 + (brushSize - 1) / 2;
            int currBotOffset = -1 - (brushSize) / 2;


            float xMinVertex = xMin;
            float xMaxVertex = xMax;

            float yBottomMinVertex = yMin + CELL_SIZE * -16 + CELL_SIZE * currBotOffset;
            float yBottomMaxVertex = yMax + CELL_SIZE * -1 + CELL_SIZE * currBotOffset;


            float yTopMinVertex = yMin + CELL_SIZE *  +1 + CELL_SIZE * currTopOffset;
            float yTopMaxVertex = yMax + CELL_SIZE * +15 + CELL_SIZE * currTopOffset;

            float depth = m_layerDepths[cellEditParams.currentEditLayer];

            m_previewBorder.SetPrimitiveType(PRIMITIVE_TYPE_TRIANGLELIST);
            m_previewBorder.Reserve(48);

            Vec3 color(0.2f, 0.2f, 0.2f);

            float scale = 2.5f;

            std::vector<Vec3> cornerVertices;
            cornerVertices.resize(8);

            // Corner vertices: Bottom panel
            cornerVertices[0] = Vec3(xMinVertex, yBottomMinVertex, depth);
            cornerVertices[1] = Vec3(xMinVertex, yBottomMinVertex, depth) + Vec3(-1.0f, -1.0f, 0.0f) * scale;

            cornerVertices[2] = Vec3(xMaxVertex, yBottomMinVertex, depth);
            cornerVertices[3] = Vec3(xMaxVertex, yBottomMinVertex, depth) + Vec3(1.0f, -1.0f, 0.0f) * scale;

            cornerVertices[4] = Vec3(xMaxVertex, yBottomMaxVertex, depth);
            cornerVertices[5] = Vec3(xMaxVertex, yBottomMaxVertex, depth) + Vec3(1.0f, 1.0f, 0.0f) * scale;

            cornerVertices[6] = Vec3(xMinVertex, yBottomMaxVertex, depth);
            cornerVertices[7] = Vec3(xMinVertex, yBottomMaxVertex, depth) + Vec3(-1.0f, 1.0f, 0.0f) * scale;

            for (int i = 0; i < 4; ++i)
            {
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 0) % 8]), color));
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 2) % 8]), color));
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 1) % 8]), color));

                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 1) % 8]), color));
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 2) % 8]), color));
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 3) % 8]), color));
            }

            // Corner vertices: Top panel
            cornerVertices[0] = Vec3(xMinVertex, yTopMinVertex, depth);
            cornerVertices[1] = Vec3(xMinVertex, yTopMinVertex, depth) + Vec3(-1.0f, -1.0f, 0.0f) * scale;

            cornerVertices[2] = Vec3(xMaxVertex, yTopMinVertex, depth);
            cornerVertices[3] = Vec3(xMaxVertex, yTopMinVertex, depth) + Vec3(1.0f, -1.0f, 0.0f) * scale;

            cornerVertices[4] = Vec3(xMaxVertex, yTopMaxVertex, depth);
            cornerVertices[5] = Vec3(xMaxVertex, yTopMaxVertex, depth) + Vec3(1.0f, 1.0f, 0.0f) * scale;

            cornerVertices[6] = Vec3(xMinVertex, yTopMaxVertex, depth);
            cornerVertices[7] = Vec3(xMinVertex, yTopMaxVertex, depth) + Vec3(-1.0f, 1.0f, 0.0f) * scale;

            for (int i = 0; i < 4; ++i)
            {
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 0) % 8]), color));
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 2) % 8]), color));
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 1) % 8]), color));

                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 1) % 8]), color));
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 2) % 8]), color));
                m_previewBorder.Add(PolygonVertex(ComputeTransformedPosition(cornerVertices[(i * 2 + 3) % 8]), color));
            }
        }

        m_previewBuffer.m_pVB->Unlock();

        m_levelGridRenderObject.m_numPrevTriangles = numPreviewTriangles * brushSize * brushSize;
        m_levelGridRenderObject.m_numPrevPatternTriangles = numPatternPreviewTriangles;
        m_levelGridRenderObject.m_offsetPrevPatternVertices = m_levelGridRenderObject.m_numPrevTriangles * 3;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::RemoveSelection()
    {
        m_selectionBuffer.Destroy();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::UpdateSelection(mapHalfCells& cells)
    {
        unsigned int numCells = cells.size();

        RemoveSelection();

        if (numCells == 0)
            return;

        UpdateLayerTransformations();

        if (!m_selectionBuffer.m_pVB)
        {
            m_selectionBuffer.m_pVB = m_pDevice->CreateVertexBuffer(numCells * 6 * sizeof(TerrainVertex), USAGE_TYPE_VERTEXINDEXBUFFERS);
        }

        TerrainVertex* pVertices = static_cast<TerrainVertex*>(m_selectionBuffer.m_pVB->Lock(0, 0, 0));
        for (mapHalfCells::iterator it = cells.begin(); it != cells.end(); ++it)
        {
            unsigned int x, y, gridIndex;
            LevelHalfCell* pFirstCell = it->second.first;
            LevelHalfCell* pSecondCell = it->second.second;
            gridIndex = it->first;
            x = gridIndex / m_numCellsY;
            y = gridIndex % m_numCellsY;

            x = (static_cast<int>(x) + m_selectionOffsetX);
            y = (static_cast<int>(y) + m_selectionOffsetY);

            unsigned int numTrianglesAdded = 0;

            UpdateTerrainCellVertices(x, y, numTrianglesAdded, it->second, pVertices);

            m_selectionBuffer.m_numTerrainTriangles += numTrianglesAdded;

            pVertices += 3 * numTrianglesAdded;
        }

        m_selectionBuffer.m_pVB->Unlock();

    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::AddIndexDataToindexBuffer(const CTerrainCellIndexer& indexer, unsigned int& numTerrainIndices, TerrainIndex* w, unsigned int baseIndex, CELL_LAYOUT cellLayout)
    {
        //TerrainBufferCell& cell = m_terrainVertexBuffers[indexer.GetVertexBufferIndex()];

        unsigned int vertexBaseIndex = indexer.GetBufferCellIndex() * 6;
        unsigned int offset = 0;

        baseIndex += vertexBaseIndex;

        if (cellLayout == CELL_LAYOUT_FULL)
        {
            for (int i = 0; i < 6; ++i)
            {
                w[numTerrainIndices++] = baseIndex + i;
            }
        }
        else if (cellLayout == CELL_LAYOUT_BOTLEFT)
        {
            w[numTerrainIndices++] = baseIndex + offset + 0;
            w[numTerrainIndices++] = baseIndex + offset + 1;
            w[numTerrainIndices++] = baseIndex + offset + 2;
        }
        else if (cellLayout == CELL_LAYOUT_BOTRIGHT)
        {
            w[numTerrainIndices++] = baseIndex + offset + 0;
            w[numTerrainIndices++] = baseIndex + offset + 1;
            w[numTerrainIndices++] = baseIndex + offset + 2;
        }
        else if (cellLayout == CELL_LAYOUT_TOPLEFT)
        {
            w[numTerrainIndices++] = baseIndex + offset + 0;
            w[numTerrainIndices++] = baseIndex + offset + 1;
            w[numTerrainIndices++] = baseIndex + offset + 2;
        }
        else if (cellLayout == CELL_LAYOUT_TOPRIGHT)
        {
            w[numTerrainIndices++] = baseIndex + offset + 0;
            w[numTerrainIndices++] = baseIndex + offset + 1;
            w[numTerrainIndices++] = baseIndex + offset + 2;
        }
    }

    //-----------------------------------------------------------------------------------
    // returns true, if a left cell has a higher depth layer
    bool CTerrainMgrGPU::CheckLeft(int x, int y, int layer)
    {
        int left = x - 1;
        if (left >= 0 && left < m_numCellsX)
        {
            int index = left * m_numCellsY + y;
            TerrainCell& cell = (*m_pCells)[index];
            LevelHalfCell* pHalfCell = NULL;
            if ((!cell.pFirst && !cell.pSecond) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_BOTRIGHT)) && pHalfCell->depthLayer > layer) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_TOPRIGHT)) && pHalfCell->depthLayer > layer) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_FULL)) && pHalfCell->depthLayer > layer) ||
                cell.LayoutEquals(CELL_LAYOUT_BOTLEFT) || cell.LayoutEquals(CELL_LAYOUT_TOPLEFT))
            {
                return true;
            }
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    bool CTerrainMgrGPU::CheckRight(int x, int y, int layer)
    {
        int right = x + 1;
        if (right >= 0 && right < m_numCellsX)
        {
            int index = right * m_numCellsY + y;
            TerrainCell& cell = (*m_pCells)[index];

            LevelHalfCell* pHalfCell = NULL;
            if ((!cell.pFirst && !cell.pSecond) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_BOTLEFT)) && pHalfCell->depthLayer > layer) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_TOPLEFT)) && pHalfCell->depthLayer > layer) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_FULL)) && pHalfCell->depthLayer > layer) ||
                cell.LayoutEquals(CELL_LAYOUT_BOTRIGHT) || cell.LayoutEquals(CELL_LAYOUT_TOPRIGHT))
            {
                return true;
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------------------
    bool CTerrainMgrGPU::CheckTop(int x, int y, int layer)
    {
        int top = y + 1;
        if (top >= 0 && top < m_numCellsY)
        {
            int index = x * m_numCellsY + top;
            TerrainCell& cell = (*m_pCells)[index];
            LevelHalfCell* pHalfCell = NULL;
            if ((!cell.pFirst && !cell.pSecond) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_BOTLEFT)) && pHalfCell->depthLayer > layer) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_BOTRIGHT)) && pHalfCell->depthLayer > layer) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_FULL)) && pHalfCell->depthLayer > layer) ||
                cell.LayoutEquals(CELL_LAYOUT_TOPLEFT) || cell.LayoutEquals(CELL_LAYOUT_TOPRIGHT))
            {
                return true;
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------------------
    bool CTerrainMgrGPU::CheckBottom(int x, int y, int layer)
    {
        int bottom = y - 1;
        if (bottom >= 0 && bottom < m_numCellsY)
        {
            int index = x * m_numCellsY + bottom;
            TerrainCell& cell = (*m_pCells)[index];
            LevelHalfCell* pHalfCell = NULL;
            if ((!cell.pFirst && !cell.pSecond) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_TOPLEFT)) && pHalfCell->depthLayer > layer) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_TOPRIGHT)) && pHalfCell->depthLayer > layer) ||
                ((pHalfCell = cell.GetHalfCell(CELL_LAYOUT_FULL)) && pHalfCell->depthLayer > layer) ||
                cell.LayoutEquals(CELL_LAYOUT_BOTLEFT) || cell.LayoutEquals(CELL_LAYOUT_BOTRIGHT))
            {
                return true;
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------------------
    Vec3 CTerrainMgrGPU::ComputeTransformedPosition(Vec3 vertex)
    {
        float targetDepth = vertex.z;

        Vec3 startVertex = vertex;
        startVertex.z = 0.0f;

        Vec3 finalVertex(startVertex);
        finalVertex.z = m_maxLayerDepth;

        finalVertex = Vec3(Vec4(finalVertex, 1.0f) * m_layerTransformations[m_layerTransformations.size() - 1]);

        float alpha = targetDepth / m_maxLayerDepth;

        Vec3 targetVertex = (1.0f - alpha) * startVertex + alpha * finalVertex;
        return targetVertex;

    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreateBorderLeft(int x, int y, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, const Vec4& color)
    {
        Vec4 colorLightingLeftTop, colorLightingLeftBot, colorLightingTop, colorLightingBot;
        GetLightingColorsBorderLeft(x, y, colorLightingLeftTop, colorLightingLeftBot, colorLightingTop, colorLightingBot);

        float texScale = 32.0f / 320.0f;
        Vec3 v;
        v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, y * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingBot;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, y * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingLeftBot;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingLeftTop;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, y * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingBot;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingLeftTop;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, (y + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingTop;
        ++vertexIndex;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreateBorderRight(int x, int y, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, const Vec4& color)
    {
        Vec4 colorLightingRightTop, colorLightingRightBot, colorLightingTop, colorLightingBot;
        GetLightingColorsBorderRight(x, y, colorLightingRightTop, colorLightingRightBot, colorLightingTop, colorLightingBot);

        float texScale = 32.0f / 320.0f;
        Vec3 v;
        v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, y * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingBot;
        ++vertexIndex;
        v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE + CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingRightTop;
        ++vertexIndex;
        v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, y * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingRightBot;
        ++vertexIndex;
        v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, y * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingBot;
        ++vertexIndex;
        v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE + CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, (y + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingTop;
        ++vertexIndex;
        v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE + CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingRightTop;
        ++vertexIndex;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreateBorderTop(int x, int y, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, const Vec4& color)
    {
        Vec4 colorLightingTopLeft, colorLightingTopRight, colorLightingLeft, colorLightingRight;
        GetLightingColorsBorderTop(x, y, colorLightingTopLeft, colorLightingTopRight, colorLightingLeft, colorLightingRight);

        float texScale = 32.0f / 320.0f;
        Vec3 v;
        v = Vec3(x * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, x * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingLeft;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, x * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingTopLeft;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE + CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, (x + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingTopRight;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, x * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingLeft;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE + CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, (x + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingTopRight;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE + CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, (x + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingRight;
        ++vertexIndex;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreateBorderBottom(int x, int y, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, const Vec4& color)
    {
        Vec4 colorLightingBotLeft, colorLightingBotRight, colorLightingLeft, colorLightingRight;
        GetLightingColorsBorderBot(x, y, colorLightingBotLeft, colorLightingBotRight, colorLightingLeft, colorLightingRight);

        float texScale = 32.0f / 320.0f;
        Vec3 v;
        v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, x * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingLeft;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, (x + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingBotRight;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, x * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingBotLeft;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, x * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingLeft;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(0.0f, (x + 1) * texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingRight;
        ++vertexIndex;
        v = Vec3(x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
        p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
        p[vertexIndex].texcoords = Vec2(1.0f, (x + 1)* texScale);
        p[vertexIndex].color = color;
        p[vertexIndex].colorLighting = colorLightingBotRight;
        ++vertexIndex;
    }

    //-----------------------------------------------------------------------------------
    Vec4 GetLightingColorHelper(LevelHalfCell* pHalfCell, unsigned int index)
    {
        if (pHalfCell == nullptr)
            return Vec4(0.0f, 0.0f, 0.0f, 0.0f);

        if (pHalfCell->lightingColor.size() > index)
            return pHalfCell->lightingColor[index];

        return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreateBorder(int x, int y, LevelHalfCell* pCurrHalfCell, LevelHalfCell* pSideHalfCell, TerrainBorderVertex* p, int vertexIndex, int minLayer, int maxLayer, CELL_LAYOUT layout, const Vec4& color)
    {
        //float depth = 0.0f;
        Matrix4x4 mtx;
        float texScale = 32.0f / 320.0f;
        switch (layout)
        {
        case CELL_LAYOUT_BOTLEFT:
        {
            Vec3 v;
            v = Vec3(x * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 1);
            ++vertexIndex;
            v = Vec3(x * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 1);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 2);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 2);
            ++vertexIndex;
            v = Vec3(x * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 1);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 0);
            ++vertexIndex;
        }
        break;
        case CELL_LAYOUT_TOPLEFT:
        {
            Vec3 v;
            v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 0);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 2);
            ++vertexIndex;
            v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 0);
            ++vertexIndex;
            v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 0);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 2);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 1);
            ++vertexIndex;
        }
        break;
        case CELL_LAYOUT_TOPRIGHT:
        {
            Vec3 v;
            v = Vec3((x)* CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 1);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 2);
            ++vertexIndex;
            v = Vec3((x)* CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 1);
            ++vertexIndex;
            v = Vec3((x)* CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 1);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 0);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 2);
            ++vertexIndex;
        }
        break;
        case CELL_LAYOUT_BOTRIGHT:
        {
            Vec3 v;
            v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 0);
            ++vertexIndex;
            v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 0);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 2);
            ++vertexIndex;
            v = Vec3(x * CELL_SIZE, y * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y)* texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 0);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[maxLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(1.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pSideHalfCell, 2);
            ++vertexIndex;
            v = Vec3((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE, m_layerDepths[minLayer]);
            p[vertexIndex].pos = Vec4(ComputeTransformedPosition(v), 1.0f);
            p[vertexIndex].texcoords = Vec2(0.0f, (y + 1) * texScale);
            p[vertexIndex].color = color;
            p[vertexIndex].colorLighting = GetLightingColorHelper(pCurrHalfCell, 1);
            ++vertexIndex;
        }
        break;
        default:
            ENV_ASSERT_DBG(false);
            break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreateBordersHalfCell(LevelHalfCell* pHalfCell, TerrainBorderVertex* pV, int x, int y, int layer, int& currentTriangle, const Vec4& color)
    {
        int neighborLayer = 3;
        if (pHalfCell->cellLayout == CELL_LAYOUT_BOTLEFT)
        {
            if (CheckLeft(x, y, layer))
            {
                neighborLayer = GetLayerLeft(x, y);
                CreateBorderLeft(x, y, pV, currentTriangle, layer, neighborLayer, color);
                currentTriangle += 6;
            }
            neighborLayer = 3;
            if (CheckBottom(x, y, layer))
            {
                neighborLayer = GetLayerBottom(x, y);
                CreateBorderBottom(x, y, pV, currentTriangle, layer, neighborLayer, color);
                currentTriangle += 6;
            }
        }
        else if (pHalfCell->cellLayout == CELL_LAYOUT_TOPLEFT)
        {
            if (CheckLeft(x, y, layer))
            {
                neighborLayer = GetLayerLeft(x, y);
                CreateBorderLeft(x, y, pV, currentTriangle, layer, neighborLayer, color);
                currentTriangle += 6;
            }
            neighborLayer = 3;
            if (CheckTop(x, y, layer))
            {
                neighborLayer = GetLayerTop(x, y);
                CreateBorderTop(x, y, pV, currentTriangle, layer, neighborLayer, color);
                currentTriangle += 6;
            }
        }
        else if (pHalfCell->cellLayout == CELL_LAYOUT_TOPRIGHT)
        {
            if (CheckTop(x, y, layer))
            {
                neighborLayer = GetLayerTop(x, y);
                CreateBorderTop(x, y, pV, currentTriangle, layer, neighborLayer, color);
                currentTriangle += 6;
            }
            neighborLayer = 3;
            if (CheckRight(x, y, layer))
            {
                neighborLayer = GetLayerRight(x, y);
                CreateBorderRight(x, y, pV, currentTriangle, layer, neighborLayer, color);
                currentTriangle += 6;
            }
        }
        else if (pHalfCell->cellLayout == CELL_LAYOUT_BOTRIGHT)
        {
            if (CheckRight(x, y, layer))
            {
                neighborLayer = GetLayerRight(x, y);
                CreateBorderRight(x, y, pV, currentTriangle, layer, neighborLayer, color);
                currentTriangle += 6;
            }
            neighborLayer = 3;
            if (CheckBottom(x, y, layer))
            {
                neighborLayer = GetLayerBottom(x, y);
                CreateBorderBottom(x, y, pV, currentTriangle, layer, neighborLayer, color);
                currentTriangle += 6;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    int CTerrainMgrGPU::GetLayerRight(int x, int y)
    {
        if (x >= (m_numCellsX - 1))
            return 0;

        int layer = 0;
        TerrainCell& rightCell = (*m_pCells)[(x + 1) * m_numCellsY + y];

        LevelHalfCell* pHalfCell = NULL;
        if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_BOTLEFT)))
            layer = pHalfCell->depthLayer;
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_TOPLEFT)))
            layer = pHalfCell->depthLayer;
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_FULL)))
            layer = pHalfCell->depthLayer;
        else
            layer = m_layerDepths.size() - 1;

        return layer;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainMgrGPU::GetLayerLeft(int x, int y)
    {
        if (x < 1)
            return 0;

        int layer = 0;
        TerrainCell& rightCell = (*m_pCells)[(x - 1) * m_numCellsY + y];

        LevelHalfCell* pHalfCell = NULL;
        if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_BOTRIGHT)))
            layer = pHalfCell->depthLayer;
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_TOPRIGHT)))
            layer = pHalfCell->depthLayer;
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_FULL)))
            layer = pHalfCell->depthLayer;
        else
            layer = m_layerDepths.size() - 1;

        return layer;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::GetLightingColorsBorderLeft(int x, int y, Vec4& leftTop, Vec4& leftBot, Vec4& top, Vec4& bot)
    {
        if (x < 1)
            return;

        TerrainCell& leftCell = (*m_pCells)[(x - 1) * m_numCellsY + y];
        TerrainCell& currCell = (*m_pCells)[x * m_numCellsY + y];

        LevelHalfCell* pHalfCell = NULL;
        if ((pHalfCell = leftCell.GetHalfCell(CELL_LAYOUT_BOTRIGHT)))
        {
            leftTop = GetLightingColorHelper(pHalfCell, 1);
            leftBot = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = leftCell.GetHalfCell(CELL_LAYOUT_TOPRIGHT)))
        {
            leftTop = GetLightingColorHelper(pHalfCell, 2);
            leftBot = GetLightingColorHelper(pHalfCell, 0);
        }
        else if ((pHalfCell = leftCell.GetHalfCell(CELL_LAYOUT_FULL)))
        {
            leftTop = GetLightingColorHelper(pHalfCell, 5);
            leftBot = GetLightingColorHelper(pHalfCell, 2);
        }
        else
        {
            leftTop = Vec4(0, 0, 0, 0);
            leftBot = Vec4(0, 0, 0, 0);
        }

        if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_BOTLEFT)))
        {
            top = GetLightingColorHelper(pHalfCell, 1);
            bot = GetLightingColorHelper(pHalfCell, 0);
        }
        else if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_TOPLEFT)))
        {
            top = GetLightingColorHelper(pHalfCell, 1);
            bot = GetLightingColorHelper(pHalfCell, 0);
        }
        else if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_FULL)))
        {
            top = GetLightingColorHelper(pHalfCell, 1);
            bot = GetLightingColorHelper(pHalfCell, 0);
        }
        else
        {
            top = Vec4(0, 0, 0, 0);
            bot = Vec4(0, 0, 0, 0);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::GetLightingColorsBorderRight(int x, int y, Vec4& rightTop, Vec4& rightBot, Vec4& top, Vec4& bot)
    {
        if (x >= (m_numCellsX - 1))
            return;

        TerrainCell& rightCell = (*m_pCells)[(x + 1) * m_numCellsY + y];
        TerrainCell& currCell = (*m_pCells)[x * m_numCellsY + y];

        LevelHalfCell* pHalfCell = NULL;
        if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_BOTLEFT)))
        {
            rightTop = GetLightingColorHelper(pHalfCell, 1);
            rightBot = GetLightingColorHelper(pHalfCell, 0);
        }
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_TOPLEFT)))
        {
            rightTop = GetLightingColorHelper(pHalfCell, 1);
            rightBot = GetLightingColorHelper(pHalfCell, 0);
        }
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_FULL)))
        {
            rightTop = GetLightingColorHelper(pHalfCell, 1);
            rightBot = GetLightingColorHelper(pHalfCell, 0);
        }
        else
        {
            rightTop = Vec4(0, 0, 0, 0);
            rightBot = Vec4(0, 0, 0, 0);
        }

        if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_BOTRIGHT)))
        {
            top = GetLightingColorHelper(pHalfCell, 1);
            bot = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_TOPRIGHT)))
        {
            top = GetLightingColorHelper(pHalfCell, 2);
            bot = GetLightingColorHelper(pHalfCell, 0);
        }
        else if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_FULL)))
        {
            top = GetLightingColorHelper(pHalfCell, 5);
            bot = GetLightingColorHelper(pHalfCell, 2);
        }
        else
        {
            top = Vec4(0, 0, 0, 0);
            bot = Vec4(0, 0, 0, 0);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::GetLightingColorsBorderTop(int x, int y, Vec4& topLeft, Vec4& topRight, Vec4& left, Vec4& right)
    {
        if (y >= (m_numCellsY - 1))
            return;

        TerrainCell& topCell = (*m_pCells)[x * m_numCellsY + y + 1];
        TerrainCell& currCell = (*m_pCells)[x * m_numCellsY + y];

        LevelHalfCell* pHalfCell = NULL;
        if ((pHalfCell = topCell.GetHalfCell(CELL_LAYOUT_BOTLEFT)))
        {
            topLeft = GetLightingColorHelper(pHalfCell, 0);
            topRight = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = topCell.GetHalfCell(CELL_LAYOUT_BOTRIGHT)))
        {
            topLeft = GetLightingColorHelper(pHalfCell, 0);
            topRight = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = topCell.GetHalfCell(CELL_LAYOUT_FULL)))
        {
            topLeft = GetLightingColorHelper(pHalfCell, 0);
            topRight = GetLightingColorHelper(pHalfCell, 2);
        }
        else
        {
            topLeft = Vec4(0, 0, 0, 0);
            topRight = Vec4(0, 0, 0, 0);
        }

        if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_TOPLEFT)))
        {
            left = GetLightingColorHelper(pHalfCell, 1);
            right = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_TOPRIGHT)))
        {
            left = GetLightingColorHelper(pHalfCell, 1);
            right = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_FULL)))
        {
            left = GetLightingColorHelper(pHalfCell, 1);
            right = GetLightingColorHelper(pHalfCell, 5);
        }
        else
        {
            left = Vec4(0, 0, 0, 0);
            right = Vec4(0, 0, 0, 0);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::GetLightingColorsBorderBot(int x, int y, Vec4& botLeft, Vec4& botRight, Vec4& left, Vec4& right)
    {
        if (y >= (m_numCellsY - 1))
            return;

        TerrainCell& botCell = (*m_pCells)[x * m_numCellsY + y + 1];
        TerrainCell& currCell = (*m_pCells)[x * m_numCellsY + y];

        LevelHalfCell* pHalfCell = NULL;
        if ((pHalfCell = botCell.GetHalfCell(CELL_LAYOUT_TOPLEFT)))
        {
            botLeft = GetLightingColorHelper(pHalfCell, 1);
            botRight = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = botCell.GetHalfCell(CELL_LAYOUT_TOPRIGHT)))
        {
            botLeft = GetLightingColorHelper(pHalfCell, 1);
            botRight = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = botCell.GetHalfCell(CELL_LAYOUT_FULL)))
        {
            botLeft = GetLightingColorHelper(pHalfCell, 1);
            botRight = GetLightingColorHelper(pHalfCell, 5);
        }
        else
        {
            botLeft = Vec4(0, 0, 0, 0);
            botRight = Vec4(0, 0, 0, 0);
        }

        if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_BOTLEFT)))
        {
            left = GetLightingColorHelper(pHalfCell, 0);
            right = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_BOTRIGHT)))
        {
            left = GetLightingColorHelper(pHalfCell, 0);
            right = GetLightingColorHelper(pHalfCell, 2);
        }
        else if ((pHalfCell = currCell.GetHalfCell(CELL_LAYOUT_FULL)))
        {
            left = GetLightingColorHelper(pHalfCell, 0);
            right = GetLightingColorHelper(pHalfCell, 2);
        }
        else
        {
            left = Vec4(0, 0, 0, 0);
            right = Vec4(0, 0, 0, 0);
        }
    }

    //-----------------------------------------------------------------------------------
    int CTerrainMgrGPU::GetLayerTop(int x, int y)
    {
        if (y >= (m_numCellsY - 1))
            return 0;

        int layer = 0;
        TerrainCell& rightCell = (*m_pCells)[x * m_numCellsY + y + 1];

        LevelHalfCell* pHalfCell = NULL;
        if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_BOTLEFT)))
            layer = pHalfCell->depthLayer;
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_BOTRIGHT)))
            layer = pHalfCell->depthLayer;
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_FULL)))
            layer = pHalfCell->depthLayer;
        else
            layer = m_layerDepths.size() - 1;

        return layer;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainMgrGPU::GetLayerBottom(int x, int y)
    {
        if (y < 1)
            return 0;

        int layer = 0;
        TerrainCell& rightCell = (*m_pCells)[x * m_numCellsY + y - 1];

        LevelHalfCell* pHalfCell = NULL;
        if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_TOPRIGHT)))
            layer = pHalfCell->depthLayer;
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_TOPLEFT)))
            layer = pHalfCell->depthLayer;
        else if ((pHalfCell = rightCell.GetHalfCell(CELL_LAYOUT_FULL)))
            layer = pHalfCell->depthLayer;
        else
            layer = m_layerDepths.size() - 1;

        return layer;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrGPU::CreateTangentVectors(TerrainVertex& p0, TerrainVertex& p1, TerrainVertex& p2)
    {
        float u0 = p0.texcoords.x;
        float v0 = p0.texcoords.y;
        float u1 = p1.texcoords.x;
        float v1 = p1.texcoords.y;
        float u2 = p2.texcoords.x;
        float v2 = p2.texcoords.y;
        float s1 = u1 - u0;
        float t1 = v1 - v0;
        float s2 = u2 - u0;
        float t2 = v2 - v0;
        Vec3 Q1 = p1.pos - p0.pos;
        Vec3 Q2 = p2.pos - p0.pos;
        Vec3 vTangent = Vec3(1.0, 1.0, 1.0) * 1.0f / (s1 * t2 - s2 * t1);
        Vec3 vBinormal = Vec3(1.0, 1.0, 1.0) * 1.0f / (s1 * t2 - s2 * t1);
        vTangent.x *= (t2 * Q1.x - t1 * Q2.x);
        vTangent.y *= (t2 * Q1.y - t1 * Q2.y);
        vTangent.z *= (t2 * Q1.z - t2 * Q2.z);
        vBinormal.x *= (-s2 * Q1.x + s1 * Q2.x);
        vBinormal.y *= (-s2 * Q1.y + s1 * Q2.y);
        vBinormal.z *= (-s2 * Q1.z + s1 * Q2.z);

        vTangent = glm::normalize(vTangent);
        vBinormal = glm::normalize(vBinormal);

        p0.tangent = vTangent;
        p0.binormal = vBinormal;
        p1.tangent = vTangent;
        p1.binormal = vBinormal;
        p2.tangent = vTangent;
        p2.binormal = vBinormal;
    }
} // env
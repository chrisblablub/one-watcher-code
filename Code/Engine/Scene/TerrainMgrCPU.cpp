#include <Engine/Scene/TerrainMgrCPU.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CTerrainMaterial::CTerrainMaterial(const CTerrainMaterial& other)
    {
        m_pMaterial = other.m_pMaterial;
        m_strMaterial = other.m_strMaterial;
        m_terrainPatternMaterials = other.m_terrainPatternMaterials;
        m_strName = other.m_strName;
        m_emissiveColor = other.m_emissiveColor;

        if(m_pMaterial)
        {
            gMaterialMgr->AddGetPtr(m_pMaterial->GetName());
        }
    }

    //-----------------------------------------------------------------------------------
    CTerrainMaterial::~CTerrainMaterial()
    {
        if(m_pMaterial)
            gMaterialMgr->Release(m_pMaterial->GetName());
    }

    //-----------------------------------------------------------------------------------
    const CTerrainMaterial& CTerrainMaterial::operator=(const CTerrainMaterial& rhs)
    {
        CTerrainMaterial tmp(rhs);
        std::swap(m_pMaterial, tmp.m_pMaterial);
        std::swap(m_strMaterial, tmp.m_strMaterial);
        std::swap(m_terrainPatternMaterials, tmp.m_terrainPatternMaterials);
        std::swap(m_strName, tmp.m_strName);
        std::swap(m_emissiveColor, tmp.m_emissiveColor);


        return *this;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMaterial::SetMaterialDiffuse(unsigned int pattern, const Vec3& diffuse)
    {
        if(m_terrainPatternMaterials.size() <= m_pMaterial->GetNumSequences())
            m_terrainPatternMaterials.resize(m_pMaterial->GetNumSequences());

        m_terrainPatternMaterials[pattern].diffuse = diffuse;
    }

    //-----------------------------------------------------------------------------------
    Vec3 CTerrainMaterial::GetMaterialDiffuse(unsigned int pattern)
    {
        return m_terrainPatternMaterials[pattern].diffuse;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMaterial::GetMaterialsDiffuse(std::vector< Vec3 >& diffuse)
    {
        diffuse.resize(m_pMaterial->GetNumSequences());

        for(int i = 0; i < m_pMaterial->GetNumSequences(); ++i)
        {
            diffuse[i] = GetMaterialDiffuse(i);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMaterial::SetMaterialSpecular(unsigned int pattern, const Vec4& specular)
    {
        if(m_terrainPatternMaterials.size() <= m_pMaterial->GetNumSequences())
            m_terrainPatternMaterials.resize(m_pMaterial->GetNumSequences());

        m_terrainPatternMaterials[pattern].specular = specular;
    }

    //-----------------------------------------------------------------------------------
    Vec4 CTerrainMaterial::GetMaterialSpecular(unsigned int pattern)
    {
        return m_terrainPatternMaterials[pattern].specular;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMaterial::GetMaterialsSpecular(std::vector< Vec4 >& specular)
    {
        specular.resize(m_pMaterial->GetNumSequences());

        for(int i = 0; i < m_pMaterial->GetNumSequences(); ++i)
        {
            specular[i] = GetMaterialSpecular(i);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMaterial::InitializeAtLoad()
    {
        m_pMaterial = gMaterialMgr->AddGetPtr(m_strMaterial);

        UpdateMaterial();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMaterial::SetMaterial(const std::string& strMaterial)
    {
        //m_strMaterial.clear();


        if(m_pMaterial)
        {
            gMaterialMgr->Release(m_pMaterial->GetName());
            m_pMaterial = NULL;
        }

        if(!strMaterial.empty())
        {
            m_pMaterial = gMaterialMgr->AddGetPtr(strMaterial);
            m_strMaterial = m_pMaterial->GetName();

            m_terrainPatternMaterials.resize(m_pMaterial->GetNumSequences());

        }

    }

    const unsigned int TERRAIN_NUM_LAYERS = 10;
    const float TERRAIN_MAX_LAYER_DEPTH = 250.0f;

    //-----------------------------------------------------------------------------------
    CTerrainMgrCPU::CTerrainMgrCPU()
        : m_numCellsX(0)
        , m_numCellsY(0)
        , m_activeMaterial(0)
        , m_currentBorderColorPreset(0)
        , m_maxDepth(100.0f)
        , m_borderRotationCenter(0.0f, 0.0f, 0.0f)
        , m_borderRotationAngle(0.05f)
        , m_pRenderObject(nullptr)
        , m_pPainterFirstMaterial(nullptr)
        , m_pPainterSecondMaterial(nullptr)
        , m_firstScale(1.0f)
        , m_secondScale(1.0f)
        , m_vertexDistance(0.0f)
        , m_brushRadius(50.0f)
        , m_brushIntensity(35.0f)
        , m_painterStrength(35.0f)
        , m_painterFirstColor(1.0f, 1.0f, 1.0f)
        , m_painterSecondColor(1.0f, 1.0f, 1.0f)
    {
        m_layerDepths.resize(TERRAIN_NUM_LAYERS);
        m_layerDepths[0] = 0.0f;
        m_layerDepths[1] = 15.0f;
        m_layerDepths[2] = 130.0f;
        m_layerDepths[3] = 150.0f;
        m_layerDepths[4] = 180.0f;
        m_layerDepths[5] = 190.0f;
        m_layerDepths[6] = 200.0f;
        m_layerDepths[7] = 220.0f;
        m_layerDepths[8] = 240.0f;
        m_layerDepths[9] = 250.0f;

        ENV_ASSERT_DBG(m_layerDepths[m_layerDepths.size() - 1] <= TERRAIN_MAX_LAYER_DEPTH);

        m_borderColorPresets.resize(TERRAIN_NUM_LAYERS);

        for(int i = 0; i < TERRAIN_NUM_LAYERS; ++i)
        {
            float lum = 0.2f - i * 0.2f /TERRAIN_NUM_LAYERS;
            m_borderColorPresets[i] = Vec3(lum, lum, lum);
        }

        m_maxDepth = m_layerDepths[m_layerDepths.size() - 1];

        m_painterVertices.resize(TERRAIN_NUM_LAYERS);
    }

    //-----------------------------------------------------------------------------------
    CTerrainMgrCPU::~CTerrainMgrCPU()
    {
        if(m_pPainterFirstMaterial)
            gMaterialMgr->Release(m_pPainterFirstMaterial->GetName());

        if(m_pPainterSecondMaterial)
            gMaterialMgr->Release(m_pPainterSecondMaterial->GetName());

        for(mapHalfCells::iterator it = m_halfCells.begin(); it != m_halfCells.end(); ++it)
        {
            pairHalfCells& hc = it->second;
            ENV_DELETE(hc.first);
            ENV_DELETE(hc.second);
        }

        m_halfCells.clear();
        m_cells.clear();

        ENV_DELETE(m_pRenderObject);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::InitializeAtCreation(unsigned int cellsX, unsigned int cellsY)
    {
        SetNumCellsX(cellsX);
        SetNumCellsY(cellsY);

        m_cells.resize(m_numCellsX * m_numCellsY);
        m_borderRotationCenter = Vec3(m_sizeX * 0.5f, m_sizeY * 0.5f, 0.0f);

        m_pRenderObject = new CTerrainMgrGPU();
        m_pRenderObject->Initialize(m_numCellsX, m_numCellsY, m_material.GetMaterialPtr(), &m_cells, &m_halfCells,
            m_borderRotationCenter, m_borderRotationAngle, m_borderColorPresets, m_layerDepths, m_maxDepth);
        m_pRenderObject->UpdateTerrainBuffers();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMaterial::UpdateMaterial()
    {
        if (m_pMaterial == nullptr)
            return;

        m_pMaterial->SetEmissiveColor(Vec3(m_emissiveColor));
        m_pMaterial->SetEmissiveEnergy(m_emissiveColor.a);

        m_pMaterial->SetEnvColor(Vec3(m_envColor));
        m_pMaterial->SetEnvReflectivity(m_envColor.a);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::InitializeAtLoad()
    {
        m_material.InitializeAtLoad();

        m_brushIntensity = std::min(m_brushIntensity, 5.0f);

        SetFirstMaterial(m_strFirstMaterial);
        SetSecondMaterial(m_strSecondMaterial);

        m_cells.clear();
        m_cells.resize(m_numCellsX * m_numCellsY);
        for(mapHalfCells::iterator it = m_halfCells.begin(); it != m_halfCells.end(); ++it)
        {
            pairHalfCells& hc = it->second;
            unsigned int gridIndex = it->first;
            TerrainCell& cell = m_cells[gridIndex];
            if(hc.first)
            {
                cell.pFirst = hc.first;

                Vec2 v1, v2, v3, v4, tmp;
                TextureRect rect;

                m_material.GetMaterialPtr()->GetTextureCoordinates(cell.pFirst->sequence, 0, rect);
                float epsHeight = 1.0f / m_material.GetMaterialPtr()->GetHeight();
                float epsWidth = 1.0f / m_material.GetMaterialPtr()->GetWidth();

                v1.x = rect.left + epsWidth;
                v1.y = rect.top + epsHeight;
                v2.x = rect.right - epsWidth;
                v2.y = rect.bottom - epsHeight;

                v3.x = v1.x;
                v3.y = v2.y;
                v4.x = v2.x;
                v4.y = v1.y;

                GetRotatedTexCoords(v1, v2, v3, v4, cell.pFirst->cellRotation);

                cell.pFirst->texCoords01.x = v1.x;
                cell.pFirst->texCoords01.y = v1.y;
                cell.pFirst->texCoords01.z = v2.x;
                cell.pFirst->texCoords01.w = v2.y;
                cell.pFirst->texCoords02.x = v3.x;
                cell.pFirst->texCoords02.y = v3.y;
                cell.pFirst->texCoords02.z = v4.x;
                cell.pFirst->texCoords02.w = v4.y;
                
            }
            if(hc.second)
            {
                cell.pSecond = hc.second;

                Vec2 v1, v2, v3, v4, tmp;
                TextureRect rect;

                m_material.GetMaterialPtr()->GetTextureCoordinates(cell.pSecond->sequence, 0, rect);
                float epsHeight = 1.0f / m_material.GetMaterialPtr()->GetHeight();
                float epsWidth = 1.0f / m_material.GetMaterialPtr()->GetWidth();

                v1.x = rect.left + epsWidth;
                v1.y = rect.top + epsHeight;
                v2.x = rect.right - epsWidth;
                v2.y = rect.bottom - epsHeight;

                v3.x = v1.x;
                v3.y = v2.y;
                v4.x = v2.x;
                v4.y = v1.y;

                GetRotatedTexCoords(v1, v2, v3, v4, cell.pSecond->cellRotation);

                cell.pSecond->texCoords01.x = v1.x;
                cell.pSecond->texCoords01.y = v1.y;
                cell.pSecond->texCoords01.z = v2.x;
                cell.pSecond->texCoords01.w = v2.y;
                cell.pSecond->texCoords02.x = v3.x;
                cell.pSecond->texCoords02.y = v3.y;
                cell.pSecond->texCoords02.z = v4.x;
                cell.pSecond->texCoords02.w = v4.y;
            }
        }

        int numLayers = m_layerDepths.size();

        m_layerDepths.resize(TERRAIN_NUM_LAYERS + 1);
        for(int i = numLayers; i <= TERRAIN_NUM_LAYERS; ++i)
        {
            m_layerDepths[i] = m_maxDepth;
        }

        m_borderColorPresets.resize(TERRAIN_NUM_LAYERS);
        for(int i = numLayers; i < TERRAIN_NUM_LAYERS; ++i)
        {
            m_borderColorPresets[i] = Vec3(0.0f, 0.0f, 0.0f);
        }

        if(m_painterVertices.size() < TERRAIN_NUM_LAYERS)
            m_painterVertices.resize(TERRAIN_NUM_LAYERS);

        if(!m_pRenderObject)
        {
            m_pRenderObject = new CTerrainMgrGPU();
            m_pRenderObject->Initialize(m_numCellsX, m_numCellsY, m_material.GetMaterialPtr(), &m_cells, &m_halfCells,
                m_borderRotationCenter, m_borderRotationAngle, m_borderColorPresets, m_layerDepths, m_maxDepth);
        }

        m_pRenderObject->SetSize(m_numCellsX, m_numCellsY);

        if (!gEngine->GetEditor())
        {
            m_pRenderObject->CreateAndInitTerrainVertexBuffer(m_numCellsX, m_numCellsY);
        }
        else
        {
            m_pRenderObject->CreateTerrainVertexBuffer(m_numCellsX, m_numCellsY);
            m_pRenderObject->UpdateTerrainBuffers();
        }


        if(m_pRenderObject->GetTerrainWireframeVertexBuffer())
            m_pRenderObject->UpdateTerrainWireframeBuffer(true);


        // BEGIN HACK: TESTING PAINTER
//         SetFirstMaterial("painter.png");
//         SetPainterStrength(50.0);
        // END HACK

        if(!m_borderColorPresets.empty())
            m_editParameters.currentEditBorderColor = m_borderColorPresets[0];
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::ApplyBorderColor(const Vec3& color)
    {
        for(mapHalfCells::iterator it = m_halfCells.begin(); it != m_halfCells.end(); ++it)
        {
            if(it->second.first)
                it->second.first->borderColor = color;

            if(it->second.second)
                it->second.second->borderColor = color;
        }
        
        if(m_pRenderObject)
            m_pRenderObject->UpdateTerrainBuffers();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::ApplyDiffuseColor(unsigned int layer, const Vec3& color)
    {
        for (mapHalfCells::iterator it = m_halfCells.begin(); it != m_halfCells.end(); ++it)
        {
            if (it->second.first && it->second.first->depthLayer == layer)
                it->second.first->layerColor = color;

            if (it->second.second && it->second.second->depthLayer == layer)
                it->second.second->layerColor = color;
        }

        if (m_pRenderObject)
            m_pRenderObject->UpdateTerrainBuffers();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::ApplyEmissiveColor(unsigned int layer, const Vec4& color)
    {
        for (mapHalfCells::iterator it = m_halfCells.begin(); it != m_halfCells.end(); ++it)
        {
            if (it->second.first && it->second.first->depthLayer == layer)
                it->second.first->emissiveColor = color;

            if (it->second.second && it->second.second->depthLayer == layer)
                it->second.second->emissiveColor = color;
        }

        if (m_pRenderObject)
            m_pRenderObject->UpdateTerrainBuffers();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetLevelSize(unsigned int numCellsX, unsigned int numCellsY)
    {
        mapHalfCells cellsOld = m_halfCells;

        m_halfCells.clear();

        for(mapHalfCells::iterator it = cellsOld.begin(); it != cellsOld.end(); ++it)
        {
            int x = it->first / m_numCellsY;
            int y = it->first % m_numCellsY;
            int newGridIndex = x * numCellsY + y;
// 			if(newGridIndex >= (numCellsX * numCellsY))
// 				continue;
            if(x >= numCellsX || y >= numCellsY)
            {
                ENV_DELETE(it->second.first);
                ENV_DELETE(it->second.second);
                continue;
            }

            m_halfCells[newGridIndex] = it->second;
        }

        for(int i = 0; i < m_painterVertices.size(); ++i)
        {
            for(mapPainterVertices::iterator it = m_painterVertices[i].begin(); it != m_painterVertices[i].end(); ++it)
            {
                unsigned int index = it->first;

                int x = index / m_numCellsY;
                int y = index % m_numCellsY;

                if(x > numCellsX || y > numCellsY)
                    it = m_painterVertices[i].erase(it);
            }
        }
        
        


        m_borderRotationCenter = Vec3(m_sizeX * 0.5f, m_sizeY * 0.5f, 0.0f);

        m_numCellsX = numCellsX;
        m_numCellsY = numCellsY;

        m_sizeX = static_cast<float>(m_numCellsX * CELL_SIZE);
        m_sizeY = static_cast<float>(m_numCellsY * CELL_SIZE);


        InitializeAtLoad();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::OffsetLevel(int offsetX, int offsetY)
    {
        if(offsetX > 0)
        {
            for(int i = m_numCellsX - 1; i >= offsetX; --i)
            {
                for(int j = 0; j < m_numCellsY; ++j)
                {
                    m_cells[i * m_numCellsY + j] = m_cells[ (i - offsetX) * m_numCellsY + j ];
                    m_cells[ (i - offsetX) * m_numCellsY + j ].Reset();

                    mapHalfCells::iterator itNew = m_halfCells.find(i * m_numCellsY + j);
                    if(itNew != m_halfCells.end())
                    {
                        ENV_DELETE(itNew->second.first);
                        ENV_DELETE(itNew->second.second);
                        m_halfCells.erase(itNew);
                    }

                    mapHalfCells::iterator itOld = m_halfCells.find((i - offsetX) * m_numCellsY + j);
                    if(itOld != m_halfCells.end())
                    {
                        m_halfCells[i * m_numCellsY + j] = m_halfCells[ (i - offsetX) * m_numCellsY + j ];
                        m_halfCells.erase(itOld);
                    }
                }
            }

            for(int i = offsetX - 1; i >= 0; --i)
            {
                for(int j = 0; j < m_numCellsY; ++j)
                {
                    m_cells[ i * m_numCellsY + j ].Reset();
                    mapHalfCells::iterator itNew = m_halfCells.find(i * m_numCellsY + j);
                    if(itNew != m_halfCells.end())
                    {
                        ENV_DELETE(itNew->second.first);
                        ENV_DELETE(itNew->second.second);
                        m_halfCells.erase(itNew);
                    }
                }
            }
        }
        else if(offsetX < 0)
        {
            for(int i = 0; i < (m_numCellsX + offsetX); ++i)
            {
                for(int j = 0; j < m_numCellsY; ++j)
                {
                    m_cells[i * m_numCellsY + j] = m_cells[ (i - offsetX) * m_numCellsY + j];
                    m_cells[ (i - offsetX) * m_numCellsY + j ].Reset();

                    mapHalfCells::iterator itNew = m_halfCells.find(i * m_numCellsY + j);
                    if(itNew != m_halfCells.end())
                    {
                        ENV_DELETE(itNew->second.first);
                        ENV_DELETE(itNew->second.second);
                        m_halfCells.erase(itNew);
                    }

                    mapHalfCells::iterator itOld = m_halfCells.find((i - offsetX) * m_numCellsY + j);
                    if(itOld != m_halfCells.end())
                    {
                        m_halfCells[i * m_numCellsY + j] = m_halfCells[ (i - offsetX) * m_numCellsY + j ];
                        m_halfCells.erase(itOld);
                    }
                }
            }

            for(int i = m_numCellsX + offsetX; i < m_numCellsY; ++i)
            {
                for(int j = 0; j < m_numCellsY; ++j)
                {
                    m_cells[ i * m_numCellsY + j ].Reset();
                    mapHalfCells::iterator itNew = m_halfCells.find(i * m_numCellsY + j);
                    if(itNew != m_halfCells.end())
                    {
                        ENV_DELETE(itNew->second.first);
                        ENV_DELETE(itNew->second.second);
                        m_halfCells.erase(itNew);
                    }
                }
            }
        }

        if(offsetY > 0)
        {
            for(int i = m_numCellsY - 1; i >= offsetY; --i)
            {
                for(int j = 0; j < m_numCellsX; ++j)
                {
                    m_cells[j * m_numCellsY + i] = m_cells[ j * m_numCellsY + i - offsetY ];
                    m_cells[ j * m_numCellsY + i - offsetY ].Reset();

                    mapHalfCells::iterator itNew = m_halfCells.find(j * m_numCellsY + i);
                    if(itNew != m_halfCells.end())
                    {
                        ENV_DELETE(itNew->second.first);
                        ENV_DELETE(itNew->second.second);
                        m_halfCells.erase(itNew);
                    }

                    mapHalfCells::iterator itOld = m_halfCells.find(j * m_numCellsY + i - offsetY);
                    if(itOld != m_halfCells.end())
                    {
                        m_halfCells[j * m_numCellsY + i] = m_halfCells[ j * m_numCellsY + i - offsetY ];
                        m_halfCells.erase(itOld);
                    }
                }
            }

        }
        else if(offsetY < 0)
        {
            for(int i = 0; i < (m_numCellsY + offsetY); ++i)
            {
                for(int j = 0; j < m_numCellsX; ++j)
                {
                    m_cells[j * m_numCellsY + i] = m_cells[ j * m_numCellsY + i - offsetY ];
                    m_cells[ j * m_numCellsY + i - offsetY ].Reset();

                    mapHalfCells::iterator itNew = m_halfCells.find(j * m_numCellsY + i);
                    if(itNew != m_halfCells.end())
                    {
                        ENV_DELETE(itNew->second.first);
                        ENV_DELETE(itNew->second.second);
                        m_halfCells.erase(itNew);
                    }

                    mapHalfCells::iterator itOld = m_halfCells.find(j * m_numCellsY + i - offsetY);
                    if(itOld != m_halfCells.end())
                    {
                        m_halfCells[j * m_numCellsY + i] = m_halfCells[ j * m_numCellsY + i - offsetY ];
                        m_halfCells.erase(itOld);
                    }
                }
            }
        }

        vecMapPainterVertices newPainterVertices;
        newPainterVertices.resize(m_painterVertices.size());

        for(int i = 0; i < m_painterVertices.size(); ++i)
        {
            for(mapPainterVertices::iterator it = m_painterVertices[i].begin(); it != m_painterVertices[i].end(); ++it)
            {
                unsigned int oldIndex = it->first;

                int x = oldIndex / (m_numCellsY + 1);
                int y = oldIndex % (m_numCellsY + 1);

                x += offsetX;
                y += offsetY;

                if(x <= m_numCellsX && y <= m_numCellsY)
                {
                    
                    unsigned int newIndex = x * (m_numCellsY + 1) + y;

                    newPainterVertices[i][newIndex] = it->second;
                }
                
            }
        }

        m_painterVertices.clear();
        m_painterVertices = newPainterVertices;

        ENV_DELETE(m_pRenderObject);

        m_pRenderObject = new CTerrainMgrGPU();
        m_pRenderObject->Initialize(m_numCellsX, m_numCellsY, m_material.GetMaterialPtr(), &m_cells, &m_halfCells,
            m_borderRotationCenter, m_borderRotationAngle, m_borderColorPresets, m_layerDepths, m_maxDepth);

        m_pRenderObject->CreateTerrainVertexBuffer(m_numCellsX, m_numCellsY);
        m_pRenderObject->UpdateTerrainBuffers();
        if(m_pRenderObject->GetTerrainWireframeVertexBuffer())
            m_pRenderObject->UpdateTerrainWireframeBuffer(true);

    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetEditSequence(int sequence)
    {
        if(!m_material.GetMaterialPtr())
            return;

        if(sequence < 0)
            sequence = m_material.GetMaterialPtr()->GetNumSequences() - 1;
        m_editParameters.currentEditSequence = (sequence % m_material.GetMaterialPtr()->GetNumSequences());
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetCell(unsigned int x, unsigned int y, LevelHalfCell* pFirst, LevelHalfCell* pSecond)
    {
        if(x >= m_numCellsX || y >= m_numCellsY)
            return;

        int gridIndex = x * m_numCellsY + y;
        
        TerrainCell& g = m_cells[gridIndex];


        RemoveHalfCell(gridIndex, g.pFirst);
        RemoveHalfCell(gridIndex, g.pSecond);
        g.Reset();

        pairHalfCells& p = m_halfCells[gridIndex];

        if(pFirst)
        {
            p.first = new LevelHalfCell(*pFirst);
            g.pFirst = p.first;
        }

        if(pSecond)
        {
            p.second = new LevelHalfCell(*pSecond);
            g.pSecond = p.second;
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetCell(unsigned int x, unsigned int y, bool bStickyCeiling)
    {
        if(!m_material.GetMaterialPtr())
            return;

        std::vector<unsigned int> cells;

        bool bEdited = false;

        int brushSize = m_editParameters.brushSize;
        int xOrig = x, yOrig = y;

        int xyMin = brushSize / 2;
        for(int i = -xyMin; i < (brushSize - xyMin); ++i)
        {
            for(int j = -xyMin; j < (brushSize - xyMin); ++j)
            {
                x = std::max(xOrig + i, 0);
                y = std::max(yOrig + j, 0);

                unsigned int gridIndex = x * m_numCellsY + y;

                cells.push_back(gridIndex);

                if(gridIndex >= m_cells.size())
                    continue;

                TerrainCell& g = m_cells[gridIndex];
                if((g.pFirst && 
                    g.pFirst->bCollision == (m_editParameters.currentEditLayer == 0) &&
                    g.pFirst->sequence == m_editParameters.currentEditSequence &&
                    g.pFirst->cellLayout == m_editParameters.currentEditCellLayout &&
                    g.pFirst->bStickyCeiling == bStickyCeiling &&
                    g.pFirst->cellRotation == m_editParameters.currentEditRotation &&
                    g.pFirst->layerColor == m_editParameters.currentEditColor &&
                    g.pFirst->emissiveColor == m_editParameters.currentEditEmissiveColor &&
                    g.pFirst->depthLayer == m_editParameters.currentEditLayer &&
                    g.pFirst->borderColor == m_editParameters.currentEditBorderColor) ||
                   (g.pSecond && 
                    g.pSecond->bCollision == (m_editParameters.currentEditLayer == 0) &&
                    g.pSecond->sequence == m_editParameters.currentEditSequence &&
                    g.pSecond->cellLayout == m_editParameters.currentEditCellLayout &&
                    g.pSecond->bStickyCeiling == bStickyCeiling &&
                    g.pSecond->cellRotation == m_editParameters.currentEditRotation &&
                    g.pSecond->layerColor == m_editParameters.currentEditColor &&
                    g.pSecond->emissiveColor == m_editParameters.currentEditEmissiveColor &&
                    g.pSecond->depthLayer == m_editParameters.currentEditLayer &&
                    g.pSecond->borderColor == m_editParameters.currentEditBorderColor))
                {
                    continue;
                }

        
                
                LevelCellEditParameters& editParams = GetCellEditParameters();
                if(editParams.bRandomRotation)
                    editParams.currentEditRotation = (CELL_ROTATION)(rand() % 4);
                

                if((g.pFirst && g.pFirst->cellLayout == m_editParameters.currentEditCellLayout) ||
                    (g.pSecond && (((g.pSecond->cellLayout + m_editParameters.currentEditCellLayout) % 2) == 0 && 
                    g.pSecond->cellLayout != m_editParameters.currentEditCellLayout && g.pSecond->cellLayout != CELL_LAYOUT_FULL && m_editParameters.currentEditCellLayout != CELL_LAYOUT_FULL)))
                {
                    RemoveHalfCell(gridIndex, g.pFirst);
                    pairHalfCells& p = m_halfCells[gridIndex];
                    p.first = new LevelHalfCell();
                    g.pFirst = p.first;
                    g.pFirst->Reset();
                    g.pFirst->sequence = m_editParameters.currentEditSequence;
                    g.pFirst->cellLayout = m_editParameters.currentEditCellLayout;
                    g.pFirst->cellRotation = m_editParameters.currentEditRotation;
                    g.pFirst->bCollision = (m_editParameters.currentEditLayer == 0);
                    g.pFirst->bStickyCeiling = bStickyCeiling;

                    SetCellAnimationPattern(x, y, g.pFirst->sequence, 0, true);

                    bEdited = true;
                }
                else if((g.pSecond && g.pSecond->cellLayout == m_editParameters.currentEditCellLayout) ||
                    (g.pFirst && (((g.pFirst->cellLayout + m_editParameters.currentEditCellLayout) % 2) == 0 && 
                    g.pFirst->cellLayout != m_editParameters.currentEditCellLayout && g.pFirst->cellLayout != CELL_LAYOUT_FULL && m_editParameters.currentEditCellLayout != CELL_LAYOUT_FULL)))
                {
                    RemoveHalfCell(gridIndex, g.pSecond);
                    pairHalfCells& p = m_halfCells[gridIndex];
                    p.second = new LevelHalfCell();
                    g.pSecond = p.second;
                    g.pSecond->Reset();
                    g.pSecond->sequence = m_editParameters.currentEditSequence;
                    g.pSecond->cellLayout = m_editParameters.currentEditCellLayout;
                    g.pSecond->cellRotation = m_editParameters.currentEditRotation;
                    g.pSecond->bCollision = (m_editParameters.currentEditLayer == 0);
                    g.pSecond->bStickyCeiling = bStickyCeiling;

                    SetCellAnimationPattern(x, y, g.pSecond->sequence, 0, false);

                    bEdited = true;
                }
                else
                {
                    RemoveHalfCell(gridIndex, g.pFirst);
                    RemoveHalfCell(gridIndex, g.pSecond);
                    g.Reset();

                    pairHalfCells& p = m_halfCells[gridIndex];
                    p.first = new LevelHalfCell();
                    g.pFirst = p.first;

                    g.pFirst->sequence = m_editParameters.currentEditSequence;
                    g.pFirst->bCollision = (m_editParameters.currentEditLayer == 0);
                    g.pFirst->cellLayout = m_editParameters.currentEditCellLayout;
                    g.pFirst->bStickyCeiling = bStickyCeiling;

                    SetCellAnimationPattern(x, y, g.pFirst->sequence, 0, true);

                    bEdited = true;
                }
            }
        }

        if(bEdited)
            m_pRenderObject->UpdateTerrainBuffers(cells);
        
    }

    //-----------------------------------------------------------------------------------
    bool CTerrainMgrCPU::FindCell(TerrainCell** ppGrid, int x, int y)
    {
        if(m_cells.size() == 0 || x < 0 || y < 0 || x > m_numCellsX - 1 || y > m_numCellsY - 1)
        {
            *ppGrid = NULL;
            return false;
        }

        unsigned int gridIndex = x * m_numCellsY + y;
        if(m_cells[gridIndex].pFirst || m_cells[gridIndex].pSecond)
        {
            *ppGrid = &m_cells[gridIndex];
            return true;
        }

        return false;

    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetCellAnimationPattern(unsigned int x, unsigned int y, unsigned int sequence, unsigned int frame, bool bFirst)
    {
        TerrainCell* pGrid = NULL;
        if(FindCell(&pGrid, x, y))
        {
            if(bFirst) 
            {
                if(m_material.GetMaterialPtr()->GetNumSequences() <= sequence)
                    sequence = 0;

                Vec2 v1, v2, v3, v4, tmp;
                TextureRect rect;

                m_material.GetMaterialPtr()->GetTextureCoordinates(sequence, frame, rect);
                float epsHeight = 1.0f / m_material.GetMaterialPtr()->GetHeight();
                float epsWidth = 1.0f / m_material.GetMaterialPtr()->GetWidth();

                v1.x = rect.left + epsWidth;
                v1.y = rect.top + epsHeight;
                v2.x = rect.right - epsWidth;
                v2.y = rect.bottom - epsHeight;

                v3.x = v1.x;
                v3.y = v2.y;
                v4.x = v2.x;
                v4.y = v1.y;

                GetRotatedTexCoords(v1, v2, v3, v4, pGrid->pFirst->cellRotation);

                pGrid->pFirst->texCoords01.x = v1.x;
                pGrid->pFirst->texCoords01.y = v1.y;
                pGrid->pFirst->texCoords01.z = v2.x;
                pGrid->pFirst->texCoords01.w = v2.y;
                pGrid->pFirst->texCoords02.x = v3.x;
                pGrid->pFirst->texCoords02.y = v3.y;
                pGrid->pFirst->texCoords02.z = v4.x;
                pGrid->pFirst->texCoords02.w = v4.y;
                
                pGrid->pFirst->borderColor = m_editParameters.currentEditBorderColor;
                pGrid->pFirst->layerColor = m_editParameters.currentEditColor;
                pGrid->pFirst->emissiveColor = m_editParameters.currentEditEmissiveColor;
                pGrid->pFirst->depthLayer = m_editParameters.currentEditLayer;
            }
            else
            {
                if(m_material.GetMaterialPtr()->GetNumSequences() <= sequence)
                    sequence = 0;

                Vec2 v1, v2, v3, v4, tmp;

                TextureRect rect;
                m_material.GetMaterialPtr()->GetTextureCoordinates(sequence, frame, rect);
                float epsHeight = 1.0f / m_material.GetMaterialPtr()->GetHeight();
                float epsWidth = 1.0f / m_material.GetMaterialPtr()->GetWidth();

                v1.x = rect.left + epsWidth;
                v1.y = rect.top + epsHeight;
                v2.x = rect.right - epsWidth;
                v2.y = rect.bottom - epsHeight;

                v3.x = v1.x;
                v3.y = v2.y;
                v4.x = v2.x;
                v4.y = v1.y;

                GetRotatedTexCoords(v1, v2, v3, v4, pGrid->pSecond->cellRotation);

                pGrid->pSecond->texCoords01.x = v1.x;
                pGrid->pSecond->texCoords01.y = v1.y;
                pGrid->pSecond->texCoords01.z = v2.x;
                pGrid->pSecond->texCoords01.w = v2.y;
                pGrid->pSecond->texCoords02.x = v3.x;
                pGrid->pSecond->texCoords02.y = v3.y;
                pGrid->pSecond->texCoords02.z = v4.x;
                pGrid->pSecond->texCoords02.w = v4.y;

                pGrid->pFirst->borderColor = m_editParameters.currentEditBorderColor;
                pGrid->pSecond->layerColor = m_editParameters.currentEditColor;
                pGrid->pSecond->emissiveColor = m_editParameters.currentEditEmissiveColor;
                pGrid->pSecond->depthLayer = m_editParameters.currentEditLayer;
            }

            
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::UnsetBothCells(unsigned int x, unsigned int y)
    {
        //RemoveHalfCell(x * numCellsY + y, true, true);
        m_halfCells.erase(x * m_numCellsY + y);
        TerrainCell* pCell;
        if(FindCell(&pCell, x, y))
        {
            pCell->Reset();
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::UnsetCell(unsigned int x, unsigned int y)
    {
        int brushSize = m_editParameters.brushSize;
        int xOrig = x, yOrig = y;
        int xyMin = brushSize / 2;

        std::vector<unsigned int> cells;

        for(int i = -xyMin; i < (brushSize - xyMin); ++i)
        {
            for(int j = -xyMin; j < (brushSize - xyMin); ++j)
            {
                x = std::max(xOrig + i, 0);
                y = std::max(yOrig + j, 0);

                unsigned int gridIndex = x * m_numCellsY + y;

                if(gridIndex >= m_cells.size())
                    return;

                TerrainCell& cell = m_cells[gridIndex];

                if(cell.pFirst && cell.pFirst->cellLayout == m_editParameters.currentEditCellLayout)
                {
                    RemoveHalfCell(gridIndex, cell.pFirst);
                    cell.pFirst = NULL;

                    cells.push_back(gridIndex);
                }
                else if(cell.pSecond && cell.pSecond->cellLayout == m_editParameters.currentEditCellLayout)
                {
                    RemoveHalfCell(gridIndex, cell.pSecond);
                    cell.pSecond = NULL;

                    cells.push_back(gridIndex);
                }
                else
                {
                    if((cell.pFirst && (cell.pFirst->cellLayout + m_editParameters.currentEditCellLayout) % 2 == 0 && 
                        cell.pFirst->cellLayout != CELL_LAYOUT_FULL && CELL_LAYOUT_FULL != m_editParameters.currentEditCellLayout) ||
                        (cell.pSecond && (cell.pSecond->cellLayout + m_editParameters.currentEditCellLayout) % 2 == 0 && 
                        cell.pSecond->cellLayout != CELL_LAYOUT_FULL && CELL_LAYOUT_FULL != m_editParameters.currentEditCellLayout))
                    {

                    }
                    else
                    {
                        RemoveHalfCell(gridIndex, cell.pFirst);
                        RemoveHalfCell(gridIndex, cell.pSecond);
                        cell.Reset();

                        cells.push_back(gridIndex);
                    }
                }
            }
        }

        m_pRenderObject->UpdateTerrainBuffers(cells);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::RemoveHalfCell(unsigned int index, LevelHalfCell* ptr)
    {
        mapHalfCells::iterator it = m_halfCells.find(index);
        TerrainCell& cell = m_cells[index];

        if(it != m_halfCells.end())
        {
            if((!cell.pFirst && !cell.pSecond) ||
               (cell.pFirst == ptr && !cell.pSecond) ||
               (cell.pSecond == ptr && !cell.pFirst))
            {
                m_halfCells.erase(it);
            }
            else
            {
                if(cell.pFirst == ptr)
                    it->second.first = NULL;
                else if(cell.pSecond == ptr)
                    it->second.second = NULL;
            }
        }

        if(cell.pFirst == ptr)
            cell.pFirst = NULL;
        else if(cell.pSecond == ptr)
            cell.pSecond = NULL;

        ENV_DELETE(ptr);
            

//      TODO: REMOVE PAINTER VERTICES!!!!
//
// 		mapPainterVertices verts = m_painterVertices;
// 
// 		for(int index = 0; index < m_cells.size(); ++index)
// 		{
// 			int x = index / m_numCellsY;
// 			int y = index % m_numCellsY;
// 
// 
// 			bool bCellFound = false;
// 			for(int k = max(x - 1, 0); k <= min(x + 1, m_numCellsX); ++k)
// 			{
// 				for(int l = max(y - 1, 0); l <= min(y + 1, m_numCellsY); ++l)
// 				{
// 					if(m_halfCells.find(k * (m_numCellsY) + l) != m_halfCells.end())
// 					{
// 						bCellFound = true;
// 						break;
// 					}
// 				}
// 			}
// 
// 			if(!bCellFound)
// 			{
// 				m_painterVertices.erase(x * (m_numCellsY + 1) + y);
// 			}
// 		}
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::GetRotatedTexCoords(Vec2& v1, Vec2& v2, Vec2& v3, Vec2& v4, CELL_ROTATION rot)
    {
        Vec2 tmp;
        switch(rot)
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
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetLayerDepth(int layer, float depth)
    {
        if(depth <= m_maxDepth)
            m_layerDepths[layer] = depth;
        else
            m_layerDepths[layer] = m_maxDepth;

        if (m_pRenderObject)
        {
            m_pRenderObject->SetLayerDepths(m_layerDepths, m_maxDepth);
            m_pRenderObject->UpdateTerrainBuffers();
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetLayerMaxDepth(float d)
    {
        m_maxDepth = d;

        if (m_pRenderObject)
        {
            m_pRenderObject->SetLayerMaxDepth(m_maxDepth);
            m_pRenderObject->UpdateTerrainBuffers();
        }
    }

    //-----------------------------------------------------------------------------------
    float CTerrainMgrCPU::GetLayerDepth(int layer)
    {
        return m_layerDepths[layer];
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetBorderColorPreset(unsigned int preset, const Vec3& color)
    {
        m_borderColorPresets.resize(preset + 1);
        m_borderColorPresets[preset] = color;

        if(preset == m_currentBorderColorPreset)
            SetActiveBorderColorPreset(preset);

        if (m_pRenderObject)
            m_pRenderObject->SetBorderColorPresets(m_borderColorPresets);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetActiveBorderColorPreset(unsigned int preset)
    {
        m_currentBorderColorPreset = preset;

        if(m_borderColorPresets.size() <= preset)
        {
            m_borderColorPresets.resize(preset + 1);
            m_borderColorPresets[preset] = Vec3(0.0f, 0.0f, 0.0f);
        }

        m_editParameters.currentEditBorderColor = m_borderColorPresets[preset];
        m_pRenderObject->UpdateTerrainBuffers();
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CTerrainMgrCPU::GetBorderColorPreset(int layer)
    {
        if(m_borderColorPresets.size() <= layer)
        {
            m_borderColorPresets.resize(layer + 1);
            m_borderColorPresets[layer] = Vec3(0.0f, 0.0f, 0.0f);
        }
        return m_borderColorPresets[layer];
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetMaterial(const std::string& strMaterial)
    {
        if (m_material.GetName() == strMaterial)
            return;

        m_material.SetMaterial(strMaterial);
        m_material.SetName(strMaterial);

        if (m_pRenderObject)
            m_pRenderObject->SetMaterial(m_material.GetMaterialPtr());

        for(mapHalfCells::iterator it = m_halfCells.begin(); it != m_halfCells.end(); ++it)
        {
            if(it->second.first)
            {
                if(it->second.first->sequence >= m_material.GetMaterialPtr()->GetNumSequences())
                {
                    it->second.first->sequence = 0;
                    ENV_LOG_WARNING("Sequence of cell does not exist in new material.");
                }
            }

            if(it->second.second)
            {
                if(it->second.second->sequence >= m_material.GetMaterialPtr()->GetNumSequences())
                {
                    it->second.second->sequence = 0;
                    ENV_LOG_WARNING("Sequence of cell does not exist in new material.");
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::GetSize(unsigned int& gridsX, unsigned int& gridsY, float& width)
    {
        gridsX = GetNumCellsX();
        gridsY = GetNumCellsY();
        width = CELL_SIZE;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetWireframe(bool bSet)
    {
        if(m_pRenderObject)
            m_pRenderObject->UpdateTerrainWireframeBuffer(bSet);	
    }

    //-----------------------------------------------------------------------------------
    bool CTerrainMgrCPU::GetWireframe()
    {
        if(m_pRenderObject)
            return m_pRenderObject->GetTerrainWireframeVertexBuffer() != NULL;

        return false;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetBorderRotationAngle( float angle )
    {
        m_borderRotationAngle = angle; 
        if (m_pRenderObject)
        {
            m_pRenderObject->SetBorderRotationAngle(m_borderRotationAngle);
            m_pRenderObject->UpdateTerrainBuffers();
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetBorderRotationCenter( const Vec3& center )
    {
        m_borderRotationCenter = center; 

        if (m_pRenderObject)
        {
            m_pRenderObject->SetBorderRotationCenter(m_borderRotationCenter);
            m_pRenderObject->UpdateTerrainBuffers();
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::Paint(bool bPaint, unsigned int cellX, unsigned int cellY, const Vec2& mouseIntersection, float dt)
    {
        if(!m_pRenderObject)
            return;

        //unsigned int cellX = m_pLevel->GetMouseHoverCellX();
        //unsigned int cellY = m_pLevel->GetMouseHoverCellY();
        m_brushIntensity = 50;

        float intensity = m_brushIntensity * dt;
        if(!bPaint)
            intensity = -intensity;

        m_vertexDistance = CELL_SIZE;

        const Vec2& v = mouseIntersection;
        int left = std::max((int)((v.x - m_brushRadius) / m_vertexDistance), 0);
        int bottom = std::max((int)((v.y - m_brushRadius) / m_vertexDistance), 0);
        int right = std::min((int)((v.x + m_brushRadius) / m_vertexDistance) + 1, (int)m_numCellsX);
        int top = std::min((int)((v.y + m_brushRadius) / m_vertexDistance) + 1, (int)m_numCellsY);

        for(int i = left; i <= right; ++i)
        {
            for(int j = bottom; j <= top; ++j)
            {
                mapPainterVertices::iterator it = m_painterVertices[m_editParameters.currentEditLayer].find(i * (m_numCellsY + 1) + j);
                if(!bPaint && it == m_painterVertices[m_editParameters.currentEditLayer].end())
                    continue;


                bool bCellFound = false;
                for (int k = std::max(i - 1, 0); k < std::min(i + 1, (int)m_numCellsX); ++k)
                {
                    for (int l = std::max(j - 1, 0); l < std::min(j + 1, (int)m_numCellsY); ++l)
                    {
                        if(m_halfCells.find(k * m_numCellsY + l) != m_halfCells.end())
                        {
                            const pairHalfCells& pHC = m_halfCells.find(k * m_numCellsY + l)->second;
                            if ((pHC.first && pHC.first->depthLayer == m_editParameters.currentEditLayer) ||
                                (pHC.second && pHC.second->depthLayer == m_editParameters.currentEditLayer))
                            {
                                bCellFound = true;
                                break;
                            }
                                
                        }
                    }
                }

                if(bPaint && !bCellFound)
                    continue;

                PainterVertex& vertex = m_painterVertices[m_editParameters.currentEditLayer][i * (m_numCellsY + 1) + j];
                Vec3 vertexPos(i * CELL_SIZE, j * CELL_SIZE, GetLayerDepth(m_editParameters.currentEditLayer));

                vertexPos = m_pRenderObject->ComputeTransformedPosition(vertexPos);

                float dist = glm::length(v - Vec2(vertexPos.x, vertexPos.y));
                float inti = intensity * (1.0f - std::min(dist / m_brushRadius, 1.0f));

                if(m_activeMaterial == 0)
                {
                    if(bPaint)
                        vertex.color01 = m_painterFirstColor;
                    vertex.alpha.x += inti;
                    if(vertex.alpha.x > 1.0f)
                        vertex.alpha.x = 1.0f;
                    else if(vertex.alpha.x < 0.0f)
                        vertex.alpha.x = 0.0f;
                }
                else 
                {
                    if(bPaint)
                        vertex.color01 = m_painterFirstColor;
                        //vertex.color02 = m_painterSecondColor;
                    vertex.alpha.y += inti;
                    if(vertex.alpha.y > 1.0f)
                        vertex.alpha.y = 1.0f;
                    else if(vertex.alpha.y < 0.0f)
                        vertex.alpha.y = 0.0f;
                }

                if(vertex.alpha.x < 0.01f && vertex.alpha.y < 0.01f && vertex.contrast < 0.01f)
                {
                    m_painterVertices[m_editParameters.currentEditLayer].erase(i * (m_numCellsY + 1) + j);
                }
                
            }
        }

        m_pRenderObject->UpdateTerrainBuffers();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetFirstMaterial(const std::string& strAnim)
    {
        if(m_pPainterFirstMaterial)
        {
            gMaterialMgr->Release(m_pPainterFirstMaterial->GetName());
            m_pPainterFirstMaterial = NULL;
        }

        m_pPainterFirstMaterial = gMaterialMgr->AddGetPtr(strAnim);
        m_strFirstMaterial = strAnim;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::SetSecondMaterial(const std::string& strAnim)
    {
        if(m_pPainterSecondMaterial)
        {
            gMaterialMgr->Release(m_pPainterSecondMaterial->GetName());
            m_pPainterSecondMaterial = NULL;
        }

        m_pPainterSecondMaterial = gMaterialMgr->AddGetPtr(strAnim);
        m_strSecondMaterial = strAnim;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgrCPU::ResetPainterCells()
    {
        for(int i = 0; i < m_painterVertices.size(); ++i)
            m_painterVertices[i].clear();

        m_painterVertices.clear();
        m_painterVertices.resize(m_layerDepths.size() - 1);

        m_pRenderObject->UpdateTerrainBuffers();
    }

} // env
#include <Engine/Scene/TerrainSelection.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CTerrainSelection::~CTerrainSelection()
    {
        for (mapHalfCells::iterator it = m_halfCellsSelection.begin(); it != m_halfCellsSelection.end(); ++it)
        {
            pairHalfCells& p = it->second;

            ENV_DELETE(p.first);
            ENV_DELETE(p.second);
        }
        m_halfCellsSelection.clear();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainSelection::SetSelectionLayer(unsigned int layer)
    {
        for (auto& halfCell : m_halfCellsSelection)
        {
            if (halfCell.second.first)
            {
                halfCell.second.first->layerColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainDiffuseColor(layer);
                halfCell.second.first->emissiveColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(layer);
                halfCell.second.first->depthLayer = layer;
                halfCell.second.first->bCollision = layer == 0;
            }

            if (halfCell.second.second)
            {
                halfCell.second.second->layerColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainDiffuseColor(layer);
                halfCell.second.second->emissiveColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(layer);
                halfCell.second.second->depthLayer = layer;
                halfCell.second.second->bCollision = layer == 0;
            }
        }

        m_levelGrid.GetLevelRenderObject()->UpdateSelection(m_halfCellsSelection);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainSelection::SetSelectionOffset(int x, int y)
    {
        m_selectionOffsetX = x;
        m_selectionOffsetY = y;

        if (m_levelGrid.GetLevelRenderObject())
            m_levelGrid.GetLevelRenderObject()->SetSelectionOffset(m_halfCellsSelection, m_selectionOffsetX, m_selectionOffsetY);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainSelection::CreateSelection(int xMin, int yMin, int xMax, int yMax)
    {
        for (mapHalfCells::iterator it = m_halfCellsSelection.begin(); it != m_halfCellsSelection.end(); ++it)
        {
            pairHalfCells& p = it->second;

            ENV_DELETE(p.first);
            ENV_DELETE(p.second);
        }
        m_halfCellsSelection.clear();

        m_selectionMinX = std::max(xMin, 0);
        m_selectionMinY = std::max(yMin, 0);
        m_selectionMaxX = xMax;
        m_selectionMaxY = yMax;

        m_selectionOffsetX = 0;
        m_selectionOffsetY = 0;

//         int minGridIndex = xMin * m_levelGrid.GetNumCellsY() + yMin;
//         int maxGridIndex = xMax * m_levelGrid.GetNumCellsY() + yMax;

        for (int i = m_selectionMinX; i <= m_selectionMaxX; ++i)
        {
            for (int j = m_selectionMinY; j <= m_selectionMaxY; ++j)
            {
                int gridIndex = i * m_levelGrid.GetNumCellsY() + j;

                if (m_levelGrid.GetHalfCells().find(gridIndex) != m_levelGrid.GetHalfCells().end())
                {
                    pairHalfCells& p = m_levelGrid.GetHalfCells()[gridIndex];

                    pairHalfCells& pSel = m_halfCellsSelection[gridIndex];
                    if (p.first)
                        pSel.first = new LevelHalfCell(*p.first);
                    if (p.second)
                        pSel.second = new LevelHalfCell(*p.second);
                }
            }
        }

        m_levelGrid.GetLevelRenderObject()->UpdateSelection(m_halfCellsSelection);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainSelection::MergeSelection()
    {
        if (m_selectionOffsetX == 0 && m_selectionOffsetY == 0)
            return;

        for (mapHalfCells::iterator it = m_halfCellsSelection.begin(); it != m_halfCellsSelection.end(); ++it)
        {
            unsigned int x, y, gridIndex;
            LevelHalfCell* pFirstCell = it->second.first;
            LevelHalfCell* pSecondCell = it->second.second;
            gridIndex = it->first;
            x = gridIndex / m_levelGrid.GetNumCellsY();
            y = gridIndex % m_levelGrid.GetNumCellsY();

            m_levelGrid.SetCell(x + m_selectionOffsetX, y + m_selectionOffsetY, pFirstCell, pSecondCell);
        }

        m_levelGrid.GetLevelRenderObject()->UpdateTerrainBuffers();
    }

    //-----------------------------------------------------------------------------------
    void CTerrainSelection::RemoveSelection()
    {
        for (mapHalfCells::iterator it = m_halfCellsSelection.begin(); it != m_halfCellsSelection.end(); ++it)
        {
            unsigned int x, y, gridIndex;

            gridIndex = it->first;
            x = gridIndex / m_levelGrid.GetNumCellsY();
            y = gridIndex % m_levelGrid.GetNumCellsY();

            m_levelGrid.UnsetBothCells(x, y);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainSelection::SaveTemplate(const std::string& strFilename)
    {
        CScript script;

        script["selectionMinX"] = (int)m_selectionMinX;
        script["selectionMaxX"] = (int)m_selectionMaxX;
        script["selectionMinY"] = (int)m_selectionMinY;
        script["selectionMaxY"] = (int)m_selectionMaxY;

        int i = 0;
        for (mapHalfCells::iterator it = m_halfCellsSelection.begin(); it != m_halfCellsSelection.end(); ++it, ++i)
        {
            LevelHalfCell* pFirstHalfCell = it->second.first;
            LevelHalfCell* pSecondHalfCell = it->second.second;

            if (pFirstHalfCell)
            {
                std::stringstream strScriptName;
                strScriptName << "cell#" << i << "#first";

                CScript& cellScript = *script.AddSubscript(strScriptName.str());

                cellScript["cellX"] = (int)(it->first / m_levelGrid.GetNumCellsY());
                cellScript["cellY"] = (int)(it->first % m_levelGrid.GetNumCellsY());

                pFirstHalfCell->SaveTemplate(cellScript);
            }

            if (pSecondHalfCell)
            {
                std::stringstream strScriptName;
                strScriptName << "cell#" << i << "#second";

                CScript& cellScript = *script.AddSubscript(strScriptName.str());

                cellScript["cellX"] = (int)(it->first / m_levelGrid.GetNumCellsY());
                cellScript["cellY"] = (int)(it->first % m_levelGrid.GetNumCellsY());

                pSecondHalfCell->SaveTemplate(cellScript);
            }
        }

        script.SaveScript(strFilename);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainSelection::LoadTemplate(const std::string& strFilename)
    {
        ClearSelection();

        CScript* pScript = gScriptMgr->AddGetPtr(strFilename);

        if (!pScript)
            return;

        int i = 0;

        m_selectionMinX = (int)(*pScript)["selectionMinX"];
        m_selectionMaxX = (int)(*pScript)["selectionMaxX"];
        m_selectionMinY = (int)(*pScript)["selectionMinY"];
        m_selectionMaxY = (int)(*pScript)["selectionMaxY"];

        bool bCellFound = true;
        for (; bCellFound; ++i)
        {
            std::stringstream strScriptFirstName;
            std::stringstream strScriptSecondName;

            strScriptFirstName << "cell#" << i << "#first";
            strScriptSecondName << "cell#" << i << "#second";

            bCellFound = false;

            if (pScript->SubscriptExists(strScriptFirstName.str()))
            {
                CScript& cellScript = *pScript->GetSubscript(strScriptFirstName.str());

                int cellX = cellScript["cellX"];
                int cellY = cellScript["cellY"];

                int index = cellX * m_levelGrid.GetNumCellsY() + cellY;

                m_halfCellsSelection[index].first = new LevelHalfCell();
                m_halfCellsSelection[index].first->LoadTemplate(cellScript);

                bCellFound = true;
            }

            if (pScript->SubscriptExists(strScriptSecondName.str()))
            {
                CScript& cellScript = *pScript->GetSubscript(strScriptSecondName.str());

                int cellX = cellScript["cellX"];
                int cellY = cellScript["cellY"];

                int index = cellX * m_levelGrid.GetNumCellsY() + cellY;

                m_halfCellsSelection[index].second = new LevelHalfCell();
                m_halfCellsSelection[index].second->LoadTemplate(cellScript);

                bCellFound = true;
            }
        }

        gScriptMgr->Release(strFilename);

        m_levelGrid.GetLevelRenderObject()->RemoveSelection();
        m_levelGrid.GetLevelRenderObject()->UpdateSelection(m_halfCellsSelection);
    }

} // env
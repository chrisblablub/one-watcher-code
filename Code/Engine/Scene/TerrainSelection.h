/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TERRAIN_SELECTION_H
#define TERRAIN_SELECTION_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/TerrainMgrCPU.h>

namespace env
{
    class CTerrainSelection
    {
    public:
        CTerrainSelection(CTerrainMgrCPU& levelGrid)
            : m_levelGrid(levelGrid) {}

        ~CTerrainSelection();

        void CreateSelection(int xMin, int yMin, int xMax, int yMax);
        void MergeSelection();
        void RemoveSelection();
        void ClearSelection()
        {
            m_halfCellsSelection.clear();
            m_levelGrid.GetLevelRenderObject()->UpdateSelection(m_halfCellsSelection);
        }

        void SetSelectionLayer(unsigned int layer);

        void SetSelectionOffset(int x, int y);
        void GetSelectionOffset(int& x, int& y) { x = m_selectionOffsetX; y = m_selectionOffsetY; }

        int GetSelectionMinX() const { return m_selectionMinX; }
        int GetSelectionMaxX() const { return m_selectionMaxX; }
        int GetSelectionMinY() const { return m_selectionMinY; }
        int GetSelectionMaxY() const { return m_selectionMaxY; }

        void SaveTemplate(const std::string& strFilename);
        void LoadTemplate(const std::string& strFilename);

    private:
        CTerrainMgrCPU& m_levelGrid;

        int m_selectionMinX, 
            m_selectionMinY, 
            m_selectionMaxX, 
            m_selectionMaxY,
            m_selectionOffsetX, 
            m_selectionOffsetY;

        mapHalfCells m_halfCellsSelection;

    };
} // env
#endif // TERRAIN_SELECTION_H
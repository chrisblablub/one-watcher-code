#include <Engine/Scene/TerrainCell.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    void TerrainCell::Reset()
    {
        pFirst = nullptr;
        pSecond = nullptr;
    }

    //-----------------------------------------------------------------------------------
    bool TerrainCell::LayoutEquals(CELL_LAYOUT layout)
    {
        if (layout == CELL_LAYOUT_FULL)
        {
            if ((pFirst && pSecond && pFirst->depthLayer == pSecond->depthLayer) ||
                (pFirst && pFirst->cellLayout == CELL_LAYOUT_FULL) ||
                (pSecond && pSecond->cellLayout == CELL_LAYOUT_FULL))
                return true;
            else
                return false;
        }
        else// if(layout == CELL_LAYOUT_BOTLEFT)
        {
            if ((pFirst && !pSecond && pFirst->cellLayout == layout) ||
                (pSecond && !pFirst && pSecond->cellLayout == layout))
                return true;
            else
                return false;
        }
    }

    //-----------------------------------------------------------------------------------
    bool TerrainCell::LayoutContains(CELL_LAYOUT layout)
    {
        if ((pFirst && pFirst->cellLayout == layout) ||
            (pSecond && pSecond->cellLayout == layout))
        {
            return true;
        }
        else
            return false;
    }

    //-----------------------------------------------------------------------------------
    LevelHalfCell* TerrainCell::GetHalfCell(CELL_LAYOUT layout)
    {
        if (pFirst && pFirst->cellLayout == layout)
            return pFirst;
        else if (pSecond && pSecond->cellLayout == layout)
            return pSecond;

        return nullptr;
    }

    //-----------------------------------------------------------------------------------
    bool TerrainCell::LayerEquals(int layer)
    {
        if (pFirst && pSecond && pFirst->depthLayer == layer && pSecond->depthLayer == layer)
            return true;
        else
        {
            if ((pFirst && !pSecond && pFirst->depthLayer == layer) ||
                (pSecond && !pFirst && pSecond->depthLayer == layer))
                return true;
        }
        return false;
    }
} // env
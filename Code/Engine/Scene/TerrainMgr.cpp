
#include <Engine/Scene/TerrainMgr.h>
#include <Engine/Scene/TerrainSelection.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
#include <Graphic/RenderObjects/TerrainRenderObject.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CTerrainMgr::CTerrainMgr()
        : m_gravityDir(GRAVITY_DOWN),
        m_initialGravityDir(GRAVITY_DOWN),
        m_mouseIntersection(0.0f, 0.0f),
        m_mouseHoverCellY(0),
        m_mouseHoverCellX(0),
        m_levelSelection(m_levelGrid)
    {
    }

    //-----------------------------------------------------------------------------------
    CTerrainMgr::~CTerrainMgr()
    {
    }

    //-----------------------------------------------------------------------------------
    bool CTerrainMgr::CheckRamps(CTerrainCollider& c, int cellXMin, int cellXMax, int cellYMin, int cellYMax, 
            CELL_LAYOUT cellLayout, float& distance)
    {
        // This method returns the distance to the ramp.
        // It will be negative, if the bounding box lies inside the ramp, and
        // positive otherwise.
        //
        //const ColliderBBox& bbox = c.GetBBox();
        
        distance = 0.0f;

        VECTOR2FIXED v1, v2, v3, v4, v;
        c.GetCorners(v1, v2, v3, v4);


        int refX = 0, refY = 0,
            cellX = 0, cellY = 0;

        switch(cellLayout)
        {
        case CELL_LAYOUT_BOTLEFT:
            refX = cellXMin * CELL_SIZE_SCALED;
            refY = cellYMin * CELL_SIZE_SCALED;
            cellX = cellXMin;
            cellY = cellYMin;
            v = v2;
            break;
        case CELL_LAYOUT_BOTRIGHT:
            refX = cellXMax * CELL_SIZE_SCALED;
            refY = cellYMin* CELL_SIZE_SCALED;
            cellX = cellXMax;
            cellY = cellYMin;

            v = v4;
            break;
        case CELL_LAYOUT_TOPLEFT:
            refX = cellXMin * CELL_SIZE_SCALED;
            refY = cellYMax * CELL_SIZE_SCALED;
            cellX = cellXMin;
            cellY = cellYMax;

            v = v1;
            break;

        case CELL_LAYOUT_TOPRIGHT:
            refX = cellXMax * CELL_SIZE_SCALED;
            refY = cellYMax * CELL_SIZE_SCALED;
            cellX = cellXMax;
            cellY = cellYMax;

            v = v3;
            break;

        default:
            break;
        }
        
        TerrainCell* pCell = NULL;
        if(FindCell(&pCell, cellX, cellY) && ((pCell->pFirst && pCell->pFirst->bCollision && pCell->pFirst->cellLayout == cellLayout) || 
                                              (pCell->pSecond && pCell->pSecond->bCollision && pCell->pSecond->cellLayout == cellLayout)))
        {
            // Check if on ramp
            int x = v.x - refX;
            int y = v.y - refY;
            int fx = CELL_SIZE_SCALED - y;
            int fy = CELL_SIZE_SCALED - x;
            if(cellLayout == CELL_LAYOUT_BOTLEFT)
                distance = static_cast<float>(x - fx);
            else if(cellLayout == CELL_LAYOUT_TOPLEFT)
                distance = static_cast<float>(x - y);
            else if(cellLayout == CELL_LAYOUT_TOPRIGHT)
                distance = static_cast<float>(fy - y);
            else if(cellLayout == CELL_LAYOUT_BOTRIGHT)
                distance = static_cast<float>(y-x);
            else
                return false;

            return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    bool CTerrainMgr::CheckTransition(CTerrainCollider& c, int dirX, int dirY, bool bOnlyX, bool bOnlyY,
            int cellXMin, int cellXMax, int cellYMin, int cellYMax, bool* pbStickyCeiling)
    {
        int offset = 0;

        TerrainCell* pCell = NULL;

        bool bTransitionPossible = true;

        if(bOnlyX)
        {
            if(dirX <= 0)
            {
                for(int i = cellYMin; i <= cellYMax; ++i)
                {
                    if(i == cellYMax && i > cellYMin)
                    {
                        if(FindCell(&pCell, cellXMin-offset, i))
                        {
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                                bTransitionPossible = false;
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_BOTRIGHT || 
                                   layout == CELL_LAYOUT_BOTLEFT || layout == CELL_LAYOUT_TOPRIGHT)
                                   bTransitionPossible = false;
                            }
                        }
                    }
                    else if(i == cellYMin && i < cellYMax)
                    {
                        if(FindCell(&pCell, cellXMin-offset, i))
                        {
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                                bTransitionPossible = false;
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_BOTRIGHT || 
                                   layout == CELL_LAYOUT_TOPLEFT || layout == CELL_LAYOUT_TOPRIGHT)
                                    bTransitionPossible = false;
                            }
                        }
                    }
                    else if (cellYMin < i && i < cellYMax)
                    {
                        if(FindCell(&pCell, cellXMin-offset, i) && ((pCell->pFirst && pCell->pFirst->bCollision) || (pCell->pSecond && pCell->pSecond->bCollision)))
                        {
                            bTransitionPossible = false;
                        }
                    }
                    else // if(cellYMin == cellYMax)
                    {
                        if(FindCell(&pCell, cellXMin-offset, i))
                        {
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                                bTransitionPossible = false;
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_TOPRIGHT || layout == CELL_LAYOUT_BOTRIGHT)
                                    bTransitionPossible = false;
                            }
                        }
                    }

                    if(!bTransitionPossible)
                    {
                        //c.GetCellsHit().push_back((cellXMin-offset) * m_grid.GetNumCellsY() + i);
                    }
                }

            }
            else // if(dirX > 0)
            {
                for(int i = cellYMin; i <= cellYMax; ++i)
                {
                    if(i == cellYMax && i > cellYMin)
                    {
                        if(FindCell(&pCell, cellXMax+offset, i))
                        {
                            //bTransitionPossible = false;
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                                bTransitionPossible = false;
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_BOTRIGHT || 
                                   layout == CELL_LAYOUT_BOTLEFT || layout == CELL_LAYOUT_TOPLEFT)
                                    bTransitionPossible = false;
                            }
                        }
                    }
                    else if(i == cellYMin && i < cellYMax)
                    {
                        if(FindCell(&pCell, cellXMax+offset, i))
                        {
                            //bTransitionPossible = false;
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                                bTransitionPossible = false;
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision) 
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_BOTLEFT || 
                                    layout == CELL_LAYOUT_TOPLEFT || layout == CELL_LAYOUT_TOPRIGHT)
                                    bTransitionPossible = false;
                            }
                        }
                    }
                    else if (cellYMin < i && i < cellYMax)
                    {
                        if(FindCell(&pCell, cellXMax+offset, i) && ((pCell->pFirst && pCell->pFirst->bCollision) || (pCell->pSecond && pCell->pSecond->bCollision)))
                        {
                            bTransitionPossible = false;
                        }
                    }
                    else // if(cellYMin == cellYMax)
                    {
                        if(FindCell(&pCell, cellXMax+offset, i))
                        {
                            //bTransitionPossible = false;
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                                bTransitionPossible = false;
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_TOPLEFT || layout == CELL_LAYOUT_BOTLEFT)
                                    bTransitionPossible = false;
                            }
                        }
                    }

                    if(!bTransitionPossible)
                    {
                        //c.GetCellsHit().push_back((cellXMax+offset) * m_grid.GetNumCellsY() + i);
                    }
                }
            }
        }
        else if(bOnlyY)
        {
            if(dirY >= 0)
            {
                for(int j = cellXMin; j <= cellXMax; ++j)
                {
                    if(j == cellXMin && j < cellXMax)
                    {
                        if(FindCell(&pCell, j, cellYMax+offset))
                        {
                            //bTransitionPossible = false;
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                            {
                                bTransitionPossible = false;
                                if(pbStickyCeiling)
                                    *pbStickyCeiling = pCell->pFirst->bStickyCeiling || pCell->pSecond->bStickyCeiling;
                            }
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_BOTRIGHT || 
                                    layout == CELL_LAYOUT_BOTLEFT || layout == CELL_LAYOUT_TOPRIGHT)
                                {
                                    bTransitionPossible = false;

                                    if(pbStickyCeiling)
                                    {
                                        if(pCell->pFirst && pCell->pFirst->bCollision)
                                            *pbStickyCeiling = pCell->pFirst->bStickyCeiling;
                                        else if(pCell->pSecond && pCell->pSecond->bCollision)
                                            *pbStickyCeiling = pCell->pSecond->bStickyCeiling;
                                    }
                                }
                            }
                        }
                    }
                    else if(j == cellXMax && cellXMin < j)
                    {
                        if(FindCell(&pCell, j, cellYMax+offset))
                        {
                            //bTransitionPossible = false;
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                            {
                                bTransitionPossible = false;

                                if(pbStickyCeiling)
                                    *pbStickyCeiling = pCell->pFirst->bStickyCeiling || pCell->pSecond->bStickyCeiling;
                            }
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_BOTRIGHT || 
                                    layout == CELL_LAYOUT_BOTLEFT || layout == CELL_LAYOUT_TOPLEFT)
                                {
                                    bTransitionPossible = false;

                                    if(pbStickyCeiling)
                                    {
                                        if(pCell->pFirst && pCell->pFirst->bCollision)
                                            *pbStickyCeiling = pCell->pFirst->bStickyCeiling;
                                        else if(pCell->pSecond && pCell->pSecond->bCollision)
                                            *pbStickyCeiling = pCell->pSecond->bStickyCeiling;
                                    }
                                }
                            }
                        }
                    }
                    else if (cellXMin < j && j < cellXMax)
                    {
                        if(FindCell(&pCell, j, cellYMax+offset) && ((pCell->pFirst && pCell->pFirst->bCollision) || (pCell->pSecond && pCell->pSecond->bCollision)))
                        {
                            bTransitionPossible = false;

                            if(pbStickyCeiling)
                            {
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    *pbStickyCeiling = pCell->pFirst->bStickyCeiling;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    *pbStickyCeiling = pCell->pSecond->bStickyCeiling;
                            }
                            
                        }
                    }
                    else // if(cellXMin == cellXMax)
                    {
                        if(FindCell(&pCell, j, cellYMax+offset))
                        {
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                            {
                                bTransitionPossible = false;

                                if(pbStickyCeiling)
                                    *pbStickyCeiling = pCell->pFirst->bStickyCeiling || pCell->pSecond->bStickyCeiling;
                            }
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_BOTLEFT || layout == CELL_LAYOUT_BOTRIGHT)
                                {
                                    bTransitionPossible = false;

                                    if(pbStickyCeiling)
                                    {
                                        if(pCell->pFirst && pCell->pFirst->bCollision)
                                            *pbStickyCeiling = pCell->pFirst->bStickyCeiling;
                                        else if(pCell->pSecond && pCell->pSecond->bCollision)
                                            *pbStickyCeiling = pCell->pSecond->bStickyCeiling;
                                    }
                                }
                            }
                        }
                    }

                    if(!bTransitionPossible)
                    {
                        //c.GetCellsHit().push_back(j * m_grid.GetNumCellsY() + cellYMax+offset);
                    }
                }
            }
            else // if(dirY < 0)
            {
                for(int j = cellXMin; j <= cellXMax; ++j)
                {
                    if(j == cellXMin && j < cellXMax)
                    {
                        if(FindCell(&pCell, j, cellYMin-offset))
                        {
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                            {
                                bTransitionPossible = false;

                                if(pbStickyCeiling)
                                    *pbStickyCeiling = pCell->pFirst->bStickyCeiling || pCell->pSecond->bStickyCeiling;
                            }
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_TOPRIGHT || 
                                    layout == CELL_LAYOUT_TOPLEFT || layout == CELL_LAYOUT_BOTRIGHT)
                                {
                                    bTransitionPossible = false;

                                    if(pbStickyCeiling)
                                    {
                                        if(pCell->pFirst && pCell->pFirst->bCollision)
                                            *pbStickyCeiling = pCell->pFirst->bStickyCeiling;
                                        else if(pCell->pSecond && pCell->pSecond->bCollision)
                                            *pbStickyCeiling = pCell->pSecond->bStickyCeiling;
                                    }
                                }
                            }
                        }
                    }
                    else if(j == cellXMax && cellXMin < j)
                    {
                        if(FindCell(&pCell, j, cellYMin-offset))
                        {
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                            {
                                bTransitionPossible = false;

                                if(pbStickyCeiling)
                                    *pbStickyCeiling = pCell->pFirst->bStickyCeiling || pCell->pSecond->bStickyCeiling;
                            }
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_TOPRIGHT || 
                                    layout == CELL_LAYOUT_TOPLEFT || layout == CELL_LAYOUT_BOTLEFT)
                                {
                                    bTransitionPossible = false;

                                    if(pbStickyCeiling)
                                    {
                                        if(pCell->pFirst && pCell->pFirst->bCollision)
                                            *pbStickyCeiling = pCell->pFirst->bStickyCeiling;
                                        else if(pCell->pSecond && pCell->pSecond->bCollision)
                                            *pbStickyCeiling = pCell->pSecond->bStickyCeiling;
                                    }
                                }
                            }
                        }
                    }
                    else if (cellXMin < j && j < cellXMax)
                    {
                        if(FindCell(&pCell, j, cellYMin-offset) && ((pCell->pFirst && pCell->pFirst->bCollision) || (pCell->pSecond && pCell->pSecond->bCollision)))
                        {
                            bTransitionPossible = false;

                            if(pbStickyCeiling)
                            {
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    *pbStickyCeiling = pCell->pFirst->bStickyCeiling;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    *pbStickyCeiling = pCell->pSecond->bStickyCeiling;
                            }
                        }
                    }
                    else // if(cellXMin == cellXMax)
                    {
                        if(FindCell(&pCell, j, cellYMin-offset))
                        {
                            //bTransitionPossible = false;
                            if((pCell->pFirst && pCell->pFirst->bCollision) && (pCell->pSecond && pCell->pSecond->bCollision))
                            {
                                bTransitionPossible = false;
                            
                                if(pbStickyCeiling)
                                    *pbStickyCeiling = pCell->pFirst->bStickyCeiling || pCell->pSecond->bStickyCeiling;
                            }
                            else
                            {
                                CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                                if(pCell->pFirst && pCell->pFirst->bCollision)
                                    layout = pCell->pFirst->cellLayout;
                                else if(pCell->pSecond && pCell->pSecond->bCollision)
                                    layout = pCell->pSecond->cellLayout;

                                if(layout == CELL_LAYOUT_FULL || layout == CELL_LAYOUT_TOPRIGHT || layout == CELL_LAYOUT_TOPLEFT)
                                {
                                    bTransitionPossible = false;

                                    if(pbStickyCeiling)
                                    {
                                        if(pCell->pFirst && pCell->pFirst->bCollision)
                                            *pbStickyCeiling = pCell->pFirst->bStickyCeiling;
                                        else if(pCell->pSecond && pCell->pSecond->bCollision)
                                            *pbStickyCeiling = pCell->pSecond->bStickyCeiling;
                                    }
                                }
                            }
                        }
                    }

                    if(!bTransitionPossible)
                    {
                        //c.GetCellsHit().push_back(j * m_grid.GetNumCellsY() + cellYMin-offset);
                    }
                }
            }
        }

        return bTransitionPossible;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainMgr::ComputeCellDistance(int xORy, int cellXORY, bool bMin)
    {
        if(bMin)
            return (xORy - cellXORY * CELL_SIZE_SCALED);
        else
            return ((cellXORY + 1) * CELL_SIZE_SCALED - xORy);
    }

    //-----------------------------------------------------------------------------------
    CollisionState CTerrainMgr::ComputeMovement(const CTerrainCollidable& collidable)
    {
        float sizeX = m_levelGrid.GetSizeX() * CELL_SCALE;
        float sizeY = m_levelGrid.GetSizeY() * CELL_SCALE;

        Vec2 vPosFloating(collidable.position.x, collidable.position.y);
        VECTOR2FIXED vPosFixed = collidable.position;
        Vec2 vVelocity(collidable.velocity.x, collidable.velocity.y);

        Vec2 vMapSize(sizeX, sizeY);
        Vec2 vMapOrigin(0.0f, 0.0f);

        Vec2 vVelocityOriginal(collidable.velocity.x, collidable.velocity.y);


        CTerrainCollider c(vPosFixed, 
            static_cast<int>(collidable.width * CELL_SCALE), 
            static_cast<int>(collidable.height * CELL_SCALE),
            VECTOR2FIXED(vVelocity.x, vVelocity.y), 
            VECTOR2FIXED(vVelocityOriginal.x, vVelocityOriginal.y), 
            false, false, false, false, false);

        CollisionState colRes;
// 		if(c.GetNewPosition().y <= (vMapOrigin.y + c.GetHalfHeight()))
// 		{
// 			c.SetPositionY(vMapOrigin.y + c.GetHalfHeight());
// 			colRes.SetOnFloor(true);
// 			c.SetMoveY(0);
// 		}
// 		else if(c.GetNewPosition().y >= (vMapSize.y - c.GetHalfHeight()))
// 		{
// 			c.SetPositionY(vMapSize.y - c.GetHalfHeight());
// 			colRes.SetHitCeiling(true);
// 			c.SetMoveY(0);
// 		}
// 
// 		if(c.GetNewPosition().x <= (vMapOrigin.x + c.GetHalfWidth()))
// 		{
// 			c.SetPositionX(vMapOrigin.x + c.GetHalfWidth());
// 			c.SetMoveX(0);
// 		}
// 		else if(c.GetNewPosition().x >= (vMapSize.x - c.GetHalfWidth()))
// 		{
// 			c.SetPositionX(vMapSize.x - c.GetHalfWidth());
// 			c.SetMoveX(0);
// 		}

        int eps = 0;
        const ColliderBBox& bbox = c.GetBBox();
        VECTOR2FIXED vMove(c.GetMove().x, c.GetMove().y);

        bool bHorizontalMove = true,
            bVerticalMove = true;

        int j = 0;
        while(bHorizontalMove && bVerticalMove && j < 3)
        {
            ++j;
            bHorizontalMove = abs(vMove.x) > eps;
            bVerticalMove = abs(vMove.y) > eps;

            if(c.GetDirX() <= 0)
            {
                int i = 0;
                while(abs(vMove.x) > eps && i < 3)
                {
                    ++i;
                    VECTOR2FIXED v1(0, 0), v2(0, 0), v3(0, 0), v4(0, 0);
                    c.GetCorners(v1, v2, v3, v4);

                    // 1. Check if a x transition has to be made and whether this is possible
                    //
                    int diffX  = c.GetCellDistanceX(v2.x, true);
                    int diffY = c.GetCellDistanceY(v1.y, false);
                    int cellYMax = bbox.cellYMax;

                    if(diffY == CELL_SIZE_SCALED)
                        cellYMax -= 1;

                    if(diffX == 0)
                    {
                        if(!CheckTransition(c, c.GetBaseDirX(), c.GetBaseDirY(), true, false, bbox.cellXMin - 1, bbox.cellXMax, bbox.cellYMin, cellYMax))
                        {
                            //c.SetHitWallFromRight(true);
                            bHorizontalMove = false;

                            break;
                        }
                        else
                        {
                            c.DoXTransition(true);
                            diffX = c.GetCellDistanceX(v2.x, true);
                        }
                    }

                    // 2. No transition needs to be made => Move player for diffX units to the left.
                    // As we are inside a cell: Check additionally if the player hits a ramp
                    //
                    int moveLeft = std::min(abs(vMove.x), diffX);

                    c.SetPositionX(c.GetPosition().x - moveLeft);
                    c.GetCorners(v1, v2, v3, v4);

                    float distanceToRamp = 0.0f;
                    if(CheckRamps(c, bbox.cellXMin, bbox.cellXMax, bbox.cellYMin, cellYMax, CELL_LAYOUT_BOTLEFT, distanceToRamp) && 
                        distanceToRamp <= 0.0f)
                    {
                        // Revert the movement, because a ramp was hit.
                        //
                        vMove.x = static_cast<SCALARFIXED>(std::min(vMove.x + (moveLeft + distanceToRamp), 0.0f));
                        c.SetPositionX(static_cast<SCALARFIXED>(c.GetPosition().x - distanceToRamp));
                        //c.SetOnRamp(true, CELL_LAYOUT_BOTLEFT);
                        bHorizontalMove = true;
                        break;
                    }
                    else if(CheckRamps(c, bbox.cellXMin, bbox.cellXMax, bbox.cellYMin, cellYMax, 
                        CELL_LAYOUT_TOPLEFT, distanceToRamp) && distanceToRamp <= 0.0f)
                    {
                        vMove.x = static_cast<SCALARFIXED>(std::min(vMove.x + (moveLeft + distanceToRamp), 0.0f));
                        c.SetPositionX(c.GetPosition().x - distanceToRamp);
                        bHorizontalMove = false;
                        break;
                    }
                    else
                    {
                        vMove.x += moveLeft;
                        bHorizontalMove = true;
                    }
                }
            }
            else // if(dirX > 0)
            {
                int i = 0;
                while(abs(vMove.x) > eps && i < 3)
                {
                    ++i;
                    VECTOR2FIXED v1(0, 0), v2(0, 0), v3(0, 0), v4(0, 0);
                    c.GetCorners(v1, v2, v3, v4);

                    // 1. Check if a x transition has to be made and whether this is possible
                    //
                    int diffX = c.GetCellDistanceX(v4.x, false);
                    int diffY = c.GetCellDistanceY(v1.y, false);
                    int cellYMax = bbox.cellYMax;
                    if(diffY == CELL_SIZE_SCALED)
                        cellYMax -= 1;

                    if(diffX == CELL_SIZE_SCALED)
                    {


                        if(!CheckTransition(c, c.GetBaseDirX(), c.GetBaseDirY(), true, false, bbox.cellXMin, bbox.cellXMax, bbox.cellYMin, cellYMax))
                        {
                            //c.SetHitWallFromLeft(true);
                            bHorizontalMove = false;
                            break;
                        }
                        else
                        {
                            diffX = c.GetCellDistanceX(v4.x, false);
                        }
                    }

                    // 2. No transitions needs to be made => Move player for diffX units to the left.
                    // As we are inside a cell: Check additionally if the player hits a ramp
                    //
                    int moveRight = std::min(abs(vMove.x), diffX);

                    int cellXMax = bbox.cellXMax;
                    c.SetPositionX(c.GetPosition().x + moveRight);
                    c.GetCorners(v1, v2, v3, v4);

                    float distanceToRamp = 0.0f;
                    if(CheckRamps(c, bbox.cellXMin, cellXMax, bbox.cellYMin, cellYMax, 
                        CELL_LAYOUT_BOTRIGHT, distanceToRamp) && distanceToRamp <= 0.0f)
                    {
                        // Revert the movement, because a ramp was hit.
                        //
                        vMove.x = static_cast<SCALARFIXED>(std::max(vMove.x - (moveRight + distanceToRamp), 0.0f));
                        c.SetPositionX(c.GetPosition().x + distanceToRamp);
                        //c.SetOnRamp(true, CELL_LAYOUT_BOTRIGHT);
                        bHorizontalMove = true;
                        break;
                    }
                    else if(CheckRamps(c, bbox.cellXMin, cellXMax, bbox.cellYMin, cellYMax, 
                        CELL_LAYOUT_TOPRIGHT, distanceToRamp) && distanceToRamp <= 0.0f)
                    {
                        //vMove.x = 0.0;
                        vMove.x = static_cast<SCALARFIXED>(std::max(vMove.x - (moveRight + distanceToRamp), 0.0f));
                        c.SetPositionX(c.GetPosition().x + distanceToRamp);
                        bHorizontalMove = false;
                        break;
                    }
                    else
                    {
                        vMove.x -= moveRight;
                        bHorizontalMove = true;
                    }
                }
            }


            if(c.GetDirY() >= 0)
            {
                int i = 0;
                while(abs(vMove.y) > eps && i < 3)
                {
                    ++i;
                    VECTOR2FIXED v1(0, 0), v2(0, 0), v3(0, 0), v4(0, 0);
                    c.GetCorners(v1, v2, v3, v4);

                    // 1. Check if a y transition has to be made and whether this is possible
                    //
                    int diffY = c.GetCellDistanceY(v1.y, false);
                    int cellXMax = bbox.cellXMax;
                    int cellXMin = bbox.cellXMin;
                    if(diffY == CELL_SIZE_SCALED)
                    {
                        // If the bounding box hits the right cell border, this causes
                        // bbox.cellXMax to be increased, which is incorrect for up/down movement.
                        //
                        int diffXMax = c.GetCellDistanceX(v3.x, false);
                        if(diffXMax == CELL_SIZE_SCALED/* && c.GetDirX() < 0*/)
                            cellXMax -= 1;

                        if(!CheckTransition(c, c.GetBaseDirX(), c.GetBaseDirY(), false, true, cellXMin, cellXMax, bbox.cellYMin, bbox.cellYMax))
                        {
                            //c.SetHitCeiling(true);
                            bVerticalMove = false;
                            break;
                        }
                        else
                        {
                            diffY = c.GetCellDistanceY(v1.y, false);
                        }
                    }

                    // 2. No transitions needs to be made => Move player for diffY units to the top.
                    // As we are inside a cell: Check additionally if the player hits a ramp
                    //
                    int moveUp = std::min(abs(vMove.y), diffY);
                    //if(moveUp == 0.0)
                    //	break;

                    int yMax = bbox.cellYMax;
                    c.SetPositionY(c.GetPosition().y + moveUp);
                    c.GetCorners(v1, v2, v3, v4);

                    bool bMovePossibleTopLeft  = true, 
                        bMovePossibleTopRight = true;
                    float tmpDistanceToRampTopRight = 0.0f, 
                        tmpDistanceToRampTopLeft = 0.0f;

                    if(CheckRamps(c, bbox.cellXMin, cellXMax, bbox.cellYMin, yMax,
                            CELL_LAYOUT_TOPLEFT, tmpDistanceToRampTopLeft) && tmpDistanceToRampTopLeft <= 0.0f)
                    {
                        bMovePossibleTopLeft = false;
                        //c.SetHitRamp(true, CELL_LAYOUT_TOPLEFT);
                        
                    }

                    if(CheckRamps(c, bbox.cellXMin, cellXMax, bbox.cellYMin, yMax,
                            CELL_LAYOUT_TOPRIGHT, tmpDistanceToRampTopRight) && tmpDistanceToRampTopRight <= 0.0f)
                    {
                        bMovePossibleTopRight = false;
                        //c.SetHitRamp(true, CELL_LAYOUT_TOPRIGHT);

                    }


                    if((!bMovePossibleTopRight || !bMovePossibleTopLeft) && c.GetDirX() != 0)
                    {
                        // Revert the movement, because a ramp was hit.
                        // Mark, that a ramp was hit, because there is velocity in x direction,
                        // which will cause a slide along the corresponding ramp.
                        //
                        float revertMovement = std::min(tmpDistanceToRampTopLeft, tmpDistanceToRampTopRight);
                        moveUp = static_cast<SCALARFIXED>(std::max(moveUp + revertMovement, 0.0f));
                        vMove.y -= moveUp;
                        c.SetPositionY(c.GetPosition().y + revertMovement);
                        bVerticalMove = false;
                        break;
                    }
                    else if(!bMovePossibleTopLeft || !bMovePossibleTopRight)
                    {
                        // Revert the movement, because a ramp was hit.
                        // Mark, that the ceiling was hit, because there is no velocity in x direction,
                        // which will cause the velocity in y direction to be set to 0.
                        //
                        //vMove.y -= moveUp;
                        //c.SetPositionY(c.GetPosition().y - moveUp);
                        float revertMovement = std::min(tmpDistanceToRampTopLeft, tmpDistanceToRampTopRight);
                        vMove.y = static_cast<SCALARFIXED>(std::max(vMove.y - (moveUp + revertMovement), 0.0f));
                        c.SetPositionY(c.GetPosition().y + revertMovement);
                        //c.SetHitCeiling(true);
                        bVerticalMove = false;
                        break;
                    }
                    else
                    {
                        vMove.y -= moveUp;
                        bVerticalMove = true;
                    }
                }
            }
            else // if(dirY < 0)
            {
                int i = 0;
                while(abs(vMove.y) > eps && i < 3)
                {
                    ++i;
                    VECTOR2FIXED v1(0, 0), v2(0, 0), v3(0, 0), v4(0, 0);
                    c.GetCorners(v1, v2, v3, v4);

                    // 1. Check if a y transition has to be made and whether this is possible
                    //
                    int diffY = c.GetCellDistanceY(v2.y, true);
                    int cellXMax = bbox.cellXMax;
                    if(diffY == 0)
                    {
                        int diffX = c.GetCellDistanceX(v3.x, false);
                        if(diffX == CELL_SIZE_SCALED)
                            cellXMax -= 1;

                        if(!CheckTransition(c, c.GetBaseDirX(), c.GetBaseDirY(), false, true, bbox.cellXMin, cellXMax, bbox.cellYMin - 1, bbox.cellYMax))
                        {
                            //c.SetOnFloor(true);
                            bVerticalMove = false;
                            break;
                        }
                        else
                        {
                            c.DoYTransition(true);
                            diffY = c.GetCellDistanceY(v2.y, true);
                        }
                    }

                    // 2. No transitions needs to be made => Move player for diffY units to the bottom.
                    // As we are inside a cell: Check additionally if the player hits a ramp
                    //
                    int moveDown = std::min(abs(vMove.y), diffY);

                    c.SetPositionY(c.GetPosition().y - moveDown);
                    c.GetCorners(v1, v2, v3, v4);

                    bool bMovePossible = true;
                    float tmpDistanceToRampBotLeft = 0.0f,
                        tmpDistanceToRampBotRight = 0.0f;
                    if(CheckRamps(c, bbox.cellXMin, cellXMax, bbox.cellYMin, bbox.cellYMax,
                            CELL_LAYOUT_BOTLEFT, tmpDistanceToRampBotLeft) && tmpDistanceToRampBotLeft <= 0.0f)
                        bMovePossible = false;

                    if(CheckRamps(c, bbox.cellXMin, cellXMax, bbox.cellYMin, bbox.cellYMax,
                            CELL_LAYOUT_BOTRIGHT, tmpDistanceToRampBotRight) && tmpDistanceToRampBotRight <= 0.0f)
                        bMovePossible = false;


                    if(!bMovePossible)
                    {
                        // Revert movement, because a ramp was hit.
                        // Mark, that the player hits the floor and a ramp.
                        //
                        float revertMovement = std::min(tmpDistanceToRampBotLeft, tmpDistanceToRampBotRight);
                        vMove.y = static_cast<SCALARFIXED>(std::min(vMove.y + (moveDown + revertMovement), 0.0f));
                        c.SetPositionY(c.GetPosition().y - revertMovement);
// 						c.SetOnFloor(true);
// 						if(tmpDistanceToRampBotLeft < tmpDistanceToRampBotRight && tmpDistanceToRampBotRight <= 0.0f)
// 							c.SetOnRamp(true, CELL_LAYOUT_BOTLEFT);
// 						else if(tmpDistanceToRampBotRight <= 0.0)
// 							c.SetOnRamp(true, CELL_LAYOUT_BOTRIGHT);
                        
                        bVerticalMove = false;
                        break;
                    }
                    else
                    {
                        vMove.y += moveDown;
                        bVerticalMove = true;
                    }
                }
            }
        }
        
        colRes.pos = c.GetPosition();

        if(c.GetPosition().y <= (vMapOrigin.y + c.GetHalfHeight()))
        {
            c.SetPositionY(vMapOrigin.y + c.GetHalfHeight());
            colRes.SetOnFloor(true);
            c.SetMoveY(0);
        }
        else if(c.GetPosition().y >= (vMapSize.y - c.GetHalfHeight()))
        {
            c.SetPositionY(vMapSize.y - c.GetHalfHeight());
            colRes.SetHitCeiling(true);
            c.SetMoveY(0);
        }

        if(c.GetPosition().x <= (vMapOrigin.x + c.GetHalfWidth()))
        {
            c.SetPositionX(vMapOrigin.x + c.GetHalfWidth());
            c.SetMoveX(0);
        }
        else if(c.GetPosition().x >= (vMapSize.x - c.GetHalfWidth()))
        {
            c.SetPositionX(vMapSize.x - c.GetHalfWidth());
            c.SetMoveX(0);
        }


        UpdateFlags(c, colRes, Vec2(0.0f, 0.0f));

        
// 		col.pos = c.GetPosition();
// 		col.cellsHit = c.GetCellsHit();
// 		col.bOnFloor = c.GetOnFloor();
// 		col.bHitCeiling = c.GetHitCeiling();
// 		col.bStickyCeiling = c.GetStickyCeiling();
// 		col.bOnRamp = c.GetOnRamp();
// 		col.bHitRamp = c.GetHitRamp();
// 		col.onRampLayout = c.GetRampLayout();
// 		col.hitRampLayout = c.GetHitRampLayout();
// 		col.bHitWallFromLeft = c.GetHitWallFromLeft();
// 		col.bHitWallFromRight = c.GetHitWallFromRight();
        
        return colRes;

    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgr::UpdateFlags(CTerrainCollider& c, CollisionState& colRes, const Vec2& vPos)
    {
        VECTOR2FIXED v1, v2, v3, v4;
        c.GetCorners(v1, v2, v3, v4);

        const ColliderBBox& bbox = c.GetBBox();
        int cellXMax = bbox.cellXMax;
        int distanceRightCellBorder = c.GetCellDistanceX(v4.x, false);
        int distanceLeftCellBorder = c.GetCellDistanceX(v2.x, true);
        int distanceBottomCellBorder = c.GetCellDistanceY(v2.y, true);
        int distanceTopCellBorder = c.GetCellDistanceY(v3.y, false);

        int xMin = bbox.cellXMin, 
            xMax = bbox.cellXMax, 
            yMin = bbox.cellYMin, 
            yMax = bbox.cellYMax;

        bool bMoveBotLeftPossible = true;
        bool bMoveBotRightPossible = true;
        float tmpDistanceToRampBotLeft = 0.0f,
            tmpDistanceToRampBotRight = 0.0f;

        int xMinTmp = xMin;
        if(distanceLeftCellBorder == 0)
            xMinTmp--;
        if(CheckRamps(c, xMinTmp, xMax, yMin, yMax, CELL_LAYOUT_BOTLEFT, tmpDistanceToRampBotLeft) && tmpDistanceToRampBotLeft <= 0.0f)
            bMoveBotLeftPossible = false;

        if(CheckRamps(c, xMin, xMax, yMin, yMax, CELL_LAYOUT_BOTRIGHT, tmpDistanceToRampBotRight) && tmpDistanceToRampBotRight <= 0.0f)
            bMoveBotRightPossible = false;

        if(!bMoveBotRightPossible && !bMoveBotLeftPossible && tmpDistanceToRampBotLeft < tmpDistanceToRampBotRight)
            colRes.SetOnRamp(true, CELL_LAYOUT_BOTLEFT);
        else if(!bMoveBotRightPossible && !bMoveBotLeftPossible)
            colRes.SetOnRamp(true, CELL_LAYOUT_BOTRIGHT);
        else if(!bMoveBotRightPossible)
            colRes.SetOnRamp(true, CELL_LAYOUT_BOTRIGHT);
        else if(!bMoveBotLeftPossible)
            colRes.SetOnRamp(true, CELL_LAYOUT_BOTLEFT);


        bool bMovePossibleTopLeft  = true, 
            bMovePossibleTopRight = true;
        float tmpDistanceToRampTopRight = 0.0f, 
            tmpDistanceToRampTopLeft = 0.0f;

        int yMaxTmp = yMax;
        xMinTmp = xMin;
        if(distanceTopCellBorder == CELL_SIZE_SCALED)
            yMaxTmp--;
        if(distanceLeftCellBorder == 0)
            xMinTmp--;

        if(CheckRamps(c, xMinTmp, xMax, yMin, yMaxTmp, CELL_LAYOUT_TOPLEFT, tmpDistanceToRampTopLeft) && tmpDistanceToRampTopLeft <= 0.0f)
            bMovePossibleTopLeft = false;

        if(distanceRightCellBorder == CELL_SIZE_SCALED)
            cellXMax -= 1;

        if(CheckRamps(c, bbox.cellXMin, cellXMax, bbox.cellYMin, yMaxTmp, CELL_LAYOUT_TOPRIGHT, tmpDistanceToRampTopRight) && tmpDistanceToRampTopRight <= 0.0f)
            bMovePossibleTopRight = false;

        if(!bMovePossibleTopLeft && !bMovePossibleTopRight && tmpDistanceToRampTopLeft < tmpDistanceToRampTopRight)
            colRes.SetHitRamp(true, CELL_LAYOUT_TOPLEFT);
        else if(!bMovePossibleTopRight && !bMovePossibleTopLeft)
            colRes.SetHitRamp(true, CELL_LAYOUT_TOPRIGHT);
        else if(!bMovePossibleTopRight)
            colRes.SetHitRamp(true, CELL_LAYOUT_TOPRIGHT);
        else if(!bMovePossibleTopLeft)
            colRes.SetHitRamp(true, CELL_LAYOUT_TOPLEFT);

        if(distanceBottomCellBorder == 0)
        {
            if(distanceRightCellBorder == CELL_SIZE_SCALED)
            {
                if(!CheckTransition(c, 0, -1, false, true, xMin, xMax - 1, yMin - 1, yMax))
                {
                    colRes.SetOnFloor(true);
                }
            }
            else
            {
                if(!CheckTransition(c, 0, -1, false, true, xMin, xMax, yMin - 1, yMax))
                {
                    colRes.SetOnFloor(true);
                }
            }
        }

        if(distanceTopCellBorder == CELL_SIZE_SCALED)
        {
            if(distanceRightCellBorder == CELL_SIZE_SCALED)
            {
                bool bSticky = false;
                if(!CheckTransition(c, 0, 1, false, true, xMin, xMax - 1, yMin, yMax, &bSticky))
                {
                    colRes.SetHitCeiling(true);
                    if(bSticky)
                        colRes.SetStickyCeiling(true);
                }
            }
            else
            {
                bool bSticky = false;
                if(!CheckTransition(c, 0, 1, false, true, xMin, xMax, yMin, yMax, &bSticky))
                {
                    colRes.SetHitCeiling(true);

                    if(bSticky)
                        colRes.SetStickyCeiling(true);
                }
            }
                    
        }

        colRes.SetGrabbedWallFromLeft(false);
        colRes.SetGrabbedWallFromRight(false);
        colRes.SetHitWallFromLeft(false);
        colRes.SetHitWallFromRight(false);

        if(distanceRightCellBorder == CELL_SIZE_SCALED)
        {
            TerrainCell* pCell = NULL;
            bool bHitFromLeft = true;
            for(int i = yMin; i <= yMax; ++i)
            {
                if(FindCell(&pCell, xMax, i))
                {
                    if(pCell->pFirst && pCell->pFirst->bCollision && pCell->pSecond && pCell->pSecond->bCollision)
                    {
                        //c.SetHitWallFromLeft(true);
                        //c.cellsHit.push_back(xMax * m_grid.GetNumCellsY() + i);

                        //break;

                        colRes.SetHitWallFromLeft(true);
                        colRes.SetGrabbedWallFromLeft(true);
                    }
                    else
                    {
                        CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                        if(pCell->pFirst && pCell->pFirst->bCollision)
                            layout = pCell->pFirst->cellLayout;
                        else if(pCell->pSecond && pCell->pSecond->bCollision)
                            layout = pCell->pSecond->cellLayout;

                        if(layout != CELL_LAYOUT_BOTRIGHT && layout != CELL_LAYOUT_TOPRIGHT && layout != CELL_LAYOUT_UNDEFINED)
                        {
                            //c.SetHitWallFromLeft(true);
                            //c.cellsHit.push_back(xMax * m_grid.GetNumCellsY() + i);

                            //break;
                            colRes.SetHitWallFromLeft(true);
                            colRes.SetGrabbedWallFromLeft(true);
                        }
                        else
                            bHitFromLeft = false;
                    }
                }
                else
                    bHitFromLeft = false;
            }
            if(bHitFromLeft)
                colRes.SetGrabbedWallFromLeft(true);
        }

                
        if(distanceLeftCellBorder /*< 10*/ == 0)
        {
            bool bHitWallFromRight = true;
            TerrainCell* pCell = NULL;
            for(int i = yMin; i <= yMax; ++i)
            {
                if(FindCell(&pCell, xMin - 1, i))
                {
                    if(pCell->pFirst && pCell->pFirst->bCollision && pCell->pSecond && pCell->pSecond->bCollision)
                    {
// 								c.SetHitWallFromRight(true);
// 								c.cellsHit.push_back((xMin-1) * m_grid.GetNumCellsY() + i);
// 
// 								break;
                        colRes.SetHitWallFromRight(true);
                        colRes.SetGrabbedWallFromRight(true);
                    }
                    else
                    {
                        CELL_LAYOUT layout = CELL_LAYOUT_UNDEFINED;
                        if(pCell->pFirst && pCell->pFirst->bCollision)
                            layout = pCell->pFirst->cellLayout;
                        else if(pCell->pSecond && pCell->pSecond->bCollision)
                            layout = pCell->pSecond->cellLayout;

                        if(layout != CELL_LAYOUT_BOTLEFT && layout != CELL_LAYOUT_TOPLEFT && layout != CELL_LAYOUT_UNDEFINED)
                        {
// 									c.SetHitWallFromRight(true);
// 									c.cellsHit.push_back((xMin-1) * m_grid.GetNumCellsY() + i);
// 
// 									break;
                            colRes.SetHitWallFromRight(true);
                            colRes.SetGrabbedWallFromRight(true);
                        }
                        else
                            bHitWallFromRight = false;
                    }
                }
                else
                    bHitWallFromRight = false;
            }
            if(bHitWallFromRight)
                colRes.SetGrabbedWallFromRight(true);
        }
    }

    /*bool CLevel::ComputeIntersectedGrid(const VECTOR3& vRayOrigin, const VECTOR3& vRayDirection, 
        unsigned int& gridX, unsigned int& gridY)
    {
        bool b = ComputeIntersectedGrid(vRayOrigin, vRayDirection, 
            VECTOR3(0, 0, 0), VECTOR3(0, 1, 0), VECTOR3(1, 0, 0), gridX, gridY);

        
        return b;

    }*/

    //-----------------------------------------------------------------------------------
    bool CTerrainMgr::ComputeIntersectedGrid(const Vec3& vRayOrigin, const Vec3& vRayDirection, 
        unsigned int& gridX, unsigned int& gridY)
    {
        int layer = m_levelGrid.GetCellEditParameters().currentEditLayer;

        const std::vector< Matrix4x4 >& transformations = m_levelGrid.GetLevelRenderObject()->GetLayerTransformations();

        Matrix4x4 mtxRot = transformations[layer];

        float layerDepth = m_levelGrid.GetLayerDepth(layer);
        float intersectionDistance = 0.0f; 

        Vec3 p1 = Vec3(Vec4(0.0f, 0.0f, layerDepth, 1.0f) * mtxRot);
        Vec3 p2 = Vec3(Vec4(1.0f, 0.0f, layerDepth, 1.0f) * mtxRot);
        Vec3 p3 = Vec3(Vec4(0.0f, 1.0f, layerDepth, 1.0f) * mtxRot);
        Vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));

        ComputeIntersectionRayPlane(vRayOrigin, vRayDirection, p1, -normal, intersectionDistance);

        Vec3 intersection = vRayOrigin + vRayDirection * intersectionDistance;

        if (intersection.x < 0.0 || intersection.y < 0.0 ||
            intersection.x > m_levelGrid.GetSizeX() || intersection.y > m_levelGrid.GetSizeY())
        {
            gridX = 0;
            gridY = 0;

            m_mouseHoverCellX = gridX;
            m_mouseHoverCellY = gridY;

            return false;
        }
        else
        {
            m_mouseIntersection = Vec2(intersection.x, intersection.y);

            gridX = (int)intersection.x / (int)CELL_SIZE;
            gridY = (int)intersection.y / (int)CELL_SIZE;

            m_mouseHoverCellX = gridX;
            m_mouseHoverCellY = gridY;

            return true;
        }
    }

    //-----------------------------------------------------------------------------------
    const Vec2& CTerrainMgr::GetMouseIntersection()
    {
        return m_mouseIntersection;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgr::AddToRenderQueue(RenderQueue& renderQueue)
    {
        m_levelGrid.GetLevelRenderObject()->AddToRenderQueue(renderQueue);
    }


    
// 	CTerrainMaterial CLevel::GetTerrainMaterial(const std::string& strName)
// 	{
// 		return m_grid.GetMaterial();
// 	}
// 
// 	void CLevel::LoadTerrainMaterials(listTerrainMaterials& terrainMaterials,const std::string& strScript)
// 	{
// 		CSingletonScriptMgr scriptMgr;
// 		CScript* pScript = scriptMgr->AddGetPtr("RegisteredResources.txt");
// 		CScript* pRegisteredTexture = pScript->GetSubscript("registeredTerrainTextures");
// 
// 		terrainMaterials.clear();
// 
// 		if(!pRegisteredTexture)
// 			return;
// 
// 		
// 		CScript::mapSubscripts& materials = pRegisteredTexture->GetSubscripts();
// 		for(CScript::mapSubscripts::iterator it = materials.begin(); it != materials.end(); ++it)
// 		{
// 			CTerrainMaterial terrainMaterial;
// 			terrainMaterial.SetName(it->first);
// 			LoadTerrainMaterial(it->second, terrainMaterial);
// 			terrainMaterials.push_back(terrainMaterial);
// 		}
// 
// 		scriptMgr->Release("RegisteredResources.txt");
// 	}
// 
// 	void CLevel::LoadTerrainMaterial(CScript* pMaterial, CTerrainMaterial& material)
// 	{
// 		if(!pMaterial)
// 			return;
// 
// 		material.SetMaterial((*pMaterial)["material"]);
// 
// 		for(int i = 0; i < pMaterial->GetSubscripts().size(); ++i)
// 		{
// 			std::stringstream str;
// 			str << "pattern" << i;
// 
// 			if(pMaterial->SubscriptExists(str.str()))
// 			{
// 				CScript* pPatternMaterial = pMaterial->GetSubscript(str.str());
// 				if(pPatternMaterial->Exists("diffuse"))
// 				{
// 					VECTOR3 diffuse = (*pPatternMaterial)["diffuse"];
// 					material.SetMaterialDiffuse(i, diffuse);
// 				}
// 				if(pPatternMaterial->Exists("specular"))
// 				{
// 					VECTOR4 specular = (*pPatternMaterial)["specularPower"];
// 					material.SetMaterialSpecular(i, specular);
// 				}
// 			}
// 		}
// 	}
// 
// 	void CLevel::SetMaterial(const std::string& strMaterial)
// 	{
// 		CSingletonScriptMgr scriptMgr;
// 		CScript* pResources = scriptMgr->AddGetPtr(REGISTERED_RESOURCES);
// 		CScript* pTextures = pResources->GetSubscript("registeredTerrainTextures");
// 		CScript* pMaterial = pTextures->GetSubscript(strMaterial);
// 
// 		CTerrainMaterial terrainMtrl;
// 		LoadTerrainMaterial(pMaterial, terrainMtrl);
// 		m_grid.SetMaterial(terrainMtrl);
// 
// 		scriptMgr->Release(pResources->GetFilename());
// 	}


    /*void CLevel::SetCellAlpha(unsigned int x, unsigned int y)
    {
        if(m_grid.materials.find(m_grid.editParameters.currentEditAnimation) == m_grid.materials.end())
            return;

        unsigned int gridIndex = x * m_grid.numCellsY + y;

        if(gridIndex > m_grid.cells.size())
            return;


        LevelCell& g = m_grid.cells[gridIndex];

        if((g.pFirst && g.pFirst->cellLayout == m_grid.editParameters.currentEditCellLayout))
        {
            g.pFirst->ResetAlpha();
            g.pFirst->alphaSequence = m_grid.editParameters.currentEditSequence;
            g.pFirst->bHasAlphaMap = true;
            SetCellAnimationPatternAlpha(x, y, g.pFirst->alphaSequence, 0, true);
        }
        else if(g.pSecond && g.pSecond->cellLayout == m_grid.editParameters.currentEditCellLayout)
        {

            g.pSecond->ResetAlpha();
            g.pSecond->alphaSequence = m_grid.editParameters.currentEditSequence;
            g.pSecond->bHasAlphaMap = true;
            SetCellAnimationPatternAlpha(x, y, g.pSecond->alphaSequence, 0, false);
        }
        else if(m_grid.editParameters.currentEditCellLayout == CELL_LAYOUT_FULL)
        {
            if(g.pFirst)
            {
                g.pFirst->ResetAlpha();
                g.pFirst->alphaSequence = m_grid.editParameters.currentEditSequence;
                g.pFirst->bHasAlphaMap = true;
                SetCellAnimationPatternAlpha(x, y, g.pFirst->alphaSequence, 0, true);
            }
            if(g.pSecond)
            {
                g.pSecond->ResetAlpha();
                g.pSecond->alphaSequence = m_grid.editParameters.currentEditSequence;
                g.pSecond->bHasAlphaMap = true;
                SetCellAnimationPatternAlpha(x, y, g.pSecond->alphaSequence, 0, false);
            }
        }
    }*/



    /*void CLevel::SetCellAnimationPatternAlpha(unsigned int x, unsigned int y, unsigned int sequence, unsigned int frame, bool bFirst)
    {
        unsigned int gridIndex = x * m_grid.numCellsY + y;
        LevelCell* pGrid = NULL;
        if(FindCell(&pGrid, x, y))
        {
            if(bFirst) 
            {
                CManagedMaterial* pAnimation = m_grid.materials[pGrid->pFirst->animation].first;
                if(pAnimation->GetNumSequences() <= sequence)
                    sequence = 0;

                VECTOR2 v1, v2, v3, v4, tmp;
                TextureRect rect;
                pAnimation->GetTextureCoordinates(sequence, frame, rect);
                v1.x = rect.left;
                v1.y = rect.top;
                v2.x = rect.right;
                v2.y = rect.bottom;

                v3.x = v1.x;
                v3.y = v2.y;
                v4.x = v2.x;
                v4.y = v1.y;

                GetRotatedTexCoords(v1, v2, v3, v4);

                pGrid->pFirst->vTexCoords03.x = v1.x;
                pGrid->pFirst->vTexCoords03.y = v1.y;
                pGrid->pFirst->vTexCoords03.z = v2.x;
                pGrid->pFirst->vTexCoords03.w = v2.y;
                pGrid->pFirst->vTexCoords04.x = v3.x;
                pGrid->pFirst->vTexCoords04.y = v3.y;
                pGrid->pFirst->vTexCoords04.z = v4.x;
                pGrid->pFirst->vTexCoords04.w = v4.y;
            }
            else
            {
                CManagedMaterial* pAnimation = m_grid.materials[pGrid->pSecond->animation].first;
                if(pAnimation->GetNumSequences() <= sequence)
                    sequence = 0;

                VECTOR2 v1, v2, v3, v4, tmp;
                TextureRect rect;
                pAnimation->GetTextureCoordinates(sequence, frame, rect);
                v1.x = rect.left;
                v1.y = rect.top;
                v2.x = rect.right;
                v2.y = rect.bottom;
                v3.x = v1.x;
                v3.y = v2.y;
                v4.x = v2.x;
                v4.y = v1.y;

                GetRotatedTexCoords(v1, v2, v3, v4);

                pGrid->pSecond->vTexCoords03.x = v1.x;
                pGrid->pSecond->vTexCoords03.y = v1.y;
                pGrid->pSecond->vTexCoords03.z = v2.x;
                pGrid->pSecond->vTexCoords03.w = v2.y;
                pGrid->pSecond->vTexCoords04.x = v3.x;
                pGrid->pSecond->vTexCoords04.y = v3.y;
                pGrid->pSecond->vTexCoords04.z = v4.x;
                pGrid->pSecond->vTexCoords04.w = v4.y;
            }
        }
    }*/

    //-----------------------------------------------------------------------------------
    bool CTerrainMgr::FindCell(TerrainCell** ppGrid, int x, int y)
    {
        return m_levelGrid.FindCell(ppGrid, x, y);
    }

    //-----------------------------------------------------------------------------------
    inline int evalFunction(int x, float m, int b)
    {
        return (int)(m * x) + b;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgr::ComputeLighting(CLight* pLight, Vec4& lightingColor, const Vec3& layerColor, const Vec3& pos)
    {
        if (!pLight)
            return;

        const Vec3& derivedPos = pLight->GetAttachedNode()->GetDerivedPosition();

        float distance = glm::length(derivedPos - pos);
        float radius   = pLight->GetRadius() - CELL_SIZE * 0.35f;

        if (distance < radius)
        {
            Vec3 lightRay = glm::normalize(derivedPos - pos);

            float attenuation = 1.0f - distance / radius;
            float diffuse = std::max(glm::dot(Vec3(0.0f, 0.0f, -1.0f), lightRay), 0.0f);
            lightingColor += Vec4(((attenuation * diffuse * pLight->GetInitialEnergy()) * layerColor * Vec3(pLight->GetDiffuseColor())), 0.0f);
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgr::BakeLighting(CLight* pLight, unsigned int x, unsigned int y, LevelHalfCell* pHC)
    {
        float xMin = (float)(x * CELL_SIZE);
        float xMax = (float)(x * CELL_SIZE + CELL_SIZE);
        float yMin = (float)(y * CELL_SIZE);
        float yMax = (float)(y * CELL_SIZE + CELL_SIZE);

        float depth = m_levelGrid.GetLayerDepth(pHC->depthLayer);

        CTerrainMgrGPU* pRenderObject = m_levelGrid.GetLevelRenderObject();

        if (pHC->cellLayout == CELL_LAYOUT_FULL)
        {
            pHC->lightingColor.resize(6);

            ComputeLighting(pLight, pHC->lightingColor[0], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMin, yMin, depth)));
            ComputeLighting(pLight, pHC->lightingColor[1], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMin, yMax, depth)));
            ComputeLighting(pLight, pHC->lightingColor[2], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMax, yMin, depth)));
            ComputeLighting(pLight, pHC->lightingColor[3], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMax, yMin, depth)));
            ComputeLighting(pLight, pHC->lightingColor[4], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMin, yMax, depth)));
            ComputeLighting(pLight, pHC->lightingColor[5], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMax, yMax, depth)));
        }
        else if (pHC->cellLayout == CELL_LAYOUT_BOTLEFT)
        {
            pHC->lightingColor.resize(3);

            ComputeLighting(pLight, pHC->lightingColor[0], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMin, yMin, depth)));
            ComputeLighting(pLight, pHC->lightingColor[1], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMin, yMax, depth)));
            ComputeLighting(pLight, pHC->lightingColor[2], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMax, yMin, depth)));
        }
        else if (pHC->cellLayout == CELL_LAYOUT_BOTRIGHT)
        {
            pHC->lightingColor.resize(3);

            ComputeLighting(pLight, pHC->lightingColor[0], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMin, yMin, depth)));
            ComputeLighting(pLight, pHC->lightingColor[1], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMax, yMax, depth)));
            ComputeLighting(pLight, pHC->lightingColor[2], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMax, yMin, depth)));
        }
        else if (pHC->cellLayout == CELL_LAYOUT_TOPLEFT)
        {
            pHC->lightingColor.resize(3);

            ComputeLighting(pLight, pHC->lightingColor[0], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMin, yMin, depth)));
            ComputeLighting(pLight, pHC->lightingColor[1], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMin, yMax, depth)));
            ComputeLighting(pLight, pHC->lightingColor[2], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMax, yMax, depth)));
        }
        else if (pHC->cellLayout == CELL_LAYOUT_TOPRIGHT)
        {
            pHC->lightingColor.resize(3);

            ComputeLighting(pLight, pHC->lightingColor[0], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMax, yMin, depth)));
            ComputeLighting(pLight, pHC->lightingColor[1], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMin, yMax, depth)));
            ComputeLighting(pLight, pHC->lightingColor[2], pHC->layerColor, pRenderObject->ComputeTransformedPosition(Vec3(xMax, yMax, depth)));
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgr::BakeLighting(std::list< CLight* >& lights)
    {
        mapHalfCells& halfCells = m_levelGrid.GetHalfCells();

        for (mapHalfCells::iterator it = halfCells.begin(); it != halfCells.end(); ++it)
        {
            LevelHalfCell* pFirst = it->second.first;
            if (pFirst)
               pFirst->lightingColor.clear();

            LevelHalfCell* pSecond = it->second.second;
            if (pSecond)
                pSecond->lightingColor.clear();
        }

//         if (lights.empty())
//         {
//             lights.push_back(0);
//         }

        for (std::list< CLight* >::iterator jt = lights.begin(); jt != lights.end(); ++jt)
        {
            CLight* pLight = *jt;

            for (mapHalfCells::iterator it = halfCells.begin(); it != halfCells.end(); ++it)
            {
                int x = it->first / m_levelGrid.GetNumCellsY();
                int y = it->first % m_levelGrid.GetNumCellsY();

              
                LevelHalfCell* pFirst = it->second.first;
                if (pFirst)
                {
                    BakeLighting(pLight, x, y, pFirst);
                }

                LevelHalfCell* pSecond = it->second.second;
                if (pSecond)
                {
                    BakeLighting(pLight, x, y, pSecond);
                }
            }
        }

        // Tonemap all lighting colors to [0, 1] per channel
        for (mapHalfCells::iterator it = halfCells.begin(); it != halfCells.end(); ++it)
        {
            LevelHalfCell* pFirst = it->second.first;
            if (pFirst)
            {
                for (unsigned int i = 0; i < pFirst->lightingColor.size(); ++i)
                {
                    float lightingColorChannelMax = std::max(pFirst->lightingColor[i].x, std::max(pFirst->lightingColor[i].y, pFirst->lightingColor[i].z));

                    if (lightingColorChannelMax > 1.0f)
                    {
                        pFirst->lightingColor[i] /= lightingColorChannelMax;
                        pFirst->lightingColor[i].w = lightingColorChannelMax;
                    }
                    else
                    {
                        pFirst->lightingColor[i].w = 1.0f;
                    }
                }

            }

            LevelHalfCell* pSecond = it->second.second;
            if (pSecond)
            {
                for (unsigned int i = 0; i < pSecond->lightingColor.size(); ++i)
                {
                    float lightingColorChannelMax = std::max(pSecond->lightingColor[i].x, std::max(pSecond->lightingColor[i].y, pSecond->lightingColor[i].z));

                    if (lightingColorChannelMax > 1.0f)
                    {
                        pSecond->lightingColor[i] /= lightingColorChannelMax;
                        pSecond->lightingColor[i].w = lightingColorChannelMax;
                    }
                    else
                    {
                        pSecond->lightingColor[i].w = 1.0f;
                    }
                }
            }
        }

        m_levelGrid.GetLevelRenderObject()->UpdateTerrainBuffers();
        /*for (mapHalfCells::iterator it = halfCells.begin(); it != halfCells.end(); ++it)
        {
            int x = it->first / m_levelGrid.GetNumCellsY();
            int y = it->first % m_levelGrid.GetNumCellsY();


            LevelHalfCell* pFirst = it->second.first;
            if (pFirst)
            {
                for (unsigned int i = 0; i < pFirst->lightingColor.size(); ++i)
                {
                    pFirst->lightingColor[i] *= pFirst->layerColor;
                }
            }

            LevelHalfCell* pSecond = it->second.first;
            if (pSecond)
            {
                for (unsigned int i = 0; i < pSecond->lightingColor.size(); ++i)
                {
                    pSecond->lightingColor[i] *= pSecond->layerColor;
                }

            }
                
        }*/
        
    }

    //-----------------------------------------------------------------------------------
    bool CTerrainMgr::IsRayIntersecting(const Vec3& vStart, const Vec3& vEnd, Vec3* pIntersection, Vec3* pNormal)
    {
        VECTOR2FIXED p0(vStart.x * CELL_SCALE, vStart.y * CELL_SCALE);
        VECTOR2FIXED p1(vEnd.x * CELL_SCALE, vEnd.y * CELL_SCALE);

        Vec2 tmpIntersection;
        ComputeIntersection2D(Vec2(0.0f, 0.0f), Vec2(0.0f, CELL_SIZE_SCALED), 
            Vec2(vStart.x * CELL_SCALE, vStart.y * CELL_SCALE), Vec2(vEnd.x * CELL_SCALE, vEnd.y * CELL_SCALE), &tmpIntersection, 0, 0);

        int b = (int)tmpIntersection.y;
        float m = 0;
        if(p1.y - p0.y != 0)
            m = (float)(p0.y - p1.y) / (float)(p0.x - p1.x);

        int x0 = 0, y0 = 0;
        DecomposeFixed(p0.x, &x0, NULL);
        DecomposeFixed(p0.y, &y0, NULL);

        int x1 = 0, y1 = 0;
        DecomposeFixed(p1.x, &x1, NULL);
        DecomposeFixed(p1.y, &y1, NULL);

        Vec2 vIntersection(0.0f, 0.0f);

        int xChange = 0;
        int yChange = 0;
        while(true)
        { 
            if (x0==x1 && y0==y1) 
                return false;
            
            int tmpX0 = x0, 
                tmpY0 = y0;

            if(x1 == x0)
            {
                if(y1 > y0)
                    tmpY0 += 1;
                else
                    tmpY0 -= 1;
            }
            else if(x1 > x0 && y1 >= y0)
            {
                int y = evalFunction((x0 + 1) * CELL_SIZE_SCALED, m, b);
                if(y <= (y0 + 1) * CELL_SIZE_SCALED)
                    tmpX0 += 1;
                if(y >= (y0 + 1) * CELL_SIZE_SCALED)
                    tmpY0 += 1;
            }
            else if(x1 > x0 && y1 < y0)
            {
                int y = evalFunction((x0 + 1) * CELL_SIZE_SCALED, m, b);
                if(y >= y0 * CELL_SIZE_SCALED)
                    tmpX0 += 1;
                if(y <= y0 * CELL_SIZE_SCALED)
                    tmpY0 -= 1;
            }
            else if(x1 < x0 && y1 >= y0)
            {
                int y = evalFunction(x0 * CELL_SIZE_SCALED, m, b);
                if(y <= (y0 + 1) * CELL_SIZE_SCALED)
                    tmpX0 -= 1;
                if(y >= (y0 + 1) * CELL_SIZE_SCALED)
                    tmpY0 += 1;
            }
            else if(x1 < x0 && y1 < y0)
            {
                int y = evalFunction(x0 * CELL_SIZE_SCALED, m, b);
                if(y <= y0 * CELL_SIZE_SCALED)
                    tmpY0 -= 1;
                if(y >= y0 * CELL_SIZE_SCALED)
                    tmpX0 -= 1;
            }

            xChange = tmpX0 - x0;
            yChange = tmpY0 - y0;

            x0 = tmpX0;
            y0 = tmpY0;

            TerrainCell* pCell = NULL;
            if(FindCell(&pCell, x0, y0))
            {
                if((pCell->pFirst && pCell->pFirst->bCollision && pCell->pSecond && pCell->pSecond->bCollision) ||
                    (pCell->pFirst && pCell->pFirst->bCollision && pCell->pFirst->cellLayout == CELL_LAYOUT_FULL) ||
                    (pCell->pSecond && pCell->pSecond->bCollision && pCell->pSecond->cellLayout == CELL_LAYOUT_FULL))
                {
                    Vec2 p3, p4;
                    if(xChange < 0)
                    {
                        p3 = Vec2(CELL_SIZE, 0.0f);
                        p4 = Vec2(CELL_SIZE, CELL_SIZE);
                        if(pNormal)
                            *pNormal = Vec3(1, 0, 0);
                    }
                    else if(xChange > 0)
                    {
                        p3 = Vec2(0.0f, 0.0f);
                        p4 = Vec2(0.0f, CELL_SIZE);
                        if(pNormal)
                            *pNormal = Vec3(-1, 0, 0);
                    }
                    else if(yChange > 0)
                    {
                        p3 = Vec2(0.0f, 0.0f);
                        p4 = Vec2(CELL_SIZE, 0.0f);
                        if(pNormal)
                            *pNormal = Vec3(0, -1, 0);
                    }
                    else if(yChange < 0)
                    {
                        p3 = Vec2(0.0f, CELL_SIZE);
                        p4 = Vec2(CELL_SIZE, CELL_SIZE);
                        if(pNormal)
                            *pNormal = Vec3(0, 1, 0);
                    }

                    float s, t;
                    ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                        Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                        &vIntersection, &s, &t);

                    /*if(t < 0.0 || t > 1.0)
                    {
                        if(yChange > 0)
                        {
                            p3 = VECTOR2(0.0f, 0.0f);
                            p4 = VECTOR2(CELL_SIZE, 0.0f);
                        }
                        else if(yChange < 0)
                        {
                            p3 = VECTOR2(0.0f, CELL_SIZE);
                            p4 = VECTOR2(CELL_SIZE, CELL_SIZE);
                        }
                        ComputeIntersection2D(VECTOR2(vStart.x, vStart.y), VECTOR2(vEnd.x, vEnd.y), 
                            VECTOR2(x0, y0) * CELL_SIZE + p3, VECTOR2(x0, y0) * CELL_SIZE + p4, 
                            &vIntersection, &s, &t);
                    }*/

                    break;
                }
                else if((pCell->pFirst && pCell->pFirst->bCollision) || (pCell->pSecond && pCell->pSecond->bCollision))
                {
                    CELL_LAYOUT layout;
                    if(pCell->pFirst && pCell->pFirst->bCollision)
                        layout = pCell->pFirst->cellLayout;
                    else
                        layout = pCell->pSecond->cellLayout;

                    // Only x or y traversal
                    if(xChange < 0 && yChange == 0)
                    {
                        if(layout == CELL_LAYOUT_BOTRIGHT || layout == CELL_LAYOUT_TOPRIGHT)
                        {
                            Vec2 p3(CELL_SIZE, 0.0f), p4(CELL_SIZE, CELL_SIZE);

                            ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, NULL, NULL);

                            if(pNormal)
                                *pNormal = Vec3(1, 0, 0);

                            break;
                        }
                        else
                        {
                            Vec2 p3, p4;
                            float s, t;
                            ComputeRampLine(layout, p3, p4);
                            if(ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, &s, &t) && t >= 0 && t <= 1)
                            {
                                if(pNormal)
                                {
                                    if(layout == CELL_LAYOUT_BOTLEFT)
                                        *pNormal = Vec3(1.0f, 1.0f, 0.0f);
                                    else if(layout == CELL_LAYOUT_TOPLEFT)
                                        *pNormal = Vec3(1.0f, -1.0f, 0.0f);
                                }
                                break;
                            }

                            


                        }
                        /* This is a possible optimization for horizontal / vertical lines!
                           It is omitted to make the code cleaner and smaller. 
                        if(p0p1.y == 0)
                        {
                            if(layout == CELL_LAYOUT_BOTLEFT)
                            {
                                int relativeX = ComputeRelativeIntersectionX(p0.y, layout);
                                
                                if(pIntersection)
                                    *pIntersection = VECTOR3(Convert2Float(x0, relativeX), Convert2Float(p0.y), 0.0f);
                                break;
                            }
                            else if(layout == CELL_LAYOUT_TOPLEFT)
                            {
                                if(p0p1.y == 0)
                                {
                                    int relativeX = ComputeRelativeIntersectionX(p0.y, layout);

                                    if(pIntersection)
                                        *pIntersection = VECTOR3(Convert2Float(x0, relativeX), Convert2Float(p0.y), 0.0f);
                                    break;
                                }
                            }
                        }*/
                    }
                    else if(xChange > 0 && yChange == 0)
                    {
                        if(layout == CELL_LAYOUT_BOTLEFT || layout == CELL_LAYOUT_TOPLEFT)
                        {
                            Vec2 p3(0.0f, 0.0f), p4(0.0f, CELL_SIZE);

                            ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, NULL, NULL);

                            if(pNormal)
                                *pNormal = Vec3(-1.0f, 0.0f, 0.0f);

                            break;
                        }
                        else
                        {
                            Vec2 p3, p4;
                            float s, t;
                            ComputeRampLine(layout, p3, p4);
                            if(ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, &s, &t) && t >= 0 && t <= 1)
                            {
                                if(pNormal)
                                {
                                    if(layout == CELL_LAYOUT_BOTRIGHT)
                                        *pNormal = Vec3(-1.0f, 1.0f, 0.0f);
                                    else if(layout == CELL_LAYOUT_TOPRIGHT)
                                        *pNormal = Vec3(-1.0f, -1.0f, 0.0f);
                                }

                                break;
                            }

                            
                        }

                    }
                    else if(xChange == 0 && yChange > 0)
                    {
                        if(layout == CELL_LAYOUT_BOTLEFT || layout == CELL_LAYOUT_BOTRIGHT)
                        {
                            Vec2 p3(0.0f, 0.0f), p4(CELL_SIZE, 0.0f);

                            ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, NULL, NULL);

                            if(pNormal)
                                *pNormal = Vec3(0.0f, -1.0f, 0.0f);

                            break;
                        }
                        else
                        {
                            Vec2 p3, p4;
                            float s, t;
                            ComputeRampLine(layout, p3, p4);
                            if(ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, &s, &t) && t >= 0 && t <= 1)
                            {
                                if(pNormal)
                                {
                                    if(layout == CELL_LAYOUT_TOPRIGHT)
                                        *pNormal = Vec3(-1.0f, -1.0f, 0.0f);
                                    else if(layout == CELL_LAYOUT_TOPLEFT)
                                        *pNormal = Vec3(1.0f, -1.0f, 0.0f);
                                }
                                break;
                            }

                            
                        }
                    }
                    else if(xChange == 0 && yChange < 0)
                    {
                        if(layout == CELL_LAYOUT_TOPLEFT || layout == CELL_LAYOUT_TOPRIGHT)
                        {
                            Vec2 p3(0.0f, CELL_SIZE), p4(CELL_SIZE, CELL_SIZE);
                            
                            ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, NULL, NULL);

                            if(pNormal)
                                *pNormal = Vec3(0.0f, 1.0f, 0.0f);

                            break;
                        }
                        else
                        {
                            Vec2 p3, p4;
                            float s, t;
                            ComputeRampLine(layout, p3, p4);
                            if(ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, &s, &t) && t >= 0 && t <= 1)
                            {
                                if(pNormal)
                                {
                                    if(layout == CELL_LAYOUT_BOTLEFT)
                                        *pNormal = Vec3(1.0f, 1.0f, 0.0f);
                                    else if(layout == CELL_LAYOUT_BOTRIGHT)
                                        *pNormal = Vec3(-1.0f, 1.0f, 0.0f);
                                }
                                break;
                            }

                            
                        }
                    }
                    // x AND y traversal
                    else if(xChange < 0 && yChange > 0)
                    {
                        if(layout == CELL_LAYOUT_TOPLEFT)
                        {
                            // Compute intersection
                            Vec2 p3, p4;
                            float s, t;
                            ComputeRampLine(layout, p3, p4);
                            if(ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, &s, &t) && t >= 0 && t <= 1)
                            {
                                if(pNormal)
                                    *pNormal = Vec3(1.0f, -1.0f, 0.0f);
                                break;
                            }

                            
                        }
                        else 
                            break;
                    }
                    else if(xChange < 0 && yChange < 0)
                    {
                        if(layout == CELL_LAYOUT_BOTLEFT)
                        {
                            Vec2 p3, p4;
                            float s, t;
                            ComputeRampLine(layout, p3, p4);
                            if(ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, &s, &t) && t >= 0 && t <= 1)
                            {
                                if(pNormal)
                                    *pNormal = Vec3(1.0f, 1.0f, 0.0f);
                                break;
                            }

                            
                        }
                        else 
                            break;
                    }
                    else if(xChange > 0 && yChange > 0)
                    {
                        if(layout == CELL_LAYOUT_TOPRIGHT)
                        {
                            Vec2 p3, p4;
                            float s, t;
                            ComputeRampLine(layout, p3, p4);
                            if(ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, &s, &t) && t >= 0 && t <= 1)
                            {
                                if(pNormal)
                                    *pNormal = Vec3(-1.0f, -1.0f, 0.0f);
                                break;
                            }

                            
                        }
                        else 
                            break;
                    }
                    else if(xChange > 0 && yChange < 0)
                    {
                        if(layout == CELL_LAYOUT_BOTRIGHT)
                        {
                            Vec2 p3, p4;
                            float s, t;
                            ComputeRampLine(layout, p3, p4);
                            if(ComputeIntersection2D(Vec2(vStart.x, vStart.y), Vec2(vEnd.x, vEnd.y), 
                                Vec2(x0, y0) * (float)CELL_SIZE + p3, Vec2(x0, y0) * (float)CELL_SIZE + p4,
                                &vIntersection, &s, &t) && t >= 0 && t <= 1)
                            {
                                if(pNormal)
                                    *pNormal = Vec3(-1.0f, 1.0f, 0.0f);
                                break;
                            }

                            
                        }
                        else 
                            break;
                    }
                }
            }
        }

        if(pIntersection)
        {
            *pIntersection = Vec3(vIntersection.x, vIntersection.y, 0.0f);
        }

        if (pNormal)
            *pNormal = glm::normalize(*pNormal);


        return true;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainMgr::ComputeRelativeIntersectionX(int absolutePosY, CELL_LAYOUT cellLayout)
    {
    
        switch(cellLayout)
        {
        case CELL_LAYOUT_BOTLEFT:
            {
                int relativeY = 0;// = absolutePosY - cellY * CELL_SIZE_SCALED;
                DecomposeFixed(absolutePosY, NULL, &relativeY);
                int relativeX = CELL_SIZE_SCALED - relativeY;
                return relativeX;
            }
            break;

        case CELL_LAYOUT_TOPLEFT:
            {
                int relativeY = 0;
                DecomposeFixed(absolutePosY, NULL, &relativeY);
                int relativeX = relativeY;
                return relativeX;
            }
            break;

        case CELL_LAYOUT_BOTRIGHT:
            {
                int relativeY = 0;// = absolutePosY - cellY * CELL_SIZE_SCALED;
                DecomposeFixed(absolutePosY, NULL, &relativeY);
                int relativeX = relativeY;
                return relativeX;
            }
            break;

        case CELL_LAYOUT_TOPRIGHT:
            {
                int relativeY = 0;
                DecomposeFixed(absolutePosY, NULL, &relativeY);
                int relativeX = CELL_SIZE_SCALED - relativeY;
                return relativeX;
            }
            break;
        default:
            break;
        }
        return 0;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainMgr::ComputeRelativeIntersectionY(int absolutePosX, CELL_LAYOUT cellLayout)
    {
        switch(cellLayout)
        {
        case CELL_LAYOUT_BOTLEFT:
            {
                int relativeX = 0;
                DecomposeFixed(absolutePosX, NULL, &relativeX);
                int relativeY = CELL_SIZE_SCALED - relativeX;
                return relativeY;
            }
            break;

        case CELL_LAYOUT_TOPLEFT:
            {
                int relativeX = 0;
                DecomposeFixed(absolutePosX, NULL, &relativeX);
                int relativeY = relativeX;
                return relativeY;
            }
            break;

        case CELL_LAYOUT_BOTRIGHT:
            {
                int relativeX = 0;// = absolutePosY - cellY * CELL_SIZE_SCALED;
                DecomposeFixed(absolutePosX, NULL, &relativeX);
                int relativeY = relativeX;
                return relativeY;
            }
            break;

        case CELL_LAYOUT_TOPRIGHT:
            {
                int relativeX = 0;
                DecomposeFixed(absolutePosX, NULL, &relativeX);
                int relativeY = CELL_SIZE_SCALED - relativeX;
                return relativeY;
            }
            break;
        default:
            break;
        }
        return 0;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgr::ComputeRampLine(CELL_LAYOUT layout, Vec2& p1, Vec2& p2)
    {
        switch(layout)
        {
        case CELL_LAYOUT_BOTLEFT:
                p1 = Vec2(0, CELL_SIZE);
                p2 = Vec2(CELL_SIZE, 0);
            break;

        case CELL_LAYOUT_TOPLEFT:
                p1 = Vec2(0, 0);
                p2 = Vec2(CELL_SIZE, CELL_SIZE);
            break;

        case CELL_LAYOUT_BOTRIGHT:
                p1 = Vec2(0, 0);
                p2 = Vec2(CELL_SIZE, CELL_SIZE);
            break;

        case CELL_LAYOUT_TOPRIGHT:
                p1 = Vec2(0, CELL_SIZE);
                p2 = Vec2(CELL_SIZE, 0);
            break;
        default:
            break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgr::DecomposeFixed(int absolutePos, int* pCell, int* pRelativePos)
    {
        if(pCell)
            *pCell = absolutePos / (CELL_SIZE_SCALED);
        if(pRelativePos)
            *pRelativePos = absolutePos - (absolutePos / (CELL_SIZE_SCALED) * CELL_SIZE_SCALED);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgr::GetCellIndices(unsigned int& cellX, unsigned int& cellY, const Vec3 posFloatingPoint)
    {
        cellX = static_cast<unsigned int>(posFloatingPoint.x / CELL_SIZE);
        cellY = static_cast<unsigned int>(posFloatingPoint.y / CELL_SIZE);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainMgr::GetMouseHoverCell(unsigned int& cellX, unsigned int& cellY)
    {
        cellX = m_mouseHoverCellX;
        cellY = m_mouseHoverCellY;
    }
} // env
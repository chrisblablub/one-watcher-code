/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/TerrainMgrCPU.h>
#include <Engine/Scene/TerrainSelection.h>
#include <Engine/Scene/Light.h>

namespace env
{
    class CTerrainMgr
    {
    public:
        CTerrainMgr();
        ~CTerrainMgr();

        CollisionState ComputeMovement(const CTerrainCollidable& collidable);

        bool ComputeIntersectedGrid(const Vec3& vRayOrigin, const Vec3& vRayDirection,
            unsigned int& gridX, unsigned int& gridY);

        void AddToRenderQueue(RenderQueue& renderQueue);
        
        const Vec2& GetMouseIntersection();
        void GetMouseHoverCell(unsigned int& cellX, unsigned int& cellY);

        void BakeLighting(std::list< CLight* >& lights);

        bool IsRayIntersecting(const Vec3& vStart, const Vec3& vEnd, Vec3* pIntersection, Vec3* pNormal = NULL);

        CTerrainMgrCPU& GetLevelGrid() { return m_levelGrid; }
        CTerrainSelection& GetLevelSelection() { return m_levelSelection; }

        unsigned int GetMouseHoverCellX() const { return m_mouseHoverCellX; }
        unsigned int GetMouseHoverCellY() const { return m_mouseHoverCellY; }
        unsigned int GetMouseHoverCellIndex() const { return m_mouseHoverCellX * m_levelGrid.GetNumCellsY() + m_mouseHoverCellY; }

    private:

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_levelGrid;
            ar & m_initialGravityDir;
        }

    private:

        void ComputeLighting(CLight* pLight, Vec4& lightingColor, const Vec3& layerColor, const Vec3& pos);

        void BakeLighting(CLight* pLight, unsigned int x, unsigned int y, LevelHalfCell* pHC);
        
        int ComputeRelativeIntersectionX(int absolutePosY, CELL_LAYOUT cellLayout);
        int ComputeRelativeIntersectionY(int absolutePosX, CELL_LAYOUT cellLayout);
        void ComputeRampLine(CELL_LAYOUT layout, Vec2& p1, Vec2& p2);
        void DecomposeFixed(int absolutePos, int* cell, int* relativePos);
        void GetCellIndices(unsigned int& cellX, unsigned int& cellY, const Vec3 posFloatingPoint);
        bool FindCell(TerrainCell** ppGrid, int x, int y);
        int ComputeCellDistance(int xORy, int cellXORY, bool bMin);
        bool CheckRamps(CTerrainCollider& c, int cellXMin, int cellXMax, int cellYMin, int cellYMax, CELL_LAYOUT cellLayout, 
                float& distance);
        bool CheckTransition(CTerrainCollider& c, int dirX, int dirY, bool bOnlyX, bool bOnlyY,
                int cellXMin, int cellXMax, int cllYMin, int cellYMax, bool* pbStickyCeiling = NULL);
        void UpdateFlags(CTerrainCollider& c, CollisionState& colRes, const Vec2& vPos);
        void Initialize(unsigned int numPlayers);

    private:
        Vec2 m_mouseIntersection;

        unsigned int m_mouseHoverCellX;
        unsigned int m_mouseHoverCellY;

        CTerrainMgrCPU m_levelGrid;
        CTerrainSelection m_levelSelection;

        GRAVITY_DIRECTION m_gravityDir;
        GRAVITY_DIRECTION m_initialGravityDir;
    };
}

#endif // 
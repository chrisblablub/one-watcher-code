/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once 
#ifndef TERRAIN_COLLIDER_H
#define TERRAIN_COLLIDER_H

#include <Engine/EngineCommon.h>
#include "TerrainCollidable.h"

namespace env
{
    struct ColliderBBox
    {
        ColliderBBox()
            : cellXMin(0), cellXMax(0), cellYMin(0), cellYMax(0) {}

        ColliderBBox(int xMin, int xMax, int yMin, int yMax)
            : cellXMin(xMin),
            cellXMax(xMax),
            cellYMin(yMin),
            cellYMax(yMax) {}

        int cellXMin;
        int cellXMax;
        int cellYMin;
        int cellYMax;
    };


    class CTerrainColliderCircle
    {
    public:
        CTerrainColliderCircle(const VECTOR2FIXED& position, const VECTOR2FIXED& move, int radius);

        void SetPosition(int x, int y);
        void SetPositionX(int x);
        void SetPositionY(int y);
        const VECTOR2FIXED& GetPosition();

        void SetMove(int x, int y);
        void SetMoveX(int x);
        void SetMoveY(int y);
        const VECTOR2FIXED& GetMove();

        int GetRadius();
        int GetDirX();
        int GetDirY();

    private:
        int m_radius;
        int m_dirX;
        int m_dirY;
        VECTOR2FIXED m_pos;
        VECTOR2FIXED m_move;
    };


    class CTerrainCollider
    {
    public:
        CTerrainCollider(const VECTOR2FIXED& vPosition, int width, int height, const VECTOR2FIXED& vMove,
                       const VECTOR2FIXED& vMoveOriginal, bool bOnFloor, bool bHitWallFromLeft, bool bHitWallFromRight, bool bHitCeiling, bool bOnRamp);

        int GetDirX();
        int GetDirY();

        int GetBaseDirX();
        int GetBaseDirY();

        void SetMove(int x, int y);
        void SetMoveX(int x);
        void SetMoveY(int y);
        const VECTOR2FIXED& GetMove();

        inline void SetPositionX(float x) { SetPositionX(static_cast<int>(x)); }
        inline void SetPositionY(float y) { SetPositionY(static_cast<int>(y)); }

        void SetPosition(int x, int y);
        void SetPositionX(int x);
        void SetPositionY(int y);
        const VECTOR2FIXED& GetPosition();

        void SetNewPositionX(int x);
        void SetNewPositionY(int y);

        const VECTOR2FIXED& GetNewPosition();

        void SetSize(int width, int height);
        int GetWidth();
        int GetHeight();
        int GetHalfWidth();
        int GetHalfHeight();

        const ColliderBBox& GetBBox();

        void DoXTransition(bool bXMin);
        void DoYTransition(bool bYMin);

        int GetCellDistanceX(int x, bool bXMin);
        int GetCellDistanceY(int y, bool bYMin);

        void GetCorners(VECTOR2FIXED& v1, VECTOR2FIXED& v2, VECTOR2FIXED& v3, VECTOR2FIXED& v4);

        // 		void SetHitCeiling(bool bSet);
        // 		bool GetHitCeiling();
        // 
        // 		void SetStickyCeiling(bool bSet) { m_bStickyCeiling = bSet; }
        // 		bool GetStickyCeiling() const { return m_bStickyCeiling; }
        // 
        // 		void SetOnFloor(bool bSet);
        // 		bool GetOnFloor();
        // 
        // 		void SetHitRamp(bool bSet, CELL_LAYOUT rampLayout);
        // 		bool GetHitRamp();
        // 
        // 		void SetOnRamp(bool bSet, CELL_LAYOUT rampLayout);
        // 		bool GetOnRamp();
        // 
        // 		CELL_LAYOUT GetHitRampLayout();
        // 		CELL_LAYOUT GetRampLayout();
        // 
        // 		void SetHitWallFromLeft(bool bSet);
        // 		bool GetHitWallFromLeft();
        // 
        // 		void SetHitWallFromRight(bool bSet);
        // 		bool GetHitWallFromRight();

        /*		listCellsHit& GetCellsHit();*/

        void SetSliding(bool bSet);
        bool GetSliding();

        // 		listCellsHit cellsHit;


    private:
        int m_width;
        int m_height;
        int m_halfWidth;
        int m_halfHeight;

        int m_dirX;
        int m_dirY;
        int m_originalDirX;
        int m_originalDirY;

        // 		bool m_bOnFloor;
        // 		bool m_bHitWallFromLeft;
        // 		bool m_bHitWallFromRight;
        // 		bool m_bHitCeiling;
        // 		bool m_bOnRamp;
        // 		bool m_bHitRamp;
        // 		bool m_bSliding;
        // 		bool m_bStickyCeiling;

        // 		CELL_LAYOUT m_onRampLayout;
        // 		CELL_LAYOUT m_hitRampLayout;

        ColliderBBox m_box;

        VECTOR2FIXED m_pos;
        VECTOR2FIXED m_newPos;
        VECTOR2FIXED m_move;
    };
} // env

#endif // TERRAIN_COLLIDER_H
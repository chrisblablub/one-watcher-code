/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifndef TERRAIN_COLLIDABLE_H
#define TERRAIN_COLLIDABLE_H

#include <Engine/EngineCommon.h>


namespace env
{
    const int CELL_SIZE = 32;
    const int CELL_SCALE = 1000;
    const int CELL_SIZE_SCALED = CELL_SIZE * CELL_SCALE;

    typedef int SCALARFIXED;

    struct VECTOR2FIXED
    {
        VECTOR2FIXED()
            : x(0), y(0) {}

        VECTOR2FIXED(float x, float y)
            : x(static_cast<int>(x))
            , y(static_cast<int>(y)) {}

        VECTOR2FIXED(SCALARFIXED x, SCALARFIXED y)
            : x(x)
            , y(y) {}

        VECTOR2FIXED operator-(const VECTOR2FIXED& rhs)
        {
            return VECTOR2FIXED(this->x - rhs.x, 
                                this->y - rhs.y);
        }

        VECTOR2FIXED operator+(const VECTOR2FIXED& rhs)
        {
            return VECTOR2FIXED(this->x + rhs.x, 
                                this->y + rhs.y);
        }

        VECTOR2FIXED operator*(float rhs)
        {
            return VECTOR2FIXED(this->x * rhs, 
                                this->y * rhs);
        }

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & x;
            ar & y;
        }

        SCALARFIXED x;
        SCALARFIXED y;
    };

    struct VECTOR3FIXED
    {
        VECTOR3FIXED()
            : x(0), y(0), z(0) {}

        VECTOR3FIXED(float x, float y, float z)
            : x(static_cast<int>(x))
            , y(static_cast<int>(y)) 
            , z(static_cast<int>(z)) {}

        VECTOR3FIXED(SCALARFIXED x, SCALARFIXED y, SCALARFIXED z)
            : x(x)
            , y(y)
            , z(z) {}

        VECTOR3FIXED operator-(const VECTOR3FIXED& rhs)
        {
            return VECTOR3FIXED(this->x - rhs.x, 
                                this->y - rhs.y, 
                                this->z - rhs.z);
        }

        SCALARFIXED x;
        SCALARFIXED y;
        SCALARFIXED z;
    };


    inline Vec2 Convert2Float(const VECTOR2FIXED& v)
    {
        return Vec2(static_cast<float>(v.x) / static_cast<float>(CELL_SCALE),
                       static_cast<float>(v.y) / static_cast<float>(CELL_SCALE));
    }

    inline SCALARFIXED Convert2Fixed(const int v)
    {
        return (SCALARFIXED)(v * CELL_SCALE);
    }

    inline SCALARFIXED Convert2Fixed(const float v)
    {
        return (SCALARFIXED)(v * CELL_SCALE);
    }

    inline VECTOR2FIXED Convert2Fixed(const float x, 
                                      const float y)
    {
        return VECTOR2FIXED(static_cast<int>(x * CELL_SCALE),
                            static_cast<int>(y * CELL_SCALE));
    }

    inline VECTOR2FIXED Convert2Fixed(const Vec2& v)
    {
        return VECTOR2FIXED(static_cast<int>(v.x * CELL_SCALE),
                            static_cast<int>(v.y * CELL_SCALE));
    }

    inline VECTOR3FIXED Convert2Fixed(const float x, const float y, const float z)
    {
        return VECTOR3FIXED(static_cast<int>(x * CELL_SCALE),
                            static_cast<int>(y * CELL_SCALE),
                            static_cast<int>(z * CELL_SCALE));
    }

    enum CELL_ROTATION
    {
        CELL_ROTATION_0DEG_CW = 0,
        CELL_ROTATION_90DEG_CW,
        CELL_ROTATION_180DEG_CW,
        CELL_ROTATION_270DEG_CW
    };

    enum CELL_LAYOUT
    {
        CELL_LAYOUT_UNDEFINED = 0,
        CELL_LAYOUT_BOTLEFT = 1,
        CELL_LAYOUT_BOTRIGHT = 2,
        CELL_LAYOUT_TOPRIGHT = 3,
        CELL_LAYOUT_TOPLEFT = 4,
        CELL_LAYOUT_FULL = 5
    };

    enum CELL_TYPE
    {
        CELL_TYPE_NORMAL = 0,
        CELL_TYPE_DAMAGE,
        CELL_TYPE_DESTRUCTABLE,
        CELL_TYPE_ACCELERATE,
        CELL_TYPE_ALPHA
    };

    enum CELL_TYPE_SIDE
    {
        CELL_TYPE_SIDE_ALL = 0,
        CELL_TYPE_SIDE_LEFT,
        CELL_TYPE_SIDE_TOP,
        CELL_TYPE_SIDE_RIGHT,
        CELL_TYPE_SIDE_BOTTOM
    };

    enum GRAVITY_DIRECTION
    {
        GRAVITY_DOWN = 0,
        GRAVITY_LEFT,
        GRAVITY_UP,
        GRAVITY_RIGHT
    };

    typedef std::list< unsigned int > listCellsHit;

    struct CollisionState
    {
        CollisionState()
            : bGrabbedWallFromLeft(false)
            , bGrabbedWallFromRight(false)
            , bOnFloor(false)
            , bOnRamp(false)
            , bHitCeiling(false)
            , bStickyCeiling(false)
            , bHitRamp(false)
            , bHitWallFromleft(false)
            , bHitWallFromRight(false)
            , onRampLayout(CELL_LAYOUT_UNDEFINED)
            , hitRampLayout(CELL_LAYOUT_UNDEFINED) {}

        void SetHitCeiling(bool bSet) { bHitCeiling = bSet; }
        void SetStickyCeiling(bool bSet) { bStickyCeiling = bSet; }
        void SetOnFloor(bool bSet) { bOnFloor = bSet; }
        void SetHitRamp(bool bSet, CELL_LAYOUT rampLayout) { bHitRamp = bSet; hitRampLayout = rampLayout; }
        void SetOnRamp(bool bSet, CELL_LAYOUT rampLayout) { bOnRamp = bSet; onRampLayout = rampLayout; }
        void SetGrabbedWallFromLeft(bool bSet) { bGrabbedWallFromLeft = bSet; }
        void SetGrabbedWallFromRight(bool bSet) { bGrabbedWallFromRight = bSet; }
        void SetHitWallFromLeft(bool bSet) { bHitWallFromleft = bSet; }
        void SetHitWallFromRight(bool bSet) { bHitWallFromRight = bSet; }

        listCellsHit cellsHit;

        VECTOR2FIXED pos;
        bool bOnFloor;
        bool bOnRamp;
        bool bGrabbedWallFromLeft;
        bool bGrabbedWallFromRight;
        bool bHitWallFromleft;
        bool bHitWallFromRight;

        bool bHitCeiling;
        bool bStickyCeiling;
        bool bHitRamp;
        CELL_LAYOUT onRampLayout;
        CELL_LAYOUT hitRampLayout;
    };


    struct CTerrainCollidable
    {
    public:
        float width;
        float height;

        VECTOR2FIXED position;
        VECTOR2FIXED velocity;
    };
} // env

#endif // TERRAIN_COLLIDABLE_H
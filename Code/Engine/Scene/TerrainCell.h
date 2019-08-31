/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TERRAIN_CELL_H
#define TERRAIN_CELL_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/TerrainCollider.h>

namespace env
{
    struct RenderQueue;

    class LevelHalfCell
    {
    public:
        LevelHalfCell()
            : cellLayout(CELL_LAYOUT_UNDEFINED)
            , cellRotation(CELL_ROTATION_0DEG_CW)
            , texCoords01(0.0, 0.0, 1.0, 1.0)
            , texCoords02(0.0f, 1.0f, 1.0f, 0.0f)
            , layerColor(1.0f, 1.0f, 1.0f)
            , emissiveColor(1.0f, 1.0f, 1.0f, 1.0f)
            , borderColor(0.0f, 0.0f, 0.0f)
            , sequence(0)
            , depthLayer(0)
            , bCollision(true)
            , bStickyCeiling(false) {}

        LevelHalfCell(const LevelHalfCell& other)
            : cellLayout(other.cellLayout)
            , cellRotation(other.cellRotation)
            , texCoords01(other.texCoords01)
            , texCoords02(other.texCoords02)
            , layerColor(other.layerColor)
            , lightingColor(other.lightingColor)
            , emissiveColor(other.emissiveColor)
            , borderColor(other.borderColor)
            , sequence(other.sequence)
            , depthLayer(other.depthLayer)
            , bCollision(other.bCollision)
            , bStickyCeiling(other.bStickyCeiling) {}

        void Reset()
        {
            cellLayout = CELL_LAYOUT_UNDEFINED;
            cellRotation = CELL_ROTATION_0DEG_CW;
            texCoords01 = Vec4(0.0, 0.0, 1.0, 1.0);
            texCoords02 = Vec4(0.0f, 1.0f, 1.0f, 0.0f);
            layerColor = Vec3(1.0f, 1.0f, 1.0f);
            emissiveColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
            borderColor = Vec3(0.0f, 0.0f, 0.0f);
            lightingColor.clear();
            sequence = 0;
            bCollision = true;
            bStickyCeiling = false;
            depthLayer = 0;
        }

        void SaveTemplate(CScript& script)
        {
            script["texCoords01"] = texCoords01;
            script["texCoords02"] = texCoords02;
            script["layerColor"] = layerColor;
            script["emissiveColor"] = emissiveColor;
            script["borderColor"] = borderColor;
            script["sequence"] = (int)sequence;
            script["depthLayer"] = (int)depthLayer;
            script["bCollision"] = bCollision;
            script["bStickyCeiling"] = bStickyCeiling;
            script["cellLayout"] = (int)cellLayout;
            script["cellRotation"] = (int)cellRotation;
        }

        void LoadTemplate(CScript& script)
        {
            texCoords01 = script["texCoords01"];
            texCoords02 = script["texCoords02"];
            layerColor = script["layerColor"];
            emissiveColor = script["emissiveColor"];
            borderColor = script["borderColor"];
            sequence = (int)script["sequence"];
            depthLayer = (int)script["depthLayer"];
            bCollision = script["bCollision"];
            bStickyCeiling = script["bStickyCeiling"];
            cellLayout = (CELL_LAYOUT)(int)script["cellLayout"];
            cellRotation = (CELL_ROTATION)(int)script["cellRotation"];
        }

        Vec4 texCoords01;
        Vec4 texCoords02;
        Vec3 layerColor;
        Vec4 emissiveColor;
        Vec3 borderColor;

        // Indices
        // FULL: 
        //        1/4 ----- 5
        //         |   \    |
        //         0 ----- 2/3
        // BotLeft: 
        //         1
        //         |   \
        //         0 ----- 2
        // BotRight: 
        //                  1
        //             /    |
        //         0 ------ 2
        // TopLeft: 
        //         1 ------ 2
        //         |   /
        //         0 
        // TopRight: 
        //         1 ------ 2
        //             \    |
        //                  0
        std::vector< Vec4 > lightingColor;

        unsigned int sequence;
        unsigned int depthLayer;

        bool bCollision;
        bool bStickyCeiling;

        CELL_LAYOUT cellLayout;
        CELL_ROTATION cellRotation;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & layerColor;
            ar & sequence;
            ar & bCollision;
            ar & bStickyCeiling;
            ar & cellLayout;
            ar & cellRotation;
            ar & depthLayer;

            if (version > 0)
                ar & borderColor;

            if (version > 1)
                ar & lightingColor;

            if (version > 2)
                ar & emissiveColor;
        }
    };

    class TerrainCell
    {
    public:
        TerrainCell()
            : pFirst(NULL)
            , pSecond(NULL)
        {
            Reset();
        }

        LevelHalfCell* pFirst;
        LevelHalfCell* pSecond;

        void Reset();
        bool LayoutEquals(CELL_LAYOUT layout);
        bool LayoutContains(CELL_LAYOUT layout);

        LevelHalfCell* GetHalfCell(CELL_LAYOUT layout);

        bool LayerEquals(int layer);
    };
} // env
#endif // TERRAIN_CELL_H
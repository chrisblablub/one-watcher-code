/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef BOUNDING_BOX_DATA_CACHE_H
#define BOUNDING_BOX_DATA_CACHE_H

#include <Engine/EngineCommon.h>
#include <Graphic/Core/DataCache.h>
#include <Common/Core/BoundingVolume.h>

namespace env
{
    class CBoundingBoxDataCache : public CVector3Cache
    {
    public:
        virtual ~CBoundingBoxDataCache() {}

        void Initialize(CBoundingVolume& boundingBox, const Vec3& color = Vec3(1.0f, 0.0f, 0.0f))
        {
            Reset();

            SetPrimitiveType(PRIMITIVE_TYPE_LINELIST);

            Reserve(24);

            Vec3 vRight = boundingBox.GetRight();
            Vec3 vUp = boundingBox.GetUp();
            Vec3 vForward = boundingBox.GetForward();
            Vec3 vHalfLengths = boundingBox.GetHalfLengths();
            Vec3 vOrigin = boundingBox.GetOrigin();

            Vec3 vPoints[8];
            int l = 0;
            for (int i = -1; i < 2; i += 2)
            {
                // right
                Vec3 v1 = vOrigin + (float)i * vRight * vHalfLengths[0];

                for (int j = -1; j < 2; j += 2)
                {
                    // Up
                    Vec3 v2 = v1 + (float)j * vUp * vHalfLengths[1];

                    for (int k = -1; k < 2; k += 2, ++l)
                    {
                        Vec3 v3 = v2 + (float)k * vForward * vHalfLengths[2];
                        // Forward
                        vPoints[l] = v3;
                    }
                }
            }

            Add(PolygonVertex(vPoints[0], color));
            Add(PolygonVertex(vPoints[1], color));
            Add(PolygonVertex(vPoints[0], color));
            Add(PolygonVertex(vPoints[2], color));
            Add(PolygonVertex(vPoints[0], color));
            Add(PolygonVertex(vPoints[4], color));
            Add(PolygonVertex(vPoints[2], color));
            Add(PolygonVertex(vPoints[3], color));
            Add(PolygonVertex(vPoints[2], color));
            Add(PolygonVertex(vPoints[6], color));
            Add(PolygonVertex(vPoints[3], color));
            Add(PolygonVertex(vPoints[7], color));
            Add(PolygonVertex(vPoints[3], color));
            Add(PolygonVertex(vPoints[1], color));
            Add(PolygonVertex(vPoints[1], color));
            Add(PolygonVertex(vPoints[5], color));
            Add(PolygonVertex(vPoints[7], color));
            Add(PolygonVertex(vPoints[6], color));
            Add(PolygonVertex(vPoints[7], color));
            Add(PolygonVertex(vPoints[5], color));
            Add(PolygonVertex(vPoints[4], color));
            Add(PolygonVertex(vPoints[5], color));
            Add(PolygonVertex(vPoints[4], color));
            Add(PolygonVertex(vPoints[6], color));
        }
    };
} // env
#endif // BOUNDING_BOX_DATA_CACHE_H
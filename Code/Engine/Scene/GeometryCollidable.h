/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef GEOMETRY_COLLIDABLE_H
#define GEOMETRY_COLLIDABLE_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/Geometry.h>

namespace env
{
    class CSceneNode;

    class CGeometryCollidable
    {
    public:
        CGeometryCollidable()
            : m_bCollision(true) {}

        /*virtual*/ bool HasCollision(CGeometryCollidable& e);
        /*virtual*/ bool HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode);

        void ReplacePolygon(unsigned int index, const CPolygon& poly);
        unsigned int AddPolygon(const CPolygon& quad);

        void RemovePolygon(const CPolygon& quad);
        void RemovePolygons();

        unsigned int AddCircle(const CCircle& c);
        void RemoveCircles();

        unsigned int GetNumPolygons() const;
        unsigned int GetNumCircles() const;

        const std::vector< CPolygon >& GetPolygons() const;
        const std::vector< CPolygon >& GetTransformedPolygons() const;

        const std::vector< CCircle >& GetCircles() const;
        const std::vector< CCircle >& GetTransformedCircles() const;

        void ApplyTransform(unsigned int index, const Matrix4x4& mtxTransform, float circleRadius);
        void ApplyTransform(unsigned int index, const Matrix4x4& mtxTransform);
        void ApplyTransform(const Matrix4x4& mtxTransform, float circleRadius = 1.0f);

        void SetCollision(bool bSet) { m_bCollision = bSet; }
        bool GetCollision() const { return m_bCollision; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_polygons;
            ar & m_circles;
            ar & m_bCollision;
        }

    private:
        std::vector< CPolygon > m_polygons;
        std::vector< CPolygon > m_transformedPolygons;

        std::vector< CCircle > m_circles;
        std::vector< CCircle > m_transformedCircles;

        bool m_bCollision;
    };
} /* End namespace */
#endif /* GEOMETRY_COLLIDABLE_H */
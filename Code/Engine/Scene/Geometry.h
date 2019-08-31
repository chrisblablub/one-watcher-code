/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <Engine/EngineCommon.h>


namespace env
{
    class CCircle
    {
    public:
        CCircle()
            : m_vCenter(0.0, 0.0, 0.0)
            , m_radius(0.0) {}

        CCircle(float radius)
            : m_vCenter(0.0f, 0.0f, 0.0f)
            , m_radius(radius) {}

        CCircle(const Vec3& v, float radius)
            : m_vCenter(v), m_radius(radius) {}

        bool ComputeIntersection(CCircle& c) const;

        inline void SetCenter(const Vec3& c) { m_vCenter = c; }
        inline const Vec3& GetCenter() const { return m_vCenter; }
        inline Vec3& GetCenter() { return m_vCenter; }
        inline float GetRadius() const { return m_radius; }
        inline void SetRadius(float radius) { m_radius = radius; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_vCenter;
            ar & m_radius;
        }

    private:
        Vec3 m_vCenter;
        float m_radius;
    };

    struct Edge
    {
        Edge()
            : p1(0.0, 0.0, 0.0), p2(0.0, 0.0, 0.0) {}

        Edge(const Vec3& p1, const Vec3& p2)
            : p1(p1), p2(p2) {}

        bool operator ==(const Edge& b) const
        {
            return (p1 == b.p1 && p2 == b.p2);
        }

        Vec3 p1;
        Vec3 p2;
    };

    class CPolygon
    {
    public:
        CPolygon();
        CPolygon(unsigned int numPoints);

        Vec3& operator[](unsigned int index);
        const Vec3& operator[](unsigned int index) const;

        bool operator==(const CPolygon& b) const;

        void AddPoint(const Vec3& e);
        void SetPoint(unsigned int index, const Vec3& p) { m_points[index] = p; }

        void SetNumPoints(unsigned int numPoints);

        inline unsigned int GetNumPoints() const { return m_points.size(); }

        bool ComputeIntersection(CPolygon& b) const;
        bool ComputeIntersection(CCircle& c) const;

    private:
        void ProjectPolygonOntoAxis(const Vec3& axis, const CPolygon& polygon, float& min, float& max) const;
        float IntervalDistance(float minA, float maxA, float minB, float maxB) const;

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_points;
            ar & m_currentIndex;
        }

    private:
        std::vector< Vec3 > m_points;
        unsigned int m_currentIndex;
    };
} // env
#endif // GEOMETRY_H
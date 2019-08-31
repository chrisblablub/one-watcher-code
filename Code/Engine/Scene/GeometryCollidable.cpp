#include <Engine/Scene/GeometryCollidable.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    bool CGeometryCollidable::HasCollision(CGeometryCollidable& e)
    {
        if (!m_bCollision)
            return false;

        std::vector< CPolygon > polysA = GetTransformedPolygons();
        std::vector< CPolygon > polysB = e.GetTransformedPolygons();

        std::vector< CCircle > circlesA = GetTransformedCircles();
        std::vector< CCircle > circlesB = e.GetTransformedCircles();

        for (int i = 0; i < polysA.size(); ++i)
        {
            /* Polys A - Polys B */
            for (int j = 0; j < polysB.size(); ++j)
            {
                if (polysA[i].ComputeIntersection(polysB[j]))
                    return true;
            }

            /* Polys A - Circles B */
            for (int j = 0; j < circlesB.size(); ++j)
            {
                if (polysA[i].ComputeIntersection(circlesB[j]))
                    return true;
            }
        }

        /* Polys B - Circles A */
        for (int i = 0; i < polysB.size(); ++i)
        {
            for (int j = 0; j < circlesA.size(); ++j)
            {
                if (polysB[i].ComputeIntersection(circlesA[j]))
                    return true;
            }
        }

        /* Circles A - Circles B */
        for (int i = 0; i < circlesA.size(); ++i)
        {
            for (int j = 0; j < circlesB.size(); ++j)
            {
                if (circlesA[i].ComputeIntersection(circlesB[j]))
                    return true;
            }
        }


        return false;
    }

    //-----------------------------------------------------------------------------------
    bool CGeometryCollidable::HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode)
    {
        return false;
    }

    //-----------------------------------------------------------------------------------
    void CGeometryCollidable::ReplacePolygon(unsigned int index, const CPolygon& poly)
    {
        if (index >= m_polygons.size())
            m_polygons.resize(index + 1);

        m_polygons[index] = poly;
    }

    //-----------------------------------------------------------------------------------
    unsigned int CGeometryCollidable::AddPolygon(const CPolygon& quad)
    {
        m_polygons.resize(m_polygons.size() + 1);
        m_polygons[m_polygons.size() - 1] = quad;
        return m_polygons.size() - 1;
    }

    //-----------------------------------------------------------------------------------
    void CGeometryCollidable::RemovePolygon(const CPolygon& quad)
    {
        for (int i = 0; i < m_polygons.size(); ++i)
        {
            if (m_polygons[i] == quad)
            {
                if (m_polygons.size() > 1)
                {
                    m_polygons[i] = m_polygons[m_polygons.size() - 1];
                    m_polygons.resize(m_polygons.size() - 1);
                }
                else
                    m_polygons.clear();
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CGeometryCollidable::RemovePolygons()
    {
        m_polygons.clear();
        m_transformedPolygons.clear();
    }

    //-----------------------------------------------------------------------------------
    void CGeometryCollidable::RemoveCircles()
    {
        m_circles.clear();
        m_transformedCircles.clear();
    }

    //-----------------------------------------------------------------------------------
    unsigned int CGeometryCollidable::AddCircle(const CCircle& c)
    {
        m_circles.resize(m_circles.size() + 1);
        m_circles[m_circles.size() - 1] = c;
        return m_circles.size() - 1;
    }

    //-----------------------------------------------------------------------------------
    unsigned int CGeometryCollidable::GetNumPolygons() const
    {
        return m_polygons.size();
    }

    //-----------------------------------------------------------------------------------
    unsigned int CGeometryCollidable::GetNumCircles() const
    {
        return m_circles.size();
    }

    //-----------------------------------------------------------------------------------
    const std::vector< CPolygon >& CGeometryCollidable::GetPolygons() const
    {
        return m_polygons;
    }

    //-----------------------------------------------------------------------------------
    const std::vector< CPolygon >& CGeometryCollidable::GetTransformedPolygons() const
    {
        if (m_transformedPolygons.empty())
            return m_polygons;
        else
            return m_transformedPolygons;
    }

    //-----------------------------------------------------------------------------------
    const std::vector< CCircle >& CGeometryCollidable::GetCircles() const
    {
        return m_circles;
    }

    //-----------------------------------------------------------------------------------
    const std::vector< CCircle >& CGeometryCollidable::GetTransformedCircles() const
    {
        if (m_transformedCircles.empty())
            return m_circles;
        else
            return m_transformedCircles;
    }

    //-----------------------------------------------------------------------------------
    void CGeometryCollidable::ApplyTransform(unsigned int index, const Matrix4x4& mtxTransform, float circleRadius)
    {
        m_transformedCircles[index].GetCenter() = Vec3(Vec4(m_circles[index].GetCenter(), 1.0f) * mtxTransform);
        m_transformedCircles[index].GetCenter().z = 0.0;
        m_transformedCircles[index].SetRadius(m_circles[index].GetRadius() * circleRadius);
    }

    //-----------------------------------------------------------------------------------
    void CGeometryCollidable::ApplyTransform(unsigned int index, const Matrix4x4& mtxTransform)
    {
        if (m_transformedPolygons.size() != m_polygons.size())
            m_transformedPolygons.resize(m_polygons.size());

        for (int j = 0; j < m_polygons[index].GetNumPoints(); ++j)
        {
            m_transformedPolygons[index].SetNumPoints(m_polygons[index].GetNumPoints());

            m_transformedPolygons[index][j] = Vec3(Vec4(m_polygons[index][j], 1.0f) * mtxTransform);
            m_transformedPolygons[index][j].z = 0.0;
        }
    }

    //-----------------------------------------------------------------------------------
    void CGeometryCollidable::ApplyTransform(const Matrix4x4& mtxTransform, float circleRadius)
    {
        if (m_transformedPolygons.size() != m_polygons.size())
            m_transformedPolygons.resize(m_polygons.size());

        for (int i = 0; i < m_polygons.size(); ++i)
        {
            for (int j = 0; j < m_polygons[i].GetNumPoints(); ++j)
            {
                m_transformedPolygons[i].SetNumPoints(m_polygons[i].GetNumPoints());

                m_transformedPolygons[i][j] = Vec3(Vec4(m_polygons[i][j], 1.0f) * mtxTransform);
                m_transformedPolygons[i][j].z = 0.0;
            }
        }

        if (m_transformedCircles.size() != m_circles.size())
            m_transformedCircles.resize(m_circles.size());

        for (int j = 0; j < GetNumCircles(); ++j)
        {
            m_transformedCircles[j].GetCenter() = Vec3(Vec4(m_circles[j].GetCenter(), 1.0f) * mtxTransform);
            m_transformedCircles[j].GetCenter().z = 0.0;
            m_transformedCircles[j].SetRadius(m_circles[j].GetRadius() * circleRadius);
        }
    }
} // env
#include <Engine/Scene/Geometry.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    bool CCircle::ComputeIntersection(CCircle& c) const
    {
        return (glm::length(m_vCenter - c.m_vCenter) < (m_radius + c.m_radius));
    }

    //-----------------------------------------------------------------------------------
    CPolygon::CPolygon()
        : m_currentIndex(0) {}

    //-----------------------------------------------------------------------------------
    CPolygon::CPolygon(unsigned int numPoints)
        : m_currentIndex(0)
    {
        SetNumPoints(numPoints);
    }

    //-----------------------------------------------------------------------------------
    Vec3& CPolygon::operator[](unsigned int index)
    {
        ENV_ASSERT_DBG(m_points.size() > index);

        return m_points[index];
    }

    const Vec3& CPolygon::operator[](unsigned int index) const
    {
        ENV_ASSERT_DBG(m_points.size() > index);

        return m_points[index];
    }

    //-----------------------------------------------------------------------------------
    bool CPolygon::operator==(const CPolygon& b) const
    {
        bool bEqual = true;
        for (int i = 0; i < m_points.size(); ++i)
        {
            bool bPointFound = false;
            for (int j = 0; j < b.m_points.size(); ++j)
            {
                if ((m_points[i] == b.m_points[j]))
                {
                    bPointFound = true;
                    break;
                }
            }
            if (!bPointFound)
            {
                bEqual = false;
                break;
            }
        }

        return bEqual;
    }

    //-----------------------------------------------------------------------------------
    void CPolygon::AddPoint(const Vec3& e)
    {
        m_points[m_currentIndex] = e;
        ++m_currentIndex;
    }

    //-----------------------------------------------------------------------------------
    void CPolygon::SetNumPoints(unsigned int numPoints)
    {
        m_points.resize(numPoints);
    }

    //-----------------------------------------------------------------------------------
    bool CPolygon::ComputeIntersection(CPolygon& b) const
    {
        bool bIntersect = true;
        unsigned int numPointsA = GetNumPoints();
        unsigned int numPointsB = b.GetNumPoints();

        if (numPointsA == 1 && numPointsB == 1)
        {
            bIntersect = false;
        }
        else if (numPointsA == 1 || numPointsB == 1)
        {
            const CPolygon* p1 = this;
            const CPolygon* p2 = &b;

            if (numPointsB == 1)
            {
                p1 = &b;
                p2 = this;
            }

            // p1 points to the polygon that consists of only one point

            bool bInside = true;
            for (int i = 0; i < p2->GetNumPoints(); ++i)
            {
                Vec3 v1, v2, v3;
                v1 = (*p2)[(i + 1) % p2->GetNumPoints()];
                v2 = (*p2)[i];
                v3 = (*p1)[0];

                Vec3 vEdge1 = v1 - v2;
                Vec3 vEdge2 = v2 - v3;
                Vec3 vEdge3 = v1 - v3;
                Vec3 vPerp(v2.y - v1.y, -(v2.x - v1.x), 0.0);
                vEdge1.z = vEdge2.z = vEdge3.z = 0.0;
                vEdge1 = glm::normalize(vEdge1);
                vEdge2 = glm::normalize(vEdge2);
                vEdge3 = glm::normalize(vEdge3);
                vPerp = glm::normalize(vPerp);

                if (glm::dot(vEdge2, vPerp) < 0.0 ||
                    glm::dot(vEdge3, vPerp) < 0.0)
                {
                    bInside = false;
                    break;
                }
            }

            bIntersect = bInside;


        }
        else
        {
            Vec3 vTranslationAxis(0.0, 0.0, 0.0), vEdge(0.0, 0.0, 0.0);

            for (int i = 0; i < numPointsA + numPointsB; ++i)
            {
                if (i < numPointsA)
                {
                    vEdge = (*this)[(i + 1) % numPointsA] - (*this)[i];
                }
                else
                {
                    vEdge = b[(i - numPointsA + 1) % numPointsB] - b[i - numPointsA];
                }

                Vec3 vAxis(-vEdge.y, vEdge.x, 0.0);
                vAxis = glm::normalize(vAxis);

                float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
                ProjectPolygonOntoAxis(vAxis, *this, minA, maxA);
                ProjectPolygonOntoAxis(vAxis, b, minB, maxB);

                // Check if the polygon projections are currentlty intersecting

                if (IntervalDistance(minA, maxA, minB, maxB) > 0)
                {
                    bIntersect = false;
                    break;
                }
            }
        }

        // 		if(bIntersect && pHitEdges)
        // 		{
        // 			for(int i = 0; i < numPointsA; ++i)
        // 			{
        // 				for(int j = 0; j < numPointsB; ++j)
        // 				{
        // 					VECTOR3 v1, v2;
        // 					v1 = ((*this)((i+1) % numPointsA) - (*this)(i));
        // 					v2 = (b((j+1) % numPointsB) - b(j)); 
        // 
        // 					VECTOR3 intersection;
        // 					if(ComputeIntersection3D((*this)(i), (*this)((i+1) % numPointsA), b(j), b((j+1) % numPointsB)))
        // 						pHitEdges->insert(i);
        // 				}
        // 				
        // 			}
        // 		}

        return bIntersect;
    }

    //-----------------------------------------------------------------------------------
    bool CPolygon::ComputeIntersection(CCircle& c) const
    {
        unsigned int numPoints = GetNumPoints();
        Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), -c.GetCenter()));
        bool bIntersect = false;
        for (int i = 0; i < numPoints; ++i)
        {
            Vec3 p1, p2;
            p1 = (*this)[(i + 1) % numPoints];
            p2 = (*this)[i];

            p1 = Vec3(Vec4(p1, 1.0f) * mtxTranslation);
            p2 = Vec3(Vec4(p2, 1.0f) * mtxTranslation);

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            float dr = sqrt(dx * dx + dy * dy);
            float r2 = c.GetRadius() * c.GetRadius();
            float d = p1.x * p2.y - p2.x * p1.y;
            float determinant = r2 * dr * dr - d * d;

            if (determinant < 0.0)
                continue;

            Vec3 v1, v2;
            v1.x = d * dy + sgn(dy) * dx * sqrt(determinant);
            v2.x = d * dy - sgn(dy) * dx * sqrt(determinant);
            v1.y = -d * dx + abs(dy) * sqrt(determinant);
            v2.y = -d * dx - abs(dy) * sqrt(determinant);
            v1 /= (dr * dr);
            v2 /= (dr * dr);

            v1.z = v2.z = 0.0;

            float x1, x2;
            float y1, y2;
            if (p1.x < p2.x)
            {
                x1 = p1.x;
                x2 = p2.x;
            }
            else
            {
                x1 = p2.x;
                x2 = p1.x;
            }

            if (p1.y < p2.y)
            {
                y1 = p1.y;
                y2 = p2.y;
            }
            else
            {
                y1 = p2.y;
                y2 = p1.y;
            }

            bool bFirstHit = x2 >= v1.x && x1 <= v1.x && y1 <= v1.y && y2 >= v1.y;
            bool bSecondHit = x2 >= v2.x && x1 <= v2.x && y1 <= v2.y && y2 >= v2.y;

            if (bFirstHit || bSecondHit)
                bIntersect = true;
        }

        if (!bIntersect)
        {
            for (int i = 0; i < numPoints; ++i)
            {
                Vec3 p;
                p = (*this)[i];

                Vec2 p_(p.x, p.y);
                Vec2 c_(c.GetCenter().x, c.GetCenter().y);
                if (glm::length(p_ - c_) < c.GetRadius())
                    return true;
            }
        }

        return bIntersect;
    }

    //-----------------------------------------------------------------------------------
    void CPolygon::ProjectPolygonOntoAxis(const Vec3& axis, const CPolygon& polygon, float& min, float& max) const
    {
        // Projects a point on an axis use the dot product
        float dotProduct = glm::dot(axis, polygon[0]);
        min = dotProduct;
        max = dotProduct;
        for (int i = 0; i < polygon.GetNumPoints(); i++)
        {
            dotProduct = glm::dot(polygon[i], axis);
            if (dotProduct < min) 
            { 
                // dotProduct correct here???
                min = dotProduct;
            }
            else 
            {
                if (dotProduct > max) 
                {
                    max = dotProduct;
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    float CPolygon::IntervalDistance(float minA, float maxA, float minB, float maxB) const
    {
        if (minA < minB)
        {
            return minB - maxA;
        }
        else
        {
            return minA - maxB;
        }
    }
} /* End namespace */
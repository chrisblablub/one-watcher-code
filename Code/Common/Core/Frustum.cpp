#include "Frustum.h"


namespace env
{
    //-----------------------------------------------------------------------------------
    CFrustum::CFrustum()
        : m_nearClipping(1.0f),
        m_farClipping(100.0f),
        m_aspectRatio(1.0f),
        m_fov(PI * 0.75f),
        m_width(0.0f),
        m_height(0.0f) {}

    //-----------------------------------------------------------------------------------
    CFrustum::CFrustum(float n, float f, float aspectRatio, float fov)
        : m_nearClipping(n),
        m_farClipping(f),
        m_aspectRatio(aspectRatio),
        m_fov(fov),
        m_width(0.0f),
        m_height(0.0f) {}

    //-----------------------------------------------------------------------------------
    void CFrustum::ComputeFrustumPoints(Vec3* pPoints, const Vec3& vPosition,
        const Vec3& vUp, const Vec3& vRight, const Vec3& vDirection)
    {
        //ENV_ASSERT_DBG(m_fov > 0.0f && m_aspectRatio > 0.0f);

        float phi = m_fov * 0.5f;
        float hznear = m_nearClipping * std::tan(phi);
        float hzfar = m_farClipping * std::tan(phi);
        float wznear = hznear * m_aspectRatio;
        float wzfar = hzfar * m_aspectRatio;

        Vec3 nc = vPosition + m_nearClipping * vDirection;
        Vec3 fc = vPosition + m_farClipping * vDirection;

        Vec3 vFrustumPoints[8];
        // Near plane points
        vFrustumPoints[0] = nc - hznear * vUp + wznear * vRight;
        vFrustumPoints[1] = nc + hznear * vUp + wznear * vRight;
        vFrustumPoints[2] = nc + hznear * vUp - wznear * vRight;
        vFrustumPoints[3] = nc - hznear * vUp - wznear * vRight;

        // Far plane points
        vFrustumPoints[4] = fc - hzfar * vUp + wzfar * vRight;
        vFrustumPoints[5] = fc + hzfar * vUp + wzfar * vRight;
        vFrustumPoints[6] = fc + hzfar * vUp - wzfar * vRight;
        vFrustumPoints[7] = fc - hzfar * vUp - wzfar * vRight;

        for (int i = 0; i < 8; ++i)
            pPoints[i] = vFrustumPoints[i];
    }

    //-----------------------------------------------------------------------------------
    void CFrustum::GetNearHalfSize(Vec2& nearHalfSize) const
    {
        float phi = m_fov * 0.5f;
        nearHalfSize.y = m_nearClipping * std::tan(phi);
        nearHalfSize.x = nearHalfSize.y * m_aspectRatio;
    }

    //-----------------------------------------------------------------------------------
    void CFrustum::SetViewProj(const Matrix4x4& mtxViewProj)
    {
        m_mtxViewProj = mtxViewProj;

        /* Left plane: col4 + col1 */
        m_ViewFrustum[0].x = mtxViewProj[3][0] + mtxViewProj[0][0];
        m_ViewFrustum[0].y = mtxViewProj[3][1] + mtxViewProj[0][1];
        m_ViewFrustum[0].z = mtxViewProj[3][2] + mtxViewProj[0][2];
        m_ViewFrustum[0].w = mtxViewProj[3][3] + mtxViewProj[0][3];

        // Right plane: col4 - col1 
        m_ViewFrustum[1].x = mtxViewProj[3][0] - mtxViewProj[0][0];
        m_ViewFrustum[1].y = mtxViewProj[3][1] - mtxViewProj[0][1];
        m_ViewFrustum[1].z = mtxViewProj[3][2] - mtxViewProj[0][2];
        m_ViewFrustum[1].w = mtxViewProj[3][3] - mtxViewProj[0][3];

        // Bottom plane: col4 + col2 
        m_ViewFrustum[2].x = mtxViewProj[3][0] + mtxViewProj[1][0];
        m_ViewFrustum[2].y = mtxViewProj[3][1] + mtxViewProj[1][1];
        m_ViewFrustum[2].z = mtxViewProj[3][2] + mtxViewProj[1][2];
        m_ViewFrustum[2].w = mtxViewProj[3][3] + mtxViewProj[1][3];

        // Top plane: col4 - col2 
        m_ViewFrustum[3].x = mtxViewProj[3][0] - mtxViewProj[1][0];
        m_ViewFrustum[3].y = mtxViewProj[3][1] - mtxViewProj[1][1];
        m_ViewFrustum[3].z = mtxViewProj[3][2] - mtxViewProj[1][2];
        m_ViewFrustum[3].w = mtxViewProj[3][3] - mtxViewProj[1][3];

        // Far plane: col4 - col3 
        m_ViewFrustum[4].x = mtxViewProj[3][0] - mtxViewProj[2][0];
        m_ViewFrustum[4].y = mtxViewProj[3][1] - mtxViewProj[2][1];
        m_ViewFrustum[4].z = mtxViewProj[3][2] - mtxViewProj[2][2];
        m_ViewFrustum[4].w = mtxViewProj[3][3] - mtxViewProj[2][3];

        for (unsigned int i = 0; i < 5; ++i)
        {
            float length = glm::length(Vec3(m_ViewFrustum[i]));
            m_ViewFrustum[i] = Vec4(Vec3(m_ViewFrustum[i]) / length, m_ViewFrustum[i].w / length);
        }
    }

    //-----------------------------------------------------------------------------------
    void CFrustum::SetNearClipping(float nearClipping)
    {
        m_nearClipping = nearClipping;
    }

    //-----------------------------------------------------------------------------------
    void CFrustum::SetFarClipping(float farClipping)
    {
        m_farClipping = farClipping;
    }

    //-----------------------------------------------------------------------------------
    void CFrustum::SetAspectRatio(float aspectRatio)
    {
        m_aspectRatio = aspectRatio;
    }

    //-----------------------------------------------------------------------------------
    void CFrustum::SetFieldOfView(float fov)
    {
        m_fov = fov;
    }

    //-----------------------------------------------------------------------------------
    void CFrustum::SetWidth(float width)
    {
        m_width = width;
        m_aspectRatio = m_width / m_height;
    }

    //-----------------------------------------------------------------------------------
    void CFrustum::SetHeight(float height)
    {
        m_height = height;
        m_aspectRatio = m_width / m_height;
    }

} // env
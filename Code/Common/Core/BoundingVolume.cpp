#include "BoundingVolume.h"

namespace env
{
    //-----------------------------------------------------------------------------------
    CBoundingVolume::CBoundingVolume()
        : m_vOrigin(Vec3(0.0f, 0.0f, 0.0f)),
        m_vRight(1.0f, 0.0f, 0.0f),
        m_vUp(0.0f, 1.0f, 0.0f),
        m_vForward(0.0f, 0.0f, 1.0f) {}

    //-----------------------------------------------------------------------------------
    CBoundingVolume::CBoundingVolume(const CBoundingVolume& other)
        : m_vHalfLengths(other.m_vHalfLengths),
        m_vOrigin(other.m_vOrigin),
        m_vMin(other.m_vMin),
        m_vMax(other.m_vMax),
        m_vRight(other.m_vRight),
        m_vUp(other.m_vUp),
        m_vForward(other.m_vForward) {}

    //-----------------------------------------------------------------------------------
    CBoundingVolume::CBoundingVolume(const CBoundingVolume* other)
        : CBoundingVolume(*other) {}

    //-----------------------------------------------------------------------------------
    CBoundingVolume::~CBoundingVolume() {}

    //-----------------------------------------------------------------------------------
    void CBoundingVolume::GetMinMax(Vec3& vMin, Vec3& vMax)
    {
        vMin = m_vMin;
        vMax = m_vMax;
    }

    //-----------------------------------------------------------------------------------
    bool CBoundingVolume::ComputeBoundingVolume(const Vec3& vMin, const Vec3& vMax)
    {
        m_vMin = vMin;
        m_vMax = vMax;
        m_vHalfLengths = ((m_vMax - m_vMin) / 2.0f);
        m_vOrigin = m_vMin + (m_vMax - m_vMin) / 2.0f;
        return true;
    }

    //-----------------------------------------------------------------------------------
    void CBoundingVolume::Transform(const Matrix4x4& mtxTransform)
    {
        m_vMin = Vec3(Vec4(m_vMin, 1.0f) * mtxTransform);
        m_vMax = Vec3(Vec4(m_vMax, 1.0f) * mtxTransform);

        Vec3 vOrientation[3];

        vOrientation[0] = Vec3(1.0f, 0.0f, 0.0f);
        vOrientation[1] = Vec3(0.0f, 1.0f, 0.0f);
        vOrientation[2] = Vec3(0.0f, 0.0f, 1.0f);

        m_vOrigin = Vec3(Vec4(m_vOrigin, 1.0f) * mtxTransform);

        Matrix4x4 mtxTransformInvTrans = glm::transpose(glm::inverse(mtxTransform));

        m_vRight = (Vec3(Vec4(m_vRight, 0.0f) * mtxTransformInvTrans));
        m_vUp = (Vec3(Vec4(m_vUp, 0.0f) * mtxTransformInvTrans));
        m_vForward = (Vec3(Vec4(m_vForward, 0.0f) * mtxTransformInvTrans));
    }




} /* End namespace */
#include <Common/Math/Quaternion.h>
#include <Common/Math/MathUtility.h>
#include <Common/Core/Log.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CQuaternion::CQuaternion()
        : m_yaw(0.0f)
        , m_pitch(0.0f)
        , m_roll(0.0f)
    {
        m_quaternion = Quaternion(Vec3(m_pitch, m_yaw, m_roll));

        UpdateRotationMatrix();

        UpdateYawPitchRoll();
    }

    //-----------------------------------------------------------------------------------
    CQuaternion::CQuaternion(float x, float y, float z, float w)
        : m_yaw(0.0f)
        , m_roll(0.0f)
        , m_pitch(0.0f)
    {
        m_quaternion.x = x;
        m_quaternion.y = y;
        m_quaternion.z = z;
        m_quaternion.w = w;

        UpdateRotationMatrix();

        UpdateYawPitchRoll();
    }

    //-----------------------------------------------------------------------------------
    CQuaternion::CQuaternion(const Matrix4x4& mtx)
        : m_yaw(0.0f)
        , m_roll(0.0f)
        , m_pitch(0.0f)
    {
        SetRotationMatrix(mtx);
    }

    //-----------------------------------------------------------------------------------
    CQuaternion::CQuaternion(const Quaternion& quat)
        : m_quaternion(quat.w, quat.x, quat.y, quat.z)
        , m_yaw(0.0f)
        , m_roll(0.0f)
        , m_pitch(0.0f)
    {
        UpdateRotationMatrix();

        UpdateYawPitchRoll();
    }

    //-----------------------------------------------------------------------------------
    CQuaternion::CQuaternion(float yaw, float pitch, float roll)
        : m_yaw(yaw)
        , m_pitch(pitch)
        , m_roll(roll)
    {
        UpdateQuaternion();

        UpdateRotationMatrix();
    }

    //-----------------------------------------------------------------------------------
    CQuaternion::CQuaternion(const Vec3& forward, const Vec3& up)
        : m_yaw(0.0f)
        , m_pitch(0.0f)
        , m_roll(0.0f)
    {
        SetDirectionUp(forward, up);
    }

    //-----------------------------------------------------------------------------------
    CQuaternion::CQuaternion(const Vec3& forward, const Vec3& right, const Vec3& up)
        : m_yaw(0.0f)
        , m_pitch(0.0f)
        , m_roll(0.0f)
    {
        for (int i = 0; i < 3; ++i)
        {
            m_mtxRotation[i][0] = right[i];
            m_mtxRotation[i][1] = up[i];
            m_mtxRotation[i][2] = forward[i];
        }

        SetRotationMatrix(m_mtxRotation);
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::SetYaw(float yaw)
    {
        m_yaw = yaw;

        SetYawPitchRoll(m_yaw, m_pitch, m_roll);
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::SetPitch(float pitch)
    {
        m_pitch = pitch;

        SetYawPitchRoll(m_yaw, m_pitch, m_roll);
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::SetRoll(float roll)
    {
        m_roll = roll;

        SetYawPitchRoll(m_yaw, m_pitch, m_roll);
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::SetYawPitchRoll(float yaw, float pitch, float roll)
    {
        m_yaw = yaw;
        m_pitch = pitch;
        m_roll = roll;

        UpdateQuaternion();

        UpdateRotationMatrix();
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::Slerp(const CQuaternion& target, float amount)
    {
        m_quaternion = glm::slerp(target.m_quaternion, m_quaternion, amount);

        UpdateRotationMatrix();

        UpdateYawPitchRoll();
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::UpdateQuaternion()
    {
        m_quaternion = glm::angleAxis(m_yaw, Vec3(0.0f, 1.0f, 0.0f)) *
            glm::angleAxis(m_pitch, Vec3(1.0f, 0.0f, 0.0f)) *
            glm::angleAxis(m_roll, Vec3(0.0f, 0.0f, 1.0f));
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::UpdateRotationMatrix()
    {
        m_mtxRotation = glm::transpose(glm::toMat4(m_quaternion));
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::SetRotationAxis(const Vec3& vAxis, float angle)
    {
        UpdateYawPitchRoll();
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::Rotate(const Vec3& vAxis, float angle)
    {
        UpdateYawPitchRoll();
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::SetRotationMatrix(const Matrix4x4& mtx)
    {
        m_mtxRotation = mtx;

        UpdateYawPitchRoll();

        UpdateQuaternion();
    }

    //const VECTOR3& CQuaternion::GetRotationAxis()
    //{
    //	assert(false);
    //	return m_rotationAxis;
    //}

    //-----------------------------------------------------------------------------------
    float CQuaternion::GetRotationAxisAngle()
    {
        return glm::angle(m_quaternion);
    }

    //-----------------------------------------------------------------------------------
    float CQuaternion::GetLength()
    {
        return 0;
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::Normalize()
    {
        m_quaternion = glm::normalize(m_quaternion);
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::Inverse()
    {
        m_quaternion = glm::inverse(m_quaternion);
        UpdateYawPitchRoll();
    }

    //-----------------------------------------------------------------------------------
    bool CQuaternion::IsIdentity()
    {
        assert(false);
        return false;
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::SetIdentity()
    {
        m_quaternion = Quaternion(Vec3(0.0f, 0.0f, 0.0f));

        UpdateYawPitchRoll();
    }

#pragma message("--> [WARNING] Will not work for all angles!")

    //-----------------------------------------------------------------------------------
    void CQuaternion::SetDirection(const Vec3& direction)
    {
        SetDirectionUp(direction, Vec3(0.0f, 1.0f, 0.0f));
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::SetDirectionUp(const Vec3& vDirection, const Vec3& vUp)
    {
        Vec3 m_vDirection = glm::normalize(vDirection);
        Vec3 m_vUp = vUp;
        Vec3 m_vRight = Vec3(1, 0, 0);

        if (m_vDirection.z < 0.0f)
            m_vRight = -m_vRight;

        float cosAngle = glm::dot(m_vUp, m_vDirection);

        if (fabs(cosAngle) > 0.999f)
        {
            m_vUp = glm::normalize(glm::cross(m_vRight, m_vDirection));
            m_vRight = glm::normalize(glm::cross(m_vDirection, m_vUp));
        }
        else
        {
            m_vRight = glm::cross(m_vDirection, m_vUp);
            m_vUp = glm::cross(m_vDirection, m_vRight);
        }

        m_vDirection = glm::normalize(m_vDirection);
        m_vUp = glm::normalize(m_vUp);
        m_vRight = glm::normalize(m_vRight);

        for (int i = 0; i < 3; ++i)
        {
            m_mtxRotation[i][0] = m_vRight[i];
            m_mtxRotation[i][1] = m_vUp[i];
            m_mtxRotation[i][2] = m_vDirection[i];
        }

        SetRotationMatrix(m_mtxRotation);
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::RotateToDirection(const Vec3& targetDirection, float angle)
    {
        Vec3 rotationAxis = glm::cross(targetDirection, Vec3(m_mtxRotation[2]));
        rotationAxis = glm::normalize(rotationAxis);

        m_quaternion = glm::angleAxis(angle, rotationAxis);

        UpdateRotationMatrix();

        UpdateYawPitchRoll();
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::RotateAroundAxis(float radAngle, const Vec3& axis, bool bLocalCoordSys)
    {
        if (bLocalCoordSys)
            m_quaternion = m_quaternion * glm::angleAxis(radAngle, axis);
        else
            m_quaternion = glm::angleAxis(radAngle, axis) * m_quaternion;

        UpdateRotationMatrix();

        UpdateYawPitchRoll();
    }

    //-----------------------------------------------------------------------------------
    void CQuaternion::UpdateYawPitchRoll()
    {
        Matrix4x4 m = glm::transpose(m_mtxRotation);

        Vec3 localRight = glm::normalize(Vec3(m[0]));
        Vec3 localForward = glm::normalize(Vec3(m[2]));

        // Compute roll first
        Vec3 localRightBeforeRoll = glm::normalize(glm::cross(Vec3(0.0f, 1.0f, 0.0f), localForward));
        Vec3 localUpBeforeRoll = glm::normalize(glm::cross(localRightBeforeRoll, localForward));

        float dot = Clamp(glm::dot(localRightBeforeRoll, localRight), -1.0f, 1.0f);

        // project localRight onto localUpBeforeRoll to determine sign
        if (glm::dot(localRight, localUpBeforeRoll) > 0.0)
        {
            m_roll = -std::acos(dot);
        }
        else
        {
            m_roll = std::acos(dot);
        }

        m_pitch = atan2(sqrt(localUpBeforeRoll.x * localUpBeforeRoll.x +
            localUpBeforeRoll.z * localUpBeforeRoll.z), -localUpBeforeRoll.y);

        if ((localRightBeforeRoll.x > 0.0f && localUpBeforeRoll.z >= 0.0f) ||
            (localRightBeforeRoll.x <= 0.0f && localUpBeforeRoll.z < 0.0f))
        {
            // yaw smaller than +/- 90 degrees
            m_pitch = -m_pitch;
        }

        if (fabs(m_pitch) >= (PI * 0.5f))
        {
            m_yaw = atan2(-localForward.x, -localForward.z);
        }
        else
        {
            m_yaw = atan2(localForward.x, localForward.z);
        }
    }

    //-----------------------------------------------------------------------------------
    CQuaternion CQuaternion::operator*(const CQuaternion& rhs) const
    {
        return CQuaternion(m_quaternion * rhs.m_quaternion);
    }

    //-----------------------------------------------------------------------------------
    CQuaternion& CQuaternion::operator*=(const CQuaternion& rhs)
    {
        m_quaternion *= rhs.m_quaternion;

        UpdateRotationMatrix();

        UpdateYawPitchRoll();

        return *this;
    }

    //-----------------------------------------------------------------------------------
    CQuaternion& CQuaternion::operator=(const CQuaternion& rhs)
    {
        m_quaternion = rhs.m_quaternion;

        UpdateRotationMatrix();

        UpdateYawPitchRoll();

        return *this;
    }
}
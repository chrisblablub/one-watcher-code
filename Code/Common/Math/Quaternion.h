/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef QUATERNION_H
#define QUATERNION_H

#include <Common/Common.h>
#include <Common/Core/Serialize.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace env
{
    class CQuaternion
    {
    public:
        CQuaternion();
        CQuaternion(float x, float y, float z, float w);
        CQuaternion(const Quaternion& quat);
        CQuaternion(const Matrix4x4& mtx);
        CQuaternion(float yaw, float pitch, float roll);
        CQuaternion(const Vec3& forward, const Vec3& vUp = Vec3(0.0f, 1.0f, 0.0f));
        CQuaternion(const Vec3& forward, const Vec3& right, const Vec3& up);

        void SetDirection(const Vec3& direction);
        void SetDirectionUp(const Vec3& vDirection, const Vec3& vUp);

        void RotateToDirection(const Vec3& targetDirection, float angle);
        void RotateAroundAxis(float radAngle, const Vec3& axis, bool bLocalCoordSys = true);

        void SetYaw(float yaw);
        void SetPitch(float pitch);
        void SetRoll(float roll);
        void SetYawPitchRoll(float yaw, float pitch, float roll);

        inline void AddYaw(float dyaw) { SetYaw(m_yaw + dyaw); }
        inline void AddPitch(float dpitch) { SetPitch(m_pitch + dpitch); }
        inline void AddRoll(float droll) { SetRoll(m_roll + droll); }

        inline float GetYaw() const { return m_yaw; }
        inline float GetPitch() const { return m_pitch; }
        inline float GetRoll() const { return m_roll; }

        void SetRotationMatrix(const Matrix4x4& mtx);
        inline const Matrix4x4& GetRotationMatrix() const { return m_mtxRotation; }

        void Slerp(const CQuaternion& target, float amount);

        void SetRotationAxis(const Vec3& vAxis, float angle);

        void Rotate(const Vec3& vAxis, float angle);

        //const VECTOR3& GetRotationAxis();
        float GetRotationAxisAngle();
        float GetLength();
        void Normalize();
        void Inverse();
        bool IsIdentity();
        void SetIdentity();

        CQuaternion  operator* (const CQuaternion& rhs) const;
        CQuaternion& operator*=(const CQuaternion& rhs);
        CQuaternion& operator= (const CQuaternion& rhs);

        void UpdateYawPitchRoll();
        void UpdateQuaternion();
        void UpdateRotationMatrix();

    private:

        friend class boost::serialization::access;

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & m_yaw;
            ar & m_pitch;
            ar & m_roll;
        }

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            ar & m_yaw;
            ar & m_pitch;
            ar & m_roll;

            SetYawPitchRoll(m_yaw, m_pitch, m_roll);
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

    private:

        glm::quat m_quaternion;

        //VECTOR3 m_rotationAxis;
        //float m_rotationAngle;
        Matrix4x4 m_mtxRotation;

        float m_yaw;
        float m_pitch;
        float m_roll;
    };

} // env

#endif // QUATERNION_H
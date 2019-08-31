/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SCENENODE_H
#define SCENENODE_H

#include <Engine/EngineCommon.h>

namespace env
{
    class CMeshInstance;
    class CAttachableObject;

    class CSceneNode
    {
    public:
        friend class CSceneMgr;
        CSceneNode(const std::string& strName, CSceneNode* pRootNode = nullptr);
        CSceneNode();

        virtual ~CSceneNode();

    public:
        // WARNING! USE ONLY FOR NON-ENTITY-MANAGER NODES!!!!
        void SetName(const std::string& strName) { m_strName = strName; }

        typedef std::map< std::string, CSceneNode* > mapChildNodes;

        void SetOrientation(const CQuaternion& quaternion, bool bUpdateRotationMatrix = true);
        void SetPositionAndOrientation(const Vec3& vPosition, const CQuaternion& quaternion, bool bUpdateRotationMatrix = true);

        void Translate(const Vec3& vTranslation);
        void SetPosition(const Vec3& vPosition);
        void SetDerivedPosition(const Vec3& derivedPosition);
        void SetScale(const Vec3& vScale);

        Vec3 Transform(const Vec3& v);
        Vec4 Transform(const Vec4& v);

        const Vec3& GetScale();
        Vec3 GetDerivedScale();
        const Vec3& GetPosition();
        inline const Vec3& GetDerivedPosition() 
        {
            ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::GetDerivedPosition);

            //if (m_bUpdated || m_bParentUpdated)
            {
                m_vDerivedPosition = Vec3(Vec4(m_vPosition, 1.0f) * m_mtxParentRST);
            }
            return m_vDerivedPosition;
        }
        const Vec3& GetDirection();
        const Vec3& GetDerivedDirection();
        const Vec3& GetRight();
        const Vec3& GetDerivedRight();
        const Vec3& GetUp();
        const Vec3& GetDerivedUp();
        const Matrix4x4& GetST();
        const Matrix4x4& GetRST();
        const Matrix4x4& GetDerivedST();
        inline const Matrix4x4& GetDerivedRST()
        {
            ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::GetDerivedRST);

            //if (m_bUpdated || m_bParentUpdated)
            {
                /*if (!m_bParentUpdated)
                {
                    m_mtxDerivedRST = GetRST();
                }
                else if (m_pParentNode == m_pRootNode)
                {
                    m_mtxDerivedRST = GetRST();
                }
                else*/
                {
                    m_mtxDerivedRST = GetRST() * m_mtxParentRST;

                    //m_bParentUpdated = false;
                }

                //m_bUpdated = false;
            }

            return m_mtxDerivedRST;
        }

        const CQuaternion& GetOrientation();
        const CQuaternion& GetDerivedOrientation();

        void SetDebugName(const std::string& strDebugName) { m_strDebugName = strDebugName; }
        const std::string& GetDebugName() const { return m_strDebugName; }

        void MapToLocal(Vec3& v);

        void SetTransform(const Matrix4x4& mtxTransform);

        void PropagateTransform();

        const std::string& GetName();

        CSceneNode* GetParentNode();
        void AttachChildNode(CSceneNode* pChildNode);
        void DetachChildNode(const std::string& strNode);
        void DetachFromParent();

        const std::string& GetAttacheeSubMesh();
        const std::string& GetAttacheeBone();
        bool IsAttachedToBone();

        void Serialize(bool bSerialize = true);
        bool IsSerialized();

        const mapChildNodes& GetChildNodes() const { return m_childNodes; }
    private:
        static void PropagateTransform(CSceneNode* pSceneNode);

        void UpdateRotation();

        friend class boost::serialization::access;

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & m_strName;
            ar & m_strBone;
            ar & m_strSubMesh;
            ar & m_orientation;
            ar & m_vScale;
            ar & m_bAttachedToBone;
            ar & m_bSerialize;
            ar & m_vPosition;
            ar & m_pParentNode;
            ar & m_childNodes;
        }

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            ar & m_strName;
            ar & m_strBone;
            ar & m_strSubMesh;
            ar & m_orientation;
            ar & m_vScale;
            ar & m_bAttachedToBone;
            ar & m_bSerialize;
            ar & m_vPosition;
            ar & m_pParentNode;
            ar & m_childNodes;

            m_mtxR = m_orientation.GetRotationMatrix();
            m_vDirection = Vec3(Vec4(0.0f, 0.0f, 1.0f, 0.0f) * m_mtxR);
            m_vRight = Vec3(Vec4(1.0f, 0.0f, 0.0f, 0.0f) * m_mtxR);
            m_vUp = Vec3(Vec4(0.0f, 1.0f, 0.0f, 0.0f) * m_mtxR);

            m_bUpdated = true;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

    private:
        std::string m_strDebugName;

        CSceneNode* m_pParentNode;
        CSceneNode* m_pRootNode;

        mapChildNodes m_childNodes;

        std::string m_strSubMesh;
        std::string m_strBone;
        Matrix4x4 m_mtxOffset;

        std::string m_strName;

        CQuaternion m_orientation;
        CQuaternion m_derivedOrientation;
        CQuaternion m_parentOrientation;


        Matrix4x4 m_mtxParentR;
        Matrix4x4 m_mtxR;
        Matrix4x4 m_mtxParentST;
        Matrix4x4 m_mtxST;
        Matrix4x4 m_mtxWorldST;
        Matrix4x4 m_mtxParentRST;
        Matrix4x4 m_mtxRST;
        Matrix4x4 m_mtxDerivedRST;

        bool m_bUpdated;
        bool m_bParentUpdated;
        bool m_bAttachedToBone;
        bool m_bSerialize;

        Vec3 m_vPosition;
        Vec3 m_vDerivedPosition;
        Vec3 m_vDirection;
        Vec3 m_vDerivedDirection;
        Vec3 m_vRight;
        Vec3 m_vDerivedRight;
        Vec3 m_vUp;
        Vec3 m_vDerivedUp;

        Vec3 m_vScale;
        Vec3 m_vParentScale;
    };



    class CSmoothRotation
    {
    public:
        //         CQuaternionRotation()
        //             : m_direction(0.0f, 0.0f, 1.0f) {}

        //         CQuaternionRotator(const Vec3& initialDirection)
        //             : m_prevDirection(initialDirection) {}

        static void ComputeRotation(CQuaternion& m_orientation, Vec3& m_direction, const Vec3& initialDirection, const Vec3& targetDirection, float speed, float dt)
        {
            float dotProduct = Clamp(glm::dot(initialDirection, targetDirection), 0.0f, 1.0f);
            float angle = glm::acos(dotProduct);

            float param = Clamp(dt * speed / angle, 0.0f, 1.0f);

            m_direction = Lerp(initialDirection, targetDirection, param);
            m_direction = glm::normalize(m_direction);

            m_orientation = CQuaternion(m_direction);
        }

        static void ApplyRotation(CSceneNode& node, const Vec3& targetDirection, float speed, float dt)
        {
            Vec3 t;
            ApplyRotation(node, targetDirection, t, speed, dt);
        }

        static void ApplyRotation(CSceneNode& node, const Vec3& targetDirection, Vec3& resultDirection, float speed, float dt)
        {
            Vec3 initialDirection = node.GetDerivedDirection();
            float dotProduct = Clamp(glm::dot(initialDirection, targetDirection), 0.0f, 1.0f);
            float angle = glm::acos(dotProduct);

            float param = Clamp(dt * speed / angle, 0.0f, 1.0f);

            resultDirection = Lerp(initialDirection, targetDirection, param);
            resultDirection = glm::normalize(resultDirection);

            node.SetOrientation(CQuaternion(resultDirection));
        }

        //         inline const Vec3& GetDirection() const { return m_direction; }
        //         inline const CQuaternion& GetQuaternion() const { return m_orientation; }

    private:
        //          CQuaternion m_orientation;
        //          Vec3 m_direction;
    };

} /* End namespace */

#endif /* SCENENODE_H */
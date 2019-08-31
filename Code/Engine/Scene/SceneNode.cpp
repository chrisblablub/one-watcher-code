#include <Engine/Scene/SceneNode.h>
#include <Engine/Core/AttachableObject.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CSceneNode::CSceneNode(const std::string& strName, CSceneNode* pRootNode)
        : m_pParentNode(nullptr),
        m_pRootNode(pRootNode),
        m_vPosition(0.0f, 0.0f, 0.0f),
        m_vDerivedPosition(0.0f, 0.0f, 0.0f),
        m_vDirection(0.0f, 0.0f, 1.0f),
        m_vDerivedDirection(0.0f, 0.0f, 1.0f),
        m_vUp(0.0f, 1.0f, 0.0f),
        m_vDerivedUp(0.0f, 1.0f, 0.0f),
        m_vRight(1.0f, 0.0f, 0.0f),
        m_vDerivedRight(1.0f, 0.0f, 0.0f),
        m_vScale(1.0, 1.0, 1.0),
        m_vParentScale(1.0f, 1.0, 1.0),
        m_bParentUpdated(true),
        m_bUpdated(true),
        m_bAttachedToBone(false),
        m_bSerialize(true),
        m_strName(strName)
    {
        m_mtxParentR = Matrix4x4(1.0f);
        m_mtxR = Matrix4x4(1.0f);
        m_mtxST = Matrix4x4(1.0f);
        m_mtxWorldST = Matrix4x4(1.0f);
        m_mtxParentST = Matrix4x4(1.0f);
        m_mtxRST = Matrix4x4(1.0f);
        m_mtxDerivedRST = Matrix4x4(1.0f);
        m_mtxParentRST = Matrix4x4(1.0f);
        m_mtxOffset = Matrix4x4(1.0f);
    }

    //-----------------------------------------------------------------------------------
    CSceneNode::CSceneNode()
        : m_pParentNode(nullptr),
        m_pRootNode(nullptr),
        m_vPosition(0.0f, 0.0f, 0.0f),
        m_vDerivedPosition(0.0f, 0.0f, 0.0f),
        m_vDirection(0.0f, 0.0f, 1.0f),
        m_vDerivedDirection(0.0f, 0.0f, 1.0f),
        m_vUp(0.0f, 1.0f, 0.0f),
        m_vDerivedUp(0.0f, 1.0f, 0.0f),
        m_vRight(1.0f, 0.0f, 0.0f),
        m_vDerivedRight(1.0f, 0.0f, 0.0f),
        m_vScale(1.0, 1.0, 1.0),
        m_vParentScale(1.0f, 1.0, 1.0),
        m_bParentUpdated(true),
        m_bUpdated(true),
        m_bAttachedToBone(false),
        m_bSerialize(true)
    {
        m_mtxParentR = Matrix4x4(1.0f);
        m_mtxR = Matrix4x4(1.0f);
        m_mtxST = Matrix4x4(1.0f);
        m_mtxWorldST = Matrix4x4(1.0f);
        m_mtxParentST = Matrix4x4(1.0f);
        m_mtxRST = Matrix4x4(1.0f);
        m_mtxDerivedRST = Matrix4x4(1.0f);
        m_mtxParentRST = Matrix4x4(1.0f);
        m_mtxOffset = Matrix4x4(1.0f);
    }

    //-----------------------------------------------------------------------------------
    CSceneNode::~CSceneNode()
    {
        DetachFromParent();

        for (mapChildNodes::iterator it = m_childNodes.begin(); it != m_childNodes.end(); ++it)
        {
            CSceneNode* pChild = it->second;

            pChild->m_pParentNode = NULL;
            pChild->m_parentOrientation.SetIdentity();
            pChild->m_vParentScale = Vec3(1.0, 1.0, 1.0);

            pChild->m_mtxParentST = Matrix4x4(1.0f);
            pChild->m_mtxParentRST = Matrix4x4(1.0f);
        }

    }


    //-----------------------------------------------------------------------------------
    void CSceneNode::SetOrientation(const CQuaternion& quaternion, bool bUpdateRotationMatrix)
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::SetOrientation);

        m_orientation = quaternion;
        if (bUpdateRotationMatrix)
            m_mtxR = m_orientation.GetRotationMatrix();

        m_vDirection = Vec3(Vec4(0.0f, 0.0f, 1.0f, 0.0f) * m_mtxR);
        m_vRight = Vec3(Vec4(1.0f, 0.0f, 0.0f, 0.0f) * m_mtxR);
        m_vUp = Vec3(Vec4(0.0f, 1.0f, 0.0f, 0.0f) * m_mtxR);

        m_bUpdated = true;
        PropagateTransform();
    }

    //-----------------------------------------------------------------------------------
    void CSceneNode::Translate(const Vec3& vTranslation)
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::Translate);

        m_vPosition += vTranslation;

        m_bUpdated = true;
        PropagateTransform();
    }

    //-----------------------------------------------------------------------------------
    void CSceneNode::SetPosition(const Vec3& vPosition)
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::SetPosition);

        m_vPosition = vPosition;

        m_bUpdated = true;
        PropagateTransform();
    }

    void CSceneNode::SetPositionAndOrientation(const Vec3& vPosition, const CQuaternion& quaternion, bool bUpdateRotationMatrix)
    {
        m_vPosition = vPosition;
        m_orientation = quaternion;
        if (bUpdateRotationMatrix)
            m_mtxR = m_orientation.GetRotationMatrix();

        m_vDirection = Vec3(Vec4(0.0f, 0.0f, 1.0f, 0.0f) * m_mtxR);
        m_vRight = Vec3(Vec4(1.0f, 0.0f, 0.0f, 0.0f) * m_mtxR);
        m_vUp = Vec3(Vec4(0.0f, 1.0f, 0.0f, 0.0f) * m_mtxR);

        m_bUpdated = true;
        PropagateTransform();
    }

    //-----------------------------------------------------------------------------------
    void CSceneNode::SetDerivedPosition(const Vec3& derivedPosition)
    {
        Matrix4x4 derivedRST = glm::inverse(m_mtxParentRST);

        Vec3 localPosition = Vec3(Vec4(derivedPosition, 1.0f) * derivedRST);

        SetPosition(localPosition);
    }

    //-----------------------------------------------------------------------------------
    void CSceneNode::MapToLocal(Vec3& v)
    {
        Matrix4x4 derivedRST = glm::inverse(GetDerivedRST());

        v = Vec3(Vec4(v, 1.0f) * derivedRST);
    }

    //-----------------------------------------------------------------------------------
    void CSceneNode::SetScale(const Vec3& vScale)
    {
        m_vScale = vScale;

        m_bUpdated = true;

        PropagateTransform();
    }

    //-----------------------------------------------------------------------------------
    Vec3 CSceneNode::Transform(const Vec3& v)
    {
        return Vec3(Vec4(v, 1.0f) * GetDerivedRST());
    }

    //-----------------------------------------------------------------------------------
    Vec4 CSceneNode::Transform(const Vec4& v)
    {
        return v * GetDerivedRST();
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CSceneNode::GetScale()
    {
        return m_vScale;
    }

    //-----------------------------------------------------------------------------------
    Vec3 CSceneNode::GetDerivedScale()
    {
        return m_vScale * m_vParentScale;
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CSceneNode::GetPosition()
    {
        return m_vPosition;
    }


    //-----------------------------------------------------------------------------------
    const Vec3& CSceneNode::GetDirection()
    {
        return m_vDirection;
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CSceneNode::GetDerivedDirection()
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::GetDerivedDirection);

        //if (m_bUpdated || m_bParentUpdated)
        {
            m_vDerivedDirection = glm::normalize(Vec3(Vec4(0.0f, 0.0f, 1.0f, 0.0f) * GetDerivedRST()));
        }
        return m_vDerivedDirection;
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CSceneNode::GetRight()
    {
        return m_vRight;
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CSceneNode::GetDerivedRight()
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::GetDerivedRight);

        //if (m_bUpdated || m_bParentUpdated)
        {
            m_vDerivedRight = glm::normalize(Vec3(Vec4(1.0f, 0.0f, 0.0f, 0.0f) * GetDerivedRST()));
        }
        return m_vDerivedRight;
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CSceneNode::GetUp()
    {
        return m_vUp;
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CSceneNode::GetDerivedUp()
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::GetDerivedUp);

        //if (m_bUpdated || m_bParentUpdated)
        {
            m_vDerivedUp = glm::normalize(Vec3(Vec4(.0f, 1.0f, 0.0f, 0.0f) * GetDerivedRST()));
        }
        return m_vDerivedUp;
    }

    //-----------------------------------------------------------------------------------
    const Matrix4x4& CSceneNode::GetST()
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::GetST);

        if (m_bUpdated)
        {
            if (m_vScale[0] != 1 || m_vScale[1] != 1 || m_vScale[2] != 1)
            {
                Matrix4x4 mtxScale = glm::transpose(glm::scale(Matrix4x4(1.0f), m_vScale));
                Matrix4x4 mtxTranslation = math::translation(m_vPosition);
                m_mtxST = mtxScale * mtxTranslation;
            }
            else
            {
                m_mtxST = math::translation(m_vPosition);
            }
        }

        return m_mtxST;
    }

    //-----------------------------------------------------------------------------------
    const Matrix4x4& CSceneNode::GetRST()
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::GetRST);

        //if (m_bUpdated)
        {
            Matrix4x4 mtxTranslation = math::translation(m_vPosition);

            if (m_vScale[0] != 1 || m_vScale[1] != 1 || m_vScale[2] != 1)
            {
                m_mtxRST = math::scaling(m_vScale) * m_orientation.GetRotationMatrix() * mtxTranslation;
            }
            else
            {
                m_mtxRST = m_orientation.GetRotationMatrix() * mtxTranslation;
            }
        }

        return m_mtxRST;
    }

    //-----------------------------------------------------------------------------------
    const Matrix4x4& CSceneNode::GetDerivedST()
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::GetDerivedST);

        if (m_bUpdated)
        {
            if (//m_pParentNode == nullptr ||
                m_pParentNode == m_pRootNode)
            {
                m_mtxWorldST = GetST();
            }
            else if (m_bParentUpdated)
            {
                m_mtxWorldST = GetST() * m_mtxParentST;

                m_bParentUpdated = false;
            }
        }

        return m_mtxWorldST;
    }

    //-----------------------------------------------------------------------------------
    

    //-----------------------------------------------------------------------------------
    const CQuaternion& CSceneNode::GetOrientation()
    {
        return m_orientation;
    }

    //-----------------------------------------------------------------------------------
    const CQuaternion& CSceneNode::GetDerivedOrientation()
    {
        if (//m_pParentNode == nullptr ||
            m_pParentNode == m_pRootNode)
        {
            m_derivedOrientation = m_orientation;
        }
        else
        {
            m_derivedOrientation = m_orientation * m_parentOrientation;
        }

        return m_derivedOrientation;
    }

    void CSceneNode::SetTransform(const Matrix4x4& mtxTransform)
    {
        assert(false);

        m_mtxRST = mtxTransform;
        m_vDirection = Vec3(0.0f, 0.0f, 1.0f);
        m_vUp = Vec3(0.0f, 1.0f, 0.0f);
        m_vRight = Vec3(1.0f, 0.0f, 0.0f);

        Matrix4x4 mtxInvTransform;
        mtxInvTransform = glm::inverse(m_mtxRST);
        mtxInvTransform = glm::transpose(m_mtxRST);

        m_vDirection = Vec3(Vec4(m_vDirection, 1.0f) * mtxInvTransform);
        m_vDirection = glm::normalize(m_vDirection);

        m_vRight = Vec3(Vec4(m_vRight, 1.0f) * mtxInvTransform);
        m_vRight = glm::normalize(m_vRight);

        m_vUp = glm::cross(m_vDirection, m_vRight);
        m_vUp = glm::normalize(m_vUp);


        m_vScale.x = 1.0;
        m_vScale.y = 1.0;
        m_vScale.z = 1.0;
        m_vPosition = Vec3(Vec4(0.0f, 0.0f, 0.0f, 1.0f) * m_mtxRST);

        m_orientation = CQuaternion(Quaternion(mtxTransform));

        m_bUpdated = true;

        PropagateTransform();
    }

    void CSceneNode::PropagateTransform()
    {
        PropagateTransform(this);
    }

    const std::string& CSceneNode::GetName()
    {
        return m_strName;
    }

    CSceneNode* CSceneNode::GetParentNode()
    {
        return m_pParentNode;
    }

    void CSceneNode::AttachChildNode(CSceneNode* pChildNode)
    {
        if (pChildNode->m_pParentNode)
        {
            pChildNode->m_pParentNode->DetachChildNode(pChildNode->GetName());
        }

        pChildNode->m_pParentNode = this;

        m_mtxWorldST = GetDerivedST();
        m_mtxDerivedRST = GetDerivedRST();
        m_derivedOrientation = GetDerivedOrientation();

        pChildNode->m_mtxParentRST = m_mtxDerivedRST;
        pChildNode->m_mtxParentST = m_mtxWorldST;
        pChildNode->m_parentOrientation = m_derivedOrientation;
        pChildNode->m_vParentScale = Vec3(m_vScale.x * m_vParentScale.x, m_vScale.y * m_vParentScale.y, m_vScale.z * m_vParentScale.z);
        pChildNode->m_bUpdated = true;


        PropagateTransform(pChildNode);

        m_childNodes[pChildNode->GetName()] = pChildNode;
    }

    void CSceneNode::DetachChildNode(const std::string& strNode)
    {
        mapChildNodes::iterator it = m_childNodes.find(strNode);
        if (it == m_childNodes.end())
        {
            ENV_LOG_WARNING("The node '" + strNode + "' cannot be detached because it is not a child of this node.");
        }
        else
        {
            it->second->m_pParentNode = NULL;
            it->second->m_parentOrientation.SetIdentity();
            it->second->m_vParentScale = Vec3(1.0, 1.0, 1.0);
            it->second->m_mtxParentST = Matrix4x4(1.0f);
            it->second->m_mtxParentRST = Matrix4x4(1.0f);

            CSceneNode* pNode = it->second;
            m_childNodes.erase(it);

            PropagateTransform(pNode);
        }
    }

    const std::string& CSceneNode::GetAttacheeSubMesh()
    {
        return m_strSubMesh;
    }

    const std::string& CSceneNode::GetAttacheeBone()
    {
        return m_strBone;
    }

    bool CSceneNode::IsAttachedToBone()
    {
        return m_bAttachedToBone;
    }

    void CSceneNode::Serialize(bool bSerialize)
    {
        m_bSerialize = bSerialize;
    }

    bool CSceneNode::IsSerialized()
    {
        return m_bSerialize;
    }

    void CSceneNode::DetachFromParent()
    {
        if (m_pParentNode)
        {
            mapChildNodes::iterator it = m_pParentNode->m_childNodes.find(m_strName);
            if (it == m_pParentNode->m_childNodes.end())
            {
                ENV_LOG_WARNING("The node '" + m_strName + "' cannot be detached because it is not a child of this node.");
            }
            else
            {
                m_pParentNode->m_childNodes.erase(it);
            }
        }

        m_pParentNode = NULL;
        m_parentOrientation.SetIdentity();
        m_vParentScale = Vec3(1.0f, 1.0f, 1.0f);
        m_mtxParentST = Matrix4x4(1.0f);
        m_mtxParentRST = Matrix4x4(1.0f);
    }

    void CSceneNode::PropagateTransform(CSceneNode* pSceneNode)
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(SceneNodes, CSceneNode::PropagateTransform);
        ENV_SCOPED_ACCUM_CPU_PROFILING(PropTrans, CSceneNode::PropagateTransform);

        if (pSceneNode->m_childNodes.size() == 0)
        {
            return;
        }

        pSceneNode->m_mtxWorldST = pSceneNode->GetDerivedST();
        pSceneNode->m_derivedOrientation = pSceneNode->GetDerivedOrientation();
        pSceneNode->m_mtxDerivedRST = pSceneNode->GetDerivedRST();

        for (mapChildNodes::iterator it = pSceneNode->m_childNodes.begin(); it != pSceneNode->m_childNodes.end(); ++it)
        {
            CSceneNode* pChildNode = it->second;
            pChildNode->m_mtxParentST = pSceneNode->m_mtxWorldST;
            pChildNode->m_mtxParentRST = pSceneNode->m_mtxDerivedRST;
            pChildNode->m_parentOrientation = pSceneNode->m_derivedOrientation;
            pChildNode->m_vParentScale = Vec3(pSceneNode->m_vScale.x * pSceneNode->m_vParentScale.x,
                                              pSceneNode->m_vScale.y * pSceneNode->m_vParentScale.y,
                                              pSceneNode->m_vScale.z * pSceneNode->m_vParentScale.z);
            pChildNode->m_bParentUpdated = true;

            PropagateTransform(pChildNode);
        }
    }


}
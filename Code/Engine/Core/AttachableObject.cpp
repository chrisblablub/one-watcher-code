#include <Engine/Core/AttachableObject.h>

namespace env
{
    void CAttachableObject::DetachNode()
    {
        m_pNode = nullptr;
    }

    CSceneNode* CAttachableObject::GetAttachedNode()
    {
        return m_pNode;
    }

    void CAttachableObject::AttachNode(CSceneNode* pNode)
    {
        m_pNode = pNode;
    }
} // env
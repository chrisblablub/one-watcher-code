#include <Engine/Core/ResolutionMgr.h>

namespace env
{
    const Vec2 CResolutionMgr::REFERENCE_RESOLUTION = Vec2(1680.0f, 1050.0f);

    CResolutionMgr::CResolutionMgr()
        : m_vCurResolution(1680.0f, 1050.0f)
    {

    }

    CResolutionMgr::~CResolutionMgr()
    {

    }

    void CResolutionMgr::RegisterNode(CSceneNode* pNode, float ratio, bool bScale)
    {
        NodeResInfo info(pNode, pNode->GetPosition(), ratio, bScale);
        ApplyResolution(info, REFERENCE_RESOLUTION, m_vCurResolution);

        m_nodes.push_back(info);
    }

    void CResolutionMgr::UnregisterNode(CSceneNode* pNode)
    {
        for (std::list< NodeResInfo >::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
        {
            NodeResInfo info = *it;
            if (info.pNode == pNode)
            {
                m_nodes.erase(it);
                break;
            }
        }
    }

    void CResolutionMgr::Update(const Vec2& vNewRes)
    {
        for (std::list< NodeResInfo >::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
        {
            NodeResInfo& info = *it;
            ApplyResolution(info, m_vCurResolution, vNewRes);
        }

        m_vCurResolution = vNewRes;
    }

    void CResolutionMgr::ApplyResolution(NodeResInfo& info, const Vec2& vCurRes, const Vec2& vNewRes)
    {
        if (info.bScale)
        {
            float scaleX = vNewRes.x / REFERENCE_RESOLUTION.x;
            float scaleY = vNewRes.y / REFERENCE_RESOLUTION.y;
            if (scaleX < scaleY)
            {
                scaleY = scaleX / info.ratio;
            }
            else
            {
                scaleX = scaleY * info.ratio;
            }
            info.pNode->SetScale(Vec3(scaleX, scaleY, 1.0f));
        }

        Vec3 vPos = info.pNode->GetPosition();
        Vec3 vLogicalPos(vPos.x / vCurRes.x, vPos.y / vCurRes.y, vPos.z);
        Vec3 vNewPos(vLogicalPos.x * vNewRes.x, vLogicalPos.y * vNewRes.y, vPos.z);
        info.pNode->SetPosition(vNewPos);
    }
} // env
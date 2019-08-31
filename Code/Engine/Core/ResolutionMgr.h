/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef RESOLUTION_MGR_H
#define RESOLUTION_MGR_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/SceneNode.h>
#include <time.h>

namespace env
{
    struct NodeResInfo
    {
        NodeResInfo(CSceneNode* pNode, const Vec3& vPos, float ratio, bool bScale)
            : pNode(pNode), vPos(vPos), ratio(ratio), bScale(bScale) {}

        CSceneNode* pNode;
        Vec3 vPos;
        float ratio;
        bool bScale;
    };

    class CResolutionMgr
    {
        friend class CSingletonStatic< CResolutionMgr >;

    private:
        CResolutionMgr();

    public:
        static const Vec2 REFERENCE_RESOLUTION;

        ~CResolutionMgr();

        void RegisterNode(CSceneNode* pNode, float ratio, bool bScale = true);
        void UnregisterNode(CSceneNode* pNode);
        void Update(const Vec2& vNewRes);

    private:
        void ApplyResolution(NodeResInfo& info, const Vec2& vCurRes, const Vec2& vNewRes);

    private:
        std::list< NodeResInfo > m_nodes;
        Vec2 m_vCurResolution;
    };
    typedef CSingletonStatic< CResolutionMgr > CSingletonResolutionMgr;

} // env

#endif // RESOLUTION_MGR_H
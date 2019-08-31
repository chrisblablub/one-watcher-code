/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once
#ifndef ATTACHABLE_OBJECT_H
#define ATTACHABLE_OBJECT_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/SceneNode.h>

namespace env
{
    class CAttachableObject
    {
    public:
        CAttachableObject()
            : m_pNode(0) {}

        void DetachNode();
        CSceneNode* GetAttachedNode();
        void AttachNode(CSceneNode* pNode);

        //protected:
        //	friend class CSceneNode;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_pNode;
        }


    protected:
        CSceneNode* m_pNode;
    };
} // env

#endif // ATTACHABLE_OBJECT_H
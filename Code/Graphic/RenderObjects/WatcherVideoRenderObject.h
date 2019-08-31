/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef WATCHER_VIDEO_RENDER_OBJECT_H
#define WATCHER_VIDEO_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/RenderObjects/MeshRenderObject.h>

namespace env
{
    struct WatcherVideoRenderObject
    {
        InstanceAndMeshesRenderObject m_meshesRenderObject;

        Matrix4x4 m_mtxTransform;

        float m_illumination;

    };

} // env
#endif // WATCHER_VIDEO_RENDER_OBJECT_H 
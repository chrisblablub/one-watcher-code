/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef HALO_RENDER_OBJECT_H
#define HALO_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/RenderObjects/MeshRenderObject.h>

namespace env
{
    struct HaloRenderObject
    {
        mapMeshInstanceRenderObjects m_meshInstances;

        float m_haloIntensityLeft;
        float m_haloIntensityRight;

    };

} // env
#endif // HALO_RENDER_OBJECT_H 
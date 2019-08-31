/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SKY_DOME_RENDER_OBJECT_H
#define SKY_DOME_RENDER_OBJECT_H

namespace env
{
    struct SkyDomeRenderObject
    {
        SkyDomeRenderObject()
            : m_pSkyTexture(nullptr) {}

        mapMeshInstanceRenderObjects m_meshInstances;

        CTextureManaged* m_pSkyTexture;

    };

} // env
#endif // SKY_DOME_RENDER_OBJECT_H 
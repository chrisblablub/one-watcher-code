/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SKY_BOX_RENDER_OBJECT_H
#define SKY_BOX_RENDER_OBJECT_H

namespace env
{
    struct SkyBoxRenderObject
    {
        SkyBoxRenderObject()
            : m_pSkyTexture(nullptr) 
            , m_mtxTransform(1.0f) {}

        CTextureManaged* m_pSkyTexture;

        Matrix4x4 m_mtxTransform;

    };

} // env
#endif // SKY_BOX_RENDER_OBJECT_H 
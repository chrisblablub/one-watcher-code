/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SPRITE_RENDER_OBJECT_H
#define SPRITE_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/Texture.h>

namespace env
{
    enum SPRITE_PIPELINE
    {
        SPRITE_PIPELINE_BEFORE_POSTPROCESSING,
        SPRITE_PIPELINE_AFTER_POSTPROCESSING,
        SPRITE_PIPELINE_AFTER_FADE_SCREEN,
        SPRITE_PIPELINE_AFTER_FADE
    };

    struct SpriteRenderObject
    {
        SpriteRenderObject()
            : m_mtxRST(1.0f)
            , m_mtxRSTPre(1.0f)
            , m_texRect(0.0f, 0.0f, 1.0f, 1.0f)
            , m_color(1.0f, 1.0f, 1.0f, 1.0f)
            , m_pTexture(nullptr)
            , m_bAlphaBlending(true)
            , m_bBillboard(true)
            , m_bScreenSpace(true)
            , m_bDepthTest(true)
            , m_pipeline(SPRITE_PIPELINE_BEFORE_POSTPROCESSING) {}

        void SetPosition(const Vec3& pos)
        {
            m_mtxRST[0][3] = pos[0];
            m_mtxRST[1][3] = pos[1];
            m_mtxRST[2][3] = pos[2];
        }

        Matrix4x4 m_mtxRST;
        Matrix4x4 m_mtxRSTPre;
        Vec4 m_color;
        Vec4 m_texRect;

        CTextureBase* m_pTexture;

        SPRITE_PIPELINE m_pipeline;

        bool m_bAlphaBlending;
        bool m_bBillboard;
        bool m_bScreenSpace;
        bool m_bDepthTest;
    };

    typedef std::list< SpriteRenderObject > listSpriteRenderObjects;

} // env
#endif // SPRITE_RENDER_OBJECT_H 
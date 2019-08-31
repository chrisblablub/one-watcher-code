/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef COLLIDABLE_RENDERER_H
#define COLLIDABLE_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/GraphicConfig.h>
#include <Graphic/Renderer/BaseRenderer.h>

namespace env
{
    class CVertexBuffer;
    class CGeometryCollidable;

    class CCollidableRenderer : public CBaseRenderer
    {
    public:
        CCollidableRenderer();
        ~CCollidableRenderer();

        void Render(const Matrix4x4& mtxView);
        void AddCollidable(CGeometryCollidable* p);

    private:
        CVertexBuffer* m_pVB;
        std::list< CGeometryCollidable* > m_collidables;
        bool m_bVisible;
    };
} // env

#endif // COLLIDABLE_RENDERER_H 
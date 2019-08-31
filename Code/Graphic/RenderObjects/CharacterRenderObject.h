/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CHARACTER_RENDER_OBJECT_H
#define CHARACTER_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/RenderObjects/SpriteRenderObject.h>
namespace env
{
    class CSprite;

    struct CharacterRenderObject
    {
        Vec3 m_position;

        listSpriteRenderObjects m_sprites;
    };

    struct Character3DRenderObject
    {
        Character3DRenderObject()
            : m_charMtrl(1.0f, 1.0f, 1.0f)
            , m_charParams(1.0f, 1.0f)
            , m_position(0.0f, 0.0f, 0.0f)
            , m_bRenderCells(true) {}

        Vec3 m_charMtrl;
        Vec2 m_charParams;
        Vec3 m_position;

        bool m_bRenderCells;
    };

} // env
#endif // CHARACTER_RENDER_OBJECT_H 
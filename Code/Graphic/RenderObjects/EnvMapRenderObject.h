/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef ENV_MAP_RENDER_OBJECT_H
#define ENV_MAP_RENDER_OBJECT_H

namespace env
{
    struct EnvMapRenderObject
    {
        EnvMapRenderObject()
            : m_pEnvMap(nullptr) {}

        CTextureManaged* m_pEnvMap;

    };

} // env
#endif // ENV_MAP_RENDER_OBJECT_H 
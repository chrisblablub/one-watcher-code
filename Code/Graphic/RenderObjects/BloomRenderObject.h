/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef BLOOM_RENDER_OBJECT_H
#define BLOOM_RENDER_OBJECT_H

namespace env
{
    struct BloomRenderObject
    {
        BloomRenderObject()
            : m_bloomThreshold(1.0f),
            m_bloomScale(1.0f) {}

        float m_bloomThreshold;
        float m_bloomScale;
    };

} // env
#endif // BLOOM_RENDER_OBJECT_H 
/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PARTICLE_SYSTEM_RENDER_OBJECT_H
#define PARTICLE_SYSTEM_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>

namespace env
{
    class CManagedMaterial;
    class CVertexBuffer;

    struct ParticleSystemRenderObject
    {
        ParticleSystemRenderObject()
            : m_numTotalParticlesAlive(0)
            , m_pMaterial(nullptr)
            , m_pVertexBuffer(nullptr) {}


        unsigned int m_numTotalParticlesAlive;

        CManagedMaterial* m_pMaterial;
        
        CVertexBuffer* m_pVertexBuffer;
    };

    typedef std::list< ParticleSystemRenderObject* > listParticleSystemRenderObjects;
    typedef std::map< std::string, listParticleSystemRenderObjects > mapParticleSystemRenderObjects;

} // env
#endif // SPRITE_RENDER_OBJECT_H 
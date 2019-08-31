/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_TEXTUREBASE_H
#define D3D9_TEXTUREBASE_H


#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D9/D3D9Config.h>
#include <Graphic/Core/TextureSampler.h>

namespace env
{
    struct TextureRect
    {
        TextureRect()
            : left(0.0f),
            top(0.0f),
            right(1.0f),
            bottom(1.0f) {}

        TextureRect(float left, float top, float right, float bottom)
            : left(left), top(top), right(right), bottom(bottom) {}

        float left, top, right, bottom;
    };

    class CD3D9TextureBase
    {
        friend class CD3D9RenderDevice;

    public:
        CD3D9TextureBase(IDirect3DDevice9* pDevice);
        CD3D9TextureBase(IDirect3DDevice9* pDevice, IDirect3DBaseTexture9* pTex, 
            int width, int height)
            : m_pDevice(pDevice), 
            m_pTexture(pTex),
            m_width(width), m_height(height) {}

        virtual ~CD3D9TextureBase();

        virtual int GetWidth() const { return m_width; }
        virtual int GetHeight() const { return m_height; }

        void SetTextureSampler(const CTextureSampler& sampler) { m_sampler = sampler; }
        const CTextureSampler& GetTextureSampler() const { return m_sampler; }

    protected:

        void Bind(unsigned int unit);

    protected:

        int m_width;
        int m_height;

        IDirect3DBaseTexture9* m_pTexture;
        IDirect3DDevice9* m_pDevice;

        CTextureSampler m_sampler;
        
    };

    typedef CD3D9TextureBase CTextureBase;

} // env



#endif // D3D9_TEXTUREBASE_H
#endif // D3D9
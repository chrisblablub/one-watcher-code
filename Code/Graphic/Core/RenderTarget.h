/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/APITypes.h>
#include <Graphic/Core/TextureSampler.h>
#include <Graphic/GraphicConfig.h>

namespace env
{
    class CBaseRenderTarget
    {
    public:
        CBaseRenderTarget(int width, int height, TEXTUREFORMAT_TYPE format, bool bMipMaps = false);
        virtual ~CBaseRenderTarget();

        virtual void OnLostDevice() {}
        virtual void OnResetDevice() {}

        void SetTextureSampler(const CTextureSampler& sampler) { m_textureSampler = sampler; }
        const CTextureSampler& GetTextureSampler() const { return m_textureSampler; }

        int GetWidth();
        int GetHeight();
        
        void SetWidth( int width );
        void SetHeight( int height );
        void SetSize( unsigned int width, unsigned int height );
        void ApplyChanges();

    protected:

        std::string m_strName;

        int m_key;
        int m_width;
        int m_height;
        int m_targetPass;
        int m_renderPass;

        bool m_bMipMaps;

        TEXTUREFORMAT_TYPE m_format;

        CTextureSampler m_textureSampler;
    };

} // env

#include GRAPHIC_API_RENDERTARGET

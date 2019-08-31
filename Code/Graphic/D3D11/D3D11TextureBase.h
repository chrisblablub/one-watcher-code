/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_TEXTUREBASE_H
#define D3D11_TEXTUREBASE_H


#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D11/D3D11Config.h>
#include <Graphic/Core/TextureSampler.h>
#include <Graphic/Core/TextureRect.h>

namespace env
{

    class CTextureBase
    {
        friend class CD3D11RenderDevice;

    public:
        CTextureBase(ID3D11Device* pDevice,
                          ID3D11DeviceContext* pDeviceContext);

        CTextureBase(ID3D11Device* pDevice, 
                          ID3D11DeviceContext* pDeviceContext,
                          ID3D11Texture2D* pTex,
                          ID3D11ShaderResourceView* pSRV,
                          int width, int height);

        virtual ~CTextureBase();

        virtual int GetWidth() const { return m_width; }
        virtual int GetHeight() const { return m_height; }

        void SetTextureSampler(const CTextureSampler& sampler) { m_sampler = sampler; }
        const CTextureSampler& GetTextureSampler() const { return m_sampler; }
        
        ID3D11ShaderResourceView* GetSRV() const { return m_pTextureSRV; }

    protected:

        int m_width;
        int m_height;

        ID3D11Resource* m_pTexture;
        ID3D11ShaderResourceView* m_pTextureSRV;
        ID3D11DeviceContext* m_pDeviceContext;
        ID3D11Device* m_pDevice;

        CTextureSampler m_sampler;

    };

} // env
#endif // D3D11_TEXTUREBASE_H
#endif // ENV_GRAPHIC_API_D3D11

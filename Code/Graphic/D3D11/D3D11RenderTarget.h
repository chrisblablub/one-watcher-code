/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/RenderTarget.h>
#include <Graphic/D3D11/D3D11Config.h>


namespace env
{
    class CTextureBase;

    class CRenderTarget : public CBaseRenderTarget
    {
        friend class CRenderDevice;

    public:
        CRenderTarget(ID3D11Device* pDevice, 
                      ID3D11DeviceContext* pDeviceContext, 
                      ID3D11Texture2D* pSurface, 
                      bool bCreateDepthStencil);

        CRenderTarget(ID3D11Device* pDevice, 
                      ID3D11DeviceContext* pDeviceContext, 
                      int width, 
                      int height, 
                      TEXTUREFORMAT_TYPE format, 
                      bool bCreateDepthStencil, bool 
                      bMipMaps = false);

        virtual ~CRenderTarget();

        virtual void OnLostDevice();
        virtual void OnResetDevice();

        virtual CTextureBase* GetTexture() const { return m_pTexture; }

        ID3D11RenderTargetView* GetRenderTargetView() const { return m_pTextureRenderTargetView; }
        ID3D11DepthStencilView* GetDepthStencilView() const { return m_pDepthStencilView; }

    private:

        void CreateDepthStencilBuffer(unsigned int width, unsigned int height);
        void CreateTextureBuffer(int width, int height, TEXTUREFORMAT_TYPE format);

    private:
        
        ID3D11Device* m_pDevice;
        ID3D11DeviceContext* m_pDeviceContext;
        ID3D11Texture2D* m_pTextureBuffer;
        ID3D11Texture2D* m_pDepthStencilBuffer;
        ID3D11RenderTargetView* m_pTextureRenderTargetView;
        ID3D11DepthStencilView* m_pDepthStencilView;

        CTextureBase* m_pTexture;

        int m_unit;

        bool m_bTextureCreated;
        bool m_bDepthStencilCreate;
    };
} // env

#endif // ENV_GRAPHIC_API_D3D11

/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/RenderTarget.h>
#include <Graphic/D3D9/D3D9Config.h>


namespace env
{
    class CD3D9TextureBase;

    class CD3D9RenderTarget : public CBaseRenderTarget
    {
        friend class CD3D9RenderDevice;
    
    public:
        CD3D9RenderTarget(IDirect3DDevice9* pDevice, IDirect3DSurface9* pSurface);
        CD3D9RenderTarget(IDirect3DDevice9* pDevice, int width, int height, TEXTUREFORMAT_TYPE format, bool bMipMaps = false);
    
        virtual ~CD3D9RenderTarget();

        virtual void OnLostDevice();
        virtual void OnResetDevice();

        virtual CD3D9TextureBase* GetTexture() const { return m_pTexture; }

    private:

        virtual void Bind(int index);

    private:
        IDirect3DSurface9* m_pSurface;
        IDirect3DDevice9* m_pDevice;

        CD3D9TextureBase* m_pTexture;

        int m_unit;

        bool m_bWriteOnly;
    };

    typedef CD3D9RenderTarget CRenderTarget;
} // env

#endif // D3D9